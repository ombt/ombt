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

all:		atppr \
		$(ENDLIST)
		touch all

atppr:		pryacc.o \
		prlex.o \
		binaryTree_AVL.o \
		binaryTree.o \
		mylist.o \
		array.o \
		queue_List.o \
		stack_List.o \
		mystring.o \
		map.o \
		semantic.o \
		symbol.o \
		prover.o \
		literal.o \
		clause.o \
		uniquename.o \
		orderedset.o \
		deletion.o \
		unification.o \
		substitution.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o atppr \
			pryacc.o \
			prlex.o \
			binaryTree_AVL.o \
			binaryTree.o \
			mylist.o \
			array.o \
			queue_List.o \
			stack_List.o \
			mystring.o \
			map.o \
			semantic.o \
			symbol.o \
			prover.o \
			literal.o \
			clause.o \
			uniquename.o \
			orderedset.o \
			deletion.o \
			unification.o \
			substitution.o \
			-ly \
			-ll \
			$(ENDLIST)
		rm -f $(HOME)/bin/atppr
		ln -s $(HOME)/provers/atp/atp1/pr/atppr $(HOME)/bin/atppr

mystring.o:	mystring.c \
		./mystring.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

substitution.o:	substitution.c \
		./substitution.h \
		./clause.h \
		./binaryTree.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./literal.h \
		./map.h \
		./tuple.h \
		./mystring.h \
		./uniquename.h \
		$(ENDLIST)

orderedset.o:	orderedset.c \
		./orderedset.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./binaryTree.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./tuple.h \
		$(ENDLIST)

uniquename.o:	uniquename.c \
		./uniquename.h \
		./debug.h \
		./mystring.h \
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
		./uniquename.h \
		$(ENDLIST)

clause.o:	clause.c \
		./clause.h \
		./binaryTree.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./literal.h \
		./map.h \
		./tuple.h \
		./mystring.h \
		./uniquename.h \
		$(ENDLIST)

mylist.o:	mylist.c \
		./mylist.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

map.o:	map.c \
		./map.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./binaryTree.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./tuple.h \
		$(ENDLIST)

deletion.o:	deletion.c \
		./deletion.h \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./clause.h \
		./binaryTree.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./literal.h \
		./map.h \
		./tuple.h \
		./mystring.h \
		./uniquename.h \
		./substitution.h \
		./unification.h \
		$(ENDLIST)

array.o:	array.c \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

prlex.o:	pr.l \
		./pr.h \
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
		./clause.h \
		./binaryTree.h \
		./literal.h \
		./map.h \
		./tuple.h \
		./mystring.h \
		./uniquename.h \
		./deletion.h \
		./substitution.h \
		./unification.h \
		./orderedset.h \
		./semantic.h \
		./symbol.h \
		pr.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) pr.l
		mv lex.yy.c prlex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c prlex.c

unification.o:	unification.c \
		./unification.h \
		./debug.h \
		./literal.h \
		./map.h \
		./absIterator.h \
		./returns.h \
		./binaryTree.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./tuple.h \
		./mystring.h \
		./uniquename.h \
		./substitution.h \
		./clause.h \
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

literal.o:	literal.c \
		./literal.h \
		./debug.h \
		./map.h \
		./absIterator.h \
		./returns.h \
		./binaryTree.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./tuple.h \
		./mystring.h \
		./uniquename.h \
		$(ENDLIST)

prover.o:	prover.c \
		./pr.h \
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
		./clause.h \
		./binaryTree.h \
		./literal.h \
		./map.h \
		./tuple.h \
		./mystring.h \
		./uniquename.h \
		./deletion.h \
		./substitution.h \
		./unification.h \
		./orderedset.h \
		./semantic.h \
		./symbol.h \
		$(ENDLIST)

queue_List.o:	queue_List.c \
		./queue_List.h \
		./absQueue.h \
		./debug.h \
		./returns.h \
		./mylist.h \
		./absIterator.h \
		$(ENDLIST)

binaryTree.o:	binaryTree.c \
		./binaryTree.h \
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

pryacc.o:	pr.y \
		./pr.h \
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
		./clause.h \
		./binaryTree.h \
		./literal.h \
		./map.h \
		./tuple.h \
		./mystring.h \
		./uniquename.h \
		./deletion.h \
		./substitution.h \
		./unification.h \
		./orderedset.h \
		./semantic.h \
		./symbol.h \
		$(ENDLIST)
		$(YACC) $(YACCFLAGS) pr.y
		mv y.tab.h pryacc.h
		mv y.tab.c pryacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c pryacc.c

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
		-rm -f atppr
		-rm -f mystring.o
		-rm -f substitution.o
		-rm -f orderedset.o
		-rm -f uniquename.o
		-rm -f semantic.o
		-rm -f clause.o
		-rm -f mylist.o
		-rm -f map.o
		-rm -f deletion.o
		-rm -f array.o
		-rm -f prlex.o
		-rm -f unification.o
		-rm -f binaryTree_AVL.o
		-rm -f literal.o
		-rm -f prover.o
		-rm -f queue_List.o
		-rm -f binaryTree.o
		-rm -f pryacc.o
		-rm -f stack_List.o
		-rm -f symbol.o

