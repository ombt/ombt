// data for elf program

// headers
#include <sys/types.h>
#include <libelf.h>

// local headers
#include "data.h"

// machine names
IntToString E_Machine[] = {
	EM_M32, "AT&T WE 32100",
	EM_SPARC, "Sun SPARC",
	EM_386, "Intel 80386",
	EM_68K, "Motorola 68000",
	EM_88K, "Motorola 88000",
	EM_486, "Intel 80486",
	EM_860, "Intel i860",
	EM_MIPS, "MIPS RS3000 Big-Endian",
	EM_UNKNOWN9, "unknown9",
	EM_MIPS_RS3_LE, "MIPS RS3000 Little-Endian",
	EM_RS6000, "RS6000",
	EM_UNKNOWN12, "unknown12",
	EM_UNKNOWN13, "unknown13",
	EM_UNKNOWN14, "unknown14",
	EM_PA_RISC, "PA-RISC",
	EM_nCUBE, "nCUBE",
	EM_VPP500, "Fujitsu VPP500",
	EM_SPARC32PLUS, "Sun SPARC 32+",
	EM_UNKNOWN19, "unknown19",
	EM_PPC, "PowerPC",
	EM_SPARCV9, "Sun SPARC V9 (64-bit)",
	EM_NONE, "none",
	-1, NULL
};

IntToString Sh_Type[] = {
	SHT_NULL, "inactive section",
	SHT_PROGBITS, "program-defined information",
	SHT_SYMTAB, "symbol table, only one per file",
	SHT_STRTAB, "string table",
	SHT_RELA, "relocation entries with addends",
	SHT_HASH, "symbol rehash table",
	SHT_DYNAMIC, "dynamic linking data",
	SHT_NOTE, "file information",
	SHT_NOBITS, "no file information",
	SHT_REL, "relocation entries, no addends",
	SHT_SHLIB, "reserved section",
	SHT_DYNSYM, "dynamic linking minimal symbol table",
	SHT_NUM, "NUM section",
	SHT_SUNW_move, "SUNW move section",
	SHT_SUNW_COMDAT, "SUNW COMDAT section",
	SHT_SUNW_syminfo, "SUNW syminfo section",
	SHT_SUNW_verdef, "SUNW verdef section",
	SHT_SUNW_verneed, "SUNW verneed section",
	SHT_SUNW_versym, "SUNW versym section",
	SHT_LOPROC, "lower-bound for indices reserved for processor-specific semantics",
	SHT_HIPROC, "upper-bound for indices reserved for processor-specific semantics",
	SHT_LOUSER, "lower-bound for indices reserved for applications",
	SHT_HIUSER, "upper-bound for indices reserved for applications",
	-1, NULL
};

StringToString Sh_Name[] = {
	".bss", "uninitialized read-write data",
	".comment", "comment section",
	".data", "initialized read-write data",
	".data1", "initialized read-write data",
	".debug", "debugging information",
	".fini", "run-time finalization instructions",
	".init", "run-time initialization instructions",
	".rodata", "read-only data",
	".rodata1", "read-only data",
	".text", "executable text",
	".line", "line number information",
	".note", "note information",
	".dynamic", "dynamic linking information",
	".dynstr", "strings for dynamic linking",
	".dynsym", "dynamic linking symbol table",
	".hash", "symbol hash table",
	".interp", "program interpreter",
	".shstrtab", "string table for section header names",
	".strtab", "string table",
	".symtab", "symbol table",
	".got", "global offset table",
	".plt", "procedure linking table",
	".rel", "relocation information for section",
	".rela", "relocation information for section",
	NULL, NULL
};

BitsToString Sh_Flags[] = {
	SHF_WRITE, "writeable,",
	SHF_ALLOC, "allocated,",
	SHF_EXECINSTR, "executable,",
	SHF_MASKPROC, "reserved,",
	0, NULL
};
