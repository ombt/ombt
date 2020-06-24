
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g
INCLDIRS = 
CC = CC
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

all:		lan \
		$(ENDLIST)
		touch all

lan:		lan.o \
		cable.o \
		call.o \
		exponential.o \
		item.o \
		poisson.o \
		probDist.o \
		station.o \
		uniform.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o lan \
			lan.o \
			cable.o \
			call.o \
			exponential.o \
			item.o \
			poisson.o \
			probDist.o \
			station.o \
			uniform.o \
			-lm \
			$(ENDLIST)
		cp lan $(HOME)/bin/lan

station.o:	station.c \
		./debug.h \
		./returns.h \
		./station.h \
		./cable.h \
		./event.h \
		./item.h \
		./poisson.h \
		./probDist.h \
		./exponential.h \
		./uniform.h \
		$(ENDLIST)

item.o:	item.c \
		./item.h \
		./returns.h \
		$(ENDLIST)

probDist.o:	probDist.c \
		./debug.h \
		./probDist.h \
		./returns.h \
		$(ENDLIST)

cable.o:	cable.c \
		./cable.h \
		./event.h \
		./item.h \
		./returns.h \
		./poisson.h \
		./probDist.h \
		./debug.h \
		$(ENDLIST)

call.o:	call.c \
		./call.h \
		./event.h \
		./item.h \
		./returns.h \
		./station.h \
		./cable.h \
		./poisson.h \
		./probDist.h \
		./exponential.h \
		./uniform.h \
		./debug.h \
		$(ENDLIST)

poisson.o:	poisson.c \
		./debug.h \
		./poisson.h \
		./probDist.h \
		./returns.h \
		$(ENDLIST)

lan.o:	lan.c \
		./cable.h \
		./event.h \
		./item.h \
		./returns.h \
		./poisson.h \
		./probDist.h \
		./call.h \
		./station.h \
		./exponential.h \
		./uniform.h \
		./debug.h \
		$(ENDLIST)

uniform.o:	uniform.c \
		./debug.h \
		./uniform.h \
		./probDist.h \
		./returns.h \
		$(ENDLIST)

exponential.o:	exponential.c \
		./debug.h \
		./exponential.h \
		./probDist.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f lan
		-rm -f station.o
		-rm -f item.o
		-rm -f probDist.o
		-rm -f cable.o
		-rm -f call.o
		-rm -f poisson.o
		-rm -f lan.o
		-rm -f uniform.o
		-rm -f exponential.o

backup:		station.c \
		item.c \
		probDist.c \
		cable.c \
		call.c \
		poisson.c \
		lan.c \
		uniform.c \
		exponential.c \
		./debug.h \
		./station.h \
		./item.h \
		./event.h \
		./probDist.h \
		./cable.h \
		./call.h \
		./returns.h \
		./poisson.h \
		./uniform.h \
		./exponential.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp station.c $(BACKUPDIR)/station.c
		cp item.c $(BACKUPDIR)/item.c
		cp probDist.c $(BACKUPDIR)/probDist.c
		cp cable.c $(BACKUPDIR)/cable.c
		cp call.c $(BACKUPDIR)/call.c
		cp poisson.c $(BACKUPDIR)/poisson.c
		cp lan.c $(BACKUPDIR)/lan.c
		cp uniform.c $(BACKUPDIR)/uniform.c
		cp exponential.c $(BACKUPDIR)/exponential.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./station.h ./$(BACKUPDIR)/station.h
		cp ./item.h ./$(BACKUPDIR)/item.h
		cp ./event.h ./$(BACKUPDIR)/event.h
		cp ./probDist.h ./$(BACKUPDIR)/probDist.h
		cp ./cable.h ./$(BACKUPDIR)/cable.h
		cp ./call.h ./$(BACKUPDIR)/call.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./poisson.h ./$(BACKUPDIR)/poisson.h
		cp ./uniform.h ./$(BACKUPDIR)/uniform.h
		cp ./exponential.h ./$(BACKUPDIR)/exponential.h
		touch backup

print:		station.c \
		item.c \
		probDist.c \
		cable.c \
		call.c \
		poisson.c \
		lan.c \
		uniform.c \
		exponential.c \
		./debug.h \
		./station.h \
		./item.h \
		./event.h \
		./probDist.h \
		./cable.h \
		./call.h \
		./returns.h \
		./poisson.h \
		./uniform.h \
		./exponential.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			station.c \
			item.c \
			probDist.c \
			cable.c \
			call.c \
			poisson.c \
			lan.c \
			uniform.c \
			exponential.c \
			./debug.h \
			./station.h \
			./item.h \
			./event.h \
			./probDist.h \
			./cable.h \
			./call.h \
			./returns.h \
			./poisson.h \
			./uniform.h \
			./exponential.h \
			$(ENDLIST)
