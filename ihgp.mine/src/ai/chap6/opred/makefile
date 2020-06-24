
.SUFFIXES:		.o .c

LEXFLAGS = -n
YACC = yacc
YACCFLAGS = -d -v
LEX = lex
MAKE = make
LDFLAGS = -g -xildoff -DDEBUG
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -xildoff -DDEBUG
INCLDIRS = 
CC = CC
LPRFLAGS = 
AR = ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		tpred \
		$(ENDLIST)
		touch all

tpred:		predyacc.o \
		predlex.o \
		array.o \
		binaryTree_AVL.o \
		clause.o \
		list.o \
		mystring.o \
		queue_List.o \
		semantic.o \
		set_List.o \
		stack_List.o \
		symbol.o \
		term.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tpred \
			predyacc.o \
			predlex.o \
			array.o \
			binaryTree_AVL.o \
			clause.o \
			list.o \
			mystring.o \
			queue_List.o \
			semantic.o \
			set_List.o \
			stack_List.o \
			symbol.o \
			term.o \
			-ll \
			-ly \
			$(ENDLIST)

mystring.o:	mystring.c \
		./mystring.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

semantic.o:	semantic.c \
		./semantic.h \
		./clause.h \
		./debug.h \
		./returns.h \
		./set_List.h \
		./list.h \
		./absIterator.h \
		./term.h \
		./mystring.h \
		./symbol.h \
		$(ENDLIST)

clause.o:	clause.c \
		./clause.h \
		./debug.h \
		./returns.h \
		./set_List.h \
		./list.h \
		./absIterator.h \
		./term.h \
		./mystring.h \
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
		./mystring.h \
		./semantic.h \
		./clause.h \
		./set_List.h \
		./term.h \
		./symbol.h \
		pred.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) pred.l
		mv lex.yy.c predlex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c predlex.c

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
		./mystring.h \
		./returns.h \
		$(ENDLIST)

queue_List.o:	queue_List.c \
		./queue_List.h \
		./absQueue.h \
		./debug.h \
		./returns.h \
		./list.h \
		./absIterator.h \
		$(ENDLIST)

set_List.o:	set_List.c \
		./set_List.h \
		./list.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
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
		./mystring.h \
		./semantic.h \
		./clause.h \
		./set_List.h \
		./term.h \
		./symbol.h \
		$(ENDLIST)
		$(YACC) $(YACCFLAGS) pred.y
		mv y.tab.h predyacc.h
		mv y.tab.c predyacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c predyacc.c

symbol.o:	symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./returns.h \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f tpred
		-rm -f mystring.o
		-rm -f semantic.o
		-rm -f clause.o
		-rm -f array.o
		-rm -f list.o
		-rm -f predlex.o
		-rm -f binaryTree_AVL.o
		-rm -f term.o
		-rm -f queue_List.o
		-rm -f set_List.o
		-rm -f stack_List.o
		-rm -f predyacc.o
		-rm -f symbol.o

backup:		mystring.c \
		semantic.c \
		clause.c \
		array.c \
		list.c \
		pred.l \
		binaryTree_AVL.c \
		term.c \
		queue_List.c \
		set_List.c \
		stack_List.c \
		pred.y \
		symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./predyacc.h \
		./semantic.h \
		./absQueue.h \
		./clause.h \
		./pred.h \
		./absStack.h \
		./array.h \
		./returns.h \
		./list.h \
		./absTree.h \
		./binaryTree_AVL.h \
		./term.h \
		./queue_List.h \
		./set_List.h \
		./stack_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mystring.c $(BACKUPDIR)/mystring.c
		cp semantic.c $(BACKUPDIR)/semantic.c
		cp clause.c $(BACKUPDIR)/clause.c
		cp array.c $(BACKUPDIR)/array.c
		cp list.c $(BACKUPDIR)/list.c
		cp pred.l $(BACKUPDIR)/pred.l
		cp binaryTree_AVL.c $(BACKUPDIR)/binaryTree_AVL.c
		cp term.c $(BACKUPDIR)/term.c
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp set_List.c $(BACKUPDIR)/set_List.c
		cp stack_List.c $(BACKUPDIR)/stack_List.c
		cp pred.y $(BACKUPDIR)/pred.y
		cp symbol.c $(BACKUPDIR)/symbol.c
		cp ./symbol.h ./$(BACKUPDIR)/symbol.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mystring.h ./$(BACKUPDIR)/mystring.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./predyacc.h ./$(BACKUPDIR)/predyacc.h
		cp ./semantic.h ./$(BACKUPDIR)/semantic.h
		cp ./absQueue.h ./$(BACKUPDIR)/absQueue.h
		cp ./clause.h ./$(BACKUPDIR)/clause.h
		cp ./pred.h ./$(BACKUPDIR)/pred.h
		cp ./absStack.h ./$(BACKUPDIR)/absStack.h
		cp ./array.h ./$(BACKUPDIR)/array.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./list.h ./$(BACKUPDIR)/list.h
		cp ./absTree.h ./$(BACKUPDIR)/absTree.h
		cp ./binaryTree_AVL.h ./$(BACKUPDIR)/binaryTree_AVL.h
		cp ./term.h ./$(BACKUPDIR)/term.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		cp ./set_List.h ./$(BACKUPDIR)/set_List.h
		cp ./stack_List.h ./$(BACKUPDIR)/stack_List.h
		touch backup

print:		mystring.c \
		semantic.c \
		clause.c \
		array.c \
		list.c \
		pred.l \
		binaryTree_AVL.c \
		term.c \
		queue_List.c \
		set_List.c \
		stack_List.c \
		pred.y \
		symbol.c \
		./symbol.h \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./predyacc.h \
		./semantic.h \
		./absQueue.h \
		./clause.h \
		./pred.h \
		./absStack.h \
		./array.h \
		./returns.h \
		./list.h \
		./absTree.h \
		./binaryTree_AVL.h \
		./term.h \
		./queue_List.h \
		./set_List.h \
		./stack_List.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mystring.c \
			semantic.c \
			clause.c \
			array.c \
			list.c \
			pred.l \
			binaryTree_AVL.c \
			term.c \
			queue_List.c \
			set_List.c \
			stack_List.c \
			pred.y \
			symbol.c \
			./symbol.h \
			./debug.h \
			./mystring.h \
			./absIterator.h \
			./predyacc.h \
			./semantic.h \
			./absQueue.h \
			./clause.h \
			./pred.h \
			./absStack.h \
			./array.h \
			./returns.h \
			./list.h \
			./absTree.h \
			./binaryTree_AVL.h \
			./term.h \
			./queue_List.h \
			./set_List.h \
			./stack_List.h \
			$(ENDLIST)
