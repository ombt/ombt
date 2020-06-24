HDRS = fd.h patchlevel.h pexOCNames.h pexRNames.h pexscope.h scope.h x11.h

OBJS = common.o decode11.o decode_pex.o fd.o print11.o print_pex.o \
       prtype.o scope.o server.o table11.o

CC = cc

CCFLAGS = -g

CCINCLUDE = -I /opt/x11r5/include

CCLIBS = -lm -lsocket -lnsl

xscope:	$(OBJS)
		cc $(CCFLAGS) -o xscope $(OBJS) $(CCLIBS) /usr/ucblib/libucb.a

common.o:	common.c $(HDRS)
		cc $(CCFLAGS) -c common.c $(CCINCLUDE)

decode11.o:	decode11.c $(HDRS)
		cc $(CCFLAGS) -c decode11.c $(CCINCLUDE)

decode_pex.o:	decode_pex.c $(HDRS)
		cc $(CCFLAGS) -c decode_pex.c $(CCINCLUDE)

fd.o:		fd.c $(HDRS)
		cc $(CCFLAGS) -c fd.c $(CCINCLUDE)

print11.o:	print11.c $(HDRS)
		cc $(CCFLAGS) -c print11.c $(CCINCLUDE)

print_pex.o:	print_pex.c $(HDRS)
		cc $(CCFLAGS) -c print_pex.c $(CCINCLUDE)

prtype.o:	prtype.c $(HDRS)
		cc $(CCFLAGS) -c prtype.c $(CCINCLUDE)

scope.o:	scope.c $(HDRS)
		cc $(CCFLAGS) -c scope.c $(CCINCLUDE)

server.o:	server.c $(HDRS)
		cc $(CCFLAGS) -c server.c $(CCINCLUDE)

table11.o:	table11.c $(HDRS)
		cc $(CCFLAGS) -c table11.c $(CCINCLUDE)

