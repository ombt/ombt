#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = \
	test1 \
	test2 \
	test3 \
	test4 \
	test5 \
	test6 \
	test7 \
	test8

TESTLIBS = logic

include $(ROOT)/build/makefile.test2

test7_run: test7 
	-test7 -Itests/atp/tests -Itests/atp/tests/axioms tests/atp/tests/current/test*.t 2>&1

test8_run: test8 
	-test8 -r -Itests/atp/tests -Itests/atp/tests/axioms tests/atp/tests/current/test*.t 2>&1
