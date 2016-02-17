// udp echo client

// headers
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local headers
#include "mysockets.h"

// request udp echo service
void
udpecho(const char *host, const char *service)
{
	char buf[BUFSIZ+1];
	int s, nchars;

	// check port base
	checkPortBase();

	// connect to service
	MustBeTrue((s = connectToUDP(host, service)) >= 0);

	// wait for response
	while (fgets(buf, sizeof(buf), stdin))
	{
		// write out buffer to network
		buf[BUFSIZ] = 0;
		nchars = strlen(buf);
		(void) write(s, buf, nchars);

		// read in response
		if (read(s, buf, nchars) < 0)
		{
			ERROR("socket read failed", errno);
			exit(1);
		}
		fputs(buf, stdout);
	}
	return;
}

// main entry point
main(int argc, char **argv)
{
	char *host = "lts08.ih.lucent.com";
	char *service = "echo";

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
		ERROR("usage: tvpdaytime [host [port]]", EINVAL);
		return(1);
	}

	// using service and host
	cout << "host is .. " << host << endl;
	cout << "service is .. " << service << endl;

	// call day/time service
	udpecho(host, service);
	return(0);
}
