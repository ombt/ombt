
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g
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

all:		mypc \
		$(ENDLIST)
		touch all

mypc:		gettoken.o \
		main.o \
		parser.o \
		symtab.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o mypc \
			gettoken.o \
			main.o \
			parser.o \
			symtab.o \
			$(ENDLIST)

gettoken.o:	gettoken.c \
		./gettoken.h \
		$(ENDLIST)

main.o:	main.c \
		./parser.h \
		./gettoken.h \
		./symtab.h \
		$(ENDLIST)

symtab.o:	symtab.c \
		./parser.h \
		./gettoken.h \
		./symtab.h \
		$(ENDLIST)

parser.o:	parser.c \
		./gettoken.h \
		./parser.h \
		./symtab.h \
		$(ENDLIST)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f mypc
		-rm -f gettoken.o
		-rm -f main.o
		-rm -f symtab.o
		-rm -f parser.o

backup:		gettoken.c \
		main.c \
		symtab.c \
		parser.c \
		./gettoken.h \
		./symtab.h \
		./parser.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp gettoken.c $(BACKUPDIR)/gettoken.c
		cp main.c $(BACKUPDIR)/main.c
		cp symtab.c $(BACKUPDIR)/symtab.c
		cp parser.c $(BACKUPDIR)/parser.c
		cp ./gettoken.h ./$(BACKUPDIR)/gettoken.h
		cp ./symtab.h ./$(BACKUPDIR)/symtab.h
		cp ./parser.h ./$(BACKUPDIR)/parser.h
		touch backup

print:		gettoken.c \
		main.c \
		symtab.c \
		parser.c \
		./gettoken.h \
		./symtab.h \
		./parser.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			gettoken.c \
			main.c \
			symtab.c \
			parser.c \
			./gettoken.h \
			./symtab.h \
			./parser.h \
			$(ENDLIST)
