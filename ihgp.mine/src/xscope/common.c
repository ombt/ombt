/* ************************************************************ *\
 *								*
 *    Common support routines for sockets			*
 *								*
 *       James L. Peterson	 				*
 *	(c) Copyright MCC, 1987                                 *
 * 				  				*
 * 				  				*
 \* *********************************************************** */

#include "scope.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/uio.h>	       /* for struct iovec, used by socket.h */
#include <sys/socket.h>	       /* for AF_INET, SOCK_STREAM, ... */

/* ********************************************** */
/*						  */
/*       Global defines for cleanup               */
/*						  */
/* ********************************************** */
	FD ConnectionSocket;
	struct sockaddr * sockaddrptr;

/* ********************************************** */
/*						  */
/*       Debugging support routines               */
/*						  */
/* ********************************************** */

enterprocedure(s)
     char   *s;
{
  debug(2,(stderr, "-> %s\n", s));
}

warn(s)
     char   *s;
{
  fprintf(stderr, "####### %s\n", s);
}

panic(s)
     char   *s;
{
  fprintf(stderr, "%s\n", s);
  shutdown(ConnectionSocket, 2);
  close(ConnectionSocket);
  exit(1);
}

/* ********************************************** */
/*						  */
/*  Debugging forms of memory management          */
/*						  */
/* ********************************************** */

extern char *malloc();

char   *Malloc (n)
     long    n;
{
  char   *p;
  p = (char *)malloc((unsigned int)n);
  debug(64,(stderr, "%x = malloc(%d)\n", p, n));
  if (p == NULL)
    panic("no more malloc space");
  return(p);
}

Free(p)
     char   *p;
{
  debug(64,(stderr, "%x = free\n", p));
  free(p);
}



/* ************************************************************ */
/*								*/
/*    Signal Handling support					*/
/*								*/
/* ************************************************************ */

#include <signal.h>

#ifdef SIGNALRETURNSINT
#define SIGVAL int
#else
#define SIGVAL void
#endif

/* ARGSUSED */
SIGVAL
SignalHUP(n) /* 1 */
    int n;
{
  debug(1,(stderr, "==> SIGHUP received\n"));
  debug(1,(stderr, "shutting down socket FD %d\n",ConnectionSocket));
  shutdown(ConnectionSocket, 2);
  debug(1,(stderr, "closing socket FD %d\n",ConnectionSocket));
  close(ConnectionSocket);
  exit(1);
}

/* ARGSUSED */
SIGVAL
SignalINT(n) /* 2 */
    int n;
{
  debug(1,(stderr, "==> SIGINT received\n"));
  debug(1,(stderr, "shutting down socket FD %d\n",ConnectionSocket));
  shutdown(ConnectionSocket, 2);
  debug(1,(stderr, "closing socket FD %d\n",ConnectionSocket));
  close(ConnectionSocket);
  exit(1);
}

/* ARGSUSED */
SIGVAL
SignalQUIT(n) /* 3 */
    int n;
{
  debug(1,(stderr, "==> SIGQUIT received\n"));
  debug(1,(stderr, "shutting down socket FD %d\n",ConnectionSocket));
  shutdown(ConnectionSocket, 2);
  debug(1,(stderr, "closing socket FD %d\n",ConnectionSocket));
  close(ConnectionSocket);
  exit(1);
}

/* ARGSUSED */
SIGVAL
SignalPIPE(n) /* 13 */
    int n;
{
  debug(1,(stderr, "==> SIGPIPE received\n"));
}

/* ARGSUSED */
SIGVAL
SignalTERM(n) /* 15 */
    int n;
{
  debug(1,(stderr, "==> SIGTERM received\n"));
  debug(1,(stderr, "shutting down socket FD %d\n",ConnectionSocket));
  shutdown(ConnectionSocket, 2);
  debug(1,(stderr, "closing socket FD %d\n",ConnectionSocket));
  close(ConnectionSocket);
  exit(1);
}

