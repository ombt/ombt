#ifndef __MYSOCKETS_H
#define __MYSOCKETS_H
// definitions for sockets interface

// standard headers
// #include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream.h>
#include <fstream.h>
#include <memory.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

// tcp/ip headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// signal handler headers
#include <signal.h>

// other headers
#include "returns.h"
#include "debug.h"

// local definitions
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

// globals
extern u_short portbase;
extern int socketToClose;

// proto-types
extern int connectToTCP(const char *, const char *);
extern int connectToUDP(const char *, const char *);
extern int connectToSocket(const char *, const char *, const char *);
extern int passiveUDP(const char *service);
extern int passiveTCP(const char *service, int);
extern int passivesock(const char *service, const char *transport, int qlen);
extern int checkPortBase(const char * = NULL);
extern void cleanup(int);
extern void reaper(int);
extern void setCleanUp();

#endif
