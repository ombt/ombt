
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -pta -xildoff -compat=4
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -pta -xildoff -compat=4
INCLDIRS = 
CC = CC
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

all:		tcoins2 \
		tcoins3 \
		tmisscann \
		teight2 \
		teight3 \
		teight \
		tqueen2 \
		tqueen3 \
		tcoins \
		tqueen \
		tmisscann2 \
		tmisscann3 \
		$(ENDLIST)
		touch all

tcoins2:		tcoins2.o \
		coins.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tcoins2 \
			tcoins2.o \
			coins.o \
			list.o \
			search.o \
			$(ENDLIST)

tcoins3:		tcoins3.o \
		coins.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tcoins3 \
			tcoins3.o \
			coins.o \
			list.o \
			search.o \
			$(ENDLIST)

tmisscann:		tmisscann.o \
		misscann.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tmisscann \
			tmisscann.o \
			misscann.o \
			list.o \
			search.o \
			$(ENDLIST)

teight2:		teight2.o \
		eight.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o teight2 \
			teight2.o \
			eight.o \
			list.o \
			search.o \
			$(ENDLIST)

teight3:		teight3.o \
		eight.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o teight3 \
			teight3.o \
			eight.o \
			list.o \
			search.o \
			$(ENDLIST)

teight:		teight.o \
		eight.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o teight \
			teight.o \
			eight.o \
			list.o \
			search.o \
			$(ENDLIST)

tqueen2:		tqueen2.o \
		queen.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tqueen2 \
			tqueen2.o \
			queen.o \
			list.o \
			search.o \
			$(ENDLIST)

tqueen3:		tqueen3.o \
		queen.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tqueen3 \
			tqueen3.o \
			queen.o \
			list.o \
			search.o \
			$(ENDLIST)

tcoins:		tcoins.o \
		coins.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tcoins \
			tcoins.o \
			coins.o \
			list.o \
			search.o \
			$(ENDLIST)

tqueen:		tqueen.o \
		queen.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tqueen \
			tqueen.o \
			queen.o \
			list.o \
			search.o \
			$(ENDLIST)

tmisscann2:		tmisscann2.o \
		misscann.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tmisscann2 \
			tmisscann2.o \
			misscann.o \
			list.o \
			search.o \
			$(ENDLIST)

tmisscann3:		tmisscann3.o \
		misscann.o \
		list.o \
		search.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tmisscann3 \
			tmisscann3.o \
			misscann.o \
			list.o \
			search.o \
			$(ENDLIST)

teight2.o:	teight2.c \
		./eight.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

tmisscann.o:	tmisscann.c \
		./misscann.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

tqueen.o:	tqueen.c \
		./queen.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

queen.o:	queen.c \
		./queen.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		$(ENDLIST)

tcoins2.o:	tcoins2.c \
		./coins.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

tqueen2.o:	tqueen2.c \
		./queen.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

teight3.o:	teight3.c \
		./eight.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

tcoins.o:	tcoins.c \
		./coins.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

coins.o:	coins.c \
		./coins.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		$(ENDLIST)

tcoins3.o:	tcoins3.c \
		./coins.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

tqueen3.o:	tqueen3.c \
		./queen.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

tmisscann2.o:	tmisscann2.c \
		./misscann.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

list.o:	list.c \
		./list.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tmisscann3.o:	tmisscann3.c \
		./misscann.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

misscann.o:	misscann.c \
		./misscann.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		$(ENDLIST)

teight.o:	teight.c \
		./eight.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		./search.h \
		$(ENDLIST)

search.o:	search.c \
		./search.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		$(ENDLIST)

eight.o:	eight.c \
		./eight.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f tcoins2
		-rm -f tcoins3
		-rm -f tmisscann
		-rm -f teight2
		-rm -f teight3
		-rm -f teight
		-rm -f tqueen2
		-rm -f tqueen3
		-rm -f tcoins
		-rm -f tqueen
		-rm -f tmisscann2
		-rm -f tmisscann3
		-rm -f teight2.o
		-rm -f tmisscann.o
		-rm -f tqueen.o
		-rm -f queen.o
		-rm -f tcoins2.o
		-rm -f tqueen2.o
		-rm -f teight3.o
		-rm -f tcoins.o
		-rm -f coins.o
		-rm -f tcoins3.o
		-rm -f tqueen3.o
		-rm -f tmisscann2.o
		-rm -f list.o
		-rm -f tmisscann3.o
		-rm -f misscann.o
		-rm -f teight.o
		-rm -f search.o
		-rm -f eight.o

