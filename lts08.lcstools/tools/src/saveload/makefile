
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
SHELLBIN = $(HOME)/tools/bin/suidtools
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

all:		copytl1 \
		configure \
		hugcopytl1 \
		bbbscripts \
		saveload \
		removeload \
		$(ENDLIST)
		touch all

copytl1:		copytl1.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/copytl1
		-cp copytl1.sh copytl1
		chmod +x copytl1
		cp copytl1 $(SHELLBIN)/copytl1
		-(rm $(HOME)/tools/bin/lcscopytl1)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcscopytl1

configure:		configure.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/configure
		-cp configure.sh configure
		chmod +x configure
		cp configure $(SHELLBIN)/configure
		-(rm $(HOME)/tools/bin/lcsconfigure)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsconfigure

hugcopytl1:		hugcopytl1.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/hugcopytl1
		-cp hugcopytl1.sh hugcopytl1
		chmod +x hugcopytl1
		cp hugcopytl1 $(SHELLBIN)/hugcopytl1
		-(rm $(HOME)/tools/bin/lcshugcopytl1)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcshugcopytl1

bbbscripts:		bbbscripts.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/bbbscripts
		-cp bbbscripts.sh bbbscripts
		chmod +x bbbscripts
		cp bbbscripts $(SHELLBIN)/bbbscripts
		-(rm $(HOME)/tools/bin/lcsbbbscripts)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsbbbscripts

saveload:		saveload.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/saveload
		-cp saveload.sh saveload
		chmod +x saveload
		cp saveload $(SHELLBIN)/saveload
		-(rm $(HOME)/tools/bin/lcssaveload)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcssaveload

removeload:		removeload.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/removeload
		-cp removeload.sh removeload
		chmod +x removeload
		cp removeload $(SHELLBIN)/removeload
		-(rm $(HOME)/tools/bin/lcsremoveload)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsremoveload

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f copytl1
		-rm -f configure
		-rm -f hugcopytl1
		-rm -f bbbscripts
		-rm -f saveload
		-rm -f removeload

backup:		hugcopytl1.sh \
		bbbscripts.sh \
		copytl1.sh \
		removeload.sh \
		configure.sh \
		saveload.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp hugcopytl1.sh $(BACKUPDIR)/hugcopytl1.sh
		cp bbbscripts.sh $(BACKUPDIR)/bbbscripts.sh
		cp copytl1.sh $(BACKUPDIR)/copytl1.sh
		cp removeload.sh $(BACKUPDIR)/removeload.sh
		cp configure.sh $(BACKUPDIR)/configure.sh
		cp saveload.sh $(BACKUPDIR)/saveload.sh
		touch backup

backupd:		hugcopytl1.sh \
		bbbscripts.sh \
		copytl1.sh \
		removeload.sh \
		configure.sh \
		saveload.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		hugcopytl1.sh \
		bbbscripts.sh \
		copytl1.sh \
		removeload.sh \
		configure.sh \
		saveload.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			hugcopytl1.sh \
			bbbscripts.sh \
			copytl1.sh \
			removeload.sh \
			configure.sh \
			saveload.sh \
			$(ENDLIST)
