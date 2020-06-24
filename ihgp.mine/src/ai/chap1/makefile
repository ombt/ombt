
.SUFFIXES:		.o .c

LEXFLAGS = -n
YACC = yacc
YACCFLAGS = -d -v
LEX = lex
MAKE = make
LDFLAGS = -g
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG
INCLDIRS = 
CC = CC
AR = ar
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		sears \
		$(ENDLIST)
		touch all

sears:		sears.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o sears \
			sears.o \
			$(ENDLIST)

sears.o:	sears.c \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f sears
		-rm -f sears.o

backup:		sears.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp sears.c $(BACKUPDIR)/sears.c
		touch backup
