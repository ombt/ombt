#ifndef __DATA_H
#define __DATA_H
// data for elf editor

// headers
#include <stdio.h>
#include <elf.h>
#include <link.h>
#if EXPTOOLS==1 || GNUVERSION==1
#include "stab.h"
#else
#include <stab.h>
#endif

// stab data 
struct staboffsetdata {
	long start;
	long end;
	long offset;
};

// externs
extern int uflag;
extern int dflag;
extern char efn[];
extern int efd;
extern Elf32_Ehdr *pehdr;
extern Elf32_Shdr *pshdrs;
extern Elf32_Phdr *pphdrs;
extern Elf32_Shdr *pshstrtblh;
extern char *pshstrtbl;
extern char **pstrtbls;
extern char **psymtbls;
extern char **pstabtbls;
extern staboffsetdata **pstaboffsets;
extern char **pdyntbls;
extern char **preloctbls;
extern char **pvertbls;
extern char **verdefnms;
extern Elf32_Word **phashtbls;

#endif
