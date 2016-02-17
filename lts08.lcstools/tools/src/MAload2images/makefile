
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = 
TCLBIN = $(HOME)/bin
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/tools/bin
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

all:		MAload2images \
		$(ENDLIST)
		touch all

MAload2images:		MAload2images.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/MAload2images
		-cp MAload2images.sh MAload2images
		chmod +x MAload2images
		cp MAload2images $(SHELLBIN)/MAload2images

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f MAload2images

backup:		MAload2images.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp MAload2images.sh $(BACKUPDIR)/MAload2images.sh
		touch backup

backupd:		MAload2images.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		MAload2images.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			MAload2images.sh \
			$(ENDLIST)