backup:		teight2.c \
		tmisscann.c \
		tqueen.c \
		queen.c \
		tcoins2.c \
		tqueen2.c \
		teight3.c \
		tcoins.c \
		coins.c \
		tcoins3.c \
		tqueen3.c \
		tmisscann2.c \
		list.c \
		tmisscann3.c \
		misscann.c \
		teight.c \
		search.c \
		eight.c \
		./misscann.h \
		./search.h \
		./debug.h \
		./eight.h \
		./absIterator.h \
		./queen.h \
		./coins.h \
		./nodeptr.h \
		./returns.h \
		./list.h \
		./refcount.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp teight2.c $(BACKUPDIR)/teight2.c
		cp tmisscann.c $(BACKUPDIR)/tmisscann.c
		cp tqueen.c $(BACKUPDIR)/tqueen.c
		cp queen.c $(BACKUPDIR)/queen.c
		cp tcoins2.c $(BACKUPDIR)/tcoins2.c
		cp tqueen2.c $(BACKUPDIR)/tqueen2.c
		cp teight3.c $(BACKUPDIR)/teight3.c
		cp tcoins.c $(BACKUPDIR)/tcoins.c
		cp coins.c $(BACKUPDIR)/coins.c
		cp tcoins3.c $(BACKUPDIR)/tcoins3.c
		cp tqueen3.c $(BACKUPDIR)/tqueen3.c
		cp tmisscann2.c $(BACKUPDIR)/tmisscann2.c
		cp list.c $(BACKUPDIR)/list.c
		cp tmisscann3.c $(BACKUPDIR)/tmisscann3.c
		cp misscann.c $(BACKUPDIR)/misscann.c
		cp teight.c $(BACKUPDIR)/teight.c
		cp search.c $(BACKUPDIR)/search.c
		cp eight.c $(BACKUPDIR)/eight.c
		cp ./misscann.h ./$(BACKUPDIR)/misscann.h
		cp ./search.h ./$(BACKUPDIR)/search.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./eight.h ./$(BACKUPDIR)/eight.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./queen.h ./$(BACKUPDIR)/queen.h
		cp ./coins.h ./$(BACKUPDIR)/coins.h
		cp ./nodeptr.h ./$(BACKUPDIR)/nodeptr.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./list.h ./$(BACKUPDIR)/list.h
		cp ./refcount.h ./$(BACKUPDIR)/refcount.h
		touch backup

print:		teight2.c \
		tmisscann.c \
		tqueen.c \
		queen.c \
		tcoins2.c \
		tqueen2.c \
		teight3.c \
		tcoins.c \
		coins.c \
		tcoins3.c \
		tqueen3.c \
		tmisscann2.c \
		list.c \
		tmisscann3.c \
		misscann.c \
		teight.c \
		search.c \
		eight.c \
		./misscann.h \
		./search.h \
		./debug.h \
		./eight.h \
		./absIterator.h \
		./queen.h \
		./coins.h \
		./nodeptr.h \
		./returns.h \
		./list.h \
		./refcount.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			teight2.c \
			tmisscann.c \
			tqueen.c \
			queen.c \
			tcoins2.c \
			tqueen2.c \
			teight3.c \
			tcoins.c \
			coins.c \
			tcoins3.c \
			tqueen3.c \
			tmisscann2.c \
			list.c \
			tmisscann3.c \
			misscann.c \
			teight.c \
			search.c \
			eight.c \
			./misscann.h \
			./search.h \
			./debug.h \
			./eight.h \
			./absIterator.h \
			./queen.h \
			./coins.h \
			./nodeptr.h \
			./returns.h \
			./list.h \
			./refcount.h \
			$(ENDLIST)
