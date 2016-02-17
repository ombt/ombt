
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

all:		ocelf58 \
		$(ENDLIST)
		touch all

ocelf58:		ocelf.o \
		compare.o \
		readdata.o \
		swabdata.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ocelf58 \
			ocelf.o \
			compare.o \
			readdata.o \
			swabdata.o \
			-ldemangle \
			$(ENDLIST)
		chmod 755 ocelf58
		rm -f /home/rumore/bin/ocelf58
		cp ocelf58 /home/rumore/bin/ocelf58

swabdata.o:	swabdata.c \
		./ocelf.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

ocelf.o:	ocelf.c \
		./ocelf.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

compare.o:	compare.c \
		./ocelf.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

readdata.o:	readdata.c \
		./ocelf.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f ocelf58
		-rm -f swabdata.o
		-rm -f ocelf.o
		-rm -f compare.o
		-rm -f readdata.o

backup:		swabdata.c \
		ocelf.c \
		compare.c \
		readdata.c \
		./debug.h \
		./ocelf.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp swabdata.c $(BACKUPDIR)/swabdata.c
		cp ocelf.c $(BACKUPDIR)/ocelf.c
		cp compare.c $(BACKUPDIR)/compare.c
		cp readdata.c $(BACKUPDIR)/readdata.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./ocelf.h ./$(BACKUPDIR)/ocelf.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup

print:		swabdata.c \
		ocelf.c \
		compare.c \
		readdata.c \
		./debug.h \
		./ocelf.h \
		./returns.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			swabdata.c \
			ocelf.c \
			compare.c \
			readdata.c \
			./debug.h \
			./ocelf.h \
			./returns.h \
			$(ENDLIST)
