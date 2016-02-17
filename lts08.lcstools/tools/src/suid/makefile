
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g
LD = gcc
TCLBIN = $(HOME)/bin
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/tools/bin
ARFLAGS = -r -v
CCFLAGS = -g
INCLDIRS = 
CC = gcc
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

all:		suidcmd \
		suid \
		$(ENDLIST)
		touch all

suidcmd:		suidcmd.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/suidcmd
		-cp suidcmd.sh suidcmd
		chmod +x suidcmd
		cp suidcmd $(SHELLBIN)/suidcmd
		-(rm -rf $(HOME)/tools/bin/suidtools)
		-(mkdir -p $(HOME)/tools/bin/suidtools)

suid:		suid.o \
		mydebug.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o suid \
			suid.o \
			mydebug.o \
			$(ENDLIST)
		-(rm $(HOME)/tools/bin/suid)
		cp suid $(HOME)/tools/bin/suid
		chmod 6755 $(HOME)/tools/bin/suid

suid.o:	suid.c \
		./mydebug.h \
		$(ENDLIST)

mydebug.o:	mydebug.c \
		./mydebug.h \
		$(ENDLIST)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f suidcmd
		-rm -f suid
		-rm -f suid.o
		-rm -f mydebug.o

backup:		suid.c \
		suidcmd.sh \
		mydebug.c \
		./mydebug.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp suid.c $(BACKUPDIR)/suid.c
		cp suidcmd.sh $(BACKUPDIR)/suidcmd.sh
		cp mydebug.c $(BACKUPDIR)/mydebug.c
		cp ./mydebug.h ./$(BACKUPDIR)/mydebug.h
		touch backup

backupd:		suid.c \
		suidcmd.sh \
		mydebug.c \
		./mydebug.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		suid.c \
		suidcmd.sh \
		mydebug.c \
		./mydebug.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			suid.c \
			suidcmd.sh \
			mydebug.c \
			./mydebug.h \
			$(ENDLIST)
