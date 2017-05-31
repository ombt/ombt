## - - - - - - - - - - - - - - - - - - - -

CXXFLAGS_STANDARD = -g -Wall -pedantic
CXXFLAGS_LOCAL = -I.
CXXFLAGS_XERCES =
CXXFLAGS = ${CXXFLAGS_STANDARD} ${CXXFLAGS_LOCAL} ${CXXFLAGS_XERCES}

LDFLAGS_STANDARD = -g
LDFLAGS_XERCES = -lxerces-c
LDFLAGS = ${LDFLAGS_STANDARD} ${LDFLAGS_XERCES}

BINARIES = step1 step2 step3 step4
OBJ_FILES = helper-classes.o helper-classes.o helper-classes.o

## - - - - - - - - - - - - - - - - - - - -

all: ${BINARIES}

## DOM: read in a doc
step1: step1.cc
	${CXX} ${CXXFLAGS} step1.cc -o ${@} ${LDFLAGS}

## DOM: revision of step1, uses helper classes for character conversion
step2: step2.cc helper-classes.o
	${CXX} ${CXXFLAGS} step2.cc helper-classes.o -o ${@} ${LDFLAGS}

## DOM: revision of step2, uses helper class to find elements
step3: step3.cc helper-classes.o
	${CXX} ${CXXFLAGS} step3.cc helper-classes.o -o ${@} ${LDFLAGS}

## DOM: modifies in-memory document, writes it back to disk
step4: step4.cc helper-classes.o
	${CXX} ${CXXFLAGS} step4.cc helper-classes.o -o ${@} ${LDFLAGS}

helper-classes.o: helper-classes.cc helper-classes.h
	${CXX} ${CXXFLAGS} -c helper-classes.cc -o ${@}


clean:
	-rm -f ${BINARIES} ${OBJ_FILES}
