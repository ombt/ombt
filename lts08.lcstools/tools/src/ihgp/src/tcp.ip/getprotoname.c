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
	struct protoent *pproto;

	MustBeTrue(argc == 2 && argv[1] != 0);

	if ((pproto = getprotobyname(argv[1])) == NULL)
	{
		ERRORD("getprotobyname failed.", argv[1], errno);
	}
	else
	{
		DUMP(pproto->p_name);
		for (int i = 0; pproto->p_aliases[i] != NULL; i++)
		{
			DUMP(pproto->p_aliases[i]);
		}
		DUMP(pproto->p_proto);
	}
	return(0);
}

