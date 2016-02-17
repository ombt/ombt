
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

all:		lynx-a \
		lynx-b \
		init-sys10 \
		lcslabloads \
		init-sys30 \
		nrtrv-eqpt-oper \
		tl1-a \
		tl1-b \
		rtrv-eqpt-oper \
		$(ENDLIST)
		touch all

lynx-a:		lynx-a.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lynx-a
		-cp lynx-a.sh lynx-a
		chmod +x lynx-a
		cp lynx-a $(SHELLBIN)/lynx-a

lynx-b:		lynx-b.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lynx-b
		-cp lynx-b.sh lynx-b
		chmod +x lynx-b
		cp lynx-b $(SHELLBIN)/lynx-b

init-sys10:		init-sys10.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/init-sys10
		-cp init-sys10.sh init-sys10
		chmod +x init-sys10
		cp init-sys10 $(SHELLBIN)/init-sys10

lcslabloads:		lcslabloads.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcslabloads
		-cp lcslabloads.sh lcslabloads
		chmod +x lcslabloads
		cp lcslabloads $(SHELLBIN)/lcslabloads

init-sys30:		init-sys30.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/init-sys30
		-cp init-sys30.sh init-sys30
		chmod +x init-sys30
		cp init-sys30 $(SHELLBIN)/init-sys30

nrtrv-eqpt-oper:		nrtrv-eqpt-oper.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nrtrv-eqpt-oper
		-cp nrtrv-eqpt-oper.sh nrtrv-eqpt-oper
		chmod +x nrtrv-eqpt-oper
		cp nrtrv-eqpt-oper $(SHELLBIN)/nrtrv-eqpt-oper
		-(rm $(HOME)/tools/bin/nrtrv-eqpt)
		-(rm $(HOME)/tools/bin/nrtrv-eqpt-all)
		-(rm $(HOME)/tools/bin/nrtrv-alm-all)
		-(rm $(HOME)/tools/bin/nrtrv-chassis-eqpt)
		-(rm $(HOME)/tools/bin/nrtrv-info-eqpt)
		ln $(HOME)/tools/bin/nrtrv-eqpt-oper $(HOME)/tools/bin/nrtrv-eqpt
		ln $(HOME)/tools/bin/nrtrv-eqpt-oper $(HOME)/tools/bin/nrtrv-eqpt-all
		ln $(HOME)/tools/bin/nrtrv-eqpt-oper $(HOME)/tools/bin/nrtrv-alm-all
		ln $(HOME)/tools/bin/nrtrv-eqpt-oper $(HOME)/tools/bin/nrtrv-chassis-eqpt
		ln $(HOME)/tools/bin/nrtrv-eqpt-oper $(HOME)/tools/bin/nrtrv-info-eqpt
		-(rm $(HOME)/tools/bin/nrtrv-enet)
		ln $(HOME)/tools/bin/nrtrv-eqpt-oper $(HOME)/tools/bin/nrtrv-enet

tl1-a:		tl1-a.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tl1-a
		-cp tl1-a.sh tl1-a
		chmod +x tl1-a
		cp tl1-a $(SHELLBIN)/tl1-a

tl1-b:		tl1-b.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tl1-b
		-cp tl1-b.sh tl1-b
		chmod +x tl1-b
		cp tl1-b $(SHELLBIN)/tl1-b

rtrv-eqpt-oper:		rtrv-eqpt-oper.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/rtrv-eqpt-oper
		-cp rtrv-eqpt-oper.sh rtrv-eqpt-oper
		chmod +x rtrv-eqpt-oper
		cp rtrv-eqpt-oper $(SHELLBIN)/rtrv-eqpt-oper
		-(rm $(HOME)/tools/bin/rtrv-eqpt)
		-(rm $(HOME)/tools/bin/rtrv-eqpt-all)
		-(rm $(HOME)/tools/bin/rtrv-alm-all)
		-(rm $(HOME)/tools/bin/rtrv-chassis-eqpt)
		-(rm $(HOME)/tools/bin/rtrv-info-eqpt)
		ln $(HOME)/tools/bin/rtrv-eqpt-oper $(HOME)/tools/bin/rtrv-eqpt
		ln $(HOME)/tools/bin/rtrv-eqpt-oper $(HOME)/tools/bin/rtrv-eqpt-all
		ln $(HOME)/tools/bin/rtrv-eqpt-oper $(HOME)/tools/bin/rtrv-alm-all
		ln $(HOME)/tools/bin/rtrv-eqpt-oper $(HOME)/tools/bin/rtrv-chassis-eqpt
		ln $(HOME)/tools/bin/rtrv-eqpt-oper $(HOME)/tools/bin/rtrv-info-eqpt
		-(rm $(HOME)/tools/bin/rtrv-enet)
		ln $(HOME)/tools/bin/rtrv-eqpt-oper $(HOME)/tools/bin/rtrv-enet

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f lynx-a
		-rm -f lynx-b
		-rm -f init-sys10
		-rm -f lcslabloads
		-rm -f init-sys30
		-rm -f nrtrv-eqpt-oper
		-rm -f tl1-a
		-rm -f tl1-b
		-rm -f rtrv-eqpt-oper

backup:		init-sys30.sh \
		lynx-b.sh \
		init-sys10.sh \
		lcslabloads.sh \
		lynx-a.sh \
		tl1-b.sh \
		rtrv-eqpt-oper.sh \
		tl1-a.sh \
		nrtrv-eqpt-oper.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp init-sys30.sh $(BACKUPDIR)/init-sys30.sh
		cp lynx-b.sh $(BACKUPDIR)/lynx-b.sh
		cp init-sys10.sh $(BACKUPDIR)/init-sys10.sh
		cp lcslabloads.sh $(BACKUPDIR)/lcslabloads.sh
		cp lynx-a.sh $(BACKUPDIR)/lynx-a.sh
		cp tl1-b.sh $(BACKUPDIR)/tl1-b.sh
		cp rtrv-eqpt-oper.sh $(BACKUPDIR)/rtrv-eqpt-oper.sh
		cp tl1-a.sh $(BACKUPDIR)/tl1-a.sh
		cp nrtrv-eqpt-oper.sh $(BACKUPDIR)/nrtrv-eqpt-oper.sh
		touch backup

backupd:		init-sys30.sh \
		lynx-b.sh \
		init-sys10.sh \
		lcslabloads.sh \
		lynx-a.sh \
		tl1-b.sh \
		rtrv-eqpt-oper.sh \
		tl1-a.sh \
		nrtrv-eqpt-oper.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		init-sys30.sh \
		lynx-b.sh \
		init-sys10.sh \
		lcslabloads.sh \
		lynx-a.sh \
		tl1-b.sh \
		rtrv-eqpt-oper.sh \
		tl1-a.sh \
		nrtrv-eqpt-oper.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			init-sys30.sh \
			lynx-b.sh \
			init-sys10.sh \
			lcslabloads.sh \
			lynx-a.sh \
			tl1-b.sh \
			rtrv-eqpt-oper.sh \
			tl1-a.sh \
			nrtrv-eqpt-oper.sh \
			$(ENDLIST)
