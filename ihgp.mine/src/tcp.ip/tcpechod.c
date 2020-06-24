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
TCPechod(int fd)
{
	char buf[BufSize+1];
	int cc;
	while ((cc = read(fd, buf, sizeof(buf))) > 0)
	{
		buf[cc] = 0;
DUMP(buf);
		if (write(fd, buf, cc) < 0)
		{
			ERROR("write failed", errno);
			exit(2);
		}
	}
	if (cc < 0)
	{
		ERROR("read failed", errno);
	}
	else
		return(0);
}

// time server
int
main(int argc, char **argv)
{
	char *service = "echo";
	struct sockaddr_in fsin;
	char buf[BUFSIZ];
	int msock, ssock;
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
	socketToClose = msock;

	// get a TCP socket for the service
	while (1)
	{
		alen = sizeof(fsin);
		ssock = accept(msock, (sockaddr *)&fsin, &alen);
		if (ssock < 0)
		{
			if (errno == EINTR)
				continue;
			ERROR("accept failed", errno);
			return(1);
		}
		switch (fork())
		{
		case 0:
			// child
			close(msock);
			socketToClose = -1;
			return(TCPechod(ssock));

		case -1:
			// oops
			ERROR("fork failed", errno);
			return(2);

		default:
			// parent
DUMP(fsin.sin_addr.s_addr);
			close(ssock);
			break;
		}	
	}
}

