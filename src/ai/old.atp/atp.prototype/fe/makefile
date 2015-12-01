#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#

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
		atpfe \
		$(ENDLIST)
		touch all

:		$(ENDLIST)
		$(LD) $(LDFLAGS) -o  \
			$(ENDLIST)

atpfe:		feyacc.o \
		felex.o \
		binaryTree_AVL.o \
		mylist.o \
		queue_List.o \
		semantic.o \
		symbol.o \
		stack_List.o \
		mystring.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o atpfe \
			feyacc.o \
			felex.o \
			binaryTree_AVL.o \
			mylist.o \
			queue_List.o \
			semantic.o \
			symbol.o \
			stack_List.o \
			mystring.o \
			-ly \
			-ll \
			$(ENDLIST)
		cp atpfe $(HOME)/bin/atpfe

mystring.o:	mystring.c \
		./mystring.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

semantic.o:	semantic.c \
		./semantic.h \
		./debug.h \
		./mylist.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./symbol.h \
		$(ENDLIST)

mylist.o:	mylist.c \
		./mylist.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

felex.o:	fe.l \
		./fe.h \
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
		fe.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) fe.l
		mv lex.yy.c felex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c felex.c

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

feyacc.o:	fe.y \
		./fe.h \
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
		$(YACC) $(YACCFLAGS) fe.y
		mv y.tab.h feyacc.h
		mv y.tab.c feyacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c feyacc.c

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
		-rm -f atpfe
		-rm -f mystring.o
		-rm -f semantic.o
		-rm -f mylist.o
		-rm -f felex.o
		-rm -f binaryTree_AVL.o
		-rm -f queue_List.o
		-rm -f feyacc.o
		-rm -f stack_List.o
		-rm -f symbol.o

backup:		mystring.c \
		semantic.c \
		mylist.c \
		fe.l \
		binaryTree_AVL.c \
		queue_List.c \
		fe.y \
		stack_List.c \
		symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./semantic.h \
		./fe.h \
		./absQueue.h \
		./mylist.h \
		./absStack.h \
		./array.h \
		./returns.h \
		./absTree.h \
		./binaryTree_AVL.h \
		./feyacc.h \
		./queue_List.h \
		./stack_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mystring.c $(BACKUPDIR)/mystring.c
		cp semantic.c $(BACKUPDIR)/semantic.c
		cp mylist.c $(BACKUPDIR)/mylist.c
		cp fe.l $(BACKUPDIR)/fe.l
		cp binaryTree_AVL.c $(BACKUPDIR)/binaryTree_AVL.c
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp fe.y $(BACKUPDIR)/fe.y
		cp stack_List.c $(BACKUPDIR)/stack_List.c
		cp symbol.c $(BACKUPDIR)/symbol.c
		cp ./symbol.h ./$(BACKUPDIR)/symbol.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mystring.h ./$(BACKUPDIR)/mystring.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./semantic.h ./$(BACKUPDIR)/semantic.h
		cp ./fe.h ./$(BACKUPDIR)/fe.h
		cp ./absQueue.h ./$(BACKUPDIR)/absQueue.h
		cp ./mylist.h ./$(BACKUPDIR)/mylist.h
		cp ./absStack.h ./$(BACKUPDIR)/absStack.h
		cp ./array.h ./$(BACKUPDIR)/array.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./absTree.h ./$(BACKUPDIR)/absTree.h
		cp ./binaryTree_AVL.h ./$(BACKUPDIR)/binaryTree_AVL.h
		cp ./feyacc.h ./$(BACKUPDIR)/feyacc.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		cp ./stack_List.h ./$(BACKUPDIR)/stack_List.h
		touch backup

print:		mystring.c \
		semantic.c \
		mylist.c \
		fe.l \
		binaryTree_AVL.c \
		queue_List.c \
		fe.y \
		stack_List.c \
		symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./semantic.h \
		./fe.h \
		./absQueue.h \
		./mylist.h \
		./absStack.h \
		./array.h \
		./returns.h \
		./absTree.h \
		./binaryTree_AVL.h \
		./feyacc.h \
		./queue_List.h \
		./stack_List.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mystring.c \
			semantic.c \
			mylist.c \
			fe.l \
			binaryTree_AVL.c \
			queue_List.c \
			fe.y \
			stack_List.c \
			symbol.c \
			./symbol.h \
			./debug.h \
			./mystring.h \
			./absIterator.h \
			./semantic.h \
			./fe.h \
			./absQueue.h \
			./mylist.h \
			./absStack.h \
			./array.h \
			./returns.h \
			./absTree.h \
			./binaryTree_AVL.h \
			./feyacc.h \
			./queue_List.h \
			./stack_List.h \
			$(ENDLIST)
