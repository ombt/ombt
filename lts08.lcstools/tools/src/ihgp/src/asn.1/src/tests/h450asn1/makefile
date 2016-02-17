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
# CVS Information:
# $Revision: 1.5 $
# $Date: 1999/04/08 19:19:11 $
#
# Filename		:	asn.mk
# Description	:	Makefile used on mmcsgp1 to generate the h450.[ch]
#					files from h450_globals.asn, em_h450.asn, etc, ...
# Caveats		:	The only real fields you may need to change are
#					PROT, INPUTFILES and ASN1FLAGS.
#
# Revision History
# Date			Revision description
#
###############################################################################

# Protocol dependent information
#

# Protocol names
#
PROT=h450
NAME=em_$(PROT)

# Source asn.1 files. The last file contains the PDU.
#
INPUTFILES=$(PROT)_globals.asn\
			x880_7_0.asn\
			x880_5_0.asn\
			h235.asn\
			h225.asn\
		   	h4501_0_11.asn\
			h4501_0_18.asn\
			h4501_0_1.asn\
			h4501_0_6.asn\
			h4501_0_9.asn\
			h4502_0_0.asn\
			h4503_0_0.asn\
			h4504_0_0.asn\
			h4505_0_0.asn\
			h4506_0_0.asn\
			h4507_0_0.asn\
			$(NAME).asn

# The asn.1 compiler flags may vary from protocol to protocol.
#
ASNFLAGS=		-1997								\
				-c++        						\
				-ansi       						\
				-per        						\
				-nouserconstraints 					\
				-compat decoderUpdatesInputAddress	\
				-compat tersecomments				\
				-prefix Em  						\
				-err $(NAME).err					\
				-out $(NAME)

###############################################################################
#
# Everything under here should be protocol independent.
#
###############################################################################

# Directories
#
# ASNDIR=d:/ossasn1/win32/5.0.3
ASNDIR=/usr/local/ossasn1/solaris-2.4/5.2.0
ASNDFLT=$(ASNDIR)/asn1dflt
OSDIR=../../os/asn1
SHELL=sh
# ASN.1 compiler and flags
#
ASN=$(ASNDIR)/bin/asn1


all: setup_links  win32 sun5 comosx86 inferno_m4700 inferno_m4000 inferno_386\
	 uw hpux vxworks_simso vxworks_m4700eb vxworks_m4700el linux

clean: 
	@rm -f *.diff *.gen *.rmv *.err
	
win32: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.ms.zp8 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/win32/$(NAME).h
	mv $(NAME).c $(OSDIR)/win32/$(NAME).c

sun5: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.solaris-sparc $(ASNFLAGS) -keepnames $(INPUTFILES)
	cp $(NAME).h $(OSDIR)/sun5/$(NAME).h.pre_mkh450
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/sun5/$(NAME).h
	mv $(NAME).c $(OSDIR)/sun5/$(NAME).c

comosx86: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.comos-x86.comos $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/comos/X86/$(NAME).h
	mv $(NAME).c $(OSDIR)/comos/X86/$(NAME).c

inferno_386: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.inferno-386 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/inferno/386/$(NAME).h
	mv $(NAME).c $(OSDIR)/inferno/386/$(NAME).c

inferno_m4000: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.inferno-mips4000 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/inferno/R4000/$(NAME).h
	mv $(NAME).c $(OSDIR)/inferno/R4000/$(NAME).c

inferno_m4700: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.inferno-mips4700 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/inferno/R4700/$(NAME).h
	mv $(NAME).c $(OSDIR)/inferno/R4700/$(NAME).c

uw: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.unixware $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/uw/$(NAME).h
	mv $(NAME).c $(OSDIR)/uw/$(NAME).c

hpux: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.hp9000ux $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/hpux/$(NAME).h
	mv $(NAME).c $(OSDIR)/hpux/$(NAME).c

vxworks_simso: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.vxworks-solaris $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/vxworks/SIMSPARCSOLARIS/$(NAME).h
	mv $(NAME).c $(OSDIR)/vxworks/SIMSPARCSOLARIS/$(NAME).c

vxworks_m4700eb: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.vxworks-mips4700-eb $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/vxworks/R4700EB/$(NAME).h
	mv $(NAME).c $(OSDIR)/vxworks/R4700EB/$(NAME).c

vxworks_m4700el: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.vxworks-mips4700-el $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/vxworks/R4700EL/$(NAME).h
	mv $(NAME).c $(OSDIR)/vxworks/R4700EL/$(NAME).c

linux: $(NAME).asn
	$(ASN) $(ASNDFLT)/asn1dflt.linux-i386 $(ASNFLAGS) $(INPUTFILES)
	$(SHELL) mkh450 $(NAME).h
	mv $(NAME).h $(OSDIR)/linux/$(NAME).h
	mv $(NAME).c $(OSDIR)/linux/$(NAME).c

setup_links:
	ln -sf h4501_0_2.asn em_h450.asn
	ln -sf ../h225asn1/em_h225.asn h225.asn
	ln -sf ../h225asn1/h235.asn h235.asn

clean_links:
	rm em_h450.asn h225.asn h235.asn
