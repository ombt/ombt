# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

# CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = reactors

FILES = IterativeReactor.cpp

include $(ROOT)/build/makefile.lib2

IterativeReactor.o:	IterativeReactor.cpp \
			IterativeReactor.h \
			$(INCLUDEDIR)/Returns.h \
			$(INCLUDEDIR)/Debug.h \
			$(INCLUDEDIR)/sockets/Address.h \
			$(INCLUDEDIR)/sockets/EndPoint.h \
			$(INCLUDEDIR)/signals/Signal.h \
			$(INCLUDEDIR)/atomic/UCBaseObject.h \
			$(INCLUDEDIR)/atomic/UseCntPtr.h \
			$(INCLUDEDIR)/timers/Timer.h \
			$(INCLUDEDIR)/logging/Logging.h \

