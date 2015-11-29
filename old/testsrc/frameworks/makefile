# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

PRODS = test1 test2 test3 test4 test5 test6 test7 test8 test9 test10

CXXEXTRAFLAGS = -O0 -pthread -lombtthreads -lombtlogging -lombtrandom -lombtframeworks -lombttimers -lombtsockets -lombtparsing

include $(ROOT)/build/makefile.test
