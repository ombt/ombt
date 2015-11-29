
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
INCLDIRS = -I/opt/x11r5/include
CC = CC
LPRFLAGS = 
AR = ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		chap10 \
		chap1 \
		chap2 \
		chap3 \
		chap4 \
		chap5 \
		chap6 \
		chap7 \
		chap8 \
		chap9 \
		$(ENDLIST)
		touch all

chap10:		bitmap.o \
		chap10.o \
		classhnt.o \
		connect.o \
		gc.o \
		icon.o \
		key.o \
		oval.o \
		pixmap.o \
		sizehint.o \
		window.o \
		wmhints.o \
		wmname.o \
		xor.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap10 \
			bitmap.o \
			chap10.o \
			classhnt.o \
			connect.o \
			gc.o \
			icon.o \
			key.o \
			oval.o \
			pixmap.o \
			sizehint.o \
			window.o \
			wmhints.o \
			wmname.o \
			xor.o \
			-lX11 \
			$(ENDLIST)

chap1:		chap1.o \
		connect.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap1 \
			chap1.o \
			connect.o \
			-lX11 \
			$(ENDLIST)

chap2:		chap2.o \
		classhnt.o \
		connect.o \
		sizehint.o \
		window.o \
		wmhints.o \
		wmname.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap2 \
			chap2.o \
			classhnt.o \
			connect.o \
			sizehint.o \
			window.o \
			wmhints.o \
			wmname.o \
			-lX11 \
			$(ENDLIST)

chap3:		chap3.o \
		classhnt.o \
		connect.o \
		sizehint.o \
		window.o \
		wmhints.o \
		wmname.o \
		gc.o \
		oval.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap3 \
			chap3.o \
			classhnt.o \
			connect.o \
			sizehint.o \
			window.o \
			wmhints.o \
			wmname.o \
			gc.o \
			oval.o \
			-lX11 \
			$(ENDLIST)

chap4:		chap4.o \
		classhnt.o \
		connect.o \
		sizehint.o \
		window.o \
		wmhints.o \
		wmname.o \
		gc.o \
		oval.o \
		colormap.o \
		color.o \
		visual.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap4 \
			chap4.o \
			classhnt.o \
			connect.o \
			sizehint.o \
			window.o \
			wmhints.o \
			wmname.o \
			gc.o \
			oval.o \
			colormap.o \
			color.o \
			visual.o \
			-lX11 \
			$(ENDLIST)

chap5:		chap5.o \
		classhnt.o \
		connect.o \
		sizehint.o \
		window.o \
		wmhints.o \
		wmname.o \
		gc.o \
		oval.o \
		colormap.o \
		color.o \
		visual.o \
		loadfont.o \
		fontht.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap5 \
			chap5.o \
			classhnt.o \
			connect.o \
			sizehint.o \
			window.o \
			wmhints.o \
			wmname.o \
			gc.o \
			oval.o \
			colormap.o \
			color.o \
			visual.o \
			loadfont.o \
			fontht.o \
			-lX11 \
			$(ENDLIST)

chap6:		chap6.o \
		classhnt.o \
		connect.o \
		sizehint.o \
		window.o \
		wmhints.o \
		wmname.o \
		gc.o \
		loadfont.o \
		fontht.o \
		drawstr.o \
		append.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap6 \
			chap6.o \
			classhnt.o \
			connect.o \
			sizehint.o \
			window.o \
			wmhints.o \
			wmname.o \
			gc.o \
			loadfont.o \
			fontht.o \
			drawstr.o \
			append.o \
			-lX11 \
			$(ENDLIST)

chap7:		chap7.o \
		classhnt.o \
		connect.o \
		sizehint.o \
		window.o \
		wmhints.o \
		wmname.o \
		gc.o \
		loadfont.o \
		fontht.o \
		drawstr.o \
		append.o \
		key.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap7 \
			chap7.o \
			classhnt.o \
			connect.o \
			sizehint.o \
			window.o \
			wmhints.o \
			wmname.o \
			gc.o \
			loadfont.o \
			fontht.o \
			drawstr.o \
			append.o \
			key.o \
			-lX11 \
			$(ENDLIST)

chap8:		chap8.o \
		classhnt.o \
		connect.o \
		drawstr.o \
		fontht.o \
		gc.o \
		key.o \
		loadfont.o \
		sizehint.o \
		window.o \
		wmhints.o \
		wmname.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap8 \
			chap8.o \
			classhnt.o \
			connect.o \
			drawstr.o \
			fontht.o \
			gc.o \
			key.o \
			loadfont.o \
			sizehint.o \
			window.o \
			wmhints.o \
			wmname.o \
			-lX11 \
			$(ENDLIST)

chap9:		bitmap.o \
		chap9.o \
		classhnt.o \
		connect.o \
		drawstr.o \
		fontht.o \
		gc.o \
		icon.o \
		key.o \
		pixmap.o \
		sizehint.o \
		window.o \
		wmhints.o \
		wmname.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap9 \
			bitmap.o \
			chap9.o \
			classhnt.o \
			connect.o \
			drawstr.o \
			fontht.o \
			gc.o \
			icon.o \
			key.o \
			pixmap.o \
			sizehint.o \
			window.o \
			wmhints.o \
			wmname.o \
			-lX11 \
			$(ENDLIST)

xor.o:	xor.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap1.o:	chap1.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

connect.o:	connect.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap6.o:	chap6.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap2.o:	chap2.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

color.o:	color.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

sizehint.o:	sizehint.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

drawstr.o:	drawstr.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap7.o:	chap7.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

wmhints.o:	wmhints.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

icon.o:	icon.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

