
.SUFFIXES:		.o .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
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
AR = /usr/ccs/bin/ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.cpp.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		dag \
		$(ENDLIST)
		touch all

dag:		dag.o \
		mystring.o \
		map.o \
		mylist.o \
		queue_List.o \
		stack_List.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o dag \
			dag.o \
			mystring.o \
			map.o \
			mylist.o \
			queue_List.o \
			stack_List.o \
			$(ENDLIST)

mystring.o:	mystring.c \
		./mystring.h \
		./debug.h \
		./returns.h \
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

dag.o:	dag.c \
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
		./pointer.h \
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

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f dag
		-rm -f mystring.o
		-rm -f mylist.o
		-rm -f map.o
		-rm -f dag.o
		-rm -f queue_List.o
		-rm -f stack_List.o

backup:		mystring.c \
		mylist.c \
		map.c \
		dag.c \
		queue_List.c \
		stack_List.c \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./absQueue.h \
		./mylist.h \
		./map.h \
		./absStack.h \
		./tuple.h \
		./returns.h \
		./pointer.h \
		./absTree.h \
		./queue_List.h \
		./binaryTree.h \
		./stack_List.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mystring.c $(BACKUPDIR)/mystring.c
		cp mylist.c $(BACKUPDIR)/mylist.c
		cp map.c $(BACKUPDIR)/map.c
		cp dag.c $(BACKUPDIR)/dag.c
		cp queue_List.c $(BACKUPDIR)/queue_List.c
		cp stack_List.c $(BACKUPDIR)/stack_List.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./mystring.h ./$(BACKUPDIR)/mystring.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./absQueue.h ./$(BACKUPDIR)/absQueue.h
		cp ./mylist.h ./$(BACKUPDIR)/mylist.h
		cp ./map.h ./$(BACKUPDIR)/map.h
		cp ./absStack.h ./$(BACKUPDIR)/absStack.h
		cp ./tuple.h ./$(BACKUPDIR)/tuple.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./pointer.h ./$(BACKUPDIR)/pointer.h
		cp ./absTree.h ./$(BACKUPDIR)/absTree.h
		cp ./queue_List.h ./$(BACKUPDIR)/queue_List.h
		cp ./binaryTree.h ./$(BACKUPDIR)/binaryTree.h
		cp ./stack_List.h ./$(BACKUPDIR)/stack_List.h
		touch backup

print:		mystring.c \
		mylist.c \
		map.c \
		dag.c \
		queue_List.c \
		stack_List.c \
		./debug.h \
		./mystring.h \
		./absIterator.h \
		./absQueue.h \
		./mylist.h \
		./map.h \
		./absStack.h \
		./tuple.h \
		./returns.h \
		./pointer.h \
		./absTree.h \
		./queue_List.h \
		./binaryTree.h \
		./stack_List.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mystring.c \
			mylist.c \
			map.c \
			dag.c \
			queue_List.c \
			stack_List.c \
			./debug.h \
			./mystring.h \
			./absIterator.h \
			./absQueue.h \
			./mylist.h \
			./map.h \
			./absStack.h \
			./tuple.h \
			./returns.h \
			./pointer.h \
			./absTree.h \
			./queue_List.h \
			./binaryTree.h \
			./stack_List.h \
			$(ENDLIST)
