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

LIBNAME = servers

FILES = IterativeServer.cpp \
	IterativeMultiServiceServer.cpp \
	MultiProcessServer.cpp \
	MultiProcessMultiServiceServer.cpp \
	IterativeServerPerProcess.cpp \
	IterativeMultiServiceServerPerProcess.cpp \
	MultiProcessServerPerProcess.cpp \
	MultiProcessMultiServiceServerPerProcess.cpp \
	InitServer.cpp \
	MultiThreadServer.cpp

HDRS = Server.h 

include $(ROOT)/build/makefile.lib2

IterativeServer.o:	IterativeServer.cpp \
		IterativeServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/sockets/EndPoint.h 

IterativeMultiServiceServer.o:	IterativeMultiServiceServer.cpp \
		IterativeMultiServiceServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/timers/Timer.h \
		$(INCLUDEDIR)/timers/TimerQueue.h 

IterativeMultiServiceServerPerProcess.o:	\
		IterativeMultiServiceServerPerProcess.cpp \
		IterativeMultiServiceServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/timers/Timer.h \
		$(INCLUDEDIR)/timers/TimerQueue.h \
		$(INCLUDEDIR)/sockets/Service.h \
		$(INCLUDEDIR)/logging/Logging.h \
		$(INCLUDEDIR)/parsing/Tokenizer.h 

MultiProcessMultiServiceServerPerProcess.o:	\
		MultiProcessMultiServiceServerPerProcess.cpp \
		MultiProcessMultiServiceServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/timers/Timer.h \
		$(INCLUDEDIR)/timers/TimerQueue.h \
		$(INCLUDEDIR)/sockets/Service.h \
		$(INCLUDEDIR)/logging/Logging.h \
		$(INCLUDEDIR)/parsing/Tokenizer.h 

MultiProcessServer.o:	MultiProcessServer.cpp \
		MultiProcessServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/sockets/EndPoint.h 

MultiProcessMultiServiceServer.o:	MultiProcessMultiServiceServer.cpp \
		MultiProcessMultiServiceServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/timers/Timer.h \
		$(INCLUDEDIR)/timers/TimerQueue.h

IterativeServerPerProcess.o:	IterativeServerPerProcess.cpp \
		IterativeServerPerProcess.h \
		IterativeServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/Service.h \
		$(INCLUDEDIR)/logging/Logging.h \
		$(INCLUDEDIR)/parsing/Tokenizer.h 

MultiProcessServerPerProcess.o:	MultiProcessServerPerProcess.cpp \
		MultiProcessServerPerProcess.h \
		MultiProcessServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/Service.h \
		$(INCLUDEDIR)/logging/Logging.h \
		$(INCLUDEDIR)/parsing/Tokenizer.h 

InitServer.o:	InitServer.cpp \
		InitServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/logging/Logging.h

MultiThreadServer.o:	MultiThreadServer.cpp \
		MultiThreadServer.h \
		Server.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/threads/SimpleThread.h 

