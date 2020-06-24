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

// time server
int
main(int argc, char **argv)
{
	struct sockaddr_in fsin;
	char *service = "time";
	char buf[BUFSIZ];
	int sock;
	time_t now;
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
		ERROR("usage: udptimed [port]", EINVAL);
		return(1);
	}

	// check of any port base offset
	checkPortBase();

	// get socket
	sock = passiveUDP(service);

	// get a UDP socket for the service
	while (1)
	{
		alen = sizeof(fsin);
		if (recvfrom(sock, buf, sizeof(buf), 0,
			(sockaddr *)&fsin, &alen) < 0)
		{
			ERROR("recvfrom failed", errno);
			return(1);
		}
DUMP(fsin.sin_addr.s_addr);
		time(&now);
		now = htonl((u_long)(now+2208988800ul));
		sendto(sock, (char *)&now, sizeof(now), 0,
			(sockaddr *)&fsin, sizeof(fsin));
	}
}
