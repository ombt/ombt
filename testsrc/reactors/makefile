# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = \
	iterativereactor \
	iterativereactor2 \
	iterativereactor3 \
	iterativereactor4 \
	iterativereactor5 \
	iterativereactor6 \
	iterativereactor7 \
	iterativereactor8 \
	iterativereactor9 \

CXXEXTRAFLAGS = 
CXXLIBFLAGS = -pthread -lombtlogging -lombtthreads -lombtreactors -lombtsockets -lombttimers -lombtsignals -lombtfiles -lombtparsing

include $(ROOT)/build/makefile.test
