
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG -L/opt/x11r6/lib
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG -L/opt/x11r6/lib
INCLDIRS = -I/opt/x11r6/include
CC = CC
LPRFLAGS = 
AR = /usr/ccs/bin/ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.C.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.cpp.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.CPP.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		xgraph \
		trans \
		rotate \
		circle \
		sin \
		rhdr \
		ahdr \
		scale \
		tan \
		cos \
		mirror \
		$(ENDLIST)
		touch all

xgraph:		xgraph.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o xgraph \
			xgraph.o \
			-lX11 \
			-lsocket \
			-lnsl \
			$(ENDLIST)
		chmod 755 xgraph
		cp xgraph $(HOME)/bin/xgraph

trans:		trans.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o trans \
			trans.o \
			-lX11 \
			-lsocket \
			-lnsl \
			-lm \
			$(ENDLIST)
		chmod 755 trans
		cp trans $(HOME)/bin/trans

rotate:		rotate.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o rotate \
			rotate.o \
			-lX11 \
			-lsocket \
			-lnsl \
			-lm \
			$(ENDLIST)
		chmod 755 rotate
		cp rotate $(HOME)/bin/rotate

circle:		circle.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o circle \
			circle.o \
			-lX11 \
			-lsocket \
			-lnsl \
			-lm \
			$(ENDLIST)
		chmod 755 circle
		cp circle $(HOME)/bin/circle

sin:		sin.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o sin \
			sin.o \
			-lX11 \
			-lsocket \
			-lnsl \
			$(ENDLIST)
		chmod 755 sin
		cp sin $(HOME)/bin/sin

rhdr:		rhdr.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o rhdr \
			rhdr.o \
			-lX11 \
			-lsocket \
			-lnsl \
			-lm \
			$(ENDLIST)
		chmod 755 rhdr
		cp rhdr $(HOME)/bin/rhdr

ahdr:		ahdr.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ahdr \
			ahdr.o \
			-lX11 \
			-lsocket \
			-lnsl \
			-lm \
			$(ENDLIST)
		chmod 755 ahdr
		cp ahdr $(HOME)/bin/ahdr

scale:		scale.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o scale \
			scale.o \
			-lX11 \
			-lsocket \
			-lnsl \
			-lm \
			$(ENDLIST)
		chmod 755 scale
		cp scale $(HOME)/bin/scale

tan:		tan.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tan \
			tan.o \
			-lX11 \
			-lsocket \
			-lnsl \
			$(ENDLIST)
		chmod 755 tan
		cp tan $(HOME)/bin/tan

cos:		cos.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o cos \
			cos.o \
			-lX11 \
			-lsocket \
			-lnsl \
			$(ENDLIST)
		chmod 755 cos
		cp cos $(HOME)/bin/cos

mirror:		mirror.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o mirror \
			mirror.o \
			-lX11 \
			-lsocket \
			-lnsl \
			-lm \
			$(ENDLIST)
		chmod 755 mirror
		cp mirror $(HOME)/bin/mirror

rotate.o:	rotate.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

sin.o:	sin.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

ahdr.o:	ahdr.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

scale.o:	scale.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

cos.o:	cos.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

mirror.o:	mirror.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

circle.o:	circle.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

rhdr.o:	rhdr.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

xgraph.o:	xgraph.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

trans.o:	trans.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tan.o:	tan.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f xgraph
		-rm -f trans
		-rm -f rotate
		-rm -f circle
		-rm -f sin
		-rm -f rhdr
		-rm -f ahdr
		-rm -f scale
		-rm -f tan
		-rm -f cos
		-rm -f mirror
		-rm -f rotate.o
		-rm -f sin.o
		-rm -f ahdr.o
		-rm -f scale.o
		-rm -f cos.o
		-rm -f mirror.o
		-rm -f circle.o
		-rm -f rhdr.o
		-rm -f xgraph.o
		-rm -f trans.o
		-rm -f tan.o

backup:		rotate.c \
		sin.c \
		ahdr.c \
		scale.c \
		cos.c \
		mirror.c \
		circle.c \
		rhdr.c \
		xgraph.c \
		trans.c \
		tan.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp rotate.c $(BACKUPDIR)/rotate.c
		cp sin.c $(BACKUPDIR)/sin.c
		cp ahdr.c $(BACKUPDIR)/ahdr.c
		cp scale.c $(BACKUPDIR)/scale.c
		cp cos.c $(BACKUPDIR)/cos.c
		cp mirror.c $(BACKUPDIR)/mirror.c
		cp circle.c $(BACKUPDIR)/circle.c
		cp rhdr.c $(BACKUPDIR)/rhdr.c
		cp xgraph.c $(BACKUPDIR)/xgraph.c
		cp trans.c $(BACKUPDIR)/trans.c
		cp tan.c $(BACKUPDIR)/tan.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup

print:		rotate.c \
		sin.c \
		ahdr.c \
		scale.c \
		cos.c \
		mirror.c \
		circle.c \
		rhdr.c \
		xgraph.c \
		trans.c \
		tan.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			rotate.c \
			sin.c \
			ahdr.c \
			scale.c \
			cos.c \
			mirror.c \
			circle.c \
			rhdr.c \
			xgraph.c \
			trans.c \
			tan.c \
			./debug.h \
			./returns.h \
			$(ENDLIST)
