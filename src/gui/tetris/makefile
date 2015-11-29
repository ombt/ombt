# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../../..
endif

include $(ROOT)/build/makefile.common

PRODS = tetris1 tetris2 tetris3 #tetris4

CXXEXTRAFLAGS = `pkg-config --cflags --libs gtk+-2.0` -lombtlogging

include $(ROOT)/build/makefile.src

