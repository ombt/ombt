
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
INCLDIRS = -I/opt/x11r5/include
CC = CC
AR = ar
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		butt \
		$(ENDLIST)
		touch all

butt:		butt.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o butt \
			butt.o \
			-lX11 \
			$(ENDLIST)

butt.o:	butt.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f butt
		-rm -f butt.o

backup:		butt.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp butt.c $(BACKUPDIR)/butt.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup
