
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
HUGSCRIPTS = $(HOME)/tools/src/hug/scripts
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

all:		lf \
		removedefaultgw \
		iomsfui \
		iprecover \
		mysfui \
		mount_nfs \
		postlcsreload \
		findcpucores \
		remotepostlcsreload \
		disablediskcleanup \
		$(ENDLIST)
		touch all

lf:		lf.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lf
		-cp lf.sh lf
		chmod +x lf
		cp lf $(SHELLBIN)/lf
		chmod 644 lf
		chmod 644 $(SHELLBIN)/lf
		cp lf $(HUGSCRIPTS)/lf

removedefaultgw:		removedefaultgw.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/removedefaultgw
		-cp removedefaultgw.sh removedefaultgw
		chmod +x removedefaultgw
		cp removedefaultgw $(SHELLBIN)/removedefaultgw
		cp removedefaultgw $(HUGSCRIPTS)/removedefaultgw

iomsfui:		iomsfui.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/iomsfui
		-cp iomsfui.sh iomsfui
		chmod +x iomsfui
		cp iomsfui $(SHELLBIN)/iomsfui

iprecover:		iprecover.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/iprecover
		-cp iprecover.sh iprecover
		chmod +x iprecover
		cp iprecover $(SHELLBIN)/iprecover
		cp iprecover $(HUGSCRIPTS)/iprecover

mysfui:		mysfui.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mysfui
		-cp mysfui.sh mysfui
		chmod +x mysfui
		cp mysfui $(SHELLBIN)/mysfui
		chmod 644 mysfui
		chmod 644 $(SHELLBIN)/mysfui
		cp mysfui $(HUGSCRIPTS)/mysfui

mount_nfs:		mount_nfs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mount_nfs
		-cp mount_nfs.sh mount_nfs
		chmod +x mount_nfs
		cp mount_nfs $(SHELLBIN)/mount_nfs

postlcsreload:		postlcsreload.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/postlcsreload
		-cp postlcsreload.sh postlcsreload
		chmod +x postlcsreload
		cp postlcsreload $(SHELLBIN)/postlcsreload

findcpucores:		findcpucores.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/findcpucores
		-cp findcpucores.sh findcpucores
		chmod +x findcpucores
		cp findcpucores $(SHELLBIN)/findcpucores
		cp findcpucores $(HUGSCRIPTS)/findcpucores

remotepostlcsreload:		remotepostlcsreload.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/remotepostlcsreload
		-cp remotepostlcsreload.sh remotepostlcsreload
		chmod +x remotepostlcsreload
		cp remotepostlcsreload $(SHELLBIN)/remotepostlcsreload
		cp remotepostlcsreload $(HUGSCRIPTS)/postreload

disablediskcleanup:		disablediskcleanup.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/disablediskcleanup
		-cp disablediskcleanup.sh disablediskcleanup
		chmod +x disablediskcleanup
		cp disablediskcleanup $(SHELLBIN)/disablediskcleanup
		cp disablediskcleanup $(HUGSCRIPTS)/disablediskcleanup

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f lf
		-rm -f removedefaultgw
		-rm -f iomsfui
		-rm -f iprecover
		-rm -f mysfui
		-rm -f mount_nfs
		-rm -f postlcsreload
		-rm -f findcpucores
		-rm -f remotepostlcsreload
		-rm -f disablediskcleanup

backup:		postlcsreload.sh \
		remotepostlcsreload.sh \
		iomsfui.sh \
		lf.sh \
		mysfui.sh \
		findcpucores.sh \
		removedefaultgw.sh \
		iprecover.sh \
		mount_nfs.sh \
		disablediskcleanup.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp postlcsreload.sh $(BACKUPDIR)/postlcsreload.sh
		cp remotepostlcsreload.sh $(BACKUPDIR)/remotepostlcsreload.sh
		cp iomsfui.sh $(BACKUPDIR)/iomsfui.sh
		cp lf.sh $(BACKUPDIR)/lf.sh
		cp mysfui.sh $(BACKUPDIR)/mysfui.sh
		cp findcpucores.sh $(BACKUPDIR)/findcpucores.sh
		cp removedefaultgw.sh $(BACKUPDIR)/removedefaultgw.sh
		cp iprecover.sh $(BACKUPDIR)/iprecover.sh
		cp mount_nfs.sh $(BACKUPDIR)/mount_nfs.sh
		cp disablediskcleanup.sh $(BACKUPDIR)/disablediskcleanup.sh
		touch backup

backupd:		postlcsreload.sh \
		remotepostlcsreload.sh \
		iomsfui.sh \
		lf.sh \
		mysfui.sh \
		findcpucores.sh \
		removedefaultgw.sh \
		iprecover.sh \
		mount_nfs.sh \
		disablediskcleanup.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		postlcsreload.sh \
		remotepostlcsreload.sh \
		iomsfui.sh \
		lf.sh \
		mysfui.sh \
		findcpucores.sh \
		removedefaultgw.sh \
		iprecover.sh \
		mount_nfs.sh \
		disablediskcleanup.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			postlcsreload.sh \
			remotepostlcsreload.sh \
			iomsfui.sh \
			lf.sh \
			mysfui.sh \
			findcpucores.sh \
			removedefaultgw.sh \
			iprecover.sh \
			mount_nfs.sh \
			disablediskcleanup.sh \
			$(ENDLIST)
