#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

# SUBDIRS = \
	gcalc \
	gtk_tutorial \
	qt_book \
	qt_filewatcher \
	qt_mdivwer \
	qt_syncmgr

# with upgrade to ubuntu 12.04, the gtk version changed from
# 2.0 to 3.0. several gtk functions were deprecated and i need
# to update the code to use the new functions. not now, so i removed
# gcalc and gtk_tutorial for now.

SUBDIRS = \
	qt_book \
	qt_filewatcher \
	qt_mdivwer \
	qt_syncmgr

include $(ROOT)/build/makefile.subdirs
