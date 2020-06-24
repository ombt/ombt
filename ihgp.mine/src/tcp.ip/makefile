
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG -Wno-deprecated
LD = g++
TCLBIN = $(HOME)/bin
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG -Wno-deprecated
INCLDIRS = 
CC = g++
LPRFLAGS = 
AR = /usr/ccs/bin/ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.C.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.cpp.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.CPP.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		udptimed \
		tcpecho \
		tcpdaytime \
		superd \
		getprotoname \
		myping \
		tcpmechod \
		udptime \
		gethostname \
		tcpechod \
		udpdaytime \
		udpecho \
		daytimed \
		tcpdaytimed \
		getservname \
		$(ENDLIST)
		touch all

udptimed:		udptimed.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o udptimed \
			udptimed.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

tcpecho:		tcpecho.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tcpecho \
			tcpecho.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

tcpdaytime:		tcpdaytime.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tcpdaytime \
			tcpdaytime.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

superd:		superd.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o superd \
			superd.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

getprotoname:		getprotoname.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o getprotoname \
			getprotoname.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

myping:		myping.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o myping \
			myping.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

tcpmechod:		tcpmechod.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tcpmechod \
			tcpmechod.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

udptime:		udptime.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o udptime \
			udptime.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

gethostname:		gethostname.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o gethostname \
			gethostname.o \
			-lnsl \
			$(ENDLIST)

tcpechod:		tcpechod.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tcpechod \
			tcpechod.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

udpdaytime:		udpdaytime.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o udpdaytime \
			udpdaytime.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

udpecho:		udpecho.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o udpecho \
			udpecho.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

daytimed:		daytimed.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o daytimed \
			daytimed.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

tcpdaytimed:		tcpdaytimed.o \
		mysockets.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tcpdaytimed \
			tcpdaytimed.o \
			mysockets.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

getservname:		getservname.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o getservname \
			getservname.o \
			-lsocket \
			-lnsl \
			$(ENDLIST)

getservname.o:	getservname.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

udpdaytime.o:	udpdaytime.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

udpecho.o:	udpecho.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

mysockets.o:	mysockets.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

superd.o:	superd.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tcpmechod.o:	tcpmechod.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tcpechod.o:	tcpechod.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

getprotoname.o:	getprotoname.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

udptimed.o:	udptimed.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tcpecho.o:	tcpecho.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

myping.o:	myping.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

gethostname.o:	gethostname.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

udptime.o:	udptime.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

daytimed.o:	daytimed.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tcpdaytimed.o:	tcpdaytimed.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tcpdaytime.o:	tcpdaytime.c \
		./mysockets.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f udptimed
		-rm -f tcpecho
		-rm -f tcpdaytime
		-rm -f superd
		-rm -f getprotoname
		-rm -f myping
		-rm -f tcpmechod
		-rm -f udptime
		-rm -f gethostname
		-rm -f tcpechod
		-rm -f udpdaytime
		-rm -f udpecho
		-rm -f daytimed
		-rm -f tcpdaytimed
		-rm -f getservname
		-rm -f getservname.o
		-rm -f udpdaytime.o
		-rm -f udpecho.o
		-rm -f mysockets.o
		-rm -f superd.o
		-rm -f tcpmechod.o
		-rm -f tcpechod.o
		-rm -f getprotoname.o
		-rm -f udptimed.o
		-rm -f tcpecho.o
		-rm -f myping.o
		-rm -f gethostname.o
		-rm -f udptime.o
		-rm -f daytimed.o
		-rm -f tcpdaytimed.o
		-rm -f tcpdaytime.o

backup:		getservname.c \
		udpdaytime.c \
		udpecho.c \
		mysockets.c \
		superd.c \
		tcpmechod.c \
		tcpechod.c \
		getprotoname.c \
		udptimed.c \
		tcpecho.c \
		myping.c \
		gethostname.c \
		udptime.c \
		daytimed.c \
		tcpdaytimed.c \
		tcpdaytime.c \
		./debug.h \
		./mysockets.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp getservname.c $(BACKUPDIR)/getservname.c
		cp udpdaytime.c $(BACKUPDIR)/udpdaytime.c
		cp udpecho.c $(BACKUPDIR)/udpecho.c
		cp mysockets.c $(BACKUPDIR)/mysockets.c
		cp superd.c $(BACKUPDIR)/superd.c
		cp tcpmechod.c $(BACKUPDIR)/tcpmechod.c
		cp tcpechod.c $(BACKUPDIR)/tcpechod.c
		cp getprotoname.c $(BACKUPDIR)/getprotoname.c
		cp udptimed.c $(BACKUPDIR)/udptimed.c
		cp tcpecho.c $(BACKUPDIR)/tcpecho.c
		cp myping.c $(BACKUPDIR)/myping.c
		cp gethostname.c $(BACKUPDIR)/gethostname.c
		cp udptime.c $(BACKUPDIR)/udptime.c
		cp daytimed.c $(BACKUPDIR)/daytimed.c
		cp tcpdaytimed.c $(BACKUPDIR)/tcpdaytimed.c
		cp tcpdaytime.c $(BACKUPDIR)/tcpdaytime.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mysockets.h ./$(BACKUPDIR)/mysockets.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup

backupd:		getservname.c \
		udpdaytime.c \
		udpecho.c \
		mysockets.c \
		superd.c \
		tcpmechod.c \
		tcpechod.c \
		getprotoname.c \
		udptimed.c \
		tcpecho.c \
		myping.c \
		gethostname.c \
		udptime.c \
		daytimed.c \
		tcpdaytimed.c \
		tcpdaytime.c \
		./debug.h \
		./mysockets.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		getservname.c \
		udpdaytime.c \
		udpecho.c \
		mysockets.c \
		superd.c \
		tcpmechod.c \
		tcpechod.c \
		getprotoname.c \
		udptimed.c \
		tcpecho.c \
		myping.c \
		gethostname.c \
		udptime.c \
		daytimed.c \
		tcpdaytimed.c \
		tcpdaytime.c \
		./debug.h \
		./mysockets.h \
		./returns.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			getservname.c \
			udpdaytime.c \
			udpecho.c \
			mysockets.c \
			superd.c \
			tcpmechod.c \
			tcpechod.c \
			getprotoname.c \
			udptimed.c \
			tcpecho.c \
			myping.c \
			gethostname.c \
			udptime.c \
			daytimed.c \
			tcpdaytimed.c \
			tcpdaytime.c \
			./debug.h \
			./mysockets.h \
			./returns.h \
			$(ENDLIST)
