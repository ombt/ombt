
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DSYSTEM5
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DSYSTEM5
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

all:		dir \
		$(ENDLIST)
		touch all

dir:		dir.o \
		regcmp.o \
		regex.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o dir \
			dir.o \
			regcmp.o \
			regex.o \
			$(ENDLIST)
		chmod 755 dir
		rcp dir eeinteg@ihees1:/home/eeinteg/rumore/bin/dir
		rcp dir integ@ihlbs22/home/integ/bin/dir
		rcp dir lcstools@lts08:/home/lcstools/bin/dir

regex.o:	regex.c \
		$(ENDLIST)

dir.o:	dir.c \
		$(ENDLIST)

regcmp.o:	regcmp.c \
		$(ENDLIST)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f dir
		-rm -f regex.o
		-rm -f dir.o
		-rm -f regcmp.o

backup:		regex.c \
		dir.c \
		regcmp.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp regex.c $(BACKUPDIR)/regex.c
		cp dir.c $(BACKUPDIR)/dir.c
		cp regcmp.c $(BACKUPDIR)/regcmp.c
		touch backup

print:		regex.c \
		dir.c \
		regcmp.c \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			regex.c \
			dir.c \
			regcmp.c \
			$(ENDLIST)
