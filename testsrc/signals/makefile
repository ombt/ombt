# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = \
	sigtest1 \
	sigtest2 \

# CXXEXTRAFLAGS = -lombtsockets -pthread -lombtlogging -lombtparsing -lombtthreads -lombttimers -lombtreactors -lombtsignals -lombtsockets 
CXXEXTRAFLAGS = 
CXXLIBFLAGS = -pthread -lombtlogging -lombtthreads -lombttimers -lombtreactors -lombtsignals -lombtsockets -lombtparsing

include $(ROOT)/build/makefile.test
