
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g
INCLDIRS = 
CC = cc
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

all:		user \
		dbms \
		$(ENDLIST)
		touch all

user:		user.o \
		dbmslib.o \
		fifomsgs.o \
		syserr.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o user \
			user.o \
			dbmslib.o \
			fifomsgs.o \
			syserr.o \
			$(ENDLIST)

dbms:		dbmsmain.o \
		dbms.o \
		fifomsgs.o \
		syserr.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o dbms \
			dbmsmain.o \
			dbms.o \
			fifomsgs.o \
			syserr.o \
			$(ENDLIST)

user.o:	user.c \
		./dbms.h \
		$(ENDLIST)

syserr.o:	syserr.c \
		$(ENDLIST)

dbms.o:	dbms.c \
		./dbms.h \
		$(ENDLIST)

dbmslib.o:	dbmslib.c \
		./dbms.h \
		$(ENDLIST)

dbmsmain.o:	dbmsmain.c \
		./dbms.h \
		$(ENDLIST)

fifomsgs.o:	fifomsgs.c \
		./fifomsgs.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f user
		-rm -f dbms
		-rm -f user.o
		-rm -f syserr.o
		-rm -f dbms.o
		-rm -f dbmslib.o
		-rm -f dbmsmain.o
		-rm -f fifomsgs.o

backup:		user.c \
		syserr.c \
		dbms.c \
		dbmslib.c \
		dbmsmain.c \
		fifomsgs.c \
		./dbms.h \
		./fifomsgs.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp user.c $(BACKUPDIR)/user.c
		cp syserr.c $(BACKUPDIR)/syserr.c
		cp dbms.c $(BACKUPDIR)/dbms.c
		cp dbmslib.c $(BACKUPDIR)/dbmslib.c
		cp dbmsmain.c $(BACKUPDIR)/dbmsmain.c
		cp fifomsgs.c $(BACKUPDIR)/fifomsgs.c
		cp ./dbms.h ./$(BACKUPDIR)/dbms.h
		cp ./fifomsgs.h ./$(BACKUPDIR)/fifomsgs.h
		touch backup

print:		user.c \
		syserr.c \
		dbms.c \
		dbmslib.c \
		dbmsmain.c \
		fifomsgs.c \
		./dbms.h \
		./fifomsgs.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			user.c \
			syserr.c \
			dbms.c \
			dbmslib.c \
			dbmsmain.c \
			fifomsgs.c \
			./dbms.h \
			./fifomsgs.h \
			$(ENDLIST)
