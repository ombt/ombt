# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = atomic1 atomic2 atomic3 atomic4 atomic5 atomic6 usecnt1 usecnt2

CXXEXTRAFLAGS = -pthread -lombtlogging -lombtthreads

include $(ROOT)/build/makefile.test
