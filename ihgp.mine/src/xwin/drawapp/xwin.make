CC = cc
CFLAGS = -g -I/opt/x11r5/include -DX11R4 -DDEBUG 
HDRS = myxlib.h debug.h returns.h toolkit.h drawapp.h

all:		chap1 chap2 chap3 chap4 chap5 chap6 chap7 chap8 chap9 chap10 \
		chap11 chap12 chap13 drawapp
		echo "all done" >all

chap1:		chap1.o \
		connect.o
		$(CC) $(CFLAGS) -o chap1 \
			chap1.o \
			connect.o \
			-lX11

chap2:		chap2.o \
		classhnt.o \
		connect.o \
		sizehint.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap2 \
			chap2.o \
			classhnt.o \
			connect.o \
			sizehint.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

chap3:		chap3.o \
		classhnt.o \
		connect.o \
		gc.o \
		oval.o \
		sizehint.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap3 \
			chap3.o \
			classhnt.o \
			connect.o \
			gc.o \
			oval.o \
			sizehint.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

chap4:		chap4.o \
		classhnt.o \
		color.o \
		colormap.o \
		connect.o \
		gc.o \
		oval.o \
		sizehint.o \
		visual.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap4 \
			chap4.o \
			classhnt.o \
			color.o \
			colormap.o \
			connect.o \
			gc.o \
			oval.o \
			sizehint.o \
			visual.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

chap5:		chap5.o \
		classhnt.o \
		connect.o \
		fontht.o \
		gc.o \
		loadfont.o \
		sizehint.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap5 \
			chap5.o \
			classhnt.o \
			connect.o \
			fontht.o \
			gc.o \
			loadfont.o \
			sizehint.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

chap6:		chap6.o \
		append.o \
		classhnt.o \
		connect.o \
		drawstr.o \
		gc.o \
		loadfont.o \
		sizehint.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap6 \
			chap6.o \
			append.o \
			classhnt.o \
			connect.o \
			drawstr.o \
			gc.o \
			loadfont.o \
			sizehint.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

chap7:		chap7.o \
		append.o \
		classhnt.o \
		connect.o \
		drawstr.o \
		gc.o \
		key.o \
		loadfont.o \
		sizehint.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap7 \
			chap7.o \
			append.o \
			classhnt.o \
			connect.o \
			drawstr.o \
			gc.o \
			key.o \
			loadfont.o \
			sizehint.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

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
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap8 \
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
			wmname.o \
			wmhints.o \
			-lX11

chap9:		chap9.o \
		bitmap.o \
		classhnt.o \
		connect.o \
		gc.o \
		icon.o \
		key.o \
		pixmap.o \
		sizehint.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap9 \
			chap9.o \
			bitmap.o \
			classhnt.o \
			connect.o \
			gc.o \
			icon.o \
			key.o \
			pixmap.o \
			sizehint.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

chap10:		chap10.o \
		bitmap.o \
		classhnt.o \
		connect.o \
		gc.o \
		icon.o \
		key.o \
		oval.o \
		pixmap.o \
		sizehint.o \
		window.o \
		wmname.o \
		wmhints.o \
		xor.o
		$(CC) $(CFLAGS) -o chap10 \
			chap10.o \
			bitmap.o \
			classhnt.o \
			connect.o \
			gc.o \
			icon.o \
			key.o \
			oval.o \
			pixmap.o \
			sizehint.o \
			window.o \
			wmname.o \
			wmhints.o \
			xor.o \
			-lX11

chap11:		chap11.o \
		args.o \
		bitmap.o \
		classhnt.o \
		connect.o \
		display.o \
		drawstr.o \
		fontht.o \
		fontname.o \
		gc.o \
		geometry.o \
		icon.o \
		key.o \
		loadfont.o \
		pixmap.o \
		sizehint.o \
		usage.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap11 \
			chap11.o \
			args.o \
			bitmap.o \
			classhnt.o \
			connect.o \
			display.o \
			drawstr.o \
			fontht.o \
			fontname.o \
			gc.o \
			geometry.o \
			icon.o \
			key.o \
			loadfont.o \
			pixmap.o \
			sizehint.o \
			usage.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

chap12:		chap12.o \
		args.o \
		bitmap.o \
		classhnt.o \
		connect.o \
		display.o \
		drawstr.o \
		error.o \
		fontht.o \
		fontname.o \
		gc.o \
		geometry.o \
		icon.o \
		key.o \
		loadfont.o \
		pixmap.o \
		sizehint.o \
		topwind.o \
		usage.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap12 \
			chap12.o \
			args.o \
			bitmap.o \
			classhnt.o \
			connect.o \
			display.o \
			drawstr.o \
			error.o \
			fontht.o \
			fontname.o \
			gc.o \
			geometry.o \
			icon.o \
			key.o \
			loadfont.o \
			pixmap.o \
			sizehint.o \
			topwind.o \
			usage.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

chap13:		chap13.o \
		args.o \
		classhnt.o \
		connect.o \
		display.o \
		error.o \
		gc.o \
		geometry.o \
		sizehint.o \
		startupx.o \
		topwind.o \
		usage.o \
		window.o \
		wmname.o \
		wmhints.o
		$(CC) $(CFLAGS) -o chap13 \
			chap13.o \
			args.o \
			classhnt.o \
			connect.o \
			display.o \
			error.o \
			gc.o \
			geometry.o \
			sizehint.o \
			startupx.o \
			topwind.o \
			usage.o \
			window.o \
			wmname.o \
			wmhints.o \
			-lX11

