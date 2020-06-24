// standard unix headers
// #include <sysent.h>
#include <stdlib.h>
#include <iostream.h>
#include <errno.h>

// tcp/ip headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// local headers
#include "returns.h"
#include "debug.h"

main(int argc, char **argv)
{
	struct servent *pserv;

	MustBeTrue(argc == 3);

	if ((pserv = getservbyname(argv[1], argv[2])) == NULL)
	{
		ERRORD("getservbyname failed.", argv[1], errno);
	}
	else
	{
		DUMP(pserv->s_name);
		for (int i = 0; pserv->s_aliases[i] != NULL; i++)
		{
			DUMP(pserv->s_aliases[i]);
		}
		DUMP(pserv->s_port);
		DUMP(pserv->s_proto);
	}
	return(0);
}

