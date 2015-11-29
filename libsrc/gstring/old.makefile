
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -DDEBUG -g
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -DDEBUG -g
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

all:		ttrie \
		myspell \
		tgstring \
		$(ENDLIST)
		touch all

ttrie:		ttrie.o \
		trie.o \
		gstring.o \
		queue_List.o \
		mylist.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ttrie \
			ttrie.o \
			trie.o \
			gstring.o \
			queue_List.o \
			mylist.o \
			$(ENDLIST)

myspell:		spell.o \
		trie.o \
		gstring.o \
		queue_List.o \
		mylist.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o myspell \
			spell.o \
			trie.o \
			gstring.o \
			queue_List.o \
			mylist.o \
			$(ENDLIST)

tgstring:		tgstring.o \
		gstring.o \
		random.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tgstring \
			tgstring.o \
			gstring.o \
			random.o \
			$(ENDLIST)

random.o:	random.c \
		$(ENDLIST)

trie.o:	trie.c \
		./trie.h \
		./debug.h \
		./gstring.h \
		./returns.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./absIterator.h \
		$(ENDLIST)

mylist.o:	mylist.c \
		./mylist.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tgstring.o:	tgstring.c \
		./gstring.h \
		./debug.h \
		./returns.h \
		./random.h \
		$(ENDLIST)

gstring.o:	gstring.c \
		./gstring.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

spell.o:	spell.c \
		./gstring.h \
		./debug.h \
		./returns.h \
		./trie.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./absIterator.h \
		$(ENDLIST)

queue_List.o:	queue_List.c \
		./queue_List.h \
		./absQueue.h \
		./debug.h \
		./returns.h \
		./mylist.h \
		./absIterator.h \
		$(ENDLIST)

ttrie.o:	ttrie.c \
		./gstring.h \
		./debug.h \
		./returns.h \
		./trie.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./absIterator.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f ttrie
		-rm -f myspell
		-rm -f tgstring
		-rm -f random.o
		-rm -f trie.o
		-rm -f mylist.o
		-rm -f tgstring.o
		-rm -f gstring.o
		-rm -f spell.o
		-rm -f queue_List.o
		-rm -f ttrie.o

backup:		random.c \
		trie.c \
		mylist.c \
		tgstring.c \
		gstring.c \
		spell.c \
		queue_List.c \
		ttrie.c \
		./debug.h \
		./random.h \
		./absIterator.h \
		./trie.h \
		./absQueue.h \
		./mylist.h \
		./returns.h \
		./gstring.h \
		./queue_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp random.c $(BACKUPDIR)/random.c
		cp trie.c $(BACKUPDIR)/trie.c
		cp mylist.c $(BACKUPDIR)/mylist.c
		cp tgstring.c $(BACKUPDIR)/tgstring.c
		cp gstring.c $(BACKUPDIR)/gstring.c
		cp spell.c $(BACKUPDIR)/spell.c
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp ttrie.c $(BACKUPDIR)/ttrie.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./random.h ./$(BACKUPDIR)/random.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./trie.h ./$(BACKUPDIR)/trie.h
		cp ./absQueue.h ./$(BACKUPDIR)/absQueue.h
		cp ./mylist.h ./$(BACKUPDIR)/mylist.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./gstring.h ./$(BACKUPDIR)/gstring.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		touch backup

print:		random.c \
		trie.c \
		mylist.c \
		tgstring.c \
		gstring.c \
		spell.c \
		queue_List.c \
		ttrie.c \
		./debug.h \
		./random.h \
		./absIterator.h \
		./trie.h \
		./absQueue.h \
		./mylist.h \
		./returns.h \
		./gstring.h \
		./queue_List.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			random.c \
			trie.c \
			mylist.c \
			tgstring.c \
			gstring.c \
			spell.c \
			queue_List.c \
			ttrie.c \
			./debug.h \
			./random.h \
			./absIterator.h \
			./trie.h \
			./absQueue.h \
			./mylist.h \
			./returns.h \
			./gstring.h \
			./queue_List.h \
			$(ENDLIST)
