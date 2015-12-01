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

PRODS = \
	udpechoserver \
	udpechoclient \
	tcpechoserver \
	tcpechoserver2 \
	tcpechoclient \
	tcpechoclient2 \
	streamechoserver \
	streamechoclient \
	datagramechoserver \
	datagramechoclient \
	datagramcombo \
	udpcombo \
	datagramcombo2 \
	streamcombo \
	streamcombo2 \
	tcpcombo \
	tcpcombo2 \
	tcpcombogc

CXXEXTRAFLAGS = -lombtsockets -pthread -lombtlogging -lombtparsing -lombtthreads -lombttimers
TESTLIBS = sockets logging parsing threads timers

include $(ROOT)/build/makefile.test2
