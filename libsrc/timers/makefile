# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = timers

FILES = Timer.cpp TimerQueue.cpp OldTimer.cpp OldTimerQueue.cpp

include $(ROOT)/build/makefile.lib2

Timer.o:	Timer.cpp \
		Timer.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

TimerQueue.o:	TimerQueue.cpp \
		TimerQueue.h \
		Timer.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h

OldTimer.o:	OldTimer.cpp \
		OldTimer.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

OldTimerQueue.o:	OldTimerQueue.cpp \
		OldTimerQueue.h \
		OldTimer.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h

