
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -DDEBUG -DUSEEXCEPTION -g -xildoff
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -DDEBUG -DUSEEXCEPTION -g -xildoff
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

all:		atp \
		$(ENDLIST)
		touch all

atp:		pryacc.o \
		prlex.o \
		array.o \
		binaryTree_AVL.o \
		clause.o \
		deletion.o \
		expandfiles.o \
		index.o \
		literal.o \
		main.o \
		map.o \
		mylist.o \
		mystring.o \
		prover.o \
		queue_List.o \
		semantic.o \
		stack_List.o \
		substitution.o \
		symbol.o \
		unification.o \
		uniquename.o \
		globals.o \
		search.o \
		bfsnode.o \
		options.o \
		memoryleak.o \
		varnames.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o atp \
			pryacc.o \
			prlex.o \
			array.o \
			binaryTree_AVL.o \
			clause.o \
			deletion.o \
			expandfiles.o \
			index.o \
			literal.o \
			main.o \
			map.o \
			mylist.o \
			mystring.o \
			prover.o \
			queue_List.o \
			semantic.o \
			stack_List.o \
			substitution.o \
			symbol.o \
			unification.o \
			uniquename.o \
			globals.o \
			search.o \
			bfsnode.o \
			options.o \
			memoryleak.o \
			varnames.o \
			-ly \
			-ll \
			$(ENDLIST)
		rm -f $(HOME)/bin/atp
		ln -s $(PWD)/atp $(HOME)/bin/atp
		cp $(PWD)/atp $(HOME)/public_html/predcal/cgi-bin/atp

mystring.o:	mystring.c \
		./mystring.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

options.o:	options.c \
		./options.h \
		./debug.h \
		./globals.h \
		./mylist.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./statistics.h \
		./map.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./stack_List.h \
		./absStack.h \
		./tuple.h \
		$(ENDLIST)

substitution.o:	substitution.c \
		./substitution.h \
		./clause.h \
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
		./literal.h \
		./index.h \
		./map.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./statistics.h \
		$(ENDLIST)

varnames.o:	varnames.c \
		./varnames.h \
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
		./map.h \
		./tuple.h \
		./mystring.h \
		$(ENDLIST)

expandfiles.o:	expandfiles.c \
		./pr.h \
		./bfsnode.h \
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
		./clause.h \
		./literal.h \
		./index.h \
		./map.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./statistics.h \
		./deletion.h \
		./array.h \
		./globals.h \
		./substitution.h \
		./unification.h \
		./options.h \
		./proxy.h \
		./search.h \
		./semantic.h \
		./symbol.h \
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
		./varnames.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./stack_List.h \
		./absStack.h \
		./map.h \
		./tuple.h \
		$(ENDLIST)

clause.o:	clause.c \
		./clause.h \
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
		./literal.h \
		./index.h \
		./map.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./statistics.h \
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
		./binaryTree_AVL.h \
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
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./literal.h \
		./index.h \
		./map.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./statistics.h \
		./globals.h \
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
		./bfsnode.h \
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
		./clause.h \
		./literal.h \
		./index.h \
		./map.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./statistics.h \
		./deletion.h \
		./array.h \
		./globals.h \
		./substitution.h \
		./unification.h \
		./options.h \
		./proxy.h \
		./search.h \
		./semantic.h \
		./symbol.h \
		pr.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) pr.l
		mv lex.yy.c prlex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c prlex.c

globals.o:	globals.c \
		./globals.h \
		./debug.h \
		./mylist.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./statistics.h \
		./map.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./stack_List.h \
		./absStack.h \
		./tuple.h \
		$(ENDLIST)

unification.o:	unification.c \
		./unification.h \
		./debug.h \
		./literal.h \
		./index.h \
		./returns.h \
		./map.h \
		./absIterator.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./substitution.h \
		./clause.h \
		./statistics.h \
		$(ENDLIST)

bfsnode.o:	bfsnode.c \
		./bfsnode.h \
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
		./clause.h \
		./literal.h \
		./index.h \
		./map.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./statistics.h \
		./deletion.h \
		./array.h \
		./globals.h \
		./substitution.h \
		./unification.h \
		./options.h \
		./proxy.h \
		./trace.h \
		./twokeys.h \
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

main.o:	main.c \
		./pr.h \
		./bfsnode.h \
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
		./clause.h \
		./literal.h \
		./index.h \
		./map.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./statistics.h \
		./deletion.h \
		./array.h \
		./globals.h \
		./substitution.h \
		./unification.h \
		./options.h \
		./proxy.h \
		./search.h \
		./semantic.h \
		./symbol.h \
		./trace.h \
		./twokeys.h \
		$(ENDLIST)

