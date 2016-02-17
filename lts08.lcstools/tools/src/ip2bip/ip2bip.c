// convert any ip address to the broadcast ip address given the
// subnet mask.

// headers
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// internal types
typedef union {
	unsigned long ip;
	unsigned char parts[4];
} ip_address;

// some constants
const unsigned long PrefixA = 0x00000000;
const unsigned long PrefixB = 0x80000000;
const unsigned long PrefixC = 0xc0000000;
const unsigned long PrefixD = 0xe0000000;
const unsigned long PrefixE = 0xf0000000;

const unsigned long PrefixMaskA = 0x80000000;
const unsigned long PrefixMaskB = 0xc0000000;
const unsigned long PrefixMaskC = 0xe0000000;
const unsigned long PrefixMaskD = 0xf0000000;
const unsigned long PrefixMaskE = 0xf8000000;

#define IsClassA(ip) (((ip)&PrefixMaskA) == PrefixA)
#define IsClassB(ip) (((ip)&PrefixMaskB) == PrefixB)
#define IsClassC(ip) (((ip)&PrefixMaskC) == PrefixC)
#define IsClassD(ip) (((ip)&PrefixMaskD) == PrefixD)
#define IsClassE(ip) (((ip)&PrefixMaskE) == PrefixE)

// check ip address format
int
IsIpOK(const char *ip)
{
	if (ip == NULL || *ip == '\0')
		return(0);
	char buf[BUFSIZ];
	strcpy(buf, ip);
	if (strtok(buf, ".") == NULL)
		return(0);
	if (strtok(NULL, ".") == NULL)
		return(0);
	if (strtok(NULL, ".") == NULL)
		return(0);
	if (strtok(NULL, ".") == NULL)
		return(0);
	if (strtok(NULL, ".") != NULL)
		return(0);
	return(1);
}

// main entry
main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s ip-address subnet-mask\n", 
			argv[0]);
		return(2);
	}
	if (!IsIpOK(argv[1]))
	{
		fprintf(stderr, "ip-address has wrong format; should be X.X.X.X.\n");
		return(2);
	}
	if (!IsIpOK(argv[2]))
	{
		fprintf(stderr, "mask has wrong format; should be X.X.X.X.\n");
		return(2);
	}

	ip_address ipaddr;
	ipaddr.parts[0] = atoi(strtok(argv[1], "."));
	ipaddr.parts[1] = atoi(strtok(NULL, "."));
	ipaddr.parts[2] = atoi(strtok(NULL, "."));
	ipaddr.parts[3] = atoi(strtok(NULL, "."));
	const char *ipclass = NULL;
	if (IsClassA(ipaddr.ip))
		ipclass = "A";
	else if (IsClassB(ipaddr.ip))
		ipclass = "B";
	else if (IsClassC(ipaddr.ip))
		ipclass = "C";
	else if (IsClassD(ipaddr.ip))
		ipclass = "D";
	else if (IsClassE(ipaddr.ip))
		ipclass = "E";
	else
	{
		fprintf(stderr, "unknown class, good bye.\n");
	}
	fprintf(stdout, "Class %s IP Address = %d.%d.%d.%d\n",
		ipclass, ipaddr.parts[0], ipaddr.parts[1],
		ipaddr.parts[2], ipaddr.parts[3]);
	
	ip_address mask;
	mask.parts[0] = atoi(strtok(argv[2], "."));
	mask.parts[1] = atoi(strtok(NULL, "."));
	mask.parts[2] = atoi(strtok(NULL, "."));
	mask.parts[3] = atoi(strtok(NULL, "."));

	ip_address broadcast_ipaddr;
	broadcast_ipaddr.ip = ipaddr.ip | ~mask.ip;
	fprintf(stdout, "Broadcast IP Address = %d.%d.%d.%d\n",
		broadcast_ipaddr.parts[0], broadcast_ipaddr.parts[1],
		broadcast_ipaddr.parts[2], broadcast_ipaddr.parts[3]);
	return(0);
}
