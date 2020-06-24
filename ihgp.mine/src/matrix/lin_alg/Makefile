#				Makefile
# 	to build and test the Numerical Math Library (libla.a)
#
# To build the library, you may want to edit the list of the library modules
# below (MODULES). Say, if you don't need/want FFT, remove all the names
# that contain fft from MODULES= below. Then make the modules you left
# into the library by
#		make lib
# simple 'make' would suffice, too.
#
# To verify the library, do
#	make check-all
# or, more specifically,
#	make vmatrix		(checks elementary matrix operations)
#	make vvector		(checks elementary vector operations)
#	make vmatrix1		(checks advanced matrix operations: multipl)
#	
#	make vzeroin		(verifies Brent's 1D root finder)
#	make vfminbr		(verifies Brent's 1D minimizer)
#
# Note, this Makefile was built (and works under) GNU make 3.71
#
# Please check RANLIB below and adjust it to your system if necessary
# (it was made for a BSD-like system)
#
CC=./c++
CCL=./c++l
.SUFFIXES: .cc
MODULES=myenv.cc matrix1.cc matrix2.cc matrix_sub.cc \
	vector.cc determinant.cc matrix_inv.cc \
	zeroin.cc fminbr.cc ali.cc hjmin.cc svd.cc
#	fft_init.cc fft_input.cc fft_output.cc
LIBRARY=libla.a
#RANLIB = (ar d $(LIBRARY) __.SYMDEF || true); ranlib $(LIBRARY) ; BSD-like
RANLIB=/bin/true		# SYSV-based
#	Rules, new style

%.o : %.cc
	$(CC) $*.cc

% : %.o $(LIBRARY)
	$(CCL) $< $(LIBRARY) -o $@
	./$@

% :: %.cc
	$(CC) $*.cc
	$(CCL) $*.o $(LIBRARY) -o $@
	./$@

#	Rules, old style
#.o:	$*.o $(LIBRARY)
#	$(CCL) $*.o $(LIBRARY) -o $*
#	./$*
#.cc: 	$*.cc $(LIBRARY)
#	$(CC) $*.cc
#	$(CCL) $*.o $(LIBRARY) -o $*
#	./$*
#.cc.o:
#	$(CC) $*.cc
#
	
# Primary goal

# Library

lib:	$(LIBRARY)
.PHONY: lib
.PRECIOUS:	$(LIBRARY)

$(LIBRARY)::    $(MODULES)
# 			Compile the source files that have been changed 
	$(CC) $?
	listobj=`echo $? | sed s/.cc/.o/g` ; \
	ar rv $(LIBRARY) $$listobj &&	\
	rm $$listobj
	$(RANLIB)

# Verification routines
check-all:	vmatrix vvector vmatrix1 vali vhjmin vfminbr vzeroin \
		vsvd vslesing

#vmatrix1:	vmatrix1.o $(LIBRARY)
#	$(CCL) vmatrix1.o $(LIBRARY) -o vmatrix1
#	./vmatrix1


# Specific dependent goals


# Dependence rules

$(LIBRARY)::	LinAlg.h
	$(MAKE) -W matrix1.cc lib
#vquadtree.o:	quadtree.h
