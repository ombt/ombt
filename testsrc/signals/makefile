# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = \
	sigtest1 \
	sigtest2 \

CXXEXTRAFLAGS = -lombtsockets -pthread -lombtlogging -lombtparsing -lombtthreads -lombttimers -lombtreactors -lombtsignals -lombtsockets 

include $(ROOT)/build/makefile.test
