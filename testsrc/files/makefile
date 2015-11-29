# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = \
	notify1 \
	notify2 \

CXXEXTRAFLAGS = -pthread -lombtlogging -lombtparsing -lombtthreads -lombttimers -lombtreactors -lombtfiles -lombtsockets 

include $(ROOT)/build/makefile.test
