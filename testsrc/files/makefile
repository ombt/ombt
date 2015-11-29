# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = \
	notify1 \
	notify2 \

CXXEXTRAFLAGS = 
CXXLIBFLAGS = -pthread -lombtlogging -lombtthreads -lombttimers -lombtreactors -lombtfiles -lombtsockets -lombtparsing

include $(ROOT)/build/makefile.test