literal.o:	literal.c \
		./literal.h \
		./debug.h \
		./index.h \
		./returns.h \
		./map.h \
		./absIterator.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./mylist.h \
		./stack_List.h \
		./absStack.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		$(ENDLIST)

queue_List.o:	queue_List.c \
		./queue_List.h \
		./absQueue.h \
		./debug.h \
		./returns.h \
		./mylist.h \
		./absIterator.h \
		$(ENDLIST)

prover.o:	prover.c \
		./pr.h \
		./bfsnode.h \
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
		./clause.h \
		./literal.h \
		./index.h \
		./map.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./statistics.h \
		./deletion.h \
		./array.h \
		./globals.h \
		./substitution.h \
		./unification.h \
		./options.h \
		./proxy.h \
		./search.h \
		./semantic.h \
		./symbol.h \
		./trace.h \
		./twokeys.h \
		$(ENDLIST)

index.o:	index.c \
		./index.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

pryacc.o:	pr.y \
		./pr.h \
		./bfsnode.h \
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
		./clause.h \
		./literal.h \
		./index.h \
		./map.h \
		./tuple.h \
		./memoryleak.h \
		./mystring.h \
		./uniquename.h \
		./varnames.h \
		./statistics.h \
		./deletion.h \
		./array.h \
		./globals.h \
		./substitution.h \
		./unification.h \
		./options.h \
		./proxy.h \
		./search.h \
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

memoryleak.o:	memoryleak.c \
		./memoryleak.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

search.o:	search.c \
		./search.h \
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
		./proxy.h \
		./varnames.h \
		./map.h \
		./tuple.h \
		./mystring.h \
		./trace.h \
		./twokeys.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f atp
		-rm -f mystring.o
		-rm -f options.o
		-rm -f substitution.o
		-rm -f varnames.o
		-rm -f expandfiles.o
		-rm -f uniquename.o
		-rm -f semantic.o
		-rm -f clause.o
		-rm -f mylist.o
		-rm -f map.o
		-rm -f deletion.o
		-rm -f array.o
		-rm -f prlex.o
		-rm -f globals.o
		-rm -f unification.o
		-rm -f bfsnode.o
		-rm -f binaryTree_AVL.o
		-rm -f main.o
		-rm -f literal.o
		-rm -f queue_List.o
		-rm -f prover.o
		-rm -f index.o
		-rm -f pryacc.o
		-rm -f stack_List.o
		-rm -f symbol.o
		-rm -f memoryleak.o
		-rm -f search.o

