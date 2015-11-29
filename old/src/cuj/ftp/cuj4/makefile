
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

all:		example \
		tgausslup \
		tepsilon \
		tglup \
		$(ENDLIST)
		touch all

example:		example.o \
		gausslup.o \
		matrix.o \
		vector.o \
		epsilon.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o example \
			example.o \
			gausslup.o \
			matrix.o \
			vector.o \
			epsilon.o \
			-lm \
			$(ENDLIST)

tgausslup:		tgausslup.o \
		gausslup.o \
		matrix.o \
		vector.o \
		epsilon.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tgausslup \
			tgausslup.o \
			gausslup.o \
			matrix.o \
			vector.o \
			epsilon.o \
			-lm \
			$(ENDLIST)

tepsilon:		tepsilon.o \
		complex.o \
		epsilon.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tepsilon \
			tepsilon.o \
			complex.o \
			epsilon.o \
			-lm \
			$(ENDLIST)

tglup:		tglup.o \
		gausslup.o \
		matrix.o \
		vector.o \
		epsilon.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tglup \
			tglup.o \
			gausslup.o \
			matrix.o \
			vector.o \
			epsilon.o \
			-lm \
			$(ENDLIST)

vector.o:	vector.c \
		./vector.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tglup.o:	tglup.c \
		./gausslup.h \
		./debug.h \
		./epsilon.h \
		./matrix.h \
		./returns.h \
		./vector.h \
		$(ENDLIST)

example.o:	example.c \
		./gausslup.h \
		./debug.h \
		./epsilon.h \
		./matrix.h \
		./returns.h \
		./vector.h \
		$(ENDLIST)

gausslup.o:	gausslup.c \
		./gausslup.h \
		./debug.h \
		./epsilon.h \
		./matrix.h \
		./returns.h \
		./vector.h \
		$(ENDLIST)

tgausslup.o:	tgausslup.c \
		./gausslup.h \
		./debug.h \
		./epsilon.h \
		./matrix.h \
		./returns.h \
		./vector.h \
		$(ENDLIST)

epsilon.o:	epsilon.c \
		./epsilon.h \
		$(ENDLIST)

tepsilon.o:	tepsilon.c \
		./complex.h \
		./debug.h \
		./returns.h \
		./epsilon.h \
		$(ENDLIST)

matrix.o:	matrix.c \
		./matrix.h \
		./debug.h \
		./epsilon.h \
		./returns.h \
		./vector.h \
		$(ENDLIST)

complex.o:	complex.c \
		./complex.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f example
		-rm -f tgausslup
		-rm -f tepsilon
		-rm -f tglup
		-rm -f vector.o
		-rm -f tglup.o
		-rm -f example.o
		-rm -f gausslup.o
		-rm -f tgausslup.o
		-rm -f epsilon.o
		-rm -f tepsilon.o
		-rm -f matrix.o
		-rm -f complex.o

backup:		vector.c \
		tglup.c \
		example.c \
		gausslup.c \
		tgausslup.c \
		epsilon.c \
		tepsilon.c \
		matrix.c \
		complex.c \
		./complex.h \
		./debug.h \
		./vector.h \
		./gausslup.h \
		./returns.h \
		./epsilon.h \
		./matrix.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp vector.c $(BACKUPDIR)/vector.c
		cp tglup.c $(BACKUPDIR)/tglup.c
		cp example.c $(BACKUPDIR)/example.c
		cp gausslup.c $(BACKUPDIR)/gausslup.c
		cp tgausslup.c $(BACKUPDIR)/tgausslup.c
		cp epsilon.c $(BACKUPDIR)/epsilon.c
		cp tepsilon.c $(BACKUPDIR)/tepsilon.c
		cp matrix.c $(BACKUPDIR)/matrix.c
		cp complex.c $(BACKUPDIR)/complex.c
		cp ./complex.h ./$(BACKUPDIR)/complex.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./vector.h ./$(BACKUPDIR)/vector.h
		cp ./gausslup.h ./$(BACKUPDIR)/gausslup.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./epsilon.h ./$(BACKUPDIR)/epsilon.h
		cp ./matrix.h ./$(BACKUPDIR)/matrix.h
		touch backup