/* ARGSUSED */
SIGVAL
SignalUSR1(n) /* 16 */
    int n;
{
  extern char ScopeEnabled;

  debug(1,(stderr, "==> SIGCONT received\n"));
  ScopeEnabled = ! ScopeEnabled;
}

/* ARGSUSED */ 
SIGVAL
SignalURG(n) /* 21 */
    int n;
{
  debug(1,(stderr, "==> SIGURG received\n"));
}

/* ARGSUSED */
SIGVAL
SignalTSTP(n) /* 24 */
    int n;
{
  debug(1,(stderr, "==> SIGTSTP received\n"));
}

/* ARGSUSED */
SIGVAL
SignalCONT(n) /* 25 */
    int n;
{
  debug(1,(stderr, "==> SIGCONT received\n"));
}

/* ARGSUSED */
SIGVAL
SignalUNIV(n) /* 25 */
    int n;
{
  debug(1,(stderr, "==> SIGNAL %d received\n",n ));
}

SetSignalHandling()
{
  extern char HandleSIGUSR1;

  enterprocedure("SetSignalHandling");
  (void)signal(SIGHUP, SignalHUP);	/* 1  */
  (void)signal(SIGINT, SignalINT);	/* 2  */
  (void)signal(SIGQUIT, SignalQUIT);	/* 3  */
  (void)signal(SIGPIPE, SignalPIPE);	/* 13 */
  (void)signal(SIGTERM, SignalTERM);	/* 15 */
  (void)signal(SIGURG, SignalURG);	/* 21 */
  (void)signal(SIGTSTP, SignalTSTP);	/* 24 */
  /*(void)signal(SIGILL, SignalUNIV);
  (void)signal(SIGTRAP, SignalUNIV);
  (void)signal(SIGIOT, SignalUNIV);
  (void)signal(SIGEMT, SignalUNIV);
  (void)signal(SIGFPE, SignalUNIV);
  (void)signal(SIGKILL, SignalUNIV);
  (void)signal(SIGBUS, SignalUNIV);
  (void)signal(SIGSEGV, SignalUNIV);
  (void)signal(SIGSYS, SignalUNIV);
  (void)signal(SIGALRM, SignalUNIV);
  (void)signal(SIGUSR2, SignalUNIV);
  (void)signal(SIGCHLD, SignalUNIV);
  (void)signal(SIGPWR, SignalUNIV);
  (void)signal(SIGWINCH, SignalUNIV);
  (void)signal(SIGPOLL, SignalUNIV);
  (void)signal(SIGSTOP, SignalUNIV);
  (void)signal(SIGCONT, SignalCONT);
  (void)signal(SIGTTIN, SignalUNIV);
  (void)signal(SIGTTOU, SignalUNIV);
  (void)signal(SIGVTALRM, SignalUNIV);
  (void)signal(SIGPROF, SignalUNIV);
  (void)signal(SIGXCPU, SignalUNIV);
  (void)signal(SIGXFSZ, SignalUNIV);
  (void)signal(SIGWAITING, SignalUNIV);
  (void)signal(SIGLWP, SignalUNIV);
  (void)signal(SIGFREEZE, SignalUNIV);
  (void)signal(SIGTHAW, SignalUNIV); */
  if (HandleSIGUSR1)
    (void)signal(SIGUSR1, SignalUSR1);	/* 16 */
}



/* ************************************************************ */
/*								*/
/*   Create a socket for a service to listen for clients        */
/*								*/
/* ************************************************************ */

#include <sys/ioctl.h>	       /* for FIONCLEX, FIONBIO, ... */
#ifdef SVR4
#include <sys/filio.h>
#endif
#include <netinet/in.h>	       /* struct sockaddr_in */
#include <netdb.h>	       /* struct servent * and struct hostent *  */

static int  ON = 1 /* used in ioctl */ ;
#define	BACKLOG	5

/* for use in the UsingFD call -- defined later */
extern int  NewConnection ();


