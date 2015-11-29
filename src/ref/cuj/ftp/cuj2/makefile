
.SUFFIXES:		.o .c

LEXFLAGS = -n
YACC = yacc
YACCFLAGS = -d -v
LEX = lex
MAKE = make
LDFLAGS = -g
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g
INCLDIRS = 
CC = CC
AR = ar
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		tbinary2 \
		$(ENDLIST)
		touch all

tbinary2:		tbinary2.o \
		binaryTree2.o \
		string.o \
		queue_List.o \
		stack_List.o \
		list.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tbinary2 \
			tbinary2.o \
			binaryTree2.o \
			string.o \
			queue_List.o \
			stack_List.o \
			list.o \
			$(ENDLIST)

binaryTree2.o:	binaryTree2.c \
		./binaryTree2.h \
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

list.o:	list.c \
		./list.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tbinary2.o:	tbinary2.c \
		./binaryTree2.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./absTree.h \
		./queue_List.h \
		./absQueue.h \
		./list.h \
		./stack_List.h \
		./absStack.h \
		./string.h \
		$(ENDLIST)

queue_List.o:	queue_List.c \
		./queue_List.h \
		./absQueue.h \
		./debug.h \
		./returns.h \
		./list.h \
		./absIterator.h \
		$(ENDLIST)

string.o:	string.c \
		./debug.h \
		./string.h \
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
		-rm -rf Templates.DB
		-rm -f tbinary2
		-rm -f binaryTree2.o
		-rm -f list.o
		-rm -f tbinary2.o
		-rm -f queue_List.o
		-rm -f string.o
		-rm -f stack_List.o

backup:		binaryTree2.c \
		list.c \
		tbinary2.c \
		queue_List.c \
		string.c \
		stack_List.c \
		./debug.h \
		./absIterator.h \
		./binaryTree2.h \
		./absQueue.h \
		./absStack.h \
		./returns.h \
		./list.h \
		./absTree.h \
		./queue_List.h \
		./string.h \
		./stack_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp binaryTree2.c $(BACKUPDIR)/binaryTree2.c
		cp list.c $(BACKUPDIR)/list.c
		cp tbinary2.c $(BACKUPDIR)/tbinary2.c
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp string.c $(BACKUPDIR)/string.c
		cp stack_List.c $(BACKUPDIR)/stack_List.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./binaryTree2.h ./$(BACKUPDIR)/binaryTree2.h
		cp ./absQueue.h ./$(BACKUPDIR)/absQueue.h
		cp ./absStack.h ./$(BACKUPDIR)/absStack.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./list.h ./$(BACKUPDIR)/list.h
		cp ./absTree.h ./$(BACKUPDIR)/absTree.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		cp ./string.h ./$(BACKUPDIR)/string.h
		cp ./stack_List.h ./$(BACKUPDIR)/stack_List.h
		touch backup
