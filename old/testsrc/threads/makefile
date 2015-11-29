# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

PRODS = test1 test2 test3 test4 test5 test6 test7 test8

CXXEXTRAFLAGS = -pthread -lombtthreads -lombtlogging

include $(ROOT)/build/makefile.test
