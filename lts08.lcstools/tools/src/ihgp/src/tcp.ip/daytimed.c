// server for time service, both udp and tcp

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
const int LineLength = 1024;
const int QueueLength = 5;

// local find-the-maximum function
template <class Type>
Type
mymax(Type x, Type y)
{
	if (x > y)
		return(x);
	else
		return(y);
}

// get current time and date
void
daytime(char *buf)
{
	char *pts;
	time_t now;
	time(&now);
	sprintf(buf, "%s", ctime(&now));
	return;
}

// time server
int
main(int argc, char **argv)
{
	char *service = "daytime";
	char buf[LineLength+1];
	struct sockaddr_in fsin;
	socklen_t alen;
	int tsock, usock, nfds;
	fd_set rfds;

	// check the arguments
	switch (argc)
	{
	case 1:
		break;
	case 2:
		service = argv[1];
		break;
	default:
		ERROR("usage: daytimed [port]", EINVAL);
		return(1);
	}

	// signal handlers for exiting
	setCleanUp();

	// check of any port base offset
	checkPortBase();

	// get socket
	tsock = passiveTCP(service, QueueLength);
	socketToClose = tsock;
	usock = passiveUDP(service);
	nfds = mymax(tsock, usock)+1;

	// zero out the bit vector
	FD_ZERO(&rfds);

	// get a TCP socket for the service
	while (1)
	{
		// set tcp and udp bits
		FD_SET(tsock, &rfds);
		FD_SET(usock, &rfds);

		// check which sockets are ready for  readsd
		if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
			(timeval *)0) < 0)
		{
			ERROR("select failed", errno);
			return(2);
		}

		// check if tcp is active
		if (FD_ISSET(tsock, &rfds))
		{
			int ssock;

			// get socket for connection
			alen = sizeof(fsin);
			ssock = accept(tsock, (sockaddr *)&fsin, &alen);
DUMP(fsin.sin_addr.s_addr);
			if (ssock < 0)
			{
				ERROR("accept failed", errno);
				return(1);
			}

			// call day time function
			daytime(buf);

			// write day and time, then close socket
			write(ssock, buf, strlen(buf));
			close(ssock);
		}

		// check if udp is active
		if (FD_ISSET(usock, &rfds))
		{
			alen = sizeof(fsin);
			if (recvfrom(usock, buf, sizeof(buf), 0,
				(sockaddr *)&fsin, &alen) < 0)
			{
				ERROR("recvfrom failed", errno);
				return(1);
			}
DUMP(fsin.sin_addr.s_addr);

			// get day/time
			daytime(buf);

			// send back the response
			sendto(usock, buf, strlen(buf), 0,
				(sockaddr *)&fsin, sizeof(fsin));
		}
	}
}

