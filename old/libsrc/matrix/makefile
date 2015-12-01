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

LIBNAME = matrix

HDRS = \
	Epsilon.cpp \
	Epsilon.h \
	GaussLUP.cpp \
	GaussLUP.h \
	Matrix.cpp \
	Matrix.h \
	MatrixOps.cpp \
	MatrixOps.h \
	Vector.cpp \
	Vector.h \
	TriDiagonal.cpp \
	TriDiagonal.h

include $(ROOT)/build/makefile.lib2
