#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
SYNCSVCSOURCEFILES = \
	DAS.cpp \
	DAS.h \
	DASQDebug.h \
	dataaccessmanagement.cpp \
	dataaccessmanagement.h \
	DHMWindow.cpp \
	DHMWindow.h \
	filesystemmonitor.cpp \
	filesystemmonitor.h \
	FileWatcherWindow.cpp \
	FileWatcherWindow.h \
	linuxfilesystemwatcher.cpp \
	linuxfilesystemwatcher.h \
	main.cpp \
	MdiMainWindow.cpp \
	MdiMainWindow.h \
	msgqueue.cpp \
	msgqueue.h \
	syncmanagement.cpp \
	syncmanagement.h \
	syncservice.cpp \
	syncservice.h \
	testfilenodesmanager.cpp \
	testfilenodesmanager.h \
	transactionlog.cpp \
	transactionlog.h

all:	syncsvc

depend:

syncsvc: $(SYNCSVCSOURCEFILES)
	qmake -project -nopwd -o syncsvc.pro $(SYNCSVCSOURCEFILES)
	qmake -o syncsvc.makefile syncsvc.pro
	make -f syncsvc.makefile

clean:	
	-rm -f *.o
	-rm -f moc_*
	-rm -f qt_*
	-rm -f syncsvc
	-rm -f syncsvc.pro
	-rm -f syncsvc.makefile

install:
	touch install

