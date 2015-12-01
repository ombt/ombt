#ident "@(#)/usr/src/uts/uts/sys/scnhdr.h	6.1 11/15/89 13:31:06 - (c) Copyright 1988, 1989, 1990 Amdahl Corporation"
/* 
 */
/*
 */

struct scnhdr {
	char		s_name[8];	/* section name */
	long		s_paddr;	/* physical address, aliased s_nlib */
	long		s_vaddr;	/* virtual address */
#ifdef UTS
	unsigned
#endif
	long		s_size;		/* section size */
	long		s_scnptr;	/* file ptr to raw data for section */
	long		s_relptr;	/* file ptr to relocation */
	long		s_lnnoptr;	/* file ptr to line numbers */
	unsigned short	s_nreloc;	/* number of relocation entries */
	unsigned short	s_nlnno;	/* number of line number entries */
#ifdef UTS
	union {
	    struct{
		unsigned char _nreloc_hi, /* Hi order byte for s_nreloc */
		_nlnno_hi;		  /* Hi order byte for s_nlnno  */
		short _flags;		  /* The flags had 2 bytes stolen. */
	    }_flg;
	    long _oldflags;}
	_flu;
	
#else
	long		s_flags;	/* flags */
#endif
	};

#ifdef UTS
/* MAXNRELOC and MAXNLNNO are both 2^24-1 since they have 3 bytes */
#define MAXNRELOC 0xffffff
#define MAXNLNNO MAXNRELOC
/* Under uts the 2 high order bytes of the flags field have been stolen
 * to be used as the high order bytes of s_nreloc and s_nlnno respectively.
 * s_nreloc, s_nlnno and s_flags should not be accessed directly.
 * instead assignment should be handled with s_put_ and references should
 * be handled with s_get_ .
 *
 * void put_s_nlnno(struct scnhdr *scn,val)
 * void put_s_flags(struct scnhdr *scn,val)
 * void put_s_nreloc(struct scnhdr *scn,val)
 * unsigned long get_s_nlnno(struct scnhdr *scn)
 * unsigned long get_s_nreloc(struct scnhdr *scn)
 *          long get_s_flags(struct scnhdr *scn)
 */

#define s_flags _flu._flg._flags
#define s_nreloc_hi _flu._flg._nreloc_hi
#define s_nlnno_hi _flu._flg._nlnno_hi
#define get_s_nreloc(scnptr)   ((scnptr)->s_nreloc+((scnptr)->_flu._flg._nreloc_hi<<16))
#define put_s_nreloc(scnptr,val)(((scnptr)->s_nreloc=(val)),\
				(scnptr)->_flu._flg._nreloc_hi=(val)>>16)
#define get_s_nlnno(scnptr) ((scnptr)->s_nlnno+((scnptr)->_flu._flg._nlnno_hi<<16))
#define put_s_nlnno(scnptr,val) (((scnptr)->s_nlnno=(val)),\
    (scnptr)->_flu._flg._nlnno_hi=(val)>>16)
#define put_s_flags(scnptr,val) ((scnptr)->_flu._flg._flags=(val))
#define get_s_flags(scnptr) ((scnptr)->_flu._flg._flags)

#endif
/* the number of shared libraries in a .lib section in an absolute output file
 * is put in the s_paddr field of the .lib section header, the following define
 * allows it to be referenced as s_nlib
 */

#define s_nlib	s_paddr
#define	SCNHDR	struct scnhdr
#define	SCNHSZ	sizeof(SCNHDR)




/*
 * Define constants for names of "special" sections
 */

#define _TEXT ".text"
#define _DATA ".data"
#define _BSS  ".bss"
#define _TV  ".tv"
#define _INIT ".init"
#define _FINI ".fini"




/*
 * The  2 bytes of s_flags are used as a section "type"
 * The high order 2 bytes are no longer available.
 */

#define STYP_REG	0x00		/* "regular" section:
						allocated, relocated, loaded */
#define STYP_DSECT	0x01		/* "dummy" section:
						not allocated, relocated,
						not loaded */
#define STYP_NOLOAD	0x02		/* "noload" section:
						allocated, relocated,
						 not loaded */
#define STYP_GROUP	0x04		/* "grouped" section:
						formed of input sections */
#define STYP_PAD	0x08		/* "padding" section:
						not allocated, not relocated,
						 loaded */
#define STYP_COPY	0x10		/* "copy" section:
						for decision function used
						by field update;  not
						allocated, not relocated,
						loaded;  reloc & lineno
						entries processed normally */
#define STYP_INFO	0x200		/* comment section : not allocated
						not relocated, not loaded */
#define STYP_LIB	0x800		/* for .lib section : same as INFO */
#define STYP_OVER	0x400		/* overlay section : relocated
						not allocated or loaded */
#define	STYP_TEXT	0x20		/* section contains text only */
#define STYP_DATA	0x40		/* section contains data only */
#define STYP_BSS	0x80		/* section contains bss only */

/*
 *  In a minimal file or an update file, a new function
 *  (as compared with a replaced function) is indicated by S_NEWFCN
 */

#define S_NEWFCN  0x100

/*
 * In 3b Update Files (output of ogen), sections which appear in SHARED
 * segments of the Pfile will have the S_SHRSEG flag set by ogen, to inform
 * dufr that updating 1 copy of the proc. will update all process invocations.
 */

#define S_SHRSEG	0x20
