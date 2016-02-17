
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = 
TCLBIN = $(LCSHUGBIN)
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(LCSHUGBIN)
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

all:		rstioms \
		recoversps \
		hugedit \
		updateioms \
		getsploads \
		cpuhugpp \
		lcsrexec \
		printiomfps \
		ohugpp \
		cmhugpp \
		printflashparts \
		getsploadsonly \
		buppi \
		rstsiglnks \
		tl1 \
		hugmknode \
		upchkcfg \
		lcshug \
		hugtail \
		prehugpp \
		buppx \
		rmvsiglnks \
		cpymems \
		labprep \
		cpu_cmhugpp \
		rtrveqpt \
		sendemail \
		localexec \
		iomhugpp \
		rmvioms \
		intl1 \
		bupp \
		auditsps \
		printiomips \
		upshi \
		uplock \
		lcsbu \
		cleanupsps \
		hugpp \
		upsh \
		nbupp \
		printcleis \
		$(ENDLIST)
		touch all

rstioms:		rstioms.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/rstioms
		-cp rstioms.TCL rstioms
		chmod +x rstioms
		cp rstioms $(TCLBIN)/rstioms

recoversps:		recoversps.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/recoversps
		-cp recoversps.TCL recoversps
		chmod +x recoversps
		cp recoversps $(TCLBIN)/recoversps

hugedit:		hugedit.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/hugedit
		-cp hugedit.sh hugedit
		chmod +x hugedit
		cp hugedit $(SHELLBIN)/hugedit

updateioms:		updateioms.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/updateioms
		-cp updateioms.TCL updateioms
		chmod +x updateioms
		cp updateioms $(TCLBIN)/updateioms

getsploads:		getsploads.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/getsploads
		-cp getsploads.TCL getsploads
		chmod +x getsploads
		cp getsploads $(TCLBIN)/getsploads

cpuhugpp:		cpuhugpp.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/cpuhugpp
		-cp cpuhugpp.TCL cpuhugpp
		chmod +x cpuhugpp
		cp cpuhugpp $(TCLBIN)/cpuhugpp

lcsrexec:		lcsrexec.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/lcsrexec
		-cp lcsrexec.TCL lcsrexec
		chmod +x lcsrexec
		cp lcsrexec $(TCLBIN)/lcsrexec

printiomfps:		printiomfps.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/printiomfps
		-cp printiomfps.TCL printiomfps
		chmod +x printiomfps
		cp printiomfps $(TCLBIN)/printiomfps

ohugpp:		ohugpp.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/ohugpp
		-cp ohugpp.TCL ohugpp
		chmod +x ohugpp
		cp ohugpp $(TCLBIN)/ohugpp

cmhugpp:		cmhugpp.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/cmhugpp
		-cp cmhugpp.TCL cmhugpp
		chmod +x cmhugpp
		cp cmhugpp $(TCLBIN)/cmhugpp

printflashparts:		printflashparts.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/printflashparts
		-cp printflashparts.TCL printflashparts
		chmod +x printflashparts
		cp printflashparts $(TCLBIN)/printflashparts

getsploadsonly:		getsploadsonly.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/getsploadsonly
		-cp getsploadsonly.TCL getsploadsonly
		chmod +x getsploadsonly
		cp getsploadsonly $(TCLBIN)/getsploadsonly

buppi:		buppi.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/buppi
		-cp buppi.sh buppi
		chmod +x buppi
		cp buppi $(SHELLBIN)/buppi

rstsiglnks:		rstsiglnks.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/rstsiglnks
		-cp rstsiglnks.TCL rstsiglnks
		chmod +x rstsiglnks
		cp rstsiglnks $(TCLBIN)/rstsiglnks

tl1:		tl1.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/tl1
		-cp tl1.TCL tl1
		chmod +x tl1
		cp tl1 $(TCLBIN)/tl1

hugmknode:		hugmknode.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/hugmknode
		-cp hugmknode.sh hugmknode
		chmod +x hugmknode
		cp hugmknode $(SHELLBIN)/hugmknode

upchkcfg:		upchkcfg.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/upchkcfg
		-cp upchkcfg.TCL upchkcfg
		chmod +x upchkcfg
		cp upchkcfg $(TCLBIN)/upchkcfg

lcshug:		lcshug.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcshug
		-cp lcshug.sh lcshug
		chmod +x lcshug
		cp lcshug $(SHELLBIN)/lcshug

hugtail:		hugtail.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/hugtail
		-cp hugtail.sh hugtail
		chmod +x hugtail
		cp hugtail $(SHELLBIN)/hugtail

