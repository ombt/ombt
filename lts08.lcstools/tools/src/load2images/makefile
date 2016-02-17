
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

all:		load2images \
		$(ENDLIST)
		touch all

load2images:		load2images.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/load2images
		-cp load2images.sh load2images
		chmod +x load2images
		cp load2images $(SHELLBIN)/load2images

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f load2images

backup:		load2images.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp load2images.sh $(BACKUPDIR)/load2images.sh
		touch backup

backupd:		load2images.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		load2images.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			load2images.sh \
			$(ENDLIST)
