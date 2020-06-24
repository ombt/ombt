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

// convenient macros
#define E_MACHINE (pehdr->e_machine)
#define MAXBUCKET(_csec) (phashtbls[(_csec)][0])
#define MAXCHAIN(_csec) (phashtbls[(_csec)][1])

// externs for functions
extern int menu();

extern void readfhdr(char *);
extern void editfhdr(char *);

extern void readshdrs(char *);
extern void editshdrs(char *);

extern void readphdrs(char *);
extern void editphdrs(char *);

extern void readstrings(char *);
extern void editstrings(char *);

extern void readsymbols(char *);
extern void editsymbols(char *);

extern void readreloc(char *);
extern void editreloc(char *);

extern void readoverwriter(char *);
extern void overwriter(char *);

extern void readhash(char *);
extern void edithash(char *);

extern void readversions(char *);
extern void editversions(char *);

extern void readdynamic(char *);
extern void editdynamic(char *);

extern void readdasm(char *);
extern void editdasm(char *);

extern void readstabs(char *);
extern void editstabs(char *);

extern void rmvnlgets(char *);
extern int nametosec(const char *, int = 0);
extern void zap(char *, char, int);
extern int REequal(const char *, const char *);
extern const char *mydemangle(const char *);
extern long MYatoi(const char *);
extern void showsymbol(int &, int &, char *&, char *&);
extern void showsymbolname(int &, int &, char *&, char *&);

#endif

