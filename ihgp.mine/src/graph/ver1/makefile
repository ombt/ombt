CCFLAGS = -g -DDEBUG -I/usr/openwin/include

PRODS = convsp corrsp fftsp makesp plotsp exp
HDRS = debug.h returns.h

all:		$(PRODS)
		echo "echo 'all done'" >all
		chmod +x all

convsp:		convsp.o \
		complex.o \
		spectrum.o
		CC $(CCFLAGS) -o convsp \
			convsp.o \
			spectrum.o \
			complex.o \
			-lm

corrsp:		corrsp.o \
		complex.o \
		spectrum.o
		CC $(CCFLAGS) -o corrsp \
			corrsp.o \
			spectrum.o \
			complex.o \
			-lm

fftsp:		fftsp.o \
		complex.o \
		spectrum.o
		CC $(CCFLAGS) -o fftsp \
			fftsp.o \
			spectrum.o \
			complex.o \
			-lm

makesp:		makesp.o
		CC $(CCFLAGS) -o makesp \
			makesp.o

exp:		exp.o
		CC $(CCFLAGS) -o exp \
			exp.o \
			-lm

plotsp:		plotsp.o \
		complex.o \
		spectrum.o
		CC $(CCFLAGS) -o plotsp \
			plotsp.o \
			spectrum.o \
			complex.o \
			-lm \
			-lX11

complex.o:	complex.c \
		complex.h \
		$(HDRS)
		CC $(CCFLAGS) -c complex.c

convsp.o:	convsp.c \
		complex.h \
		spectrum.h \
		$(HDRS)
		CC $(CCFLAGS) -c convsp.c

corrsp.o:	corrsp.c \
		complex.h \
		spectrum.h \
		$(HDRS)
		CC $(CCFLAGS) -c corrsp.c

fftsp.o:	fftsp.c \
		complex.h \
		spectrum.h \
		$(HDRS)
		CC $(CCFLAGS) -c fftsp.c

makesp.o:	makesp.c \
		$(HDRS)
		CC $(CCFLAGS) -c makesp.c

plotsp.o:	plotsp.c \
		complex.h \
		spectrum.h \
		$(HDRS)
		CC $(CCFLAGS) -c plotsp.c

spectrum.o:	spectrum.c \
		complex.h \
		spectrum.h \
		$(HDRS)
		CC $(CCFLAGS) -c spectrum.c

exp.o:		exp.c \
		$(HDRS)
		CC $(CCFLAGS) -c exp.c

