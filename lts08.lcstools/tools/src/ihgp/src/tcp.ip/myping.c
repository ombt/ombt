// tcp ping client

// headers
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local headers
#include "mysockets.h"

// request tcp echo service
void
tcpecho(const char *host, const char *service)
{
	char buf[BUFSIZ+1];
	int s, n;
	int outchars, inchars;

	// check port base
	checkPortBase();

	// connect to service
	if ((s = connectToTCP(host, service)) < 0)
	{
		ERROR("connectToSocket failed", errno);
		exit(1);
	}

	// write out buffer to network
	strcpy(buf,"I AM ALIVE\n");
	buf[BUFSIZ] = 0;
	outchars = strlen(buf);
	(void) write(s, buf, outchars);

	// read in response
	for (inchars = 0; inchars < outchars; inchars += n)
	{
		n = read(s, buf+inchars, outchars - inchars);
		if (n < 0)
		{
			ERROR("socket read failed", errno);
			exit(1);
		}
	}
	fputs(buf, stdout);
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
	case 2:
		host = argv[1];
		break;
	default:
		ERROR("usage: myping host", EINVAL);
		return(1);
	}

	// call day/time service
	tcpecho(host, service);

	// all done
	return(0);
}
