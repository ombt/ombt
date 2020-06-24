#ifndef __OCELF_H
#define __OCELF_H
// header file for elf object-file comparer

// headers
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <link.h>
#include <string.h>
#include <libgen.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"

// definitions
#define UnknownStatus 0
#define IdenticalFiles 1
#define DifferentFiles 2
#define StopProcessing 4
#define ErrorCondition 8
#define Ignore 16

// section names
#define INITSECTION ".init"
#define TEXTSECTION ".text"
#define FINISECTION ".fini"
#define RODATASECTION ".rodata"
#define DATASECTION ".data"
#define BSSSECTION ".bss"

// check and set status
#define CheckStatus(_status, _flag) \
	(((_status)&(_flag)) != 0)
#define IsIdentical(_status) \
	CheckStatus((_status), IdenticalFiles)
#define IsDifferent(_status) \
	CheckStatus((_status), DifferentFiles)
#define IsStop(_status) \
	CheckStatus((_status), StopProcessing)
#define IsError(_status) \
	CheckStatus((_status), ErrorCondition)
#define IsUnknown(_status) \
	CheckStatus((_status), UnknownStatus)
#define SetStatus(_status, _flag) \
	(_status) |= (_flag)
#define ClearStatus(_status, _flag) \
	(_status) &= (~_flag)

// externs for globals
extern int tflag;
extern int aflag;
extern int sflag;
extern int fflag;
extern int Sflag;
extern int Pflag;
extern int Yflag;
extern int Tflag;
extern int Bflag;
extern long maxBytesToPrint;
extern const char *REsecnames;
extern long offsetfldwidth;

// externs for functions 
extern int compare(int, int);
extern void readehdr(int, Elf32_Ehdr *&);
extern void readshdr(int, Elf32_Ehdr *, Elf32_Shdr *&, char *&);
extern void readphdr(int, Elf32_Ehdr *, Elf32_Phdr *&);
extern void readsdata(int, Elf32_Ehdr *, Elf32_Shdr *, int, char *&);
extern void readpdata(int, Elf32_Ehdr *, Elf32_Phdr *, int, char *&);
extern void readpdata(int, Elf32_Ehdr *, Elf32_Phdr *, int, char *&);
extern void swabehdr(Elf32_Ehdr *);
extern void swabshdr(Elf32_Ehdr *, Elf32_Shdr *);
extern void swabphdr(Elf32_Ehdr *, Elf32_Phdr *);
extern unsigned long swabit(unsigned char *, int);
extern int isSwabable(const Elf32_Ehdr *);

#endif

