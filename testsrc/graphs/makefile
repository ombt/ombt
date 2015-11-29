# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = test1 test2

CXXEXTRAFLAGS = -lombtgraphs -lombtparsing

include $(ROOT)/build/makefile.test
