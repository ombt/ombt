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

PRODS = \
	iterativereactor \
	iterativereactor2 \
	iterativereactor3 \
	iterativereactor4 \
	iterativereactor5 \
	iterativereactor6 \
	iterativereactor7 \
	iterativereactor8 \
	iterativereactor9 \

CXXEXTRAFLAGS = -pthread 

TESTLIBS = logging threads reactors signals files timers sockets parsing

include $(ROOT)/build/makefile.test2
