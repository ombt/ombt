# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = \
	random1 \

CXXEXTRAFLAGS = 
CXXLIBFLAGS = -lombtrandom -lombtlogging

include $(ROOT)/build/makefile.test
