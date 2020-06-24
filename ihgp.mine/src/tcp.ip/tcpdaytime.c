// tcp day/time client

// headers
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local headers
#include "mysockets.h"

// request tcp day/time service
void
tcpdaytime(const char *host, const char *service)
{
	char buf[BUFSIZ+1];
	int s, n;

	// check port base
	checkPortBase();

	// connect to service
	MustBeTrue((s = connectToTCP(host, service)) >= 0);

	// wait for response
	while ((n = read(s, buf, BUFSIZ)) > 0)
	{
		buf[n] = 0;
		(void)fputs(buf, stdout);
	}
	return;
}

// main entry point
main(int argc, char **argv)
{
	char *host = "lts08.ih.lucent.com";
	char *service = "daytime";

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
	tcpdaytime(host, service);
	return(0);
}
