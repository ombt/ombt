
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -compat=4 -xildoff -DDEBUG
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -compat=4 -xildoff -DDEBUG
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

all:		tunify \
		tpredpp \
		tsub2 \
		tsub3 \
		tunify_tree2 \
		tunify_tree \
		tsub \
		tterm \
		$(ENDLIST)
		touch all

tunify:		tunify.o \
		term.o \
		substitution.o \
		mystring.o \
		list.o \
		unification.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tunify \
			tunify.o \
			term.o \
			substitution.o \
			mystring.o \
			list.o \
			unification.o \
			$(ENDLIST)

tpredpp:		predyacc.o \
		predlex.o \
		array.o \
		binaryTree_AVL.o \
		irclause.o \
		list.o \
		mystring.o \
		queue_List.o \
		semantic.o \
		stack_List.o \
		symbol.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tpredpp \
			predyacc.o \
			predlex.o \
			array.o \
			binaryTree_AVL.o \
			irclause.o \
			list.o \
			mystring.o \
			queue_List.o \
			semantic.o \
			stack_List.o \
			symbol.o \
			-ll \
			-ly \
			$(ENDLIST)
		chmod 755 tpredpp
		cp tpredpp /home/rumore/bin/tpredpp

tsub2:		tsub2.o \
		term.o \
		substitution.o \
		mystring.o \
		list.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tsub2 \
			tsub2.o \
			term.o \
			substitution.o \
			mystring.o \
			list.o \
			$(ENDLIST)

tsub3:		tsub3.o \
		term.o \
		substitution.o \
		mystring.o \
		list.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tsub3 \
			tsub3.o \
			term.o \
			substitution.o \
			mystring.o \
			list.o \
			$(ENDLIST)

tunify_tree2:		tunify_tree2.o \
		mystring.o \
		list.o \
		term_tree.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tunify_tree2 \
			tunify_tree2.o \
			mystring.o \
			list.o \
			term_tree.o \
			$(ENDLIST)

tunify_tree:		tunify_tree.o \
		mystring.o \
		list.o \
		term_tree.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tunify_tree \
			tunify_tree.o \
			mystring.o \
			list.o \
			term_tree.o \
			$(ENDLIST)

tsub:		tsub.o \
		term.o \
		substitution.o \
		mystring.o \
		list.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tsub \
			tsub.o \
			term.o \
			substitution.o \
			mystring.o \
			list.o \
			$(ENDLIST)

tterm:		tterm.o \
		term.o \
		mystring.o \
		list.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tterm \
			tterm.o \
			term.o \
			mystring.o \
			list.o \
			$(ENDLIST)

mystring.o:	mystring.c \
		./mystring.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

irclause.o:	irclause.c \
		./irclause.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		$(ENDLIST)

substitution.o:	substitution.c \
		./substitution.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./term.h \
		$(ENDLIST)

tsub2.o:	tsub2.c \
		./substitution.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./term.h \
		$(ENDLIST)

semantic.o:	semantic.c \
		./semantic.h \
		./debug.h \
		./irclause.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./symbol.h \
		$(ENDLIST)

term_tree.o:	term_tree.c \
		./term_tree.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		$(ENDLIST)

tsub3.o:	tsub3.c \
		./substitution.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./term.h \
		$(ENDLIST)

tunify_tree.o:	tunify_tree.c \
		./term_tree.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		$(ENDLIST)

tterm.o:	tterm.c \
		./term.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		$(ENDLIST)

tunify.o:	tunify.c \
		./substitution.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./term.h \
		./unification.h \
		$(ENDLIST)

array.o:	array.c \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

list.o:	list.c \
		./list.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tsub.o:	tsub.c \
		./substitution.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		./term.h \
		$(ENDLIST)

predlex.o:	pred.l \
		./pred.h \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./list.h \
		./stack_List.h \
		./absStack.h \
		./irclause.h \
		./mystring.h \
		./semantic.h \
		./symbol.h \
		pred.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) pred.l
		mv lex.yy.c predlex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c predlex.c

