
.SUFFIXES:		.o .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -DDEBUG -g -xildoff
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -DDEBUG -g -xildoff
INCLDIRS = 
CC = CC
LPRFLAGS = 
AR = /usr/ccs/bin/ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.cpp.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		 \
		atpsplit \
		$(ENDLIST)
		touch all

:		$(ENDLIST)
		$(LD) $(LDFLAGS) -o  \
			$(ENDLIST)

atpsplit:		splityacc.o \
		splitlex.o \
		binaryTree_AVL.o \
		mylist.o \
		queue_List.o \
		semantic.o \
		symbol.o \
		stack_List.o \
		mystring.o \
		strategy.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o atpsplit \
			splityacc.o \
			splitlex.o \
			binaryTree_AVL.o \
			mylist.o \
			queue_List.o \
			semantic.o \
			symbol.o \
			stack_List.o \
			mystring.o \
			strategy.o \
			-ly \
			-ll \
			$(ENDLIST)
		cp atpsplit $(HOME)/bin/atpsplit

mystring.o:	mystring.c \
		./mystring.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

splitlex.o:	split.l \
		./split.h \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./mystring.h \
		./semantic.h \
		./symbol.h \
		split.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) split.l
		mv lex.yy.c splitlex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c splitlex.c

semantic.o:	semantic.c \
		./semantic.h \
		./debug.h \
		./mylist.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./symbol.h \
		$(ENDLIST)

splityacc.o:	split.y \
		./split.h \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./mystring.h \
		./semantic.h \
		./symbol.h \
		$(ENDLIST)
		$(YACC) $(YACCFLAGS) split.y
		mv y.tab.h splityacc.h
		mv y.tab.c splityacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c splityacc.c

mylist.o:	mylist.c \
		./mylist.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

strategy.o:	strategy.c \
		./split.h \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./mystring.h \
		./semantic.h \
		./symbol.h \
		$(ENDLIST)

binaryTree_AVL.o:	binaryTree_AVL.c \
		./binaryTree_AVL.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		$(ENDLIST)

queue_List.o:	queue_List.c \
		./queue_List.h \
		./absQueue.h \
		./debug.h \
		./returns.h \
		./mylist.h \
		./absIterator.h \
		$(ENDLIST)

stack_List.o:	stack_List.c \
		./stack_List.h \
		./absStack.h \
		./debug.h \
		./returns.h \
		./mylist.h \
		./absIterator.h \
		$(ENDLIST)

symbol.o:	symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f atpsplit
		-rm -f mystring.o
		-rm -f splitlex.o
		-rm -f semantic.o
		-rm -f splityacc.o
		-rm -f mylist.o
		-rm -f strategy.o
		-rm -f binaryTree_AVL.o
		-rm -f queue_List.o
		-rm -f stack_List.o
		-rm -f symbol.o

backup:		mystring.c \
		split.l \
		semantic.c \
		split.y \
		mylist.c \
		strategy.c \
		binaryTree_AVL.c \
		queue_List.c \
		stack_List.c \
		symbol.c \
		./symbol.h \
		./split.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./semantic.h \
		./absQueue.h \
		./mylist.h \
		./absStack.h \
		./splityacc.h \
		./array.h \
		./returns.h \
		./absTree.h \
		./binaryTree_AVL.h \
		./queue_List.h \
		./stack_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mystring.c $(BACKUPDIR)/mystring.c
		cp split.l $(BACKUPDIR)/split.l
		cp semantic.c $(BACKUPDIR)/semantic.c
		cp split.y $(BACKUPDIR)/split.y
		cp mylist.c $(BACKUPDIR)/mylist.c
		cp strategy.c $(BACKUPDIR)/strategy.c
		cp binaryTree_AVL.c $(BACKUPDIR)/binaryTree_AVL.c
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp stack_List.c $(BACKUPDIR)/stack_List.c
		cp symbol.c $(BACKUPDIR)/symbol.c
		cp ./symbol.h ./$(BACKUPDIR)/symbol.h
		cp ./split.h ./$(BACKUPDIR)/split.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mystring.h ./$(BACKUPDIR)/mystring.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./semantic.h ./$(BACKUPDIR)/semantic.h
		cp ./absQueue.h ./$(BACKUPDIR)/absQueue.h
		cp ./mylist.h ./$(BACKUPDIR)/mylist.h
		cp ./absStack.h ./$(BACKUPDIR)/absStack.h
		cp ./splityacc.h ./$(BACKUPDIR)/splityacc.h
		cp ./array.h ./$(BACKUPDIR)/array.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./absTree.h ./$(BACKUPDIR)/absTree.h
		cp ./binaryTree_AVL.h ./$(BACKUPDIR)/binaryTree_AVL.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		cp ./stack_List.h ./$(BACKUPDIR)/stack_List.h
		touch backup

print:		mystring.c \
		split.l \
		semantic.c \
		split.y \
		mylist.c \
		strategy.c \
		binaryTree_AVL.c \
		queue_List.c \
		stack_List.c \
		symbol.c \
		./symbol.h \
		./split.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./semantic.h \
		./absQueue.h \
		./mylist.h \
		./absStack.h \
		./splityacc.h \
		./array.h \
		./returns.h \
		./absTree.h \
		./binaryTree_AVL.h \
		./queue_List.h \
		./stack_List.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mystring.c \
			split.l \
			semantic.c \
			split.y \
			mylist.c \
			strategy.c \
			binaryTree_AVL.c \
			queue_List.c \
			stack_List.c \
			symbol.c \
			./symbol.h \
			./split.h \
			./debug.h \
			./mystring.h \
			./absIterator.h \
			./semantic.h \
			./absQueue.h \
			./mylist.h \
			./absStack.h \
			./splityacc.h \
			./array.h \
			./returns.h \
			./absTree.h \
			./binaryTree_AVL.h \
			./queue_List.h \
			./stack_List.h \
			$(ENDLIST)
