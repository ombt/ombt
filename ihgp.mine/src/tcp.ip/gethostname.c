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

struct {
	int family;
	char *description;
} addrfam[] = {
AF_UNSPEC, " unspecified ",
AF_UNIX, " local to host (pipes, portals) ",
AF_INET, " internetwork: UDP, TCP, etc. ",
AF_IMPLINK, " arpanet imp addresses ",
AF_PUP, " pup protocols: e.g. BSP ",
AF_CHAOS, " mit CHAOS protocols ",
AF_NS, " XEROX NS protocols ",
AF_NBS, " nbs protocols ",
AF_ECMA, " european computer manufacturers ",
AF_DATAKIT, " datakit protocols ",
AF_CCITT, " CCITT protocols, X.25 etc ",
AF_SNA, " IBM SNA ",
AF_DECnet, " DECnet ",
AF_DLI, " Direct data link interface ",
AF_LAT, " LAT ",
AF_HYLINK, " NSC Hyperchannel ",
AF_APPLETALK, " Apple Talk ",
AF_NIT, " Network Interface Tap ",
AF_802, " IEEE 802.2, also ISO 8802 ",
AF_OSI, " umbrella for all families used ",
AF_X25, " CCITT X.25 in particular ",
AF_OSINET, " AFI = 47, IDI = 4 ",
AF_GOSIP, " U.S. Government OSI ",
AF_IPX, " Novell Internet Protocol ",
AF_MAX, " MAX FAMILY ",
	
};

main(int argc, char **argv)
{
	struct hostent *phost;

	MustBeTrue(argc == 2 && argv[1] != 0);

	if ((phost = gethostbyname(argv[1])) == NULL)
	{
		ERRORD("gethostbyname failed.", argv[1], errno);
	}
	else
	{
		DUMP(phost->h_name);
		int i;
		for (i = 0; phost->h_aliases[i] != NULL; i++)
		{
			DUMP(phost->h_aliases[i]);
		}
		for (i = 0; addrfam[i].family < AF_MAX; i++)
		{
			if (phost->h_addrtype == addrfam[i].family)
			{
				DUMP(addrfam[i].description);
			}
		}
#if 0
		for (i = 0; phost->h_addr_list[i] != NULL; i++)
		{
			DUMP(phost->h_addr_list[i]);
		}
#endif
	}
	return(0);
}

