#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
all:	\
	mkdata \
	testSdiMainWindow \
	testMdiMainWindow \
	testTreeNode \
	testTreeModel \
	testTreeView

depend:

mkdata: mkdata.cpp

testSdiMainWindow: \
	testSdiMainWindow.cpp \
	SdiMainWindow.h \
	SdiMainWindow.cpp \
	AbstractItemModelAdaptor.h \
	TreeModel.h \
	TreeModel.cpp \
	TreeNode.h \
	TreeNode.cpp
	qmake -project -nopwd -o testSdiMainWindow.pro \
		testSdiMainWindow.cpp \
		SdiMainWindow.h \
		SdiMainWindow.cpp \
		AbstractItemModelAdaptor.h \
		TreeModel.h \
		TreeModel.cpp \
		TreeNode.h \
		TreeNode.cpp
	qmake -o testSdiMainWindow.makefile testSdiMainWindow.pro
	make -f testSdiMainWindow.makefile

testMdiMainWindow: \
	testMdiMainWindow.cpp \
	MdiMainWindow.h \
	MdiMainWindow.cpp \
	MdiTreeView.h \
	MdiTreeView.cpp \
	DummyTreeModel.h \
	DummyTreeModel.cpp \
	AbstractItemModelAdaptor.h \
	TreeModel.h \
	TreeModel.cpp \
	TreeNode.h \
	TreeNode.cpp
	qmake -project -nopwd -o testMdiMainWindow.pro \
		testMdiMainWindow.cpp \
		MdiMainWindow.h \
		MdiMainWindow.cpp \
		MdiTreeView.h \
		MdiTreeView.cpp \
		DummyTreeModel.h \
		DummyTreeModel.cpp \
		AbstractItemModelAdaptor.h \
		TreeModel.h \
		TreeModel.cpp \
		TreeNode.h \
		TreeNode.cpp
	qmake -o testMdiMainWindow.makefile testMdiMainWindow.pro
	make -f testMdiMainWindow.makefile

testTreeModel:	\
	testTreeModel.cpp \
	AbstractItemModelAdaptor.h \
	TreeModel.h \
	TreeModel.cpp \
	TreeNode.h \
	TreeNode.cpp
	qmake -project -nopwd -o testTreeModel.pro \
		testMdiMainWindow.cpp \
		MdiMainWindow.h \
		MdiMainWindow.cpp \
		MdiTreeView.h \
		MdiTreeView.cpp \
		DummyTreeModel.h \
		DummyTreeModel.cpp \
		AbstractItemModelAdaptor.h \
		TreeModel.h \
		TreeModel.cpp \
		TreeNode.h \
		TreeNode.cpp
	qmake -o testTreeModel.makefile testTreeModel.pro
	make -f testTreeModel.makefile

testTreeNode:	\
	testTreeNode.cpp \
	TreeNode.h \
	TreeNode.cpp
	qmake -project -nopwd -o testTreeNode.pro \
		testTreeNode.cpp \
		TreeNode.h \
		TreeNode.cpp
	qmake -o testTreeNode.makefile testTreeNode.pro
	make -f testTreeNode.makefile

testTreeView: \
	testTreeView.cpp \
	AbstractItemModelAdaptor.h \
	TreeModel.h \
	TreeModel.cpp \
	TreeNode.h \
	TreeNode.cpp
	qmake -project -nopwd -o testTreeView.pro \
		testTreeView.cpp \
		AbstractItemModelAdaptor.h \
		TreeModel.h \
		TreeModel.cpp \
		TreeNode.h \
		TreeNode.cpp
	qmake -o testTreeView.makefile testTreeView.pro
	make -f testTreeView.makefile

clean:	
	-rm -f install
	-rm -f *.o
	-rm -f moc_*
	-rm -f qt_*
	-rm -f mkdata
	-rm -f testMdiMainWindow
	-rm -f testMdiMainWindow.pro
	-rm -f testMdiMainWindow.makefile
	-rm -f testSdiMainWindow
	-rm -f testSdiMainWindow.pro
	-rm -f testSdiMainWindow.makefile
	-rm -f testTreeNode
	-rm -f testTreeNode.pro
	-rm -f testTreeNode.makefile
	-rm -f testTreeModel
	-rm -f testTreeModel.pro
	-rm -f testTreeModel.makefile
	-rm -f testTreeView
	-rm -f testTreeView.pro
	-rm -f testTreeView.makefile

install:
	touch install