SetUpConnectionSocket(iport)
     int     iport;
{
  struct sockaddr_in  sin;
  short port;
#ifndef	SO_DONTLINGER
  struct linger linger;
#endif /* SO_DONTLINGER */
  int	ret,i;

  enterprocedure("SetUpConnectionSocket");

  /* create the connection socket and set its parameters of use */
  ConnectionSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (ConnectionSocket < 0)
    {
      perror("socket");
      exit(-1);
    }
  i=1;
  ret=setsockopt(ConnectionSocket, SOL_SOCKET, SO_REUSEADDR,   (char *)&i, sizeof(i));
  debug(4,(stderr, "setsockopt #1 return=%d, error=%d\n", ret, errno));
  i=0;
  ret=setsockopt(ConnectionSocket, SOL_SOCKET, SO_USELOOPBACK,   (char *)&i, sizeof(i));
  debug(4,(stderr, "setsockopt #2 return=%d, error=%d\n", ret, errno));
#ifdef	SO_DONTLINGER
  (void)setsockopt(ConnectionSocket, SOL_SOCKET, SO_DONTLINGER,   (char *)&i, sizeof(i));
#else /* SO_DONTLINGER */
  linger.l_onoff = 0;
  linger.l_linger = 0;
  ret=setsockopt(ConnectionSocket, SOL_SOCKET, SO_LINGER, (char *)&linger, sizeof linger);
  debug(4,(stderr, "setsockopt #4 return=%d, error=%d\n", ret, errno));
#endif /* SO_DONTLINGER */

  /* define the name and port to be used with the connection socket */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;

  /* the address of the socket is composed of two parts: the host machine and
     the port number.  We need the host machine address for the current host
   */
  {
    /* define the host part of the address */
    char    MyHostName[256];
    struct hostent *hp;

    (void) gethostname(MyHostName, sizeof(MyHostName));
    ScopeHost = (char *) Malloc((long)strlen(MyHostName));
    (void)strcpy(ScopeHost, MyHostName);
    hp = gethostbyname(MyHostName);
    if (hp == NULL)
      panic("No address for our host");
    bcopy((char *)hp->h_addr, (char*)&sin.sin_addr, hp->h_length);
  }
    /* new code -- INADDR_ANY should be better than using the name of the
       host machine.  The host machine may have several different network
       addresses.  INADDR_ANY should work with all of them at once. */
  sin.sin_addr.s_addr = INADDR_ANY;

  port = iport;
  sin.sin_port = htons (port);
  ScopePort = port;
  sockaddrptr = (struct sockaddr *)&sin;


  /* bind the name and port number to the connection socket */
  if (ret=(bind(ConnectionSocket, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
  debug(4,(stderr, "bind failed, error=%d\n",errno));
      perror("bind");
      close(ConnectionSocket);
      exit(-1);
    }

  debug(4,(stderr, "Socket is FD %d for %s,%d\n",
	   ConnectionSocket, ScopeHost, ScopePort));

  /* now activate the named connection socket to get messages */
  if (listen(ConnectionSocket, BACKLOG) < 0)
    {
      perror("listen");
      shutdown(ConnectionSocket, 2);
      close(ConnectionSocket);
      exit(-1);
    };

  /* a few more parameter settings */
#ifdef FD_CLOEXEC
  (void)fcntl(ConnectionSocket, F_SETFD, FD_CLOEXEC);
#else
  (void)ioctl(ConnectionSocket, FIOCLEX, 0);
#endif
  /* ultrix reads hang on Unix sockets, hpux reads fail */
#if defined(O_NONBLOCK) && (!defined(ultrix) && !defined(hpux))
  (void) fcntl (ConnectionSocket, F_SETFL, O_NONBLOCK);
#else
#ifdef FIOSNBIO
  (void) ioctl (ConnectionSocket, FIOSNBIO, &ON);
#else
  (void) fcntl (ConnectionSocket, F_SETFL, FNDELAY);
#endif
#endif

  debug(4,(stderr, "Listening on FD %d\n", ConnectionSocket));
  UsingFD(ConnectionSocket, NewConnection);
}
