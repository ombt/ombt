# ROOT = /home/ombt/ombt

ifndef ROOT
	$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

SUBDIRS = \
	shsrc \
	perlsrc \
	hdrsrc \
	libsrc \
	src

TESTSUBDIRS = \
	testsrc 

include $(ROOT)/build/makefile.all
