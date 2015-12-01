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

PRODS = tgstring tspell ttrie ttrie2 ttrie3 ttrie4

CXXEXTRAFLAGS = -Wfatal-errors

TESTLIBS = random parsing

include $(ROOT)/build/makefile.test2