backup:		mystring.c \
		options.c \
		substitution.c \
		varnames.c \
		expandfiles.c \
		uniquename.c \
		semantic.c \
		clause.c \
		mylist.c \
		map.c \
		deletion.c \
		array.c \
		pr.l \
		globals.c \
		unification.c \
		bfsnode.c \
		binaryTree_AVL.c \
		main.c \
		literal.c \
		queue_List.c \
		prover.c \
		index.c \
		pr.y \
		stack_List.c \
		symbol.c \
		memoryleak.c \
		search.c \
		./symbol.h \
		./memoryleak.h \
		./search.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./options.h \
		./substitution.h \
		./varnames.h \
		./pryacc.h \
		./uniquename.h \
		./semantic.h \
		./twokeys.h \
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
		./statistics.h \
		./absTree.h \
		./globals.h \
		./unification.h \
		./bfsnode.h \
		./binaryTree_AVL.h \
		./proxy.h \
		./literal.h \
		./trace.h \
		./index.h \
		./queue_List.h \
		./stack_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mystring.c $(BACKUPDIR)/mystring.c
		cp options.c $(BACKUPDIR)/options.c
		cp substitution.c $(BACKUPDIR)/substitution.c
		cp varnames.c $(BACKUPDIR)/varnames.c
		cp expandfiles.c $(BACKUPDIR)/expandfiles.c
		cp uniquename.c $(BACKUPDIR)/uniquename.c
		cp semantic.c $(BACKUPDIR)/semantic.c
		cp clause.c $(BACKUPDIR)/clause.c
		cp mylist.c $(BACKUPDIR)/mylist.c
		cp map.c $(BACKUPDIR)/map.c
		cp deletion.c $(BACKUPDIR)/deletion.c
		cp array.c $(BACKUPDIR)/array.c
		cp pr.l $(BACKUPDIR)/pr.l
		cp globals.c $(BACKUPDIR)/globals.c
		cp unification.c $(BACKUPDIR)/unification.c
		cp bfsnode.c $(BACKUPDIR)/bfsnode.c
		cp binaryTree_AVL.c $(BACKUPDIR)/binaryTree_AVL.c
		cp main.c $(BACKUPDIR)/main.c
		cp literal.c $(BACKUPDIR)/literal.c
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp prover.c $(BACKUPDIR)/prover.c
		cp index.c $(BACKUPDIR)/index.c
		cp pr.y $(BACKUPDIR)/pr.y
		cp stack_List.c $(BACKUPDIR)/stack_List.c
		cp symbol.c $(BACKUPDIR)/symbol.c
		cp memoryleak.c $(BACKUPDIR)/memoryleak.c
		cp search.c $(BACKUPDIR)/search.c
		cp ./symbol.h ./$(BACKUPDIR)/symbol.h
		cp ./memoryleak.h ./$(BACKUPDIR)/memoryleak.h
		cp ./search.h ./$(BACKUPDIR)/search.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mystring.h ./$(BACKUPDIR)/mystring.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./options.h ./$(BACKUPDIR)/options.h
		cp ./substitution.h ./$(BACKUPDIR)/substitution.h
		cp ./varnames.h ./$(BACKUPDIR)/varnames.h
		cp ./pryacc.h ./$(BACKUPDIR)/pryacc.h
		cp ./uniquename.h ./$(BACKUPDIR)/uniquename.h
		cp ./semantic.h ./$(BACKUPDIR)/semantic.h
		cp ./twokeys.h ./$(BACKUPDIR)/twokeys.h
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
		cp ./statistics.h ./$(BACKUPDIR)/statistics.h
		cp ./absTree.h ./$(BACKUPDIR)/absTree.h
		cp ./globals.h ./$(BACKUPDIR)/globals.h
		cp ./unification.h ./$(BACKUPDIR)/unification.h
		cp ./bfsnode.h ./$(BACKUPDIR)/bfsnode.h
		cp ./binaryTree_AVL.h ./$(BACKUPDIR)/binaryTree_AVL.h
		cp ./proxy.h ./$(BACKUPDIR)/proxy.h
		cp ./literal.h ./$(BACKUPDIR)/literal.h
		cp ./trace.h ./$(BACKUPDIR)/trace.h
		cp ./index.h ./$(BACKUPDIR)/index.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		cp ./stack_List.h ./$(BACKUPDIR)/stack_List.h
		touch backup

print:		mystring.c \
		options.c \
		substitution.c \
		varnames.c \
		expandfiles.c \
		uniquename.c \
		semantic.c \
		clause.c \
		mylist.c \
		map.c \
		deletion.c \
		array.c \
		pr.l \
		globals.c \
		unification.c \
		bfsnode.c \
		binaryTree_AVL.c \
		main.c \
		literal.c \
		queue_List.c \
		prover.c \
		index.c \
		pr.y \
		stack_List.c \
		symbol.c \
		memoryleak.c \
		search.c \
		./symbol.h \
		./memoryleak.h \
		./search.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./options.h \
		./substitution.h \
		./varnames.h \
		./pryacc.h \
		./uniquename.h \
		./semantic.h \
		./twokeys.h \
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
		./statistics.h \
		./absTree.h \
		./globals.h \
		./unification.h \
		./bfsnode.h \
		./binaryTree_AVL.h \
		./proxy.h \
		./literal.h \
		./trace.h \
		./index.h \
		./queue_List.h \
		./stack_List.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mystring.c \
			options.c \
			substitution.c \
			varnames.c \
			expandfiles.c \
			uniquename.c \
			semantic.c \
			clause.c \
			mylist.c \
			map.c \
			deletion.c \
			array.c \
			pr.l \
			globals.c \
			unification.c \
			bfsnode.c \
			binaryTree_AVL.c \
			main.c \
			literal.c \
			queue_List.c \
			prover.c \
			index.c \
			pr.y \
			stack_List.c \
			symbol.c \
			memoryleak.c \
			search.c \
			./symbol.h \
			./memoryleak.h \
			./search.h \
			./debug.h \
			./mystring.h \
			./absIterator.h \
			./options.h \
			./substitution.h \
			./varnames.h \
			./pryacc.h \
			./uniquename.h \
			./semantic.h \
			./twokeys.h \
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
			./statistics.h \
			./absTree.h \
			./globals.h \
			./unification.h \
			./bfsnode.h \
			./binaryTree_AVL.h \
			./proxy.h \
			./literal.h \
			./trace.h \
			./index.h \
			./queue_List.h \
			./stack_List.h \
			$(ENDLIST)
