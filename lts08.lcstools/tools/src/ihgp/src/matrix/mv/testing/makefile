
include ../makefile.def



MV_INCLUDE_DIR = ../include
INCLUDES 	= -I$(MV_INCLUDE_DIR)


VECTOR_H		= "mvvall.h"
VECTOR_DOUBLE 	= MV_Vector_double
VECTOR_FLOAT   	= MV_Vector_float
VECTOR_INT		= MV_Vector_int
VECTOR_COMPLEX	= MV_Vector_COMPLEX
MATRIX_H		= "mvm.h"
MATRIX_DOUBLE	= MV_ColMat_double
MATRIX_FLOAT	= MV_ColMat_float
MATRIX_INT		= MV_ColMat_int
MATRIX_COMPLEX	= MV_ColMat_COMPLEX



#  Non-templated version
#
SRCS =	  tvecd.cc 	\
		  tmatd.cc 	\
		  tblas1d.cc	\
		  tveci.cc 		\
		  tmati.cc 		\
		  tvecf.cc 	\
		  tmatf.cc 	\
		  tblas1f.cc 	\
		  tvecc.cc 	\
		  tmatc.cc 	\
		  tblas1c.cc 

OBJS =	  tvecd.o 	\
		  tmatd.o 	\
		  tblas1d.o	\
		  tveci.o 		\
		  tmati.o 		\
		  tvecf.o 	\
		  tmatf.o 	\
		  tblas1f.o \
		  tvecc.o 	\
		  tmatc.o 	\
		  tblas1c.o 

EXECS	= main testio 

all: $(EXECS)

main : main.o $(OBJS)
	$(CCC) $(CCCFLAGS) -o main main.o $(OBJS) $(LDFLAGS)

testio : testio.o
	$(CCC) $(CCCFLAGS) -o testio testio.o $(LDFLAGS)


tvecd.o : tvecd.cc  \
  $(MV_INCLUDE_DIR)/mvvd.h $(MV_INCLUDE_DIR)/mvvind.h

tmatd.o : tmatd.cc  \
  $(MV_INCLUDE_DIR)/mvmd.h $(MV_INCLUDE_DIR)/mvvd.h \
	$(MV_INCLUDE_DIR)/mvvind.h

tblas1d.o : tblas1d.cc \
  $(MV_INCLUDE_DIR)/mvmd.h $(MV_INCLUDE_DIR)/mvvd.h \
	$(MV_INCLUDE_DIR)/mvvind.h


tvecf.o : tvecf.cc  \
  $(MV_INCLUDE_DIR)/mvvf.h $(MV_INCLUDE_DIR)/mvvind.h

tmatf.o : tmatf.cc  \
  $(MV_INCLUDE_DIR)/mvmf.h $(MV_INCLUDE_DIR)/mvvf.h \
	$(MV_INCLUDE_DIR)/mvvind.h

tblas1f.o : tblas1f.cc \
  $(MV_INCLUDE_DIR)/mvmf.h $(MV_INCLUDE_DIR)/mvvf.h \
	$(MV_INCLUDE_DIR)/mvvind.h


tveci.o : tveci.cc  \
  $(MV_INCLUDE_DIR)/mvvi.h $(MV_INCLUDE_DIR)/mvvind.h

tmati.o : tmati.cc  \
  $(MV_INCLUDE_DIR)/mvmi.h $(MV_INCLUDE_DIR)/mvvi.h \
	$(MV_INCLUDE_DIR)/mvvind.h


tvecc.o : tvecc.cc  \
  $(MV_INCLUDE_DIR)/mvvc.h $(MV_INCLUDE_DIR)/mvvind.h

tmatc.o : tmatc.cc  \
  $(MV_INCLUDE_DIR)/mvmc.h $(MV_INCLUDE_DIR)/mvvc.h \
	$(MV_INCLUDE_DIR)/mvvind.h

tblas1c.o : tblas1c.cc \
  $(MV_INCLUDE_DIR)/mvmc.h $(MV_INCLUDE_DIR)/mvvc.h \
	$(MV_INCLUDE_DIR)/mvvind.h

testio.o : testio.cc \
  $(MV_INCLUDE_DIR)/mvmd.h $(MV_INCLUDE_DIR)/mvvi.h \
	$(MV_INCLUDE_DIR)/mvvind.h
wipe: clean

clean:
	/bin/rm -f $(OBJS) $(EXECS) main.o testio.o main testio;
	/bin/rm -r -f ptrepository;
	/bin/rm -r -f Templates.DB;

