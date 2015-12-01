#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

#PRODS = \
	datagramechoserver \
	datagramechoserverperproc \
	datagramepollmpmsechoserver \
	datagramepollmsechoserver2 \
	datagramepollmsechoserver3 \
	datagramepollmsechoserver \
	datagramepollmsserver \
	datagrammpmsechoserver \
	datagrammsechoserver2 \
	datagrammsechoserver3 \
	datagrammsechoserver \
	datagrammsechoserverperproc \
	datagrammsserver \
	datagramserver \
	initserver \
	streamechoserver \
	streamechoserverperproc \
	streamepollmpmsechoserver \
	streamepollmsechoserver2 \
	streamepollmsechoserver3 \
	streamepollmsechoserver \
	streamepollmsserver \
	streammpechoserver \
	streammpechoserverperproc \
	streammpmsechoserver \
	streammpmsechoserverperproc \
	streammsechoserver2 \
	streammsechoserver3 \
	streammsechoserver \
	streammsechoserverperproc \
	streammsserver \
	streamserver \
	streamthrdechoserver \
	tcpechoserver \
	tcpechoserverperproc \
	tcpepollmpmsechoserver \
	tcpepollmsechoserver2 \
	tcpepollmsechoserver \
	tcpmpechoserver \
	tcpmpmsechoserver \
	tcpmsechoserver2 \
	tcpmsechoserver \
	tcpserver \
	udpechoserver \
	udpechoserverperproc \
	udpepollmsechoserver2 \
	udpepollmsechoserver \
	udpmpmsechoserver \
	udpmsechoserver2 \
	udpmsechoserver \
	udpserver

PRODS = \
	datagramechoserver \
	datagramechoserverperproc \
	datagramepollmpmsechoserver \
	datagramepollmsechoserver2 \
	datagramepollmsechoserver3 \
	datagramepollmsechoserver \
	datagramepollmsserver \
	datagrammpmsechoserver \
	datagrammsechoserver2 \
	datagrammsechoserver3 \
	datagrammsechoserver \
	datagrammsechoserverperproc \
	datagrammsserver \
	datagramserver \
	initserver \
	streamechoserver  \
	streamechoserverperproc \
	streamepollmpmsechoserver \
	streamepollmsechoserver2 \
	streamepollmsechoserver3 \
	streamepollmsechoserver \
	streamepollmsserver \
	streammpechoserver \
	streammpechoserverperproc \
	streammpmsechoserver \
	streammpmsechoserverperproc \
	streammsechoserver2 \
	streammsechoserver3 \
	streammsechoserver \
	streammsechoserverperproc \
	streammsserver \
	streamserver \
	streamthrdechoserver \
	tcpechoserver \
	tcpechoserverperproc \
	tcpepollmpmsechoserver \
	tcpepollmsechoserver2 \
	tcpepollmsechoserver \
	tcpmpechoserver \
	tcpmpmsechoserver \
	tcpmsechoserver2 \
	tcpmsechoserver \
	tcpserver \
	udpechoserver \
	udpechoserverperproc \
	udpepollmsechoserver2 \
	udpepollmsechoserver \
	udpmpmsechoserver \
	udpmsechoserver2 \
	udpmsechoserver \
	udpserver

CXXEXTRAFLAGS = -pthread 

TESTLIBS = servers timers logging threads sockets parsing

include $(ROOT)/build/makefile.test2

