# SCCS id:	@(#)makefile	7.1.2.2	10/28/99	
#
# 
#              Copyright (c) 1996-1998 Lucent Technologies
#                      All Rights Reserved
#
#                      THIS IS UNPUBLISHED
#                      PROPRIETARY SOURCE
#                  CODE OF Lucent Technologies
#
#          The copyright notice above does not evidence any
#         actual or intended publication of such source code
#
#
# Define these macros(if required) after including makefile.common
# PROJ_LOCAL_INCS : This defines any includes that are effective only for the
#                  build within this directory.
# PROJ_LOCAL_CDEFINES :This defines any -D macros that are effective
#						 only for the build within this directory.


include $(ROOTDIR)/makefiles/makefile.common

#
# List the source files that you need to build in this directory.
# In case this directory does not have any files but subdirs leave this
# empty.
#
# If SRCS contain a list of sources, make sure that corresponding
# objects are added in $(ROOTDIR)/makefiles/objects

SRCS = 		em_h225.c\
			em_h225anxg.c\
			em_h245.c\
			em_h450.c\
			em_oids.c\
			em_asn1.c

#
# List of objects that will be built in this directory. In case this
# directory does not contain any source files, then leave this empty.
#
# NOTE: The list of objects have to be declared in the file 
# $(ROOTDIR)/makefile/objects. Define a macro in that file with
# the list of objects that will get built for this subdirectory.
# The list of objects must have a path starting with $(ROOTDIR)

OBJS = $(EM_ASN1_OBJS)

#
# The action to take in this directory. The following are three common
# types of actions.
# 
# Scenario 1.  no files but only subdirs.
#
# all: 
# 	cd subdir-1; make
#	...
#	cd subdir-n; make
# 
# Scenario 2: only the objects need to be built.
# 
# all: $(OBJS)  
# 
# Scenario 3: building an archive. 
# 
# all:  
# 	cd subdir; make #if there are any subdirectories to be made.
# 	$(AR) $(ARFLAGS) $(LIB_NAME) $(LIB_OBJS) 
#	# LIB_NAME is defined in $(ROOTDIR)/makefiles/targets.
# 	# LIB_OBJS is defined in $(ROOTDIR)/makefiles/targets.

all: $(EM_ASN1_LIB)

$(EM_ASN1_LIB): $(OBJS)
	@$(RM) $(EM_ASN1_LIB)
	$(AR) $(ARFLAGS) $(OBJS)
	$(CP) $(EM_ASN1_LIB) $(DSTLIBDIR)/$(EM_ASN1_LIB_NAME)

clean:
	@$(RM) -f $(OBJS) *.pch

clobber:
	@$(MAKE) clean
	@$(RM) $(EM_ASN1_LIB)

depend:
		$(MAKEDEPEND_CMD_LINE) $(SRCS)
# DO NOT DELETE THIS LINE -- make depend depends on it.

