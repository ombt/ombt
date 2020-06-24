# SCCS id:	@(#)makefile	7.1.2.4	03/14/00
#
###############################################################################
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
# Filename		:	makefile
# Description	:	Makefile used to generate the h245.[ch]
#					files from h245_globals.asn, and em_h225.asn.
#
# Revision History
# Date			Revision description
#
###############################################################################

#################################################################
#
#  Instructions for compiling the .c & .h files:
#  ---------------------------------------------
#	1) Obtain from OSS the compiler with files to compile for
#      the TARGET platform
#	2) Edit the makefile and change ASNDIR for the path to the
#      OSS compiler
#	3) run "make TARGET" - where TARGET is the name of the
#      target OS - see "all" below
#	Note:
#		a. Files are moved to the directory $(OSDIR)/TARGET
#		   which must exist.
#		b. The command $(SHELL) mkh225 $(NAME).h below requires 
#		   a unix shell (for example MKS shell)
#
#################################################################

# Protocol dependent information
#

# Protocol names
#
PROT=h245
NAME=em_$(PROT)

# Source asn.1 files. The last file contains the PDU.
#
INPUTFILES=$(PROT)_globals.asn $(NAME).asn

# The asn.1 compiler flags may vary from protocol to protocol.
#
ASNFLAGS=		-1997								\
				-c++        						\
				-ansi       						\
				-per        						\
				-nouserconstraints					\
				-nostaticvalues						\
				-compat decoderUpdatesInputAddress	\
				-compat tersecomments				\
				-prefix Em  						\
				-err $(NAME).err					\
				-out $(NAME)

ASNFLAGS_SHIP   =   $(ASNFLAGS) -shippable

###############################################################################
#
# Everything under here should be protocol independent.
#
###############################################################################

# Directories
#

# ASNDIR=d:/ossasn1/win32/5.0.3
# ASNDIR=d:/ossasn1/win32/5.0.8
ASNDIR=/usr/local/ossasn1/solaris-2.4/5.2.0
ASNDFLT=$(ASNDIR)/asn1dflt
OSDIR=../../os/asn1
SHELL=sh

OSSINFO_FILE=$(ASNDIR)/ossinfo
OSSINFO_ORIG=$(ASNDIR)/ossinfo_original
OSSINFO_SHIP=$(ASNDIR)/ossinfo_ship
 
# ASN.1 compiler and flags
#
ASN=$(ASNDIR)/bin/asn1


all: win32 sun5 comosx86 inferno_m4700 inferno_m4000 inferno_386\
	 uw hpux vxworks_simso vxworks_m4700eb vxworks_sh4 vxworks_m4700el \
	 win32eval sun5eval

clean: 
	@rm -f *.gen *.rmv *.diff *.err
	
win32: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.ms.zp8 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/win32/$(NAME).h
	mv $(NAME).c $(OSDIR)/win32/$(NAME).c

win32eval: $(NAME).asn
	cp $(OSSINFO_SHIP) $(OSSINFO_FILE)
	$(ASN) $(ASNDFLT)/asn1dflt.ms.zp8 $(ASNFLAGS_SHIP) $(INPUTFILES)
	cp $(OSSINFO_ORIG) $(OSSINFO_FILE)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/win32/$(NAME)_ship.h

sun5: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.solaris-sparc $(ASNFLAGS) -keepnames $(INPUTFILES)
	cp $(NAME).h $(OSDIR)/sun5/$(NAME).h.pre_mkh245
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/sun5/$(NAME).h
	mv $(NAME).c $(OSDIR)/sun5/$(NAME).c

sun5eval: $(NAME).asn
	cp $(OSSINFO_SHIP) $(OSSINFO_FILE)
	$(ASN) $(ASNDFLT)/asn1dflt.solaris-sparc $(ASNFLAGS_SHIP) -keepnames $(INPUTFILES)
	cp $(OSSINFO_ORIG) $(OSSINFO_FILE)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/sun5/$(NAME)_ship.h

comosx86: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.comos-x86.comos $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/comos/X86/$(NAME).h
	mv $(NAME).c $(OSDIR)/comos/X86/$(NAME).c

inferno_386: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.inferno-386 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/inferno/386/$(NAME).h
	mv $(NAME).c $(OSDIR)/inferno/386/$(NAME).c

inferno_m4000: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.inferno-mips4000 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/inferno/R4000/$(NAME).h
	mv $(NAME).c $(OSDIR)/inferno/R4000/$(NAME).c

inferno_m4700: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.inferno-mips4700 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/inferno/R4700/$(NAME).h
	mv $(NAME).c $(OSDIR)/inferno/R4700/$(NAME).c

uw: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.unixware $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/uw/$(NAME).h
	mv $(NAME).c $(OSDIR)/uw/$(NAME).c

hpux: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.hpux9000 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/hpux/$(NAME).h
	mv $(NAME).c $(OSDIR)/hpux/$(NAME).c

vxworks_simso: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.vxworks-solaris $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/vxworks/SIMSPARCSOLARIS/$(NAME).h
	mv $(NAME).c $(OSDIR)/vxworks/SIMSPARCSOLARIS/$(NAME).c

vxworks_m4700eb: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.vxworks-mips4700-eb $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/vxworks/R4700EB/$(NAME).h
	mv $(NAME).c $(OSDIR)/vxworks/R4700EB/$(NAME).c

vxworks_sh4: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.vxworks-sh4 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/vxworks/SH4/$(NAME).h
	mv $(NAME).c $(OSDIR)/vxworks/SH4/$(NAME).c

vxworks_m4700el: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.vxworks-mips4700-el $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh245 $(NAME).h
	mv $(NAME).h $(OSDIR)/vxworks/R4700EL/$(NAME).h
	mv $(NAME).c $(OSDIR)/vxworks/R4700EL/$(NAME).c
