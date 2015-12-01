#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
all:

test:	testtrie

tokenizer.o:	\
		tokenizer.cpp \
		tokenizer.h \
		gstring.cpp \
		gstring.h \
		trie.cpp \
		trie.h

testtrie:	\
		testtrie.cpp \
		tokenizer.o 

clean:
	-rm -f *.o
	-rm -f testtrie
	-rm -f install

install:
	touch install
