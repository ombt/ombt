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

LIBNAME = timers

FILES = Timer.cpp TimerQueue.cpp OldTimer.cpp OldTimerQueue.cpp

include $(ROOT)/build/makefile.lib2

Timer.o:	Timer.cpp \
		Timer.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

TimerQueue.o:	TimerQueue.cpp \
		TimerQueue.h \
		Timer.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

OldTimer.o:	OldTimer.cpp \
		OldTimer.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

OldTimerQueue.o:	OldTimerQueue.cpp \
		OldTimerQueue.h \
		OldTimer.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

