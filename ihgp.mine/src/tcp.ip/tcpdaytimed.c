// server for time service

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

// TCP day/time protocol
void
TCPdaytimed(int fd)
{
	char *pts;
	time_t now;
	pts = ctime(&now);
	write(fd, pts, strlen(pts));
	return;
}

// time server
int
main(int argc, char **argv)
{
	struct sockaddr_in fsin;
	char *service = "daytime";
	char buf[BUFSIZ];
	socklen_t alen;
	int msock, ssock;

	// check the arguments
	switch (argc)
	{
	case 1:
		break;
	case 2:
		service = argv[1];
		break;
	default:
		ERROR("usage: tcpdaytimed [port]", EINVAL);
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
		ssock = accept(msock, (sockaddr *)&fsin, &alen);
		if (ssock < 0)
		{
			ERROR("accept failed", errno);
			return(1);
		}
DUMP(fsin.sin_addr.s_addr);
		TCPdaytimed(ssock);
		close(ssock);
	}
}

