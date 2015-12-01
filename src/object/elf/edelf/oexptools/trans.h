#ifndef __TRANS_H
#define __TRANS_H
// translate internal values to user-friendly values

// headers
#include <stdio.h>
#include <elf.h>
#include <link.h>
#include <string.h>
#if EXPTOOLS==1 || GNUVERSION==1
#include "stab.h"
#else
#include <stab.h>
#endif
#include <sys/elf_386.h>
#include <sys/elf_M32.h>
#include <sys/elf_SPARC.h>
#ifdef OS5_5
#include <sys/elf_ppc.h>
#endif

// local headers
#include "returns.h"
#include "debug.h"

// data structures for translating values
struct IntegerToString {
	long key;
	const char *data;
};
 
struct StringToString {
	const char *key;
	const char *data;
};
 
struct RangeToString {
	long lokey;
	long hikey;
	const char *data;
};

// translation tables
extern unsigned char num2ch[];
extern IntegerToString e_class[];
extern IntegerToString e_data[];
extern IntegerToString e_type[];
extern RangeToString re_type[];
extern IntegerToString e_machine[];
extern IntegerToString e_version[];
extern IntegerToString sh_type[];
extern RangeToString rsh_type[];
extern IntegerToString sh_flags[];
extern IntegerToString p_types[];
extern RangeToString rp_types[];
extern IntegerToString p_flags[];
extern IntegerToString st_bind[];
extern RangeToString rst_bind[];
extern IntegerToString st_type[];
extern RangeToString rst_type[];
extern IntegerToString st_shndx[];
extern RangeToString rst_shndx[];
extern IntegerToString r_type_386[];
extern IntegerToString r_type_M32[];
extern IntegerToString r_type_SPARC[];
extern IntegerToString r_type_PPC[];
extern IntegerToString vd_flags[];
extern IntegerToString vn_flags[];
extern IntegerToString vd_version[];
extern IntegerToString vn_version[];
extern IntegerToString d_tag[];
extern RangeToString rd_tag[];
extern IntegerToString n_type[];

// translation functions
extern const char *b2s(const IntegerToString *, long);
extern const char *i2s(const IntegerToString *, long);
extern const char *r2s(const RangeToString *, long);
extern const char *ir2s(const IntegerToString *, const RangeToString *, long);
extern const char *s2s(const StringToString *, const char *);
extern long s2b(const IntegerToString *, const char *);
extern long s2i(const IntegerToString *, const char *);

#endif

