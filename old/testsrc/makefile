# ROOT = /home/ombt/ombt

ifndef ROOT
	$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

TESTSUBDIRS = \
	sockets \
	servers \
	threads \
	timers \
	complex \
	matrix \
	adt \
	atomic \
	numerics \
	interpolation \
	gstring \
	frameworks


include $(ROOT)/build/makefile.testsubdirs
