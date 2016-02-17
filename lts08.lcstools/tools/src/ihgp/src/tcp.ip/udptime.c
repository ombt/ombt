// udp time client

// headers
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local headers
#include "mysockets.h"

// local definitions
#define UNIXBASETIME 2208988800ul
#define MSG "what time is it?"

// main entry point
main(int argc, char **argv)
{
	char *host = "lts08.ih.lucent.com";
	char *service = "time";
	time_t now;
	int s, n;

	// check arguments
	switch (argc)
	{
	case 1:
		host = "lts08.ih.lucent.com";
		break;
	case 3:
		service = argv[2];
	case 2:
		host = argv[1];
		break;
	default:
		ERROR("usage: udptime [host [port]]", EINVAL);
		return(1);
	}

	// using service and host
	cout << "host is .. " << host << endl;
	cout << "service is .. " << service << endl;

	// check port base
	checkPortBase();

	// set up connection
	MustBeTrue((s = connectToUDP(host, service)) >= 0);

	// write the message
	(void) write(s, MSG, strlen(MSG));

	// read the response
	n = read(s, &now, sizeof(now));
	if (n < 0)
	{
		ERROR("read failed", errno);
		return(1);
	}

	// write time
	now = ntohl((u_long)now);
	now -= UNIXBASETIME;
	cout << ctime(&now) << endl;

	// all done
	return(0);
}
