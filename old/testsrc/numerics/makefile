#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

PRODS = tpolicy trational tsignedint2 tsignedint3 tsignedint tsignedintrational tunsignedint2 tunsignedint3 tunsignedint

CXXEXTRAFLAGS = -DRATIONAL_USE_DIV_GCD -DUSEEXCEPTION 
# CXXEXTRAFLAGS += -Wfatal-errors

include $(ROOT)/build/makefile.test
