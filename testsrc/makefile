# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/..
endif

include $(ROOT)/build/makefile.common

TESTSUBDIRS = \
	atomic \
	threads \
	adt \
	complex \
	gstring \
	interpolation \
	matrix \
	numerics \
	timers \
	sockets \
	reactors \
	servers \
	random \
	files \
	gstring2 \

include $(ROOT)/build/makefile.testsubdirs