prehugpp:		prehugpp.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/prehugpp
		-cp prehugpp.TCL prehugpp
		chmod +x prehugpp
		cp prehugpp $(TCLBIN)/prehugpp

buppx:		buppx.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/buppx
		-cp buppx.sh buppx
		chmod +x buppx
		cp buppx $(SHELLBIN)/buppx

rmvsiglnks:		rmvsiglnks.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/rmvsiglnks
		-cp rmvsiglnks.TCL rmvsiglnks
		chmod +x rmvsiglnks
		cp rmvsiglnks $(TCLBIN)/rmvsiglnks

cpymems:		cpymems.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/cpymems
		-cp cpymems.TCL cpymems
		chmod +x cpymems
		cp cpymems $(TCLBIN)/cpymems

labprep:		labprep.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/labprep
		-cp labprep.TCL labprep
		chmod +x labprep
		cp labprep $(TCLBIN)/labprep

cpu_cmhugpp:		cpu_cmhugpp.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/cpu_cmhugpp
		-cp cpu_cmhugpp.TCL cpu_cmhugpp
		chmod +x cpu_cmhugpp
		cp cpu_cmhugpp $(TCLBIN)/cpu_cmhugpp

rtrveqpt:		rtrveqpt.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/rtrveqpt
		-cp rtrveqpt.TCL rtrveqpt
		chmod +x rtrveqpt
		cp rtrveqpt $(TCLBIN)/rtrveqpt

sendemail:		sendemail.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/sendemail
		-cp sendemail.sh sendemail
		chmod +x sendemail
		cp sendemail $(SHELLBIN)/sendemail

localexec:		localexec.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/localexec
		-cp localexec.TCL localexec
		chmod +x localexec
		cp localexec $(TCLBIN)/localexec

iomhugpp:		iomhugpp.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/iomhugpp
		-cp iomhugpp.TCL iomhugpp
		chmod +x iomhugpp
		cp iomhugpp $(TCLBIN)/iomhugpp

rmvioms:		rmvioms.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/rmvioms
		-cp rmvioms.TCL rmvioms
		chmod +x rmvioms
		cp rmvioms $(TCLBIN)/rmvioms

intl1:		intl1.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/intl1
		-cp intl1.sh intl1
		chmod +x intl1
		cp intl1 $(SHELLBIN)/intl1

bupp:		bupp.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/bupp
		-cp bupp.sh bupp
		chmod +x bupp
		cp bupp $(SHELLBIN)/bupp

auditsps:		auditsps.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/auditsps
		-cp auditsps.TCL auditsps
		chmod +x auditsps
		cp auditsps $(TCLBIN)/auditsps

printiomips:		printiomips.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/printiomips
		-cp printiomips.TCL printiomips
		chmod +x printiomips
		cp printiomips $(TCLBIN)/printiomips

upshi:		upshi.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/upshi
		-cp upshi.sh upshi
		chmod +x upshi
		cp upshi $(SHELLBIN)/upshi

uplock:		uplock.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/uplock
		-cp uplock.TCL uplock
		chmod +x uplock
		cp uplock $(TCLBIN)/uplock

lcsbu:		lcsbu.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcsbu
		-cp lcsbu.sh lcsbu
		chmod +x lcsbu
		cp lcsbu $(SHELLBIN)/lcsbu

cleanupsps:		cleanupsps.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/cleanupsps
		-cp cleanupsps.TCL cleanupsps
		chmod +x cleanupsps
		cp cleanupsps $(TCLBIN)/cleanupsps

hugpp:		hugpp.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/hugpp
		-cp hugpp.TCL hugpp
		chmod +x hugpp
		cp hugpp $(TCLBIN)/hugpp

upsh:		upsh.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/upsh
		-cp upsh.TCL upsh
		chmod +x upsh
		cp upsh $(TCLBIN)/upsh

nbupp:		nbupp.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nbupp
		-cp nbupp.sh nbupp
		chmod +x nbupp
		cp nbupp $(SHELLBIN)/nbupp

