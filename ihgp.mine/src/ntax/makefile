
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG
LD = cc
BACKUPDIR = BACKUP
LDS = CC
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG
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

all:		interest \
		ttax \
		ntax \
		usury \
		interest2 \
		$(ENDLIST)
		touch all

interest:		interest.o \
		db.o \
		money.o \
		number.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o interest \
			interest.o \
			db.o \
			money.o \
			number.o \
			-lm \
			-liostream \
			$(ENDLIST)

ttax:		ttax.o \
		1040.o \
		federal.o \
		il.o \
		fica.o \
		db.o \
		taxTable.o \
		scheduleA.o \
		money.o \
		number.o \
		realEstate.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ttax \
			ttax.o \
			1040.o \
			federal.o \
			il.o \
			fica.o \
			db.o \
			taxTable.o \
			scheduleA.o \
			money.o \
			number.o \
			realEstate.o \
			-lm \
			$(ENDLIST)

ntax:		ntax.o \
		1040.o \
		federal.o \
		il.o \
		db.o \
		taxTable.o \
		scheduleA.o \
		money.o \
		number.o \
		realEstate.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ntax \
			ntax.o \
			1040.o \
			federal.o \
			il.o \
			db.o \
			taxTable.o \
			scheduleA.o \
			money.o \
			number.o \
			realEstate.o \
			-lm \
			$(ENDLIST)

usury:		usury.o \
		db.o \
		money.o \
		number.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o usury \
			usury.o \
			db.o \
			money.o \
			number.o \
			-lm \
			$(ENDLIST)

interest2:		interest2.o \
		db.o \
		money.o \
		number.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o interest2 \
			interest2.o \
			db.o \
			money.o \
			number.o \
			-lm \
			$(ENDLIST)

db.o:	db.c \
		./db.h \
		./boolean.h \
		./debug.h \
		./number.h \
		./returns.h \
		$(ENDLIST)

interest.o:	interest.c \
		./money.h \
		./boolean.h \
		./db.h \
		./debug.h \
		./number.h \
		./returns.h \
		$(ENDLIST)

fica.o:	fica.c \
		./debug.h \
		./fica.h \
		./returns.h \
		./taxTable.h \
		./1040.h \
		./boolean.h \
		./money.h \
		./db.h \
		./number.h \
		$(ENDLIST)

taxTable.o:	taxTable.c \
		./debug.h \
		./returns.h \
		./taxTable.h \
		./1040.h \
		./boolean.h \
		./money.h \
		./db.h \
		./number.h \
		$(ENDLIST)

il.o:	il.c \
		./debug.h \
		./il.h \
		./returns.h \
		./taxTable.h \
		./1040.h \
		./boolean.h \
		./money.h \
		./db.h \
		./number.h \
		$(ENDLIST)

ntax.o:	ntax.c \
		./1040.h \
		./boolean.h \
		./debug.h \
		./money.h \
		./db.h \
		./number.h \
		./returns.h \
		$(ENDLIST)

federal.o:	federal.c \
		./debug.h \
		./federal.h \
		./returns.h \
		./taxTable.h \
		./1040.h \
		./boolean.h \
		./money.h \
		./db.h \
		./number.h \
		$(ENDLIST)

ttax.o:	ttax.c \
		./federal.h \
		./debug.h \
		./returns.h \
		./taxTable.h \
		./1040.h \
		./boolean.h \
		./money.h \
		./db.h \
		./number.h \
		./fica.h \
		./il.h \
		./realEstate.h \
		$(ENDLIST)

usury.o:	usury.c \
		./money.h \
		./boolean.h \
		./db.h \
		./debug.h \
		./number.h \
		./returns.h \
		$(ENDLIST)

interest2.o:	interest2.c \
		./money.h \
		./boolean.h \
		./db.h \
		./debug.h \
		./number.h \
		./returns.h \
		$(ENDLIST)

scheduleA.o:	scheduleA.c \
		./debug.h \
		./il.h \
		./returns.h \
		./taxTable.h \
		./1040.h \
		./boolean.h \
		./money.h \
		./db.h \
		./number.h \
		./realEstate.h \
		./scheduleA.h \
		$(ENDLIST)

money.o:	money.c \
		./money.h \
		./boolean.h \
		./db.h \
		./debug.h \
		./number.h \
		./returns.h \
		$(ENDLIST)

number.o:	number.c \
		./debug.h \
		./number.h \
		$(ENDLIST)

