# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

PRODS = udpechoserver udpechoclient \
	tcpechoserver tcpechoclient tcpechoclient2 \
	streamechoserver streamechoclient \
	datagramechoserver datagramechoclient 

CXXEXTRAFLAGS = -lombtsockets -pthread -lombtlogging -lombtparsing

include $(ROOT)/build/makefile.test
