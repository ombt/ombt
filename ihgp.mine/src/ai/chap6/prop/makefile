
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -compat=4 -DDEBUG -xildoff
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -compat=4 -DDEBUG -xildoff
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

all:		tprop \
		$(ENDLIST)
		touch all

tprop:		propyacc.o \
		proplex.o \
		array.o \
		list.o \
		set_List.o \
		stack_List.o \
		queue_List.o \
		binaryTree_AVL.o \
		atom.o \
		clause.o \
		semantic.o \
		mystring.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tprop \
			propyacc.o \
			proplex.o \
			array.o \
			list.o \
			set_List.o \
			stack_List.o \
			queue_List.o \
			binaryTree_AVL.o \
			atom.o \
			clause.o \
			semantic.o \
			mystring.o \
			-ly \
			-ll \
			$(ENDLIST)
		chmod 755 tprop
		cp tprop /home/rumore/bin/tprop

mystring.o:	mystring.c \
		./mystring.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

atom.o:	atom.c \
		./atom.h \
		./debug.h \
		./mystring.h \
		./returns.h \
		$(ENDLIST)

semantic.o:	semantic.c \
		./semantic.h \
		./clause.h \
		./atom.h \
		./debug.h \
		./mystring.h \
		./returns.h \
		./set_List.h \
		./list.h \
		./absIterator.h \
		$(ENDLIST)

clause.o:	clause.c \
		./clause.h \
		./atom.h \
		./debug.h \
		./mystring.h \
		./returns.h \
		./set_List.h \
		./list.h \
		./absIterator.h \
		$(ENDLIST)

proplex.o:	prop.l \
		./debug.h \
		./prop.h \
		./array.h \
		./absIterator.h \
		./returns.h \
		./binaryTree_AVL.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./list.h \
		./stack_List.h \
		./absStack.h \
		./clause.h \
		./atom.h \
		./mystring.h \
		./set_List.h \
		./semantic.h \
		prop.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) prop.l
		mv lex.yy.c proplex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c proplex.c

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

propyacc.o:	prop.y \
		./prop.h \
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
		./clause.h \
		./atom.h \
		./mystring.h \
		./set_List.h \
		./semantic.h \
		$(ENDLIST)
		$(YACC) $(YACCFLAGS) prop.y
		mv y.tab.h propyacc.h
		mv y.tab.c propyacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c propyacc.c

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

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f tprop
		-rm -f mystring.o
		-rm -f atom.o
		-rm -f semantic.o
		-rm -f clause.o
		-rm -f proplex.o
		-rm -f array.o
		-rm -f list.o
		-rm -f binaryTree_AVL.o
		-rm -f propyacc.o
		-rm -f queue_List.o
		-rm -f set_List.o
		-rm -f stack_List.o

backup:		mystring.c \
		atom.c \
		semantic.c \
		clause.c \
		prop.l \
		array.c \
		list.c \
		binaryTree_AVL.c \
		prop.y \
		queue_List.c \
		set_List.c \
		stack_List.c \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./propyacc.h \
		./atom.h \
		./prop.h \
		./semantic.h \
		./absQueue.h \
		./clause.h \
		./absStack.h \
		./array.h \
		./returns.h \
		./list.h \
		./absTree.h \
		./binaryTree_AVL.h \
		./queue_List.h \
		./set_List.h \
		./stack_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mystring.c $(BACKUPDIR)/mystring.c
		cp atom.c $(BACKUPDIR)/atom.c
		cp semantic.c $(BACKUPDIR)/semantic.c
		cp clause.c $(BACKUPDIR)/clause.c
		cp prop.l $(BACKUPDIR)/prop.l
		cp array.c $(BACKUPDIR)/array.c
		cp list.c $(BACKUPDIR)/list.c
		cp binaryTree_AVL.c $(BACKUPDIR)/binaryTree_AVL.c
		cp prop.y $(BACKUPDIR)/prop.y
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp set_List.c $(BACKUPDIR)/set_List.c
		cp stack_List.c $(BACKUPDIR)/stack_List.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mystring.h ./$(BACKUPDIR)/mystring.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./propyacc.h ./$(BACKUPDIR)/propyacc.h
		cp ./atom.h ./$(BACKUPDIR)/atom.h
		cp ./prop.h ./$(BACKUPDIR)/prop.h
		cp ./semantic.h ./$(BACKUPDIR)/semantic.h
		cp ./absQueue.h ./$(BACKUPDIR)/absQueue.h
		cp ./clause.h ./$(BACKUPDIR)/clause.h
		cp ./absStack.h ./$(BACKUPDIR)/absStack.h
		cp ./array.h ./$(BACKUPDIR)/array.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./list.h ./$(BACKUPDIR)/list.h
		cp ./absTree.h ./$(BACKUPDIR)/absTree.h
		cp ./binaryTree_AVL.h ./$(BACKUPDIR)/binaryTree_AVL.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		cp ./set_List.h ./$(BACKUPDIR)/set_List.h
		cp ./stack_List.h ./$(BACKUPDIR)/stack_List.h
		touch backup

print:		mystring.c \
		atom.c \
		semantic.c \
		clause.c \
		prop.l \
		array.c \
		list.c \
		binaryTree_AVL.c \
		prop.y \
		queue_List.c \
		set_List.c \
		stack_List.c \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./propyacc.h \
		./atom.h \
		./prop.h \
		./semantic.h \
		./absQueue.h \
		./clause.h \
		./absStack.h \
		./array.h \
		./returns.h \
		./list.h \
		./absTree.h \
		./binaryTree_AVL.h \
		./queue_List.h \
		./set_List.h \
		./stack_List.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mystring.c \
			atom.c \
			semantic.c \
			clause.c \
			prop.l \
			array.c \
			list.c \
			binaryTree_AVL.c \
			prop.y \
			queue_List.c \
			set_List.c \
			stack_List.c \
			./debug.h \
			./mystring.h \
			./absIterator.h \
			./propyacc.h \
			./atom.h \
			./prop.h \
			./semantic.h \
			./absQueue.h \
			./clause.h \
			./absStack.h \
			./array.h \
			./returns.h \
			./list.h \
			./absTree.h \
			./binaryTree_AVL.h \
			./queue_List.h \
			./set_List.h \
			./stack_List.h \
			$(ENDLIST)
