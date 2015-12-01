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

LIBNAME = sockets

FILES = Socket.cpp Address.cpp InetAddress.cpp LocalAddress.cpp EndPoint.cpp Service.cpp

include $(ROOT)/build/makefile.lib2

Socket.o:	Socket.cpp \
		Socket.h \
		EndPoint.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Service.o:	Service.cpp \
		Service.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Address.o:	Address.cpp \
		Address.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/parsing/Tokenizer.h

InetAddress.o:	InetAddress.cpp \
		InetAddress.h \
		Address.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

LocalAddress.o:	LocalAddress.cpp \
		LocalAddress.h \
		Address.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

EndPoint.o:	EndPoint.cpp \
		EndPoint.h \
		Socket.h \
		Address.h \
		LocalAddress.h \
		InetAddress.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Connection.o:	Connection.cpp \
		Connection.h \
		Socket.h \
		Address.h \
		LocalAddress.h \
		InetAddress.h \
		$(INCLUDEDIR)/atomic/BaseObject.h
