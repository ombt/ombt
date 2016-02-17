CCFLAGS = -g -I/opt/x11r5/include

ASCII_HDRS = asciiWindow.h window.h debug.h returns.h
X_HDRS = xWindow.h xWindowDisplay.h window.h debug.h returns.h ftrc.h
GARY_HDRS = xWindow.h xWindowDisplay.h window.h uniform.h probDist.h debug.h returns.h ftrc.h

all:		tascii tx gary
		echo > all

tascii:		tascii.o \
		asciiWindow.o
		CC $(CCFLAGS) -o tascii \
			tascii.o \
			asciiWindow.o \
			-lm

tascii.o:	tascii.c \
		$(ASCII_HDRS)
		CC $(CCFLAGS) -c tascii.c

asciiWindow.o:	asciiWindow.c \
		$(ASCII_HDRS)
		CC $(CCFLAGS) -c asciiWindow.c

gary:		gary.o \
		ftrc.o \
		xWindow.o \
		xWindowDisplay.o \
		uniform.o \
		probDist.o
		CC $(CCFLAGS) -o gary \
			gary.o \
			ftrc.o \
			xWindowDisplay.o \
			xWindow.o \
			uniform.o \
			probDist.o \
			-lm \
			-lX11

tx:		tx.o \
		ftrc.o \
		xWindow.o \
		xWindowDisplay.o
		CC $(CCFLAGS) -o tx \
			tx.o \
			ftrc.o \
			xWindowDisplay.o \
			xWindow.o \
			-lm \
			-lX11

tx.o:		tx.c \
		$(X_HDRS)
		CC $(CCFLAGS) -c tx.c

ftrc.o:		ftrc.c \
		$(X_HDRS)
		CC $(CCFLAGS) -c ftrc.c

xWindow.o:	xWindow.c \
		$(X_HDRS)
		CC $(CCFLAGS) -c xWindow.c

xWindowDisplay.o:	xWindowDisplay.c \
		$(X_HDRS)
		CC $(CCFLAGS) -c xWindowDisplay.c

gary.o:		gary.c \
		$(GARY_HDRS)
		CC $(CCFLAGS) -c gary.c

uniform.o:	uniform.c \
		$(GARY_HDRS)
		CC $(CCFLAGS) -c uniform.c

probDist.o:	probDist.c \
		$(GARY_HDRS)
		CC $(CCFLAGS) -c probDist.c