drawapp:	args.o \
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
		wmname.o \
		wmhints.o \
		xor.o
		$(CC) $(CFLAGS) -o drawapp \
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
			wmname.o \
			wmhints.o \
			xor.o \
			-lX11

chap1.o:	chap1.c $(HDRS)
		$(CC) $(CFLAGS) -c chap1.c

chap2.o:	chap2.c $(HDRS)
		$(CC) $(CFLAGS) -c chap2.c

chap3.o:	chap3.c $(HDRS)
		$(CC) $(CFLAGS) -c chap3.c

chap4.o:	chap4.c $(HDRS)
		$(CC) $(CFLAGS) -c chap4.c

chap5.o:	chap5.c $(HDRS)
		$(CC) $(CFLAGS) -c chap5.c

chap6.o:	chap6.c $(HDRS)
		$(CC) $(CFLAGS) -c chap6.c

chap7.o:	chap7.c $(HDRS)
		$(CC) $(CFLAGS) -c chap7.c

chap8.o:	chap8.c $(HDRS)
		$(CC) $(CFLAGS) -c chap8.c

chap9.o:	chap9.c $(HDRS)
		$(CC) $(CFLAGS) -c chap9.c

chap10.o:	chap10.c $(HDRS)
		$(CC) $(CFLAGS) -c chap10.c

chap11.o:	chap11.c $(HDRS)
		$(CC) $(CFLAGS) -c chap11.c

chap12.o:	chap12.c $(HDRS)
		$(CC) $(CFLAGS) -c chap12.c

chap13.o:	chap13.c $(HDRS)
		$(CC) $(CFLAGS) -c chap13.c

append.o:	append.c $(HDRS)
		$(CC) $(CFLAGS) -c append.c

args.o:		args.c $(HDRS)
		$(CC) $(CFLAGS) -c args.c

classhnt.o:	classhnt.c $(HDRS)
		$(CC) $(CFLAGS) -c classhnt.c

color.o:	color.c $(HDRS)
		$(CC) $(CFLAGS) -c color.c

colormap.o:	colormap.c $(HDRS)
		$(CC) $(CFLAGS) -c colormap.c

connect.o:	connect.c $(HDRS)
		$(CC) $(CFLAGS) -c connect.c

da_draw.o:	da_draw.c $(HDRS)
		$(CC) $(CFLAGS) -c da_draw.c

da_edit.o:	da_edit.c $(HDRS)
		$(CC) $(CFLAGS) -c da_edit.c

da_file.o:	da_file.c $(HDRS)
		$(CC) $(CFLAGS) -c da_file.c

da_meta.o:	da_meta.c $(HDRS)
		$(CC) $(CFLAGS) -c da_meta.c

da_pix.o:	da_pix.c $(HDRS)
		$(CC) $(CFLAGS) -c da_pix.c

da_undo.o:	da_undo.c $(HDRS)
		$(CC) $(CFLAGS) -c da_undo.c

dialog.o:	dialog.c $(HDRS)
		$(CC) $(CFLAGS) -c dialog.c

display.o:	display.c $(HDRS)
		$(CC) $(CFLAGS) -c display.c

drawapp.o:	drawapp.c $(HDRS)
		$(CC) -DNOCOLOR $(CFLAGS) -c drawapp.c

drawstr.o:	drawstr.c $(HDRS)
		$(CC) $(CFLAGS) -c drawstr.c

entry.o:	entry.c $(HDRS)
		$(CC) $(CFLAGS) -c entry.c

error.o:	error.c $(HDRS)
		$(CC) $(CFLAGS) -c error.c

fontht.o:	fontht.c $(HDRS)
		$(CC) $(CFLAGS) -c fontht.c

fontname.o:	fontname.c $(HDRS)
		$(CC) $(CFLAGS) -c fontname.c

gc.o:		gc.c $(HDRS)
		$(CC) $(CFLAGS) -c gc.c

geometry.o:	geometry.c $(HDRS)
		$(CC) $(CFLAGS) -c geometry.c

key.o:		key.c $(HDRS)
		$(CC) $(CFLAGS) -c key.c

label.o:	label.c $(HDRS)
		$(CC) $(CFLAGS) -c label.c

loadfont.o:	loadfont.c $(HDRS)
		$(CC) $(CFLAGS) -c loadfont.c

oval.o:		oval.c $(HDRS)
		$(CC) $(CFLAGS) -c oval.c

pushb.o:	pushb.c $(HDRS)
		$(CC) $(CFLAGS) -c pushb.c

query.o:	query.c $(HDRS)
		$(CC) $(CFLAGS) -c query.c

sizehint.o:	sizehint.c $(HDRS)
		$(CC) $(CFLAGS) -c sizehint.c

startupx.o:	startupx.c $(HDRS)
		$(CC) $(CFLAGS) -c startupx.c

toolkit.o:	toolkit.c $(HDRS)
		$(CC) $(CFLAGS) -c toolkit.c

topwind.o:	topwind.c $(HDRS)
		$(CC) $(CFLAGS) -c topwind.c

tranwind.o:	tranwind.c $(HDRS)
		$(CC) $(CFLAGS) -c tranwind.c

visual.o:	visual.c $(HDRS)
		$(CC) $(CFLAGS) -c visual.c

usage.o:	usage.c $(HDRS)
		$(CC) $(CFLAGS) -c usage.c

window.o:	window.c $(HDRS)
		$(CC) $(CFLAGS) -c window.c

wmhints.o:	wmhints.c $(HDRS)
		$(CC) $(CFLAGS) -c wmhints.c

wmname.o:	wmname.c $(HDRS)
		$(CC) $(CFLAGS) -c wmname.c

xor.o:		xor.c $(HDRS)
		$(CC) $(CFLAGS) -c xor.c
