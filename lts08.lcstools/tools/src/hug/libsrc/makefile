
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = 
TCLBIN = $(LCSHUGLIB)
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

all:		hwmaint.tcl \
		ncleis.tcl \
		nhwmaint.tcl \
		flashparts.tcl \
		scripts.tcl \
		checkenv.tcl \
		hughardware.tcl \
		hugftp.tcl \
		chooseone.tcl \
		lock.tcl \
		cleis.tcl \
		iomips.tcl \
		dequeue.tcl \
		hugtelnet.tcl \
		iomfps.tcl \
		checkretval.tcl \
		getoptval.tcl \
		local.tcl \
		hugtl1.tcl \
		hugmisc.tcl \
		oscripts.tcl \
		cfgdata.tcl \
		db.tcl \
		$(ENDLIST)
		touch all

hwmaint.tcl:		hwmaint.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/hwmaint.tcl
		-cp hwmaint.TCL hwmaint.tcl
		chmod +x hwmaint.tcl
		cp hwmaint.tcl $(TCLBIN)/hwmaint.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

ncleis.tcl:		ncleis.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/ncleis.tcl
		-cp ncleis.TCL ncleis.tcl
		chmod +x ncleis.tcl
		cp ncleis.tcl $(TCLBIN)/ncleis.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

nhwmaint.tcl:		nhwmaint.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/nhwmaint.tcl
		-cp nhwmaint.TCL nhwmaint.tcl
		chmod +x nhwmaint.tcl
		cp nhwmaint.tcl $(TCLBIN)/nhwmaint.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

flashparts.tcl:		flashparts.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/flashparts.tcl
		-cp flashparts.TCL flashparts.tcl
		chmod +x flashparts.tcl
		cp flashparts.tcl $(TCLBIN)/flashparts.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

scripts.tcl:		scripts.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/scripts.tcl
		-cp scripts.TCL scripts.tcl
		chmod +x scripts.tcl
		cp scripts.tcl $(TCLBIN)/scripts.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

checkenv.tcl:		checkenv.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/checkenv.tcl
		-cp checkenv.TCL checkenv.tcl
		chmod +x checkenv.tcl
		cp checkenv.tcl $(TCLBIN)/checkenv.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

hughardware.tcl:		hughardware.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/hughardware.tcl
		-cp hughardware.TCL hughardware.tcl
		chmod +x hughardware.tcl
		cp hughardware.tcl $(TCLBIN)/hughardware.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

hugftp.tcl:		hugftp.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/hugftp.tcl
		-cp hugftp.TCL hugftp.tcl
		chmod +x hugftp.tcl
		cp hugftp.tcl $(TCLBIN)/hugftp.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

chooseone.tcl:		chooseone.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/chooseone.tcl
		-cp chooseone.TCL chooseone.tcl
		chmod +x chooseone.tcl
		cp chooseone.tcl $(TCLBIN)/chooseone.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

lock.tcl:		lock.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/lock.tcl
		-cp lock.TCL lock.tcl
		chmod +x lock.tcl
		cp lock.tcl $(TCLBIN)/lock.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

cleis.tcl:		cleis.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/cleis.tcl
		-cp cleis.TCL cleis.tcl
		chmod +x cleis.tcl
		cp cleis.tcl $(TCLBIN)/cleis.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

iomips.tcl:		iomips.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/iomips.tcl
		-cp iomips.TCL iomips.tcl
		chmod +x iomips.tcl
		cp iomips.tcl $(TCLBIN)/iomips.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

dequeue.tcl:		dequeue.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/dequeue.tcl
		-cp dequeue.TCL dequeue.tcl
		chmod +x dequeue.tcl
		cp dequeue.tcl $(TCLBIN)/dequeue.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

hugtelnet.tcl:		hugtelnet.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/hugtelnet.tcl
		-cp hugtelnet.TCL hugtelnet.tcl
		chmod +x hugtelnet.tcl
		cp hugtelnet.tcl $(TCLBIN)/hugtelnet.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

iomfps.tcl:		iomfps.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/iomfps.tcl
		-cp iomfps.TCL iomfps.tcl
		chmod +x iomfps.tcl
		cp iomfps.tcl $(TCLBIN)/iomfps.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

checkretval.tcl:		checkretval.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/checkretval.tcl
		-cp checkretval.TCL checkretval.tcl
		chmod +x checkretval.tcl
		cp checkretval.tcl $(TCLBIN)/checkretval.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

getoptval.tcl:		getoptval.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/getoptval.tcl
		-cp getoptval.TCL getoptval.tcl
		chmod +x getoptval.tcl
		cp getoptval.tcl $(TCLBIN)/getoptval.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

local.tcl:		local.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/local.tcl
		-cp local.TCL local.tcl
		chmod +x local.tcl
		cp local.tcl $(TCLBIN)/local.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

hugtl1.tcl:		hugtl1.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/hugtl1.tcl
		-cp hugtl1.TCL hugtl1.tcl
		chmod +x hugtl1.tcl
		cp hugtl1.tcl $(TCLBIN)/hugtl1.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

hugmisc.tcl:		hugmisc.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/hugmisc.tcl
		-cp hugmisc.TCL hugmisc.tcl
		chmod +x hugmisc.tcl
		cp hugmisc.tcl $(TCLBIN)/hugmisc.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

oscripts.tcl:		oscripts.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/oscripts.tcl
		-cp oscripts.TCL oscripts.tcl
		chmod +x oscripts.tcl
		cp oscripts.tcl $(TCLBIN)/oscripts.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

