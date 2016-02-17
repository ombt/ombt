// super server for multiple protocols

// headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

// local headers
#include "mysockets.h"

// local definitions
const int QueueLength = 5;
const int NoSocket = -1;
const int LineLength = 72;
const int BufferSize = 4096;
const unsigned long UnixBaseTime = 2208988800ul;

// local enumerations
enum Protocol {
	NoProtocol, UseUDP, UseTCP
};

// service data structure
struct Service {
	char *service;
	Protocol protocol;
	int socket;
	int (*server)(int);
};

// local find-the-maximum function
template <class Type>
Type
mymax(Type x, Type y)
{
DUMP("mymax");
DUMP(getpid());
	if (x > y)
		return(x);
	else
		return(y);
}

// UDP day/time protocol
int
UDPechod(int fd)
{
DUMP("UDPechod");
DUMP(getpid());
	sockaddr_in fsin;
	int buflen;
	socklen_t alen;
	char buf[BUFSIZ];

	alen = sizeof(fsin);
	buflen = recvfrom(fd, buf, BUFSIZ, 0, (sockaddr *)&fsin, &alen);
	if (buflen < 0)
	{
		ERROR("recvfrom failed", errno);
		close(fd);
		return(1);
	}
	sendto(fd, buf, buflen, 0, (sockaddr *)&fsin, sizeof(fsin));
	return(0);
}

// UDP day/time protocol
int
UDPdaytimed(int fd)
{
DUMP("UDPdaytimed");
DUMP(getpid());
	time_t now;
	sockaddr_in fsin;
	socklen_t alen;
	char buf[BUFSIZ];

	alen = sizeof(fsin);
	if (recvfrom(fd, buf, sizeof(buf), 0, (sockaddr *)&fsin, &alen) < 0)
	{
		ERROR("recvfrom failed", errno);
		close(fd);
		return(1);
	}
	time(&now);
	sprintf(buf, "%s", ctime(&now));
	sendto(fd, buf, strlen(buf), 0, 
		(sockaddr *)&fsin, sizeof(fsin));
	return(0);
}

// UDP time protocol
int
UDPtimed(int fd)
{
DUMP("UDPtimed");
DUMP(getpid());
	time_t now;
	sockaddr_in fsin;
	socklen_t alen;
	char buf[BUFSIZ];

	alen = sizeof(fsin);
	if (recvfrom(fd, buf, sizeof(buf), 0, (sockaddr *)&fsin, &alen) < 0)
	{
		ERROR("recvfrom failed", errno);
		close(fd);
		return(1);
	}
	time(&now);
	now = htonl((u_long)(now+UnixBaseTime));
	sendto(fd, (char *)&now, sizeof(now), 0, 
		(sockaddr *)&fsin, sizeof(fsin));
	return(0);
}

// TCP chargen protocol
int
TCPchargend(int fd)
{
DUMP("TCPchargend");
DUMP(getpid());
	char c, buf[LineLength+2];
	c = ' ';
	buf[LineLength] = '\r';
	buf[LineLength+1] = '\n';
	while ( 1 )
	{
		int i;
		for (i = 0; i < LineLength; i++)
		{
			buf[i] = c++;
			if (c > '~')
				c = ' ';
		}
		if (write(fd, buf, LineLength+2) < 0)
			break;
	}
	close(fd);
	return(0);
}

// TCP day/time protocol
int
TCPechod(int fd)
{
DUMP("TCPechod");
DUMP(getpid());
	char buf[BufferSize];
	int cc;
	while (cc = read(fd, buf, sizeof(buf)))
	{
		if (cc < 0)
		{
			ERROR("TCPechod read failed", errno);
			exit(2);
		}
		if (write(fd, buf, cc) < 0)
		{
			ERROR("TCPechod write failed", errno);
			exit(2);
		}
	}
	close(fd);
	return(0);
}

// TCP day/time protocol
int
TCPdaytimed(int fd)
{
DUMP("TCPdaytimed");
DUMP(getpid());
	char buf[LineLength];
	time_t now;

	time(&now);
	sprintf(buf, "%s", ctime(&now));
	write(fd, buf, strlen(buf));
	close(fd);
	return(0);
}