1040.o:	1040.c \
		./1040.h \
		./boolean.h \
		./debug.h \
		./money.h \
		./db.h \
		./number.h \
		./returns.h \
		./federal.h \
		./taxTable.h \
		./scheduleA.h \
		./realEstate.h \
		$(ENDLIST)

realEstate.o:	realEstate.c \
		./realEstate.h \
		./debug.h \
		./money.h \
		./boolean.h \
		./db.h \
		./number.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f interest
		-rm -f ttax
		-rm -f ntax
		-rm -f usury
		-rm -f interest2
		-rm -f db.o
		-rm -f interest.o
		-rm -f fica.o
		-rm -f taxTable.o
		-rm -f il.o
		-rm -f ntax.o
		-rm -f federal.o
		-rm -f ttax.o
		-rm -f usury.o
		-rm -f interest2.o
		-rm -f scheduleA.o
		-rm -f money.o
		-rm -f number.o
		-rm -f 1040.o
		-rm -f realEstate.o

backup:		db.c \
		interest.c \
		fica.c \
		taxTable.c \
		il.c \
		ntax.c \
		federal.c \
		ttax.c \
		usury.c \
		interest2.c \
		scheduleA.c \
		money.c \
		number.c \
		1040.c \
		realEstate.c \
		./realEstate.h \
		./debug.h \
		./boolean.h \
		./db.h \
		./fica.h \
		./taxTable.h \
		./il.h \
		./federal.h \
		./returns.h \
		./scheduleA.h \
		./money.h \
		./number.h \
		./1040.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp db.c $(BACKUPDIR)/db.c
		cp interest.c $(BACKUPDIR)/interest.c
		cp fica.c $(BACKUPDIR)/fica.c
		cp taxTable.c $(BACKUPDIR)/taxTable.c
		cp il.c $(BACKUPDIR)/il.c
		cp ntax.c $(BACKUPDIR)/ntax.c
		cp federal.c $(BACKUPDIR)/federal.c
		cp ttax.c $(BACKUPDIR)/ttax.c
		cp usury.c $(BACKUPDIR)/usury.c
		cp interest2.c $(BACKUPDIR)/interest2.c
		cp scheduleA.c $(BACKUPDIR)/scheduleA.c
		cp money.c $(BACKUPDIR)/money.c
		cp number.c $(BACKUPDIR)/number.c
		cp 1040.c $(BACKUPDIR)/1040.c
		cp realEstate.c $(BACKUPDIR)/realEstate.c
		cp ./realEstate.h ./$(BACKUPDIR)/realEstate.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./boolean.h ./$(BACKUPDIR)/boolean.h
		cp ./db.h ./$(BACKUPDIR)/db.h
		cp ./fica.h ./$(BACKUPDIR)/fica.h
		cp ./taxTable.h ./$(BACKUPDIR)/taxTable.h
		cp ./il.h ./$(BACKUPDIR)/il.h
		cp ./federal.h ./$(BACKUPDIR)/federal.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./scheduleA.h ./$(BACKUPDIR)/scheduleA.h
		cp ./money.h ./$(BACKUPDIR)/money.h
		cp ./number.h ./$(BACKUPDIR)/number.h
		cp ./1040.h ./$(BACKUPDIR)/1040.h
		touch backup

print:		db.c \
		interest.c \
		fica.c \
		taxTable.c \
		il.c \
		ntax.c \
		federal.c \
		ttax.c \
		usury.c \
		interest2.c \
		scheduleA.c \
		money.c \
		number.c \
		1040.c \
		realEstate.c \
		./realEstate.h \
		./debug.h \
		./boolean.h \
		./db.h \
		./fica.h \
		./taxTable.h \
		./il.h \
		./federal.h \
		./returns.h \
		./scheduleA.h \
		./money.h \
		./number.h \
		./1040.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			db.c \
			interest.c \
			fica.c \
			taxTable.c \
			il.c \
			ntax.c \
			federal.c \
			ttax.c \
			usury.c \
			interest2.c \
			scheduleA.c \
			money.c \
			number.c \
			1040.c \
			realEstate.c \
			./realEstate.h \
			./debug.h \
			./boolean.h \
			./db.h \
			./fica.h \
			./taxTable.h \
			./il.h \
			./federal.h \
			./returns.h \
			./scheduleA.h \
			./money.h \
			./number.h \
			./1040.h \
			$(ENDLIST)
