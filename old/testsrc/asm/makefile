ifndef ROOT
$(error ROOT is not set; see README)
endif

include $(ROOT)/build/makefile.common

LIBDIR = $(ROOT)/lib

INCLUDEDIR = $(ROOT)/hdr

PRODS = test1 \
	test2 \
	test3 \
	test4 \
	ex1

all:

test:	$(PRODS)

test1:	test1.o
	ld -o test1 -O0 test1.o

test1.o:	test1.asm
	as -a=test1.memmap --gstabs -o test1.o test1.asm

test2:	test2.o
	ld -o test2 -O0 test2.o

test2.o:	test2.asm
	as -a=test2.memmap --gstabs -o test2.o test2.asm

test3:	test3.o
	ld -o test3 -O0 test3.o

test3.o:	test3.asm
	as -a=test3.memmap --gstabs -o test3.o test3.asm

test4:	test4.o
	# messy, but it starts with main()
	ld -O0 -o test4 -static /usr/lib/crt1.o /usr/lib/crti.o -lc test4.o /usr/lib/crtn.o -L/usr/lib/gcc/i486-linux-gnu/4.3/ -lgcc_eh -lgcc -lc

test4.o:	test4.asm
	as -a=test4.memmap --gstabs -o test4.o test4.asm

ex1:	ex1.s
	as -a=ex1.memmap --gstabs -o ex1.o ex1.s
	ld -o ex1 -O0 ex1.o

clean:
	-rm -f *core*
	-rm -f *.o
	-rm -f *.memmap
	-rm -f $(PRODS)

install:
	touch install

backup:
	bkup

backupd:
	bkup -d

