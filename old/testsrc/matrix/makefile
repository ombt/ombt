#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

PRODS = tgausslup2 tgausslup tmatrix tmatrixops ttridiagonal tvector

CXXEXTRAFLAGS = -Wfatal-errors

include $(ROOT)/build/makefile.test
