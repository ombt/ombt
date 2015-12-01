#ifndef __EDELF_H
#define __EDELF_H
// header file for elf file editor

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <elf.h>
#include <sys/link.h>
#include <signal.h>
#include <demangle.h>
#include <re_comp.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"
#include "tokenize.h"
#include "elfobj.h"

// convenient macros
#define MAXBUCKET(_eo, _csec) ((_eo).pshdrdata((_csec))[0])
#define MAXCHAIN(_eo, _csec) ((_eo).pshdrdata((_csec))[1])

// externs for functions
extern int menu(ElfObject &);

extern void readfhdr(ElfObject &, char *);
extern void editfhdr(ElfObject &, char *);

extern void readshdrs(ElfObject &, char *);
extern void editshdrs(ElfObject &, char *);

extern void readphdrs(ElfObject &, char *);
extern void editphdrs(ElfObject &, char *);

extern void readstrings(ElfObject &, char *);
extern void editstrings(ElfObject &, char *);

extern void readsymbols(ElfObject &, char *);
extern void editsymbols(ElfObject &, char *);

extern void readreloc(ElfObject &, char *);
extern void editreloc(ElfObject &, char *);

extern void readoverwriter(ElfObject &, char *);
extern void overwriter(ElfObject &, char *);

extern void readhash(ElfObject &, char *);
extern void edithash(ElfObject &, char *);

extern void readversions(ElfObject &, char *);
extern void editversions(ElfObject &, char *);

extern void readdynamic(ElfObject &, char *);
extern void editdynamic(ElfObject &, char *);

extern void readdasm(ElfObject &, char *);
extern void editdasm(ElfObject &, char *);

extern void readstabs(ElfObject &, char *);
extern void editstabs(ElfObject &, char *);

extern void rmvnlgets(char *);
extern int nametosec(ElfObject &, const char *, int = 0);
extern void zap(char *, char, int);
extern int REequal(const char *, const char *);
extern const char *mydemangle(const char *);
extern long MYatoi(const char *);
extern void showsymbol(ElfObject &, int &, int &, char *&, char *&);
extern void showsymbolname(ElfObject &, int &, int &, char *&, char *&);

#endif

