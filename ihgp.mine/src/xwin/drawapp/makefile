
.SUFFIXES:		.o .c

LEXFLAGS = -n
YACC = yacc
YACCFLAGS = -d -v
LEX = lex
MAKE = make
cc = CC
LDFLAGS = -g
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g
INCLDIRS = -I/opt/x11r5/include
CC = cc
AR = ar
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		drawapp \
		$(ENDLIST)
		touch all

drawapp:		append.o \
		args.o \
		bitmap.o \
		classhnt.o \
		color.o \
		colormap.o \
		connect.o \
		da_draw.o \
		da_edit.o \
		da_file.o \
		da_meta.o \
		da_pix.o \
		da_undo.o \
		dialog.o \
		display.o \
		drawapp.o \
		drawstr.o \
		entry.o \
		error.o \
		fontht.o \
		fontname.o \
		gc.o \
		geometry.o \
		icon.o \
		key.o \
		label.o \
		loadfont.o \
		oval.o \
		pixmap.o \
		pushb.o \
		query.o \
		sizehint.o \
		startupx.o \
		toolkit.o \
		topwind.o \
		tranwind.o \
		usage.o \
		visual.o \
		window.o \
		wmhints.o \
		wmname.o \
		xor.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o drawapp \
			append.o \
			args.o \
			bitmap.o \
			classhnt.o \
			color.o \
			colormap.o \
			connect.o \
			da_draw.o \
			da_edit.o \
			da_file.o \
			da_meta.o \
			da_pix.o \
			da_undo.o \
			dialog.o \
			display.o \
			drawapp.o \
			drawstr.o \
			entry.o \
			error.o \
			fontht.o \
			fontname.o \
			gc.o \
			geometry.o \
			icon.o \
			key.o \
			label.o \
			loadfont.o \
			oval.o \
			pixmap.o \
			pushb.o \
			query.o \
			sizehint.o \
			startupx.o \
			toolkit.o \
			topwind.o \
			tranwind.o \
			usage.o \
			visual.o \
			window.o \
			wmhints.o \
			wmname.o \
			xor.o \
			-lX11 \
			$(ENDLIST)

xor.o:	xor.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

drawapp.o:	drawapp.c \
		./debug.h \
		./drawapp.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

error.o:	error.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

display.o:	display.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

connect.o:	connect.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

args.o:	args.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

toolkit.o:	toolkit.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

color.o:	color.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

pushb.o:	pushb.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

fontname.o:	fontname.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

da_draw.o:	da_draw.c \
		./debug.h \
		./drawapp.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

sizehint.o:	sizehint.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

da_edit.o:	da_edit.c \
		./debug.h \
		./drawapp.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

drawstr.o:	drawstr.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

wmhints.o:	wmhints.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

query.o:	query.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

icon.o:	icon.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

bitmap.o:	bitmap.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

classhnt.o:	classhnt.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

visual.o:	visual.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

dialog.o:	dialog.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

oval.o:	oval.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

da_file.o:	da_file.c \
		./debug.h \
		./drawapp.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

wmname.o:	wmname.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

da_undo.o:	da_undo.c \
		./debug.h \
		./drawapp.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

fontht.o:	fontht.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

usage.o:	usage.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

colormap.o:	colormap.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

pixmap.o:	pixmap.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

da_meta.o:	da_meta.c \
		./debug.h \
		./drawapp.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

geometry.o:	geometry.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

gc.o:	gc.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

append.o:	append.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

tranwind.o:	tranwind.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

topwind.o:	topwind.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

entry.o:	entry.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

startupx.o:	startupx.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

loadfont.o:	loadfont.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

key.o:	key.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

window.o:	window.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		$(ENDLIST)

label.o:	label.c \
		./debug.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

da_pix.o:	da_pix.c \
		./debug.h \
		./drawapp.h \
		./myxlib.h \
		./returns.h \
		./toolkit.h \
		$(ENDLIST)

