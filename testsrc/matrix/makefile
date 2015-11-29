# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = tgausslup2 tgausslup tmatrix tmatrixops ttridiagonal tvector

CXXEXTRAFLAGS = -Wfatal-errors

include $(ROOT)/build/makefile.test
