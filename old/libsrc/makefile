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

SUBDIRS = \
	atomic \
	logging \
	parsing \
	adt \
	complex \
	matrix \
	random \
	timers \
	sockets \
	servers \
	threads \
	numerics \
	interpolation \
	gstring \
	frameworks

include $(ROOT)/build/makefile.libsubdirs
