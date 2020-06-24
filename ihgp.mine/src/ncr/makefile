
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG
LD = CC
BACKUPDIR = BACKUP
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

all:		 \
		omyncr \
		key2num \
		myncr \
		ncr \
		$(ENDLIST)
		touch all

:		$(ENDLIST)
		$(LD) $(LDFLAGS) -o  \
			$(ENDLIST)

omyncr:		omyncr.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o omyncr \
			omyncr.o \
			$(ENDLIST)
		rm $(HOME)/bin/omyncr
		cp omyncr $(HOME)/bin/omyncr

key2num:		key2num.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o key2num \
			key2num.o \
			$(ENDLIST)
		rm $(HOME)/bin/key2num
		cp key2num $(HOME)/bin/key2num

myncr:		myncr.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o myncr \
			myncr.o \
			$(ENDLIST)
		rm $(HOME)/bin/myncr
		cp myncr $(HOME)/bin/myncr

ncr:		ncr.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ncr \
			ncr.o \
			$(ENDLIST)
		rm $(HOME)/bin/ncr
		cp ncr $(HOME)/bin/ncr

omyncr.o:	omyncr.c \
		./debug.h \
		$(ENDLIST)

key2num.o:	key2num.c \
		$(ENDLIST)

myncr.o:	myncr.c \
		./debug.h \
		$(ENDLIST)

ncr.o:	ncr.c \
		./debug.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f omyncr
		-rm -f key2num
		-rm -f myncr
		-rm -f ncr
		-rm -f omyncr.o
		-rm -f key2num.o
		-rm -f myncr.o
		-rm -f ncr.o

backup:		omyncr.c \
		key2num.c \
		myncr.c \
		ncr.c \
		./debug.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp omyncr.c $(BACKUPDIR)/omyncr.c
		cp key2num.c $(BACKUPDIR)/key2num.c
		cp myncr.c $(BACKUPDIR)/myncr.c
		cp ncr.c $(BACKUPDIR)/ncr.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		touch backup

print:		omyncr.c \
		key2num.c \
		myncr.c \
		ncr.c \
		./debug.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			omyncr.c \
			key2num.c \
			myncr.c \
			ncr.c \
			./debug.h \
			$(ENDLIST)
