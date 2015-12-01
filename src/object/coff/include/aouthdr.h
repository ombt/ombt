#ident "@(#)/usr/src/uts/uts/sys/aouthdr.h	3.1.1.1 6/2/90 06:46:46 - (c) Copyright 1988, 1989, 1990 Amdahl Corporation"
/* Copyright (c) 1984 AT&T */

typedef	struct aouthdr {
#ifdef U370
	int     magic;          /* see filehdr.h                        */
	int     vstamp;         /* version stamp                        */
#else
	short     magic;          /* see filehdr.h                        */
	short     vstamp;         /* version stamp                        */
#endif
	long	tsize;		/* text size in bytes, padded to FW
				   bdry					*/
	long	dsize;		/* initialized data "  "		*/
	long	bsize;		/* uninitialized data "   "		*/
#if u3b
	long	dum1;
	long	dum2;		/* pad to entry point	*/
#endif
	long	entry;		/* entry pt.				*/
	long	text_start;	/* base of text used for this file	*/
	long	data_start;	/* base of data used for this file	*/
} AOUTHDR;
