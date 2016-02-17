
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG
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

all:		 \
		compdist \
		$(ENDLIST)
		touch all

:		$(ENDLIST)
		$(LD) $(LDFLAGS) -o  \
			$(ENDLIST)

compdist:		compdist.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o compdist \
			compdist.o \
			$(ENDLIST)

compdist.o:	compdist.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f compdist
		-rm -f compdist.o

backup:		compdist.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp compdist.c $(BACKUPDIR)/compdist.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup

print:		compdist.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			compdist.c \
			./debug.h \
			./returns.h \
			$(ENDLIST)
