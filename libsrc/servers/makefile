# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
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

IterativeServer.o:	\
		IterativeServer.cpp \
		IterativeServer.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/sockets/EndPoint.h 

IterativeMultiServiceServer.o:	\
		IterativeMultiServiceServer.cpp \
		IterativeMultiServiceServer.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/timers/Timer.h \
		$(INCLUDEDIR)/adt/Tuple.h \
		$(INCLUDEDIR)/adt/PriorityQueue_Array.h \
		$(INCLUDEDIR)/logging/Logging.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h 

MultiProcessServer.o:	\
		MultiProcessServer.cpp \
		MultiProcessServer.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h 

MultiProcessMultiServiceServer.o: \
		MultiProcessMultiServiceServer.cpp \
		MultiProcessMultiServiceServer.h \
		Server.h \
		$(INCLUDEDIR)/adt/PriorityQueue_Array.h \
		$(INCLUDEDIR)/adt/Tuple.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/timers/Timer.h

IterativeServerPerProcess.o: \
		IterativeServerPerProcess.cpp \
		IterativeServerPerProcess.h \
		IterativeServer.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/sockets/Service.h \
		$(INCLUDEDIR)/parsing/Tokenizer.h \
		$(INCLUDEDIR)/logging/Logging.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h 

IterativeMultiServiceServerPerProcess.o:	\
		IterativeMultiServiceServerPerProcess.cpp \
		IterativeMultiServiceServer.h \
		IterativeMultiServiceServerPerProcess.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/logging/Logging.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/sockets/Service.h 

MultiProcessMultiServiceServerPerProcess.o:	\
		MultiProcessMultiServiceServerPerProcess.cpp \
		MultiProcessMultiServiceServerPerProcess.h \
		MultiProcessMultiServiceServer.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/logging/Logging.h \
		$(INCLUDEDIR)/parsing/Tokenizer.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/sockets/Service.h 

MultiProcessMultiServiceServer.o:	\
		MultiProcessMultiServiceServer.cpp \
		MultiProcessMultiServiceServer.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/adt/PriorityQueue_Array.h \
		$(INCLUDEDIR)/adt/Tuple.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/timers/Timer.h 

MultiProcessServerPerProcess.o:	\
		MultiProcessServerPerProcess.cpp \
		MultiProcessServerPerProcess.h \
		MultiProcessServer.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/logging/Logging.h \
		$(INCLUDEDIR)/sockets/Address.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/sockets/Service.h

InitServer.o: \
		InitServer.cpp \
		InitServer.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/logging/Logging.h

MultiThreadServer.o:	\
		MultiThreadServer.cpp \
		MultiThreadServer.h \
		Server.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/atomic/UCBaseObject.h \
		$(INCLUDEDIR)/atomic/UseCntPtr.h \
		$(INCLUDEDIR)/sockets/EndPoint.h \
		$(INCLUDEDIR)/threads/SimpleThread.h
