// data for elf editor

// headers
#include "data.h"

// globals
int uflag = 0;
int dflag = 0;
char efn[BUFSIZ];
int efd = -1;
Elf32_Ehdr *pehdr = NULL;
Elf32_Shdr *pshdrs = NULL;
Elf32_Phdr *pphdrs = NULL;
Elf32_Shdr *pshstrtblh = NULL;
char *pshstrtbl = NULL;
char **pstrtbls = NULL;
char **psymtbls = NULL;
char **pstabtbls = NULL;
staboffsetdata **pstaboffsets;
char **pdyntbls = NULL;
char **preloctbls = NULL;
char **pvertbls = NULL;
char **verdefnms = NULL;
Elf32_Word **phashtbls = NULL;

