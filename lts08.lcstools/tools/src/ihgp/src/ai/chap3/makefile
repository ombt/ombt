
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
CCFLAGS = -g
INCLDIRS = 
CC = CC
AR = ar
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		stable \
		$(ENDLIST)
		touch all

stable:		stable.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o stable \
			stable.o \
			$(ENDLIST)

stable.o:	stable.c \
		./debug.h \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f stable
		-rm -f stable.o

backup:		stable.c \
		./debug.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp stable.c $(BACKUPDIR)/stable.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		touch backup
