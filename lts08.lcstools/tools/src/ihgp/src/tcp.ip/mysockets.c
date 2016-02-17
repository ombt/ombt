// front-end to sockets interface

// headers
#include "mysockets.h"

// globals
u_short portbase = 0;
int socketToClose = -1;

// check if portbase should be used
int
checkPortBase(const char *portbasefile)
{
	const char *defaultportbasefile = "/tmp/portbase";

	// check if a file name was given
	if (portbasefile == NULL || *portbasefile == 0)
	{
		// use default file name
		portbasefile = defaultportbasefile;
	}

	// check if file exists
	if (access(portbasefile, R_OK) == 0)
	{
		// file exists, read in value
		ifstream fin(portbasefile);
		if (!fin)
		{
			ERRORD("unable to read portbase file", 
				portbasefile, errno);
			return(NOTOK);
		}
		fin >> portbase;
		fin.close();
DUMP(portbase);
	}

	// all done
	return(OK);
}

// connect to UDP service
int
connectToUDP(const char *host, const char *service)
{
	return(connectToSocket(host, service, "udp"));
}

// connect to TCP service
int
connectToTCP(const char *host, const char *service)
{
	return(connectToSocket(host, service, "tcp"));
}

// generic connect utility
int
connectToSocket(const char *host, const char *service, const char *transport)
{
	struct hostent *phost;
	struct servent *pservice;
	struct protoent *pprotocol;
	struct sockaddr_in socketaddr;
	int s, type;

	// clear address buffer
	memset(&socketaddr, 0, sizeof(struct sockaddr_in));
	socketaddr.sin_family = AF_INET;

	// map service name to port name
	if ((pservice = getservbyname(service, transport)) != NULL)
	{
		socketaddr.sin_port = 
			htons(ntohs((u_short)(pservice->s_port))+portbase);
	}
	else if ((socketaddr.sin_port = htons((u_short)atoi(service))) == 0)
	{
		ERRORD("getservbyname failed", service, errno);
		return(NOTOK);
	}

	// map host name to ip address
	if ((phost = gethostbyname(host)) != NULL)
	{
		memcpy(&socketaddr.sin_addr, phost->h_addr, phost->h_length);
	}
	else if ((socketaddr.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
	{
		ERRORD("gethostbyname failed", host, errno);
		return(NOTOK);
	}

	// map transport protocol name to protocol number
	if ((pprotocol = getprotobyname(transport)) == NULL)
	{
		ERRORD("getprotobyname failed", transport, errno);
		return(NOTOK);
	}

	// use protocol to choose socket type
	if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

	// allocate a socket
	if ((s = socket(PF_INET, type, pprotocol->p_proto)) < 0)
	{
		ERROR("socket failed", errno);
		return(NOTOK);
	}

	// connect to socket
	if (connect(s, (struct sockaddr *)&socketaddr, sizeof(socketaddr)) < 0)
	{
		ERROR("connect failed", errno);
		return(NOTOK);
	}

	// return socket
	return(s);
}

// connect to a socket in passive mode, used by servers.
int
passivesock(const char *service, const char *transport, int qlen)
{
	struct servent *pse;
	struct protoent *ppe;
	struct sockaddr_in sin;
	int s, type;

	// initialize socket structure
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

	// map service name to port name
	if ((pse = getservbyname(service, transport)) != NULL)
	{
		sin.sin_port = htons(ntohs((u_short)(pse->s_port))+portbase);
	}
	else if ((sin.sin_port = htons((u_short)atoi(service))) == 0)
	{
		ERRORD("unable to convert service to a port", 
			service, errno);
		return(NOTOK);
	}

	// map protocol name to a protocol number
	if ((ppe = getprotobyname(transport)) == NULL)
	{
		ERRORD("unable to get protocol number for transport", 
			transport, errno);
		return(NOTOK);
	}

	// choose a socket type
	if (strcmp(transport, "udp") == 0)
	{
		type = SOCK_DGRAM;
	}
	else if (strcmp(transport, "tcp") == 0)
	{
		type = SOCK_STREAM;
	}
	else
	{
		ERRORD("invalid transport type", transport, errno);
		return(NOTOK);
	}

	// allocate a socket 
	if ((s = socket(PF_INET, type, ppe->p_proto)) < 0)
	{
		ERRORD("unable to create a socket", transport, errno);
		return(NOTOK);
	}

	// bind the socket
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		ERRORD("unable to bind to port", service, errno);
		return(NOTOK);
	}
	if (type == SOCK_STREAM && listen(s, qlen) < 0)
	{
		ERRORD("can not listen at port for service", service, errno);
		return(NOTOK);
	}

	// all done
	return(s);
}

// connect a passive socket for a UDP server.
int
passiveUDP(const char *service)
{
	return(passivesock(service, "udp", 0));
}

// connect a passive socket for a TCP server.
int
passiveTCP(const char *service, int qlen)
{
	return(passivesock(service, "tcp", qlen));
}

// signal handlers and setup routines
void
reaper(int sig)
{
	ERRORD("signal caught", sig, errno);
	int status;

	// catch child signal
	while (waitpid((pid_t)-1, &status, WNOHANG) >= 0) ;

	// reset in case of disabling
	setCleanUp();
	return;
}
void
cleanup(int sig)
{
	ERRORD("signal caught", sig, errno);
	if (socketToClose != -1)
	{
		DUMP(socketToClose);
		close(socketToClose);
	}
	socketToClose = -1;
	reaper(SIGCLD);
	exit(2);
}

void
setCleanUp()
{
	signal(SIGHUP, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGCLD, reaper);
	return;
}

