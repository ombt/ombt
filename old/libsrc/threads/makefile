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

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = threads

FILES = SimpleThread.cpp 

HDRS = EventQueue.h ThreadBarrier.h ThreadConditionVariable.h ThreadMutex.h ThreadSafePtr.h

include $(ROOT)/build/makefile.lib2

SimpleThread.o:	SimpleThread.cpp \
		SimpleThread.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/logging/Logging.h
