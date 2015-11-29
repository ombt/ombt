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
