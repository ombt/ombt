# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = threads

FILES = \
	Job.cpp \
	SimpleThread.cpp \
	Task.cpp \
	Thread.cpp 

HDRS = \
	PriorityQueue.h \
	Queue.h \
	ThreadBarrier.h \
	ThreadConditionVariable.h \
	ThreadMutex.h \
	ThreadPool.h \
	ThreadSafePtr.h

include $(ROOT)/build/makefile.lib2

Job.o:	Job.cpp \
	Job.h \
	ThreadMutex.h \
	$(INCLUDEDIR)/Returns.h \
	$(INCLUDEDIR)/Debug.h \
	$(INCLUDEDIR)/atomic/UCBaseObject.h

Task.o:	Task.cpp \
	Task.h \
	$(INCLUDEDIR)/Returns.h \
	$(INCLUDEDIR)/Debug.h \
	$(INCLUDEDIR)/Returns.h \
	$(INCLUDEDIR)/Debug.h \
	$(INCLUDEDIR)/atomic/UCBaseObject.h \
	$(INCLUDEDIR)/atomic/UseCntPtr.h

SimpleThread.o:	SimpleThread.cpp \
		SimpleThread.h \
		ThreadMutex.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h

Thread.o:	Thread.cpp \
		Thread.h \
		ThreadMutex.h \
		Task.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \

