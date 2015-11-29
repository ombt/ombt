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

CXXEXTRAFLAGS = -lombtsockets -pthread -lombtlogging -lombtparsing -lombtthreads -lombttimers -lombtreactors -lombtsignals -lombtfiles

include $(ROOT)/build/makefile.test