printcleis:		printcleis.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/printcleis
		-cp printcleis.TCL printcleis
		chmod +x printcleis
		cp printcleis $(TCLBIN)/printcleis

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f rstioms
		-rm -f recoversps
		-rm -f hugedit
		-rm -f updateioms
		-rm -f getsploads
		-rm -f cpuhugpp
		-rm -f lcsrexec
		-rm -f printiomfps
		-rm -f ohugpp
		-rm -f cmhugpp
		-rm -f printflashparts
		-rm -f getsploadsonly
		-rm -f buppi
		-rm -f rstsiglnks
		-rm -f tl1
		-rm -f hugmknode
		-rm -f upchkcfg
		-rm -f lcshug
		-rm -f hugtail
		-rm -f prehugpp
		-rm -f buppx
		-rm -f rmvsiglnks
		-rm -f cpymems
		-rm -f labprep
		-rm -f cpu_cmhugpp
		-rm -f rtrveqpt
		-rm -f sendemail
		-rm -f localexec
		-rm -f iomhugpp
		-rm -f rmvioms
		-rm -f intl1
		-rm -f bupp
		-rm -f auditsps
		-rm -f printiomips
		-rm -f upshi
		-rm -f uplock
		-rm -f lcsbu
		-rm -f cleanupsps
		-rm -f hugpp
		-rm -f upsh
		-rm -f nbupp
		-rm -f printcleis

backup:		printflashparts.TCL \
		printcleis.TCL \
		rstioms.TCL \
		rmvioms.TCL \
		getsploadsonly.TCL \
		labprep.TCL \
		lcsrexec.TCL \
		hugmknode.sh \
		recoversps.TCL \
		lcsbu.sh \
		localexec.TCL \
		cleanupsps.TCL \
		cpuhugpp.TCL \
		sendemail.sh \
		buppi.sh \
		bupp.sh \
		lcshug.sh \
		hugtail.sh \
		printiomips.TCL \
		hugpp.TCL \
		upsh.TCL \
		prehugpp.TCL \
		updateioms.TCL \
		hugedit.sh \
		intl1.sh \
		rmvsiglnks.TCL \
		printiomfps.TCL \
		iomhugpp.TCL \
		buppx.sh \
		rtrveqpt.TCL \
		upshi.sh \
		cpymems.TCL \
		nbupp.sh \
		cpu_cmhugpp.TCL \
		rstsiglnks.TCL \
		ohugpp.TCL \
		tl1.TCL \
		cmhugpp.TCL \
		auditsps.TCL \
		uplock.TCL \
		getsploads.TCL \
		upchkcfg.TCL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp printflashparts.TCL $(BACKUPDIR)/printflashparts.TCL
		cp printcleis.TCL $(BACKUPDIR)/printcleis.TCL
		cp rstioms.TCL $(BACKUPDIR)/rstioms.TCL
		cp rmvioms.TCL $(BACKUPDIR)/rmvioms.TCL
		cp getsploadsonly.TCL $(BACKUPDIR)/getsploadsonly.TCL
		cp labprep.TCL $(BACKUPDIR)/labprep.TCL
		cp lcsrexec.TCL $(BACKUPDIR)/lcsrexec.TCL
		cp hugmknode.sh $(BACKUPDIR)/hugmknode.sh
		cp recoversps.TCL $(BACKUPDIR)/recoversps.TCL
		cp lcsbu.sh $(BACKUPDIR)/lcsbu.sh
		cp localexec.TCL $(BACKUPDIR)/localexec.TCL
		cp cleanupsps.TCL $(BACKUPDIR)/cleanupsps.TCL
		cp cpuhugpp.TCL $(BACKUPDIR)/cpuhugpp.TCL
		cp sendemail.sh $(BACKUPDIR)/sendemail.sh
		cp buppi.sh $(BACKUPDIR)/buppi.sh
		cp bupp.sh $(BACKUPDIR)/bupp.sh
		cp lcshug.sh $(BACKUPDIR)/lcshug.sh
		cp hugtail.sh $(BACKUPDIR)/hugtail.sh
		cp printiomips.TCL $(BACKUPDIR)/printiomips.TCL
		cp hugpp.TCL $(BACKUPDIR)/hugpp.TCL
		cp upsh.TCL $(BACKUPDIR)/upsh.TCL
		cp prehugpp.TCL $(BACKUPDIR)/prehugpp.TCL
		cp updateioms.TCL $(BACKUPDIR)/updateioms.TCL
		cp hugedit.sh $(BACKUPDIR)/hugedit.sh
		cp intl1.sh $(BACKUPDIR)/intl1.sh
		cp rmvsiglnks.TCL $(BACKUPDIR)/rmvsiglnks.TCL
		cp printiomfps.TCL $(BACKUPDIR)/printiomfps.TCL
		cp iomhugpp.TCL $(BACKUPDIR)/iomhugpp.TCL
		cp buppx.sh $(BACKUPDIR)/buppx.sh
		cp rtrveqpt.TCL $(BACKUPDIR)/rtrveqpt.TCL
		cp upshi.sh $(BACKUPDIR)/upshi.sh
		cp cpymems.TCL $(BACKUPDIR)/cpymems.TCL
		cp nbupp.sh $(BACKUPDIR)/nbupp.sh
		cp cpu_cmhugpp.TCL $(BACKUPDIR)/cpu_cmhugpp.TCL
		cp rstsiglnks.TCL $(BACKUPDIR)/rstsiglnks.TCL
		cp ohugpp.TCL $(BACKUPDIR)/ohugpp.TCL
		cp tl1.TCL $(BACKUPDIR)/tl1.TCL
		cp cmhugpp.TCL $(BACKUPDIR)/cmhugpp.TCL
		cp auditsps.TCL $(BACKUPDIR)/auditsps.TCL
		cp uplock.TCL $(BACKUPDIR)/uplock.TCL
		cp getsploads.TCL $(BACKUPDIR)/getsploads.TCL
		cp upchkcfg.TCL $(BACKUPDIR)/upchkcfg.TCL
		touch backup

