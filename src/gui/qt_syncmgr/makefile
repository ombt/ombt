SYNCMGRSOURCEFILES = \
	DASQDebug.h \
	main.cpp \
	MdiMainWindow.cpp \
	MdiMainWindow.h \
	FileWatcherWindow.cpp \
	FileWatcherWindow.h \
	DAS.h \
	DAS.cpp \
	FileNode.cpp \
	FileNode.h \
	MsgQueue.cpp \
	MsgQueue.h \
	DBMgr.cpp \
	DBMgr.h \
	FileNodeMgr.cpp \
	FileNodeMgr.h \
	FileSystemMonitor.cpp \
	FileSystemMonitor.h \
	FileSystemWatcher.cpp \
	FileSystemWatcher.h \
	SynchMgr.cpp \
	SynchMgr.h

TESTFILEDATASOURCEFILES = \
	testfiledata.cpp \
	FileData.h \
	FileData.cpp 

TESTFILEDATA2SOURCEFILES = \
	testfiledata2.cpp \
	FileData.h \
	FileData.cpp 

TESTFILEDATA3SOURCEFILES = \
	testfiledata3.cpp \
	FileData.h \
	FileData.cpp 

TESTFILEDATA4SOURCEFILES = \
	testfiledata4.cpp \
	Returns.h \
	Debug.h \
	List.h \
	List.cpp \
	FileData.h \
	FileData.cpp 

all:	syncmgr testfiledata testfiledata2 testfiledata3 testfiledata4

syncmgr: $(SYNCMGRSOURCEFILES)
	qmake -project -nopwd -o syncmgr.pro $(SYNCMGRSOURCEFILES)
	qmake -o syncmgr.makefile syncmgr.pro
	make -f syncmgr.makefile

testfiledata: $(TESTFILEDATASOURCEFILES)
	qmake -project -nopwd -o testfiledata.pro $(TESTFILEDATASOURCEFILES)
	qmake -o testfiledata.makefile testfiledata.pro
	make -f testfiledata.makefile

testfiledata2: $(TESTFILEDATA2SOURCEFILES)
	qmake -project -nopwd -o testfiledata2.pro $(TESTFILEDATA2SOURCEFILES)
	qmake -o testfiledata2.makefile testfiledata2.pro
	make -f testfiledata2.makefile

testfiledata3: $(TESTFILEDATA3SOURCEFILES)
	qmake -project -nopwd -o testfiledata3.pro $(TESTFILEDATA3SOURCEFILES)
	qmake -o testfiledata3.makefile testfiledata3.pro
	make -f testfiledata3.makefile

testfiledata4: $(TESTFILEDATA4SOURCEFILES)
	qmake -project -nopwd -o testfiledata4.pro $(TESTFILEDATA4SOURCEFILES)
	qmake -o testfiledata4.makefile testfiledata4.pro
	make -f testfiledata4.makefile

clean:	
	-rm -f *.o
	-rm -f moc_*
	-rm -f qt_*
	-rm -f syncmgr
	-rm -f syncmgr.pro
	-rm -f syncmgr.makefile
	-rm -f testfiledata
	-rm -f testfiledata.pro
	-rm -f testfiledata.makefile
	-rm -f testfiledata2
	-rm -f testfiledata2.pro
	-rm -f testfiledata2.makefile
	-rm -f testfiledata3
	-rm -f testfiledata3.pro
	-rm -f testfiledata3.makefile
	-rm -f testfiledata4
	-rm -f testfiledata4.pro
	-rm -f testfiledata4.makefile

install:
	touch install

