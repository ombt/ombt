
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

all:		dbquery \
		odbquery \
		$(ENDLIST)
		touch all

dbquery:		dbquery.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/dbquery
		-cp dbquery.sh dbquery
		chmod +x dbquery
		cp dbquery $(SHELLBIN)/dbquery

odbquery:		odbquery.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/odbquery
		-cp odbquery.sh odbquery
		chmod +x odbquery
		cp odbquery $(SHELLBIN)/odbquery

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f dbquery
		-rm -f odbquery

backup:		odbquery.sh \
		dbquery.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp odbquery.sh $(BACKUPDIR)/odbquery.sh
		cp dbquery.sh $(BACKUPDIR)/dbquery.sh
		touch backup

backupd:		odbquery.sh \
		dbquery.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		odbquery.sh \
		dbquery.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			odbquery.sh \
			dbquery.sh \
			$(ENDLIST)
