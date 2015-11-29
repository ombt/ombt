# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

# CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = files

FILES = Notify.cpp 

include $(ROOT)/build/makefile.lib2

Notify.o:	Notify.cpp \
		Notify.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/sockets/Socket.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \


