CC = cc
CCFLAGS = -g -I../src -DDEBUG

MTWRITE_OBJS = mtwrite.o blockio.o tio.o hash.o
MTREAD_OBJS = mtread.o makedir.o blockio.o bufferio.o tio.o hash.o

HDRS = ../src/blockio.h ../src/bufferio.h ../src/debug.h \
       ../src/returns.h ../src/mtio.h ../src/tio.h ../src/hash.h

all:	mtwrite mtread
	-echo "echo \"all done\"" >all
	-chmod +x all

mtwrite:	$(MTWRITE_OBJS)
		-rm -f $(HOME)/bin/mtwrite
		$(CC) $(CCFLAGS) -o mtwrite $(MTWRITE_OBJS)
		-ln mtwrite $(HOME)/bin/mtwrite

mtread:		$(MTREAD_OBJS)
		-rm -f $(HOME)/bin/mtread
		$(CC) $(CCFLAGS) -o mtread $(MTREAD_OBJS)
		-ln mtread $(HOME)/bin/mtread

mtwrite.o:	../src/mtwrite.c $(HDRS)
		$(CC) $(CCFLAGS) -c ../src/mtwrite.c

mtread.o:	../src/mtread.c $(HDRS)
		$(CC) $(CCFLAGS) -c ../src/mtread.c

blockio.o:	../src/blockio.c $(HDRS)
		$(CC) $(CCFLAGS) -c ../src/blockio.c

bufferio.o:	../src/bufferio.c $(HDRS)
		$(CC) $(CCFLAGS) -c ../src/bufferio.c

hash.o:		../src/hash.c $(HDRS)
		$(CC) $(CCFLAGS) -c ../src/hash.c

makedir.o:	../src/makedir.c $(HDRS)
		$(CC) $(CCFLAGS) -c ../src/makedir.c

tio.o:		../src/tio.c $(HDRS)
		$(CC) $(CCFLAGS) -c ../src/tio.c