unification.o:	unification.c \
		./unification.h \
		./debug.h \
		./mystring.h \
		./returns.h \
		./substitution.h \
		./list.h \
		./absIterator.h \
		./term.h \
		$(ENDLIST)

binaryTree_AVL.o:	binaryTree_AVL.c \
		./binaryTree_AVL.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./list.h \
		./stack_List.h \
		./absStack.h \
		$(ENDLIST)

term.o:	term.c \
		./term.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
		$(ENDLIST)

queue_List.o:	queue_List.c \
		./queue_List.h \
		./absQueue.h \
		./debug.h \
		./returns.h \
		./list.h \
		./absIterator.h \
		$(ENDLIST)

stack_List.o:	stack_List.c \
		./stack_List.h \
		./absStack.h \
		./debug.h \
		./returns.h \
		./list.h \
		./absIterator.h \
		$(ENDLIST)

predyacc.o:	pred.y \
		./pred.h \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./list.h \
		./stack_List.h \
		./absStack.h \
		./irclause.h \
		./mystring.h \
		./semantic.h \
		./symbol.h \
		$(ENDLIST)
		$(YACC) $(YACCFLAGS) pred.y
		mv y.tab.h predyacc.h
		mv y.tab.c predyacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c predyacc.c

tunify_tree2.o:	tunify_tree2.c \
		./term_tree.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./mystring.h \
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
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f tunify
		-rm -f tpredpp
		-rm -f tsub2
		-rm -f tsub3
		-rm -f tunify_tree2
		-rm -f tunify_tree
		-rm -f tsub
		-rm -f tterm
		-rm -f mystring.o
		-rm -f irclause.o
		-rm -f substitution.o
		-rm -f tsub2.o
		-rm -f semantic.o
		-rm -f term_tree.o
		-rm -f tsub3.o
		-rm -f tunify_tree.o
		-rm -f tterm.o
		-rm -f tunify.o
		-rm -f array.o
		-rm -f list.o
		-rm -f tsub.o
		-rm -f predlex.o
		-rm -f unification.o
		-rm -f binaryTree_AVL.o
		-rm -f term.o
		-rm -f queue_List.o
		-rm -f stack_List.o
		-rm -f predyacc.o
		-rm -f tunify_tree2.o
		-rm -f symbol.o

