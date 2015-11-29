# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

PRODS = tgstring tspell ttrie

CXXEXTRAFLAGS = -lombtrandom
CXXEXTRAFLAGS += -Wfatal-errors

include $(ROOT)/build/makefile.test
