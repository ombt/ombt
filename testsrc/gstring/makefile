# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = tgstring tspell ttrie ttrie2 ttrie3 ttrie4

CXXEXTRAFLAGS = -Wfatal-errors
CXXLIBFLAGS = -lombtrandom -lombtparsing

include $(ROOT)/build/makefile.test
