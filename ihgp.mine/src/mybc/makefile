
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

all:		mybc \
		$(ENDLIST)
		touch all

mybc:		mybcyacc.o \
		mybclex.o \
		mycomplex.o \
		epsilon.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o mybc \
			mybcyacc.o \
			mybclex.o \
			mycomplex.o \
			epsilon.o \
			-ll \
			-ly \
			-lm \
			$(ENDLIST)
		chmod 755 mybc
		cp mybc /home/rumore/bin/mybc
		rcp mybc integ@lts00:/home/integ/bin/MYBC
		rcp mybc integ@ihlbb2:/home/integ/bin/MYBC

mybcyacc.o:	mybc.y \
		./mybc.h \
		./epsilon.h \
		./mycomplex.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		$(YACC) $(YACCFLAGS) mybc.y
		mv y.tab.h mybcyacc.h
		mv y.tab.c mybcyacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c mybcyacc.c

mycomplex.o:	mycomplex.c \
		./mycomplex.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

epsilon.o:	epsilon.c \
		./epsilon.h \
		$(ENDLIST)

mybclex.o:	mybc.l \
		./mybc.h \
		./epsilon.h \
		./mycomplex.h \
		./debug.h \
		./returns.h \
		mybc.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) mybc.l
		mv lex.yy.c mybclex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c mybclex.c

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f mybc
		-rm -f mybcyacc.o
		-rm -f mycomplex.o
		-rm -f epsilon.o
		-rm -f mybclex.o

backup:		mybc.y \
		mycomplex.c \
		epsilon.c \
		mybc.l \
		./debug.h \
		./mycomplex.h \
		./returns.h \
		./mybc.h \
		./mybcyacc.h \
		./epsilon.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mybc.y $(BACKUPDIR)/mybc.y
		cp mycomplex.c $(BACKUPDIR)/mycomplex.c
		cp epsilon.c $(BACKUPDIR)/epsilon.c
		cp mybc.l $(BACKUPDIR)/mybc.l
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mycomplex.h ./$(BACKUPDIR)/mycomplex.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./mybc.h ./$(BACKUPDIR)/mybc.h
		cp ./mybcyacc.h ./$(BACKUPDIR)/mybcyacc.h
		cp ./epsilon.h ./$(BACKUPDIR)/epsilon.h
		touch backup

print:		mybc.y \
		mycomplex.c \
		epsilon.c \
		mybc.l \
		./debug.h \
		./mycomplex.h \
		./returns.h \
		./mybc.h \
		./mybcyacc.h \
		./epsilon.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mybc.y \
			mycomplex.c \
			epsilon.c \
			mybc.l \
			./debug.h \
			./mycomplex.h \
			./returns.h \
			./mybc.h \
			./mybcyacc.h \
			./epsilon.h \
			$(ENDLIST)