clean:
		-rm -f drawapp
		-rm -f xor.o
		-rm -f drawapp.o
		-rm -f error.o
		-rm -f display.o
		-rm -f connect.o
		-rm -f args.o
		-rm -f toolkit.o
		-rm -f color.o
		-rm -f pushb.o
		-rm -f fontname.o
		-rm -f da_draw.o
		-rm -f sizehint.o
		-rm -f da_edit.o
		-rm -f drawstr.o
		-rm -f wmhints.o
		-rm -f query.o
		-rm -f icon.o
		-rm -f bitmap.o
		-rm -f classhnt.o
		-rm -f visual.o
		-rm -f dialog.o
		-rm -f oval.o
		-rm -f da_file.o
		-rm -f wmname.o
		-rm -f da_undo.o
		-rm -f fontht.o
		-rm -f usage.o
		-rm -f colormap.o
		-rm -f pixmap.o
		-rm -f da_meta.o
		-rm -f geometry.o
		-rm -f gc.o
		-rm -f append.o
		-rm -f tranwind.o
		-rm -f topwind.o
		-rm -f entry.o
		-rm -f startupx.o
		-rm -f loadfont.o
		-rm -f key.o
		-rm -f window.o
		-rm -f label.o
		-rm -f da_pix.o

backup:		xor.c \
		drawapp.c \
		error.c \
		display.c \
		connect.c \
		args.c \
		toolkit.c \
		color.c \
		pushb.c \
		fontname.c \
		da_draw.c \
		sizehint.c \
		da_edit.c \
		drawstr.c \
		wmhints.c \
		query.c \
		icon.c \
		bitmap.c \
		classhnt.c \
		visual.c \
		dialog.c \
		oval.c \
		da_file.c \
		wmname.c \
		da_undo.c \
		fontht.c \
		usage.c \
		colormap.c \
		pixmap.c \
		da_meta.c \
		geometry.c \
		gc.c \
		append.c \
		tranwind.c \
		topwind.c \
		entry.c \
		startupx.c \
		loadfont.c \
		key.c \
		window.c \
		label.c \
		da_pix.c \
		./debug.h \
		./drawapp.h \
		./toolkit.h \
		./returns.h \
		./myxlib.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp xor.c $(BACKUPDIR)/xor.c
		cp drawapp.c $(BACKUPDIR)/drawapp.c
		cp error.c $(BACKUPDIR)/error.c
		cp display.c $(BACKUPDIR)/display.c
		cp connect.c $(BACKUPDIR)/connect.c
		cp args.c $(BACKUPDIR)/args.c
		cp toolkit.c $(BACKUPDIR)/toolkit.c
		cp color.c $(BACKUPDIR)/color.c
		cp pushb.c $(BACKUPDIR)/pushb.c
		cp fontname.c $(BACKUPDIR)/fontname.c
		cp da_draw.c $(BACKUPDIR)/da_draw.c
		cp sizehint.c $(BACKUPDIR)/sizehint.c
		cp da_edit.c $(BACKUPDIR)/da_edit.c
		cp drawstr.c $(BACKUPDIR)/drawstr.c
		cp wmhints.c $(BACKUPDIR)/wmhints.c
		cp query.c $(BACKUPDIR)/query.c
		cp icon.c $(BACKUPDIR)/icon.c
		cp bitmap.c $(BACKUPDIR)/bitmap.c
		cp classhnt.c $(BACKUPDIR)/classhnt.c
		cp visual.c $(BACKUPDIR)/visual.c
		cp dialog.c $(BACKUPDIR)/dialog.c
		cp oval.c $(BACKUPDIR)/oval.c
		cp da_file.c $(BACKUPDIR)/da_file.c
		cp wmname.c $(BACKUPDIR)/wmname.c
		cp da_undo.c $(BACKUPDIR)/da_undo.c
		cp fontht.c $(BACKUPDIR)/fontht.c
		cp usage.c $(BACKUPDIR)/usage.c
		cp colormap.c $(BACKUPDIR)/colormap.c
		cp pixmap.c $(BACKUPDIR)/pixmap.c
		cp da_meta.c $(BACKUPDIR)/da_meta.c
		cp geometry.c $(BACKUPDIR)/geometry.c
		cp gc.c $(BACKUPDIR)/gc.c
		cp append.c $(BACKUPDIR)/append.c
		cp tranwind.c $(BACKUPDIR)/tranwind.c
		cp topwind.c $(BACKUPDIR)/topwind.c
		cp entry.c $(BACKUPDIR)/entry.c
		cp startupx.c $(BACKUPDIR)/startupx.c
		cp loadfont.c $(BACKUPDIR)/loadfont.c
		cp key.c $(BACKUPDIR)/key.c
		cp window.c $(BACKUPDIR)/window.c
		cp label.c $(BACKUPDIR)/label.c
		cp da_pix.c $(BACKUPDIR)/da_pix.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./drawapp.h ./$(BACKUPDIR)/drawapp.h
		cp ./toolkit.h ./$(BACKUPDIR)/toolkit.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./myxlib.h ./$(BACKUPDIR)/myxlib.h
		touch backup