bitmap.o:	bitmap.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap3.o:	chap3.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

classhnt.o:	classhnt.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap8.o:	chap8.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

visual.o:	visual.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

oval.o:	oval.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

wmname.o:	wmname.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

fontht.o:	fontht.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap4.o:	chap4.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

colormap.o:	colormap.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

pixmap.o:	pixmap.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

gc.o:	gc.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

append.o:	append.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap9.o:	chap9.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

loadfont.o:	loadfont.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

key.o:	key.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap5.o:	chap5.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

window.o:	window.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

chap10.o:	chap10.c \
		./myxlib.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f chap10
		-rm -f chap1
		-rm -f chap2
		-rm -f chap3
		-rm -f chap4
		-rm -f chap5
		-rm -f chap6
		-rm -f chap7
		-rm -f chap8
		-rm -f chap9
		-rm -f xor.o
		-rm -f chap1.o
		-rm -f connect.o
		-rm -f chap6.o
		-rm -f chap2.o
		-rm -f color.o
		-rm -f sizehint.o
		-rm -f drawstr.o
		-rm -f chap7.o
		-rm -f wmhints.o
		-rm -f icon.o
		-rm -f bitmap.o
		-rm -f chap3.o
		-rm -f classhnt.o
		-rm -f chap8.o
		-rm -f visual.o
		-rm -f oval.o
		-rm -f wmname.o
		-rm -f fontht.o
		-rm -f chap4.o
		-rm -f colormap.o
		-rm -f pixmap.o
		-rm -f gc.o
		-rm -f append.o
		-rm -f chap9.o
		-rm -f loadfont.o
		-rm -f key.o
		-rm -f chap5.o
		-rm -f window.o
		-rm -f chap10.o

backup:		xor.c \
		chap1.c \
		connect.c \
		chap6.c \
		chap2.c \
		color.c \
		sizehint.c \
		drawstr.c \
		chap7.c \
		wmhints.c \
		icon.c \
		bitmap.c \
		chap3.c \
		classhnt.c \
		chap8.c \
		visual.c \
		oval.c \
		wmname.c \
		fontht.c \
		chap4.c \
		colormap.c \
		pixmap.c \
		gc.c \
		append.c \
		chap9.c \
		loadfont.c \
		key.c \
		chap5.c \
		window.c \
		chap10.c \
		./debug.h \
		./returns.h \
		./myxlib.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp xor.c $(BACKUPDIR)/xor.c
		cp chap1.c $(BACKUPDIR)/chap1.c
		cp connect.c $(BACKUPDIR)/connect.c
		cp chap6.c $(BACKUPDIR)/chap6.c
		cp chap2.c $(BACKUPDIR)/chap2.c
		cp color.c $(BACKUPDIR)/color.c
		cp sizehint.c $(BACKUPDIR)/sizehint.c
		cp drawstr.c $(BACKUPDIR)/drawstr.c
		cp chap7.c $(BACKUPDIR)/chap7.c
		cp wmhints.c $(BACKUPDIR)/wmhints.c
		cp icon.c $(BACKUPDIR)/icon.c
		cp bitmap.c $(BACKUPDIR)/bitmap.c
		cp chap3.c $(BACKUPDIR)/chap3.c
		cp classhnt.c $(BACKUPDIR)/classhnt.c
		cp chap8.c $(BACKUPDIR)/chap8.c
		cp visual.c $(BACKUPDIR)/visual.c
		cp oval.c $(BACKUPDIR)/oval.c
		cp wmname.c $(BACKUPDIR)/wmname.c
		cp fontht.c $(BACKUPDIR)/fontht.c
		cp chap4.c $(BACKUPDIR)/chap4.c
		cp colormap.c $(BACKUPDIR)/colormap.c
		cp pixmap.c $(BACKUPDIR)/pixmap.c
		cp gc.c $(BACKUPDIR)/gc.c
		cp append.c $(BACKUPDIR)/append.c
		cp chap9.c $(BACKUPDIR)/chap9.c
		cp loadfont.c $(BACKUPDIR)/loadfont.c
		cp key.c $(BACKUPDIR)/key.c
		cp chap5.c $(BACKUPDIR)/chap5.c
		cp window.c $(BACKUPDIR)/window.c
		cp chap10.c $(BACKUPDIR)/chap10.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./myxlib.h ./$(BACKUPDIR)/myxlib.h
		touch backup

print:		xor.c \
		chap1.c \
		connect.c \
		chap6.c \
		chap2.c \
		color.c \
		sizehint.c \
		drawstr.c \
		chap7.c \
		wmhints.c \
		icon.c \
		bitmap.c \
		chap3.c \
		classhnt.c \
		chap8.c \
		visual.c \
		oval.c \
		wmname.c \
		fontht.c \
		chap4.c \
		colormap.c \
		pixmap.c \
		gc.c \
		append.c \
		chap9.c \
		loadfont.c \
		key.c \
		chap5.c \
		window.c \
		chap10.c \
		./debug.h \
		./returns.h \
		./myxlib.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			xor.c \
			chap1.c \
			connect.c \
			chap6.c \
			chap2.c \
			color.c \
			sizehint.c \
			drawstr.c \
			chap7.c \
			wmhints.c \
			icon.c \
			bitmap.c \
			chap3.c \
			classhnt.c \
			chap8.c \
			visual.c \
			oval.c \
			wmname.c \
			fontht.c \
			chap4.c \
			colormap.c \
			pixmap.c \
			gc.c \
			append.c \
			chap9.c \
			loadfont.c \
			key.c \
			chap5.c \
			window.c \
			chap10.c \
			./debug.h \
			./returns.h \
			./myxlib.h \
			$(ENDLIST)