backup:		mystring.c \
		substitution.c \
		orderedset.c \
		uniquename.c \
		semantic.c \
		clause.c \
		mylist.c \
		map.c \
		deletion.c \
		array.c \
		pr.l \
		unification.c \
		binaryTree_AVL.c \
		literal.c \
		prover.c \
		queue_List.c \
		binaryTree.c \
		pr.y \
		stack_List.c \
		symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./orderedset.h \
		./substitution.h \
		./pryacc.h \
		./uniquename.h \
		./semantic.h \
		./absQueue.h \
		./clause.h \
		./mylist.h \
		./pr.h \
		./map.h \
		./absStack.h \
		./tuple.h \
		./deletion.h \
		./array.h \
		./returns.h \
		./absTree.h \
		./unification.h \
		./binaryTree_AVL.h \
		./literal.h \
		./queue_List.h \
		./binaryTree.h \
		./stack_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mystring.c $(BACKUPDIR)/mystring.c
		cp substitution.c $(BACKUPDIR)/substitution.c
		cp orderedset.c $(BACKUPDIR)/orderedset.c
		cp uniquename.c $(BACKUPDIR)/uniquename.c
		cp semantic.c $(BACKUPDIR)/semantic.c
		cp clause.c $(BACKUPDIR)/clause.c
		cp mylist.c $(BACKUPDIR)/mylist.c
		cp map.c $(BACKUPDIR)/map.c
		cp deletion.c $(BACKUPDIR)/deletion.c
		cp array.c $(BACKUPDIR)/array.c
		cp pr.l $(BACKUPDIR)/pr.l
		cp unification.c $(BACKUPDIR)/unification.c
		cp binaryTree_AVL.c $(BACKUPDIR)/binaryTree_AVL.c
		cp literal.c $(BACKUPDIR)/literal.c
		cp prover.c $(BACKUPDIR)/prover.c
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp binaryTree.c $(BACKUPDIR)/binaryTree.c
		cp pr.y $(BACKUPDIR)/pr.y
		cp stack_List.c $(BACKUPDIR)/stack_List.c
		cp symbol.c $(BACKUPDIR)/symbol.c
		cp ./symbol.h ./$(BACKUPDIR)/symbol.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mystring.h ./$(BACKUPDIR)/mystring.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./orderedset.h ./$(BACKUPDIR)/orderedset.h
		cp ./substitution.h ./$(BACKUPDIR)/substitution.h
		cp ./pryacc.h ./$(BACKUPDIR)/pryacc.h
		cp ./uniquename.h ./$(BACKUPDIR)/uniquename.h
		cp ./semantic.h ./$(BACKUPDIR)/semantic.h
		cp ./absQueue.h ./$(BACKUPDIR)/absQueue.h
		cp ./clause.h ./$(BACKUPDIR)/clause.h
		cp ./mylist.h ./$(BACKUPDIR)/mylist.h
		cp ./pr.h ./$(BACKUPDIR)/pr.h
		cp ./map.h ./$(BACKUPDIR)/map.h
		cp ./absStack.h ./$(BACKUPDIR)/absStack.h
		cp ./tuple.h ./$(BACKUPDIR)/tuple.h
		cp ./deletion.h ./$(BACKUPDIR)/deletion.h
		cp ./array.h ./$(BACKUPDIR)/array.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./absTree.h ./$(BACKUPDIR)/absTree.h
		cp ./unification.h ./$(BACKUPDIR)/unification.h
		cp ./binaryTree_AVL.h ./$(BACKUPDIR)/binaryTree_AVL.h
		cp ./literal.h ./$(BACKUPDIR)/literal.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		cp ./binaryTree.h ./$(BACKUPDIR)/binaryTree.h
		cp ./stack_List.h ./$(BACKUPDIR)/stack_List.h
		touch backup

print:		mystring.c \
		substitution.c \
		orderedset.c \
		uniquename.c \
		semantic.c \
		clause.c \
		mylist.c \
		map.c \
		deletion.c \
		array.c \
		pr.l \
		unification.c \
		binaryTree_AVL.c \
		literal.c \
		prover.c \
		queue_List.c \
		binaryTree.c \
		pr.y \
		stack_List.c \
		symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./orderedset.h \
		./substitution.h \
		./pryacc.h \
		./uniquename.h \
		./semantic.h \
		./absQueue.h \
		./clause.h \
		./mylist.h \
		./pr.h \
		./map.h \
		./absStack.h \
		./tuple.h \
		./deletion.h \
		./array.h \
		./returns.h \
		./absTree.h \
		./unification.h \
		./binaryTree_AVL.h \
		./literal.h \
		./queue_List.h \
		./binaryTree.h \
		./stack_List.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mystring.c \
			substitution.c \
			orderedset.c \
			uniquename.c \
			semantic.c \
			clause.c \
			mylist.c \
			map.c \
			deletion.c \
			array.c \
			pr.l \
			unification.c \
			binaryTree_AVL.c \
			literal.c \
			prover.c \
			queue_List.c \
			binaryTree.c \
			pr.y \
			stack_List.c \
			symbol.c \
			./symbol.h \
			./debug.h \
			./mystring.h \
			./absIterator.h \
			./orderedset.h \
			./substitution.h \
			./pryacc.h \
			./uniquename.h \
			./semantic.h \
			./absQueue.h \
			./clause.h \
			./mylist.h \
			./pr.h \
			./map.h \
			./absStack.h \
			./tuple.h \
			./deletion.h \
			./array.h \
			./returns.h \
			./absTree.h \
			./unification.h \
			./binaryTree_AVL.h \
			./literal.h \
			./queue_List.h \
			./binaryTree.h \
			./stack_List.h \
			$(ENDLIST)
