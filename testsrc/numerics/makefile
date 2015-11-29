# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = tpolicy trational tsignedint2 tsignedint3 tsignedint tsignedintrational tunsignedint2 tunsignedint3 tunsignedint

CXXEXTRAFLAGS = -DRATIONAL_USE_DIV_GCD -DUSEEXCEPTION 
# CXXEXTRAFLAGS += -Wfatal-errors

include $(ROOT)/build/makefile.test
