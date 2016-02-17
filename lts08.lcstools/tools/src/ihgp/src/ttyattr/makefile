
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DVAX
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DVAX
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

all:		 \
		ttyattr \
		$(ENDLIST)
		touch all

:		$(ENDLIST)
		$(LD) $(LDFLAGS) -o  \
			$(ENDLIST)

ttyattr:		ttyattr.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ttyattr \
			ttyattr.o \
			$(ENDLIST)
		cp ttyattr $(HOME)/bin/ttyattr

ttyattr.o:	ttyattr.c \
		$(ENDLIST)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f ttyattr
		-rm -f ttyattr.o

backup:		ttyattr.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp ttyattr.c $(BACKUPDIR)/ttyattr.c
		touch backup

print:		ttyattr.c \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			ttyattr.c \
			$(ENDLIST)
