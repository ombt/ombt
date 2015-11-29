# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)
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
