# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

# CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = frameworks

FILES = Signal.cpp Task.cpp Thread.cpp Job.cpp IterativeServer.cpp

HDRS = Queue.h BufferQueue.h Client.h Event.h Message.h PriorityQueue.h Scheduler.h Server.h Timer.h ThreadPool.h

include $(ROOT)/build/makefile.lib2

Job.o:		Job.cpp \
		Job.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Signal.o:	Signal.cpp \
		Signal.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Task.o:		Task.cpp \
		Task.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Thread.o:	Thread.cpp \
		Thread.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/threads/ThreadMutex.h \
		$(INCLUDEDIR)/frameworks/Task.h

IterativeServer.o:	IterativeServer.cpp \
		IterativeServer.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/timers/Timer.h \
		$(INCLUDEDIR)/timers/TimerQueue.h

