# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = adt

FILES = Prime.cpp \
	Set_BitVector.cpp \
	String.cpp

HDRS = AbsDeque.h AbsHashTable.h AbsIterator.h AbsPriorityQueue.h AbsQueue.h AbsStack.h AbsTree.h Alphabet.cpp Alphabet.h Array.cpp Array.h BinaryTree2.cpp BinaryTree2.h BinaryTree_Array.cpp BinaryTree_Array.h BinaryTree_AVL.cpp BinaryTree_AVL.h BinaryTree.cpp BinaryTree.h BinaryTree_NR.cpp BinaryTree_NR.h Buffer.h Deque_List.cpp Deque_List.h HashTable_Array.cpp HashTable_Array.h HashTable_List.cpp HashTable_List.h Map.cpp Map.h List.cpp List.h NEWpointer.h OrderedSet.cpp OrderedSet.h Pointer.h PriorityQueue_Array.cpp PriorityQueue_Array.h PriorityQueue_AVLTree.cpp PriorityQueue_AVLTree.h PriorityQueue_List.cpp PriorityQueue_List.h Proxy.h Ptr.h Ptrlist.cpp Ptrlist.h Queue_Array.cpp Queue_Array.h Queue_List.cpp Queue_List.h Set_HashTable.cpp Set_HashTable.h Set_List.cpp Set_List.h Stack_Array.cpp Stack_Array.h Stack_List.cpp Stack_List.h Tuple.h Twokeys.h Set_BinaryTree.cpp Set_BinaryTree.h


include $(ROOT)/build/makefile.lib2

Prime.o:	Prime.cpp \
		Prime.h \
		Set_BitVector.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h

Set_BitVector.o:	Set_BitVector.cpp \
		Set_BitVector.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h

String.o:	String.cpp \
		String.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h

