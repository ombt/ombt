
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

all:		lcslabs \
		auditloads \
		lcsfiles \
		lcsdu \
		lcslabioms \
		lcsstat \
		lcsloads \
		lcsdf \
		lcssendemail \
		nlcsstat \
		$(ENDLIST)
		touch all

lcslabs:		lcslabs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcslabs
		-cp lcslabs.sh lcslabs
		chmod +x lcslabs
		cp lcslabs $(SHELLBIN)/lcslabs

auditloads:		auditloads.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/auditloads
		-cp auditloads.sh auditloads
		chmod +x auditloads
		cp auditloads $(SHELLBIN)/auditloads

lcsfiles:		lcsfiles.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcsfiles
		-cp lcsfiles.sh lcsfiles
		chmod +x lcsfiles
		cp lcsfiles $(SHELLBIN)/lcsfiles

lcsdu:		lcsdu.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcsdu
		-cp lcsdu.sh lcsdu
		chmod +x lcsdu
		cp lcsdu $(SHELLBIN)/lcsdu

lcslabioms:		lcslabioms.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcslabioms
		-cp lcslabioms.sh lcslabioms
		chmod +x lcslabioms
		cp lcslabioms $(SHELLBIN)/lcslabioms

lcsstat:		lcsstat.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcsstat
		-cp lcsstat.sh lcsstat
		chmod +x lcsstat
		cp lcsstat $(SHELLBIN)/lcsstat

lcsloads:		lcsloads.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcsloads
		-cp lcsloads.sh lcsloads
		chmod +x lcsloads
		cp lcsloads $(SHELLBIN)/lcsloads

lcsdf:		lcsdf.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcsdf
		-cp lcsdf.sh lcsdf
		chmod +x lcsdf
		cp lcsdf $(SHELLBIN)/lcsdf

lcssendemail:		lcssendemail.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcssendemail
		-cp lcssendemail.sh lcssendemail
		chmod +x lcssendemail
		cp lcssendemail $(SHELLBIN)/lcssendemail

nlcsstat:		nlcsstat.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nlcsstat
		-cp nlcsstat.sh nlcsstat
		chmod +x nlcsstat
		cp nlcsstat $(SHELLBIN)/nlcsstat

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f lcslabs
		-rm -f auditloads
		-rm -f lcsfiles
		-rm -f lcsdu
		-rm -f lcslabioms
		-rm -f lcsstat
		-rm -f lcsloads
		-rm -f lcsdf
		-rm -f lcssendemail
		-rm -f nlcsstat

backup:		lcslabioms.sh \
		nlcsstat.sh \
		lcssendemail.sh \
		lcsloads.sh \
		lcsdf.sh \
		lcslabs.sh \
		auditloads.sh \
		lcsfiles.sh \
		lcsstat.sh \
		lcsdu.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp lcslabioms.sh $(BACKUPDIR)/lcslabioms.sh
		cp nlcsstat.sh $(BACKUPDIR)/nlcsstat.sh
		cp lcssendemail.sh $(BACKUPDIR)/lcssendemail.sh
		cp lcsloads.sh $(BACKUPDIR)/lcsloads.sh
		cp lcsdf.sh $(BACKUPDIR)/lcsdf.sh
		cp lcslabs.sh $(BACKUPDIR)/lcslabs.sh
		cp auditloads.sh $(BACKUPDIR)/auditloads.sh
		cp lcsfiles.sh $(BACKUPDIR)/lcsfiles.sh
		cp lcsstat.sh $(BACKUPDIR)/lcsstat.sh
		cp lcsdu.sh $(BACKUPDIR)/lcsdu.sh
		touch backup

backupd:		lcslabioms.sh \
		nlcsstat.sh \
		lcssendemail.sh \
		lcsloads.sh \
		lcsdf.sh \
		lcslabs.sh \
		auditloads.sh \
		lcsfiles.sh \
		lcsstat.sh \
		lcsdu.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		lcslabioms.sh \
		nlcsstat.sh \
		lcssendemail.sh \
		lcsloads.sh \
		lcsdf.sh \
		lcslabs.sh \
		auditloads.sh \
		lcsfiles.sh \
		lcsstat.sh \
		lcsdu.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			lcslabioms.sh \
			nlcsstat.sh \
			lcssendemail.sh \
			lcsloads.sh \
			lcsdf.sh \
			lcslabs.sh \
			auditloads.sh \
			lcsfiles.sh \
			lcsstat.sh \
			lcsdu.sh \
			$(ENDLIST)
