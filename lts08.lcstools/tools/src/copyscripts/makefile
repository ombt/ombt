
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

all:		copyscripts \
		auditlabloads \
		mkcurrdirs \
		auditimages \
		audittext \
		auditscripts \
		$(ENDLIST)
		touch all

copyscripts:		copyscripts.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/copyscripts
		-cp copyscripts.sh copyscripts
		chmod +x copyscripts
		cp copyscripts $(SHELLBIN)/copyscripts

auditlabloads:		auditlabloads.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/auditlabloads
		-cp auditlabloads.sh auditlabloads
		chmod +x auditlabloads
		cp auditlabloads $(SHELLBIN)/auditlabloads

mkcurrdirs:		mkcurrdirs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mkcurrdirs
		-cp mkcurrdirs.sh mkcurrdirs
		chmod +x mkcurrdirs
		cp mkcurrdirs $(SHELLBIN)/mkcurrdirs

auditimages:		auditimages.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/auditimages
		-cp auditimages.sh auditimages
		chmod +x auditimages
		cp auditimages $(SHELLBIN)/auditimages

audittext:		audittext.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/audittext
		-cp audittext.sh audittext
		chmod +x audittext
		cp audittext $(SHELLBIN)/audittext

auditscripts:		auditscripts.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/auditscripts
		-cp auditscripts.sh auditscripts
		chmod +x auditscripts
		cp auditscripts $(SHELLBIN)/auditscripts

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f copyscripts
		-rm -f auditlabloads
		-rm -f mkcurrdirs
		-rm -f auditimages
		-rm -f audittext
		-rm -f auditscripts

backup:		mkcurrdirs.sh \
		auditimages.sh \
		auditscripts.sh \
		copyscripts.sh \
		audittext.sh \
		auditlabloads.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mkcurrdirs.sh $(BACKUPDIR)/mkcurrdirs.sh
		cp auditimages.sh $(BACKUPDIR)/auditimages.sh
		cp auditscripts.sh $(BACKUPDIR)/auditscripts.sh
		cp copyscripts.sh $(BACKUPDIR)/copyscripts.sh
		cp audittext.sh $(BACKUPDIR)/audittext.sh
		cp auditlabloads.sh $(BACKUPDIR)/auditlabloads.sh
		touch backup

backupd:		mkcurrdirs.sh \
		auditimages.sh \
		auditscripts.sh \
		copyscripts.sh \
		audittext.sh \
		auditlabloads.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		mkcurrdirs.sh \
		auditimages.sh \
		auditscripts.sh \
		copyscripts.sh \
		audittext.sh \
		auditlabloads.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mkcurrdirs.sh \
			auditimages.sh \
			auditscripts.sh \
			copyscripts.sh \
			audittext.sh \
			auditlabloads.sh \
			$(ENDLIST)