backupd:		printflashparts.TCL \
		printcleis.TCL \
		rstioms.TCL \
		rmvioms.TCL \
		getsploadsonly.TCL \
		labprep.TCL \
		lcsrexec.TCL \
		hugmknode.sh \
		recoversps.TCL \
		lcsbu.sh \
		localexec.TCL \
		cleanupsps.TCL \
		cpuhugpp.TCL \
		sendemail.sh \
		buppi.sh \
		bupp.sh \
		lcshug.sh \
		hugtail.sh \
		printiomips.TCL \
		hugpp.TCL \
		upsh.TCL \
		prehugpp.TCL \
		updateioms.TCL \
		hugedit.sh \
		intl1.sh \
		rmvsiglnks.TCL \
		printiomfps.TCL \
		iomhugpp.TCL \
		buppx.sh \
		rtrveqpt.TCL \
		upshi.sh \
		cpymems.TCL \
		nbupp.sh \
		cpu_cmhugpp.TCL \
		rstsiglnks.TCL \
		ohugpp.TCL \
		tl1.TCL \
		cmhugpp.TCL \
		auditsps.TCL \
		uplock.TCL \
		getsploads.TCL \
		upchkcfg.TCL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		printflashparts.TCL \
		printcleis.TCL \
		rstioms.TCL \
		rmvioms.TCL \
		getsploadsonly.TCL \
		labprep.TCL \
		lcsrexec.TCL \
		hugmknode.sh \
		recoversps.TCL \
		lcsbu.sh \
		localexec.TCL \
		cleanupsps.TCL \
		cpuhugpp.TCL \
		sendemail.sh \
		buppi.sh \
		bupp.sh \
		lcshug.sh \
		hugtail.sh \
		printiomips.TCL \
		hugpp.TCL \
		upsh.TCL \
		prehugpp.TCL \
		updateioms.TCL \
		hugedit.sh \
		intl1.sh \
		rmvsiglnks.TCL \
		printiomfps.TCL \
		iomhugpp.TCL \
		buppx.sh \
		rtrveqpt.TCL \
		upshi.sh \
		cpymems.TCL \
		nbupp.sh \
		cpu_cmhugpp.TCL \
		rstsiglnks.TCL \
		ohugpp.TCL \
		tl1.TCL \
		cmhugpp.TCL \
		auditsps.TCL \
		uplock.TCL \
		getsploads.TCL \
		upchkcfg.TCL \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			printflashparts.TCL \
			printcleis.TCL \
			rstioms.TCL \
			rmvioms.TCL \
			getsploadsonly.TCL \
			labprep.TCL \
			lcsrexec.TCL \
			hugmknode.sh \
			recoversps.TCL \
			lcsbu.sh \
			localexec.TCL \
			cleanupsps.TCL \
			cpuhugpp.TCL \
			sendemail.sh \
			buppi.sh \
			bupp.sh \
			lcshug.sh \
			hugtail.sh \
			printiomips.TCL \
			hugpp.TCL \
			upsh.TCL \
			prehugpp.TCL \
			updateioms.TCL \
			hugedit.sh \
			intl1.sh \
			rmvsiglnks.TCL \
			printiomfps.TCL \
			iomhugpp.TCL \
			buppx.sh \
			rtrveqpt.TCL \
			upshi.sh \
			cpymems.TCL \
			nbupp.sh \
			cpu_cmhugpp.TCL \
			rstsiglnks.TCL \
			ohugpp.TCL \
			tl1.TCL \
			cmhugpp.TCL \
			auditsps.TCL \
			uplock.TCL \
			getsploads.TCL \
			upchkcfg.TCL \
			$(ENDLIST)
