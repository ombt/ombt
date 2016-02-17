// server for echo service

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
const int BufSize = 4096;

// TCP echo protocol
int
echo(int fd)
{
TRACE();
	char buf[BufSize+1];
	int cc;
TRACE();
	cc = read(fd, buf, sizeof(buf));
TRACE();
	if (cc < 0)
	{
TRACE();
		ERROR("read failed", errno);
		return(0);
	}
TRACE();
	if (cc > 0 && (write(fd, buf, cc) < 0))
	{
TRACE();
		ERROR("write failed", errno);
		return(0);
	}
TRACE();
	return(cc);
}

// time server
int
main(int argc, char **argv)
{
	char *service = "echo";
	struct sockaddr_in fsin;
	int msock;
	fd_set rfds, afds;
	int fd, nfds;
	socklen_t alen;

	// check the arguments
	switch (argc)
	{
	case 1:
		break;
	case 2:
		service = argv[1];
		break;
	default:
		ERROR("usage: tcpechod [port]", EINVAL);
		return(1);
	}

	// signal handlers for exiting
	setCleanUp();

	// check of any port base offset
	checkPortBase();

	// get socket
	msock = passiveTCP(service, QueueLength);
	if (msock < 0)
	{
		ERROR("passiveTCP failed", errno);
		return(2);
	}
DUMP(msock);
	socketToClose = msock;
DUMP(socketToClose);

	// set bit vector
	nfds = getdtablesize();
	FD_ZERO(&afds);
	FD_SET(msock, &afds);

	// get a TCP socket for the service
TRACE();
	while (1)
	{
		// copy vector
TRACE();
		memcpy(&rfds, &afds, sizeof(rfds));

		// select a socket to handle
TRACE();
		if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
			(timeval *)0) < 0)
		{
			ERROR("select failed", errno);
			return(2);
		}

		// check if master socket needs tending
TRACE();
		if (FD_ISSET(msock, &rfds))
		{
TRACE();
			int ssock;
			alen = sizeof(fsin);
TRACE();
			ssock = accept(msock, (sockaddr *)&fsin, &alen);
TRACE();
			if (ssock < 0)
			{
				ERROR("accept failed", errno);
				return(2);
			}
TRACE();
			FD_SET(ssock, &afds);
		}

		// check of a slave socket needs tending
TRACE();
		for (fd = 0; fd < nfds; fd++)
		{
TRACE();
			if (fd != msock && FD_ISSET(fd, &rfds))
			{
TRACE();
				if (echo(fd) == 0)
				{
TRACE();
					close(fd);
					FD_CLR(fd, &afds);
				}
			}
		}
	}
}

