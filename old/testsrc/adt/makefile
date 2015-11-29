# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

PRODS = talphabet tarray2 tarray tbinary2 tbinary_array tbinary_AVL2 \
	tbinary_AVL tbinary tbinaryCUJ tbinary_mutable tbinary_NR \
	tdeque_list thash_array thash_list tlist2 tlist tmap2 tmap \
	toset2 toset tpqueue_array tpqueue_avltree tpqueue_list tprime \
	tqueue_array tqueue_list tset_bitvector tset_hashtable tset_list \
	tstack_array tstack_list

CXXEXTRAFLAGS = -DDEBUG -DUSEOSTREAM=1 -Wfatal-errors -lombtrandom -lombtadt

include $(ROOT)/build/makefile.test
