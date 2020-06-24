
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG -DUSEEXCEPTION
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG -DUSEEXCEPTION
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

all:		rle \
		$(ENDLIST)
		touch all

rle:		rle.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o rle \
			rle.o \
			$(ENDLIST)

rle.o:	rle.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f rle
		-rm -f rle.o

backup:		rle.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp rle.c $(BACKUPDIR)/rle.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup

print:		rle.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			rle.c \
			./debug.h \
			./returns.h \
			$(ENDLIST)