cfgdata.tcl:		cfgdata.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/cfgdata.tcl
		-cp cfgdata.TCL cfgdata.tcl
		chmod +x cfgdata.tcl
		cp cfgdata.tcl $(TCLBIN)/cfgdata.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

db.tcl:		db.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/db.tcl
		-cp db.TCL db.tcl
		chmod +x db.tcl
		cp db.tcl $(TCLBIN)/db.tcl
		pkg_mkindex $(LCSHUGLIB) '*.tcl'

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f hwmaint.tcl
		-rm -f ncleis.tcl
		-rm -f nhwmaint.tcl
		-rm -f flashparts.tcl
		-rm -f scripts.tcl
		-rm -f checkenv.tcl
		-rm -f hughardware.tcl
		-rm -f hugftp.tcl
		-rm -f chooseone.tcl
		-rm -f lock.tcl
		-rm -f cleis.tcl
		-rm -f iomips.tcl
		-rm -f dequeue.tcl
		-rm -f hugtelnet.tcl
		-rm -f iomfps.tcl
		-rm -f checkretval.tcl
		-rm -f getoptval.tcl
		-rm -f local.tcl
		-rm -f hugtl1.tcl
		-rm -f hugmisc.tcl
		-rm -f oscripts.tcl
		-rm -f cfgdata.tcl
		-rm -f db.tcl

backup:		iomips.TCL \
		dequeue.TCL \
		hugtelnet.TCL \
		iomfps.TCL \
		checkretval.TCL \
		getoptval.TCL \
		local.TCL \
		hugtl1.TCL \
		hugmisc.TCL \
		oscripts.TCL \
		cfgdata.TCL \
		db.TCL \
		hwmaint.TCL \
		ncleis.TCL \
		nhwmaint.TCL \
		flashparts.TCL \
		scripts.TCL \
		checkenv.TCL \
		hughardware.TCL \
		hugftp.TCL \
		chooseone.TCL \
		lock.TCL \
		cleis.TCL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp iomips.TCL $(BACKUPDIR)/iomips.TCL
		cp dequeue.TCL $(BACKUPDIR)/dequeue.TCL
		cp hugtelnet.TCL $(BACKUPDIR)/hugtelnet.TCL
		cp iomfps.TCL $(BACKUPDIR)/iomfps.TCL
		cp checkretval.TCL $(BACKUPDIR)/checkretval.TCL
		cp getoptval.TCL $(BACKUPDIR)/getoptval.TCL
		cp local.TCL $(BACKUPDIR)/local.TCL
		cp hugtl1.TCL $(BACKUPDIR)/hugtl1.TCL
		cp hugmisc.TCL $(BACKUPDIR)/hugmisc.TCL
		cp oscripts.TCL $(BACKUPDIR)/oscripts.TCL
		cp cfgdata.TCL $(BACKUPDIR)/cfgdata.TCL
		cp db.TCL $(BACKUPDIR)/db.TCL
		cp hwmaint.TCL $(BACKUPDIR)/hwmaint.TCL
		cp ncleis.TCL $(BACKUPDIR)/ncleis.TCL
		cp nhwmaint.TCL $(BACKUPDIR)/nhwmaint.TCL
		cp flashparts.TCL $(BACKUPDIR)/flashparts.TCL
		cp scripts.TCL $(BACKUPDIR)/scripts.TCL
		cp checkenv.TCL $(BACKUPDIR)/checkenv.TCL
		cp hughardware.TCL $(BACKUPDIR)/hughardware.TCL
		cp hugftp.TCL $(BACKUPDIR)/hugftp.TCL
		cp chooseone.TCL $(BACKUPDIR)/chooseone.TCL
		cp lock.TCL $(BACKUPDIR)/lock.TCL
		cp cleis.TCL $(BACKUPDIR)/cleis.TCL
		touch backup

backupd:		iomips.TCL \
		dequeue.TCL \
		hugtelnet.TCL \
		iomfps.TCL \
		checkretval.TCL \
		getoptval.TCL \
		local.TCL \
		hugtl1.TCL \
		hugmisc.TCL \
		oscripts.TCL \
		cfgdata.TCL \
		db.TCL \
		hwmaint.TCL \
		ncleis.TCL \
		nhwmaint.TCL \
		flashparts.TCL \
		scripts.TCL \
		checkenv.TCL \
		hughardware.TCL \
		hugftp.TCL \
		chooseone.TCL \
		lock.TCL \
		cleis.TCL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		iomips.TCL \
		dequeue.TCL \
		hugtelnet.TCL \
		iomfps.TCL \
		checkretval.TCL \
		getoptval.TCL \
		local.TCL \
		hugtl1.TCL \
		hugmisc.TCL \
		oscripts.TCL \
		cfgdata.TCL \
		db.TCL \
		hwmaint.TCL \
		ncleis.TCL \
		nhwmaint.TCL \
		flashparts.TCL \
		scripts.TCL \
		checkenv.TCL \
		hughardware.TCL \
		hugftp.TCL \
		chooseone.TCL \
		lock.TCL \
		cleis.TCL \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			iomips.TCL \
			dequeue.TCL \
			hugtelnet.TCL \
			iomfps.TCL \
			checkretval.TCL \
			getoptval.TCL \
			local.TCL \
			hugtl1.TCL \
			hugmisc.TCL \
			oscripts.TCL \
			cfgdata.TCL \
			db.TCL \
			hwmaint.TCL \
			ncleis.TCL \
			nhwmaint.TCL \
			flashparts.TCL \
			scripts.TCL \
			checkenv.TCL \
			hughardware.TCL \
			hugftp.TCL \
			chooseone.TCL \
			lock.TCL \
			cleis.TCL \
			$(ENDLIST)
