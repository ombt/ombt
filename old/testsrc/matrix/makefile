# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

PRODS = tgausslup2 tgausslup tmatrix tmatrixops ttridiagonal tvector

CXXEXTRAFLAGS = -Wfatal-errors

include $(ROOT)/build/makefile.test
