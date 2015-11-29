all:	filewatcher testfilewatcher testfilewatcherthread testdasdbmgr

filewatcher:	\
	main.cpp \
	MdiMainWindow.cpp \
	MdiMainWindow.h \
	FileWatcherWindow.cpp \
	FileWatcherWindow.h \
	linuxfilesystemwatcher.cpp \
	linuxfilesystemwatcher.h \
	filesystemmonitor.cpp \
	filesystemmonitor.h \
	DASSynchMgr.cpp \
	DASSynchMgr.h \
	RouterStorageMgr.cpp \
	RouterStorageMgr.h \
	DASQDebug.h \
	MsgQueue.h \
	MsgQueue.cpp \
	DASHash.h \
	DASHash.cpp
	qmake -project -nopwd -o filewatcher.pro \
		main.cpp \
		MdiMainWindow.cpp \
		MdiMainWindow.h \
		FileWatcherWindow.cpp \
		FileWatcherWindow.h \
		linuxfilesystemwatcher.cpp \
		linuxfilesystemwatcher.h \
		filesystemmonitor.cpp \
		filesystemmonitor.h \
		DASSynchMgr.cpp \
		DASSynchMgr.h \
		RouterStorageMgr.cpp \
		RouterStorageMgr.h \
		DASQDebug.h \
		MsgQueue.h \
		MsgQueue.cpp \
		DASHash.h \
		DASHash.cpp
	qmake -o filewatcher.makefile filewatcher.pro
	make -f filewatcher.makefile

testfilewatcher: \
	DASQDebug.h \
	testfilewatcher.h \
	testfilewatcher.cpp
	qmake -project -nopwd -o testfilewatcher.pro \
		DASQDebug.h \
		testfilewatcher.h \
		testfilewatcher.cpp
	qmake -o testfilewatcher.makefile testfilewatcher.pro
	make -f testfilewatcher.makefile

testfilewatcherthread: \
	DASQDebug.h \
	testfilewatcherthread.h \
	testfilewatcherthread.cpp
	qmake -project -nopwd -o testfilewatcherthread.pro \
		DASQDebug.h \
		testfilewatcherthread.h \
		testfilewatcherthread.cpp
	qmake -o testfilewatcherthread.makefile testfilewatcherthread.pro
	make -f testfilewatcherthread.makefile

testdasdbmgr: \
	DASQDebug.h \
	testdasdbmgr.cpp \
	DASDbMgr.h \
	DASDbMgr.cpp
	qmake -project -nopwd -o testdasdbmgr.pro \
		DASQDebug.h \
		testdasdbmgr.cpp \
		DASDbMgr.h \
		DASDbMgr.cpp
	qmake -o testdasdbmgr.makefile testdasdbmgr.pro
	make -f testdasdbmgr.makefile

clean:	
	-rm -f *.o
	-rm -f moc_*
	-rm -f qt_*
	-rm -f filewatcher
	-rm -f filewatcher.pro
	-rm -f filewatcher.makefile
	-rm -f testfilewatcher
	-rm -f testfilewatcher.pro
	-rm -f testfilewatcher.makefile
	-rm -f testfilewatcherthread
	-rm -f testfilewatcherthread.pro
	-rm -f testfilewatcherthread.makefile
	-rm -f testdasdbmgr
	-rm -f testdasdbmgr.pro
	-rm -f testdasdbmgr.makefile


install:
	touch install

