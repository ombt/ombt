CC = cc
CCFLAGS = -g -I../src -DDEBUG -xtarget=ultra -xarch=v9

MTWRITE_OBJS = mtwrite64.o blockio64.o tio64.o hash64.o
MTREAD_OBJS = mtread64.o makedir64.o blockio64.o bufferio64.o tio64.o hash64.o

HDRS = ../src/blockio.h ../src/bufferio.h ../src/debug.h \
       ../src/returns.h ../src/mtio.h ../src/tio.h ../src/hash.h

all:	mtwrite64 mtread64
	-echo "echo \"all done\"" >all
	-chmod +x all

mtwrite64:	$(MTWRITE_OBJS)
		$(CC) $(CCFLAGS) -o mtwrite64 $(MTWRITE_OBJS)

mtread64:	$(MTREAD_OBJS)
		$(CC) $(CCFLAGS) -o mtread64 $(MTREAD_OBJS)

mtwrite64.o:	../src/mtwrite.c $(HDRS)
		$(CC) $(CCFLAGS) -o mtwrite64.o -c ../src/mtwrite.c

mtread64.o:	../src/mtread.c $(HDRS)
		$(CC) $(CCFLAGS) -o mtread64.o -c ../src/mtread.c

blockio64.o:	../src/blockio.c $(HDRS)
		$(CC) $(CCFLAGS) -o blockio64.o -c ../src/blockio.c

bufferio64.o:	../src/bufferio.c $(HDRS)
		$(CC) $(CCFLAGS) -o bufferio64.o -c ../src/bufferio.c

hash64.o:	../src/hash.c $(HDRS)
		$(CC) $(CCFLAGS) -o hash64.o -c ../src/hash.c

makedir64.o:	../src/makedir.c $(HDRS)
		$(CC) $(CCFLAGS) -o makedir64.o -c ../src/makedir.c

tio64.o:	../src/tio.c $(HDRS)
		$(CC) $(CCFLAGS) -o tio64.o -c ../src/tio.c