backup:		mystring.c \
		irclause.c \
		substitution.c \
		tsub2.c \
		semantic.c \
		term_tree.c \
		tsub3.c \
		tunify_tree.c \
		tterm.c \
		tunify.c \
		array.c \
		list.c \
		tsub.c \
		pred.l \
		unification.c \
		binaryTree_AVL.c \
		term.c \
		queue_List.c \
		stack_List.c \
		pred.y \
		tunify_tree2.c \
		symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./irclause.h \
		./substitution.h \
		./predyacc.h \
		./semantic.h \
		./term_tree.h \
		./absQueue.h \
		./pred.h \
		./absStack.h \
		./array.h \
		./returns.h \
		./list.h \
		./absTree.h \
		./unification.h \
		./binaryTree_AVL.h \
		./term.h \
		./queue_List.h \
		./stack_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mystring.c $(BACKUPDIR)/mystring.c
		cp irclause.c $(BACKUPDIR)/irclause.c
		cp substitution.c $(BACKUPDIR)/substitution.c
		cp tsub2.c $(BACKUPDIR)/tsub2.c
		cp semantic.c $(BACKUPDIR)/semantic.c
		cp term_tree.c $(BACKUPDIR)/term_tree.c
		cp tsub3.c $(BACKUPDIR)/tsub3.c
		cp tunify_tree.c $(BACKUPDIR)/tunify_tree.c
		cp tterm.c $(BACKUPDIR)/tterm.c
		cp tunify.c $(BACKUPDIR)/tunify.c
		cp array.c $(BACKUPDIR)/array.c
		cp list.c $(BACKUPDIR)/list.c
		cp tsub.c $(BACKUPDIR)/tsub.c
		cp pred.l $(BACKUPDIR)/pred.l
		cp unification.c $(BACKUPDIR)/unification.c
		cp binaryTree_AVL.c $(BACKUPDIR)/binaryTree_AVL.c
		cp term.c $(BACKUPDIR)/term.c
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp stack_List.c $(BACKUPDIR)/stack_List.c
		cp pred.y $(BACKUPDIR)/pred.y
		cp tunify_tree2.c $(BACKUPDIR)/tunify_tree2.c
		cp symbol.c $(BACKUPDIR)/symbol.c
		cp ./symbol.h ./$(BACKUPDIR)/symbol.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mystring.h ./$(BACKUPDIR)/mystring.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./irclause.h ./$(BACKUPDIR)/irclause.h
		cp ./substitution.h ./$(BACKUPDIR)/substitution.h
		cp ./predyacc.h ./$(BACKUPDIR)/predyacc.h
		cp ./semantic.h ./$(BACKUPDIR)/semantic.h
		cp ./term_tree.h ./$(BACKUPDIR)/term_tree.h
		cp ./absQueue.h ./$(BACKUPDIR)/absQueue.h
		cp ./pred.h ./$(BACKUPDIR)/pred.h
		cp ./absStack.h ./$(BACKUPDIR)/absStack.h
		cp ./array.h ./$(BACKUPDIR)/array.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./list.h ./$(BACKUPDIR)/list.h
		cp ./absTree.h ./$(BACKUPDIR)/absTree.h
		cp ./unification.h ./$(BACKUPDIR)/unification.h
		cp ./binaryTree_AVL.h ./$(BACKUPDIR)/binaryTree_AVL.h
		cp ./term.h ./$(BACKUPDIR)/term.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		cp ./stack_List.h ./$(BACKUPDIR)/stack_List.h
		touch backup

print:		mystring.c \
		irclause.c \
		substitution.c \
		tsub2.c \
		semantic.c \
		term_tree.c \
		tsub3.c \
		tunify_tree.c \
		tterm.c \
		tunify.c \
		array.c \
		list.c \
		tsub.c \
		pred.l \
		unification.c \
		binaryTree_AVL.c \
		term.c \
		queue_List.c \
		stack_List.c \
		pred.y \
		tunify_tree2.c \
		symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./irclause.h \
		./substitution.h \
		./predyacc.h \
		./semantic.h \
		./term_tree.h \
		./absQueue.h \
		./pred.h \
		./absStack.h \
		./array.h \
		./returns.h \
		./list.h \
		./absTree.h \
		./unification.h \
		./binaryTree_AVL.h \
		./term.h \
		./queue_List.h \
		./stack_List.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mystring.c \
			irclause.c \
			substitution.c \
			tsub2.c \
			semantic.c \
			term_tree.c \
			tsub3.c \
			tunify_tree.c \
			tterm.c \
			tunify.c \
			array.c \
			list.c \
			tsub.c \
			pred.l \
			unification.c \
			binaryTree_AVL.c \
			term.c \
			queue_List.c \
			stack_List.c \
			pred.y \
			tunify_tree2.c \
			symbol.c \
			./symbol.h \
			./debug.h \
			./mystring.h \
			./absIterator.h \
			./irclause.h \
			./substitution.h \
			./predyacc.h \
			./semantic.h \
			./term_tree.h \
			./absQueue.h \
			./pred.h \
			./absStack.h \
			./array.h \
			./returns.h \
			./list.h \
			./absTree.h \
			./unification.h \
			./binaryTree_AVL.h \
			./term.h \
			./queue_List.h \
			./stack_List.h \
			$(ENDLIST)
