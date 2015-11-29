
.SUFFIXES:		.o .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -DDEBUG -g -xildoff
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -DDEBUG -g -xildoff
INCLDIRS = 
CC = CC
LPRFLAGS = 
AR = /usr/ccs/bin/ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.cpp.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		atppp \
		$(ENDLIST)
		touch all

atppp:		pp.o \
		mylist.o \
		mystring.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o atppp \
			pp.o \
			mylist.o \
			mystring.o \
			$(ENDLIST)
		cp atppp $(HOME)/bin/atppp

mystring.o:	mystring.c \
		./mystring.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

mylist.o:	mylist.c \
		./mylist.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

pp.o:	pp.c \
		./debug.h \
		./mylist.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./tuple.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f atppp
		-rm -f mystring.o
		-rm -f mylist.o
		-rm -f pp.o

backup:		mystring.c \
		mylist.c \
		pp.c \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./mylist.h \
		./tuple.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mystring.c $(BACKUPDIR)/mystring.c
		cp mylist.c $(BACKUPDIR)/mylist.c
		cp pp.c $(BACKUPDIR)/pp.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mystring.h ./$(BACKUPDIR)/mystring.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./mylist.h ./$(BACKUPDIR)/mylist.h
		cp ./tuple.h ./$(BACKUPDIR)/tuple.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup

print:		mystring.c \
		mylist.c \
		pp.c \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./mylist.h \
		./tuple.h \
		./returns.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mystring.c \
			mylist.c \
			pp.c \
			./debug.h \
			./mystring.h \
			./absIterator.h \
			./mylist.h \
			./tuple.h \
			./returns.h \
			$(ENDLIST)