// TCP time protocol
int
TCPtimed(int fd)
{
DUMP("TCPtimed");
DUMP(getpid());
	time_t now;

	time(&now);
	now = htonl((ulong)(now+UnixBaseTime));
	write(fd, (char *)&now, sizeof(now));
	close(fd);
	return(0);
}

// service data
Service services[] = {
	{ "echo", UseUDP, NoSocket, UDPechod },
	{ "daytime", UseUDP, NoSocket, UDPdaytimed },
	{ "time", UseUDP, NoSocket, UDPtimed },
	{ "chargen", UseTCP, NoSocket, TCPchargend },
	{ "echo", UseTCP, NoSocket, TCPechod },
	{ "daytime", UseTCP, NoSocket, TCPdaytimed },
	{ "time", UseTCP, NoSocket, TCPtimed },
	{ NULL, NoProtocol, NoSocket, NULL },
};

// handle tcp services
void
doTCP(Service *psv)
{
	sockaddr_in fsin;
	socklen_t alen;
	int fd, ssock;

	alen = sizeof(fsin);
	ssock = accept(psv->socket, (sockaddr *)&fsin, &alen);
	if (ssock < 0)
	{
		ERROR("accept failed", errno);
		exit(2);
	}

	switch (fork())
	{
	case 0:
		for (fd = NOFILE; fd >= 0; fd--)
		{
			if (fd == fileno(stdout) || 
			    fd == fileno(stderr) || fd == ssock)
				continue;
			else
				close(fd);
		}
		exit(psv->server(ssock));

	case -1:
		ERROR("fork failed", errno);
		exit(2);

	default:
		close(ssock);
		return;
	}
}

// multiple-protocol super server
int
main(int argc, char **argv)
{
	Service *psv, *fd2sv[NOFILE];
	int fd, nfds;
	fd_set afds, rfds;

	// set port base
	portbase = 0;

	// check the arguments
	switch (argc)
	{
	case 1:
		break;
	case 2:
		portbase = atoi(argv[1]);
		break;
	default:
		ERROR("usage: superd [portbase]", EINVAL);
		return(1);
	}

	// signal handlers for exiting
	setCleanUp();

	// check of any port base offset
	if (portbase == 0)
		checkPortBase();

	// initialize array of services
	nfds = 0;
	FD_ZERO(&afds);
	for (psv = services; psv->service != NULL; psv++)
	{
		// allocate a socket
		switch (psv->protocol)
		{
		case UseUDP:
			psv->socket = passiveUDP(psv->service);
			if (psv->socket < 0)
			{
				ERRORD("passiveUDP failed", 
					psv->socket, errno);
				return(2);
			}
			break;

		case UseTCP:
			psv->socket = passiveTCP(psv->service, QueueLength);
			if (psv->socket < 0)
			{
				ERRORD("passiveTCP failed", 
					psv->socket, errno);
				return(2);
			}
			break;

		default:
			ERROR("invalid protocol in service table", EINVAL);
			return(2);
		}

		// map descriptor to service
		fd2sv[psv->socket] = psv;
		nfds = mymax(psv->socket+1, nfds);
		FD_SET(psv->socket, &afds);
	}

	// wait for a message and process it.
	while (1)
	{
		// copy active descriptors
		memcpy(&rfds, &afds, sizeof(rfds));

		// wait for a calling
		if (select(nfds, &rfds, 0, 0, 0) < 0)
		{
			if (errno == EINTR)
				continue;
			ERROR("select error", errno);
			return(2);
		}

		// process active descriptors
		for (fd = 0; fd < nfds; fd++)
		{
			// check if active
			if ( ! FD_ISSET(fd, &rfds))
				continue;

			// active, process according to protocol
			psv = fd2sv[fd];
			switch (psv->protocol)
			{
			case UseTCP:
				doTCP(psv);
				break;

			case UseUDP:
				psv->server(psv->socket);
				break;

			default:
				ERRORD("invalid protocol", 
					psv->protocol, EINVAL);
				return(2);
			}
		}
	}
}

