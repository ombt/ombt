# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

# CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = signals

FILES = Signal.cpp 

include $(ROOT)/build/makefile.lib2

Signal.o:	Signal.cpp \
		Signal.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/sockets/Socket.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/LocalAddress.h \
		$(INCLUDEDIR)/sockets/InetAddress.h \
		$(INCLUDEDIR)/atomic/BaseObject.h


