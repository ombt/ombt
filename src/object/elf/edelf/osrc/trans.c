// translate internal values to external values

// headers 
#include "trans.h"
#include "tokenize.h"

// translate internal to external form
unsigned char num2ch[] = {
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	' ', '!', '"', '#', '$', '%', '&', '\'',
	'(', ')', '*', '+', ',', '-', '.', '/',
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', ':', ';', '<', '=', '>', '?',
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
	'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
	'x', 'y', 'z', '{', '|', '}', '~', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
};

// tables for translating file header fields
IntegerToString e_class[] = {
	ELFCLASSNONE, "ELFCLASSNONE", 
	ELFCLASS32, "ELFCLASS32", 
	ELFCLASS64, "ELFCLASS64", 
	ELFCLASSNUM, "ELFCLASSNUM", 
	-1, NULL,
};

IntegerToString e_data[] = {
	ELFDATANONE, "ELFDATANONE", 
	ELFDATA2LSB, "ELFDATA2LSB", 
	ELFDATA2MSB, "ELFDATA2MSB", 
	ELFDATANUM, "ELFDATANUM", 
	-1, NULL,
};

IntegerToString e_type[] = {
	ET_NONE, "ET_NONE", 
	ET_REL, "ET_REL", 
	ET_EXEC, "ET_EXEC", 
	ET_DYN, "ET_DYN", 
	ET_CORE, "ET_CORE", 
	ET_NUM, "ET_NUM", 
	-1, NULL,
};

RangeToString re_type[] = {
	ET_LOPROC, ET_HIPROC, "ET_LOPROC <= value <= ET_HIPROC",
	-1, -1, NULL,
};

IntegerToString e_machine[] = {
#ifdef OS5_5
	EM_NONE, "EM_NONE",
	EM_M32, "EM_M32",
	EM_SPARC, "EM_SPARC",
	EM_386, "EM_386",
	EM_68K, "EM_68K",
	EM_88K, "EM_88K",
	EM_486, "EM_486",
	EM_860, "EM_860",
	EM_MIPS, "EM_MIPS",
	EM_UNKNOWN9, "EM_UNKNOWN9",
	EM_MIPS_RS3_LE, "EM_MIPS_RS3_LE",
	EM_RS6000, "EM_RS6000",
	EM_UNKNOWN12, "EM_UNKNOWN12",
	EM_UNKNOWN13, "EM_UNKNOWN13",
	EM_UNKNOWN14, "EM_UNKNOWN14",
	EM_PA_RISC, "EM_PA_RISC",
	EM_nCUBE, "EM_nCUBE",
	EM_VPP500, "EM_VPP500",
	EM_SPARC32PLUS, "EM_SPARC32PLUS",
	EM_UNKNOWN19, "EM_UNKNOWN19",
	EM_PPC, "EM_PPC",
	EM_NUM, "EM_NUM",
#else
	EM_NONE, "EM_NONE",
	EM_M32, "EM_M32",
	EM_SPARC, "EM_SPARC",
	EM_386, "EM_386",
	EM_68K, "EM_68K",
	EM_88K, "EM_88K",
	EM_486, "EM_486",
	EM_860, "EM_860",
	EM_MIPS, "EM_MIPS",
	EM_S370, "EM_S370",
	EM_MIPS_RS3_LE, "EM_MIPS_RS3_LE",
	EM_RS6000, "EM_RS6000",
	EM_UNKNOWN12, "EM_UNKNOWN12",
	EM_UNKNOWN13, "EM_UNKNOWN13",
	EM_UNKNOWN14, "EM_UNKNOWN14",
	EM_PA_RISC, "EM_PA_RISC",
	EM_nCUBE, "EM_nCUBE",
	EM_VPP500, "EM_VPP500",
	EM_SPARC32PLUS, "EM_SPARC32PLUS",
	EM_960, "EM_960",
	EM_PPC, "EM_PPC",
	EM_PPC64, "EM_PPC64",
	EM_UNKNOWN22, "EM_UNKNOWN22",
	EM_UNKNOWN23, "EM_UNKNOWN23",
	EM_UNKNOWN24, "EM_UNKNOWN24",
	EM_UNKNOWN25, "EM_UNKNOWN25",
	EM_UNKNOWN26, "EM_UNKNOWN26",
	EM_UNKNOWN27, "EM_UNKNOWN27",
	EM_UNKNOWN28, "EM_UNKNOWN28",
	EM_UNKNOWN29, "EM_UNKNOWN29",
	EM_UNKNOWN30, "EM_UNKNOWN30",
	EM_UNKNOWN31, "EM_UNKNOWN31",
	EM_UNKNOWN32, "EM_UNKNOWN32",
	EM_UNKNOWN33, "EM_UNKNOWN33",
	EM_UNKNOWN34, "EM_UNKNOWN34",
	EM_UNKNOWN35, "EM_UNKNOWN35",
	EM_V800, "EM_V800",
	EM_FR20, "EM_FR20",
	EM_RH32, "EM_RH32",
	EM_RCE, "EM_RCE",
	EM_ARM, "EM_ARM",
	EM_ALPHA, "EM_ALPHA",
	EM_SH, "EM_SH",
	EM_SPARCV9, "EM_SPARCV9",
	EM_TRICORE, "EM_TRICORE",
	EM_ARC, "EM_ARC",
	EM_H8_300, "EM_H8_300",
	EM_H8_300H, "EM_H8_300H",
	EM_H8S, "EM_H8S",
	EM_H8_500, "EM_H8_500",
	EM_IA_64, "EM_IA_64",
	EM_MIPS_X, "EM_MIPS_X",
	EM_COLDFIRE, "EM_COLDFIRE",
	EM_68HC12, "EM_68HC12",
	EM_MMA, "EM_MMA",
	EM_PCP, "EM_PCP",
	EM_NCPU, "EM_NCPU",
	EM_NDR1, "EM_NDR1",
	EM_STARCORE, "EM_STARCORE",
	EM_ME16, "EM_ME16",
	EM_ST100, "EM_ST100",
	EM_TINYJ, "EM_TINYJ",
	EM_UNKNOWN62, "EM_UNKNOWN62",
	EM_UNKNOWN63, "EM_UNKNOWN63",
	EM_UNKNOWN64, "EM_UNKNOWN64",
	EM_UNKNOWN65, "EM_UNKNOWN65",
	EM_FX66, "EM_FX66",
	EM_ST9PLUS, "EM_ST9PLUS",
	EM_ST7, "EM_ST7",
	EM_68HC16, "EM_68HC16",
	EM_68HC11, "EM_68HC11",
	EM_68HC08, "EM_68HC08",
	EM_68HC05, "EM_68HC05",
	EM_SVX, "EM_SVX",
	EM_ST19, "EM_ST19",
	EM_VAX, "EM_VAX",
	EM_CRIS, "EM_CRIS",
	EM_JAVELIN, "EM_JAVELIN",
	EM_FIREPATH, "EM_FIREPATH",
	EM_ZSP, "EM_ZSP",
	EM_MMIX, "EM_MMIX",
	EM_HUANY, "EM_HUANY",
	EM_NUM, "EM_NUM",
#endif
	-1, NULL,
};

IntegerToString e_version[] = {
	EV_NONE, "EV_NONE", 
	EV_CURRENT, "EV_CURRENT", 
	EV_NUM, "EV_NUM", 
	-1, NULL,
};

// tables for translating section header fields
IntegerToString sh_type[] = {
#ifdef OS5_5
	SHT_NULL, "SHT_NULL",
	SHT_PROGBITS, "SHT_PROGBITS",
	SHT_SYMTAB, "SHT_SYMTAB",
	SHT_STRTAB, "SHT_STRTAB",
	SHT_RELA, "SHT_RELA",
	SHT_HASH, "SHT_HASH",
	SHT_DYNAMIC, "SHT_DYNAMIC",
	SHT_NOTE, "SHT_NOTE",
	SHT_NOBITS, "SHT_NOBITS",
	SHT_REL, "SHT_REL",
	SHT_SHLIB, "SHT_SHLIB",
	SHT_DYNSYM, "SHT_DYNSYM",
	SHT_NUM, "SHT_NUM",
	SHT_SUNW_verdef, "SHT_SUNW_verdef",
	SHT_SUNW_verneed, "SHT_SUNW_verneed",
	SHT_SUNW_versym, "SHT_SUNW_versym",
#else
	SHT_NULL, "SHT_NULL",
	SHT_PROGBITS, "SHT_PROGBITS",
	SHT_SYMTAB, "SHT_SYMTAB",
	SHT_STRTAB, "SHT_STRTAB",
	SHT_RELA, "SHT_RELA",
	SHT_HASH, "SHT_HASH",
	SHT_DYNAMIC, "SHT_DYNAMIC",
	SHT_NOTE, "SHT_NOTE",
	SHT_NOBITS, "SHT_NOBITS",
	SHT_REL, "SHT_REL",
	SHT_SHLIB, "SHT_SHLIB",
	SHT_DYNSYM, "SHT_DYNSYM",
	SHT_UNKNOWN12, "SHT_UNKNOWN12",
	SHT_UNKNOWN13, "SHT_UNKNOWN13",
	SHT_INIT_ARRAY, "SHT_INIT_ARRAY",
	SHT_FINI_ARRAY, "SHT_FINI_ARRAY",
	SHT_PREINIT_ARRAY, "SHT_PREINIT_ARRAY",
	SHT_GROUP, "SHT_GROUP",
	SHT_NUM, "SHT_NUM",
	SHT_SUNW_move, "SHT_SUNW_move",
	SHT_SUNW_COMDAT, "SHT_SUNW_COMDAT",
	SHT_SUNW_syminfo, "SHT_SUNW_syminfo",
	SHT_SUNW_verdef, "SHT_SUNW_verdef",
	SHT_SUNW_verneed, "SHT_SUNW_verneed",
	SHT_SUNW_versym, "SHT_SUNW_versym",
#endif
	-1, NULL,
};

RangeToString rsh_type[] = {
	SHT_LOSUNW, SHT_HISUNW, "SHT_LOSUNW <= value <= SHT_HISUNW",
	SHT_LOPROC, SHT_HIPROC, "SHT_LOPROC <= value <= SHT_HIPROC",
	SHT_LOUSER, SHT_HIUSER, "SHT_LOUSER <= value <= SHT_HIUSER",
	-1, -1, NULL,
};

IntegerToString sh_flags[] = {
#ifdef OS5_5
	SHF_WRITE, "SHF_WRITE",
	SHF_ALLOC, "SHF_ALLOC",
	SHF_EXECINSTR, "SHF_EXECINSTR",
	SHF_MASKPROC, "SHF_MASKPROC",
#else
	SHF_WRITE, "SHF_WRITE",
	SHF_ALLOC, "SHF_ALLOC",
	SHF_EXECINSTR, "SHF_EXECINSTR",
	SHF_MERGE, "SHF_MERGE",
	SHF_STRINGS, "SHF_STRINGS",
	SHF_INFO_LINK, "SHF_INFO_LINK	",
	SHF_LINK_ORDER, "SHF_LINK_ORDER",
	SHF_OS_NONCONFORMING, "SHF_OS_NONCONFORMING",
	SHF_GROUP, "SHF_GROUP",
	SHF_MASKOS, "SHF_MASKOS",
	SHF_MASKPROC, "SHF_MASKPROC",
#endif
	-1, NULL,
};

// tables for translating program header fields
IntegerToString p_types[] = {
#ifdef OS5_5
	PT_NULL, "PT_NULL",
	PT_LOAD, "PT_LOAD",
	PT_DYNAMIC, "PT_DYNAMIC",
	PT_INTERP, "PT_INTERP",
	PT_NOTE, "PT_NOTE",
	PT_SHLIB, "PT_SHLIB",
	PT_PHDR, "PT_PHDR",
	PT_NUM, "PT_NUM",
#else
	PT_NULL, "PT_NULL",
	PT_LOAD, "PT_LOAD",
	PT_DYNAMIC, "PT_DYNAMIC",
	PT_INTERP, "PT_INTERP",
	PT_NOTE, "PT_NOTE",
	PT_SHLIB, "PT_SHLIB",
	PT_PHDR, "PT_PHDR",
	PT_SUNWBSS, "PT_SUNWBSS",
	PT_SUNWSTACK, "PT_SUNWSTACK",
	PT_NUM, "PT_NUM",
#endif
	-1, NULL,
};

RangeToString rp_types[] = {
#ifdef OS5_5
	PT_LOPROC, PT_HIPROC, "PT_LOPROC <= value <= PT_HIPROC", 
#else
	PT_LOSUNW, PT_HISUNW, "PT_LOSUNW <= value <= PT_HISUNW", 
	PT_LOPROC, PT_HIPROC, "PT_LOPROC <= value <= PT_HIPROC", 
#endif
	-1, -1, NULL,
};

IntegerToString p_flags[] = {
#ifdef OS5_5
	PF_R, "PF_R",
	PF_W, "PF_W",
	PF_X, "PF_X",
	PF_MASKPROC, "PF_MASKPROC",
#else
	PF_R, "PF_R",
	PF_W, "PF_W",
	PF_X, "PF_X",
	PF_MASKOS, "PF_MASKOS",
	PF_MASKPROC, "PF_MASKPROC",
#endif
	-1, NULL,
};

// tables for translating symbol table fields
IntegerToString st_bind[] = {
	STB_LOCAL, "STB_LOCAL",
	STB_GLOBAL, "STB_GLOBAL",
	STB_WEAK, "STB_WEAK",
	STB_NUM, "STB_NUM",
	-1, NULL,
};

RangeToString rst_bind[] = {
	STB_LOPROC, STB_HIPROC, "STB_LOPROC <= value <= STB_HIPROC",
	-1, -1, NULL,
};

IntegerToString st_type[] = {
	STT_NOTYPE, "STT_NOTYPE",
	STT_OBJECT, "STT_OBJECT",
	STT_FUNC, "STT_FUNC",
	STT_SECTION, "STT_SECTION",
	STT_FILE, "STT_FILE",
	STT_NUM, "STT_NUM",
	-1, NULL,
};

RangeToString rst_type[] = {
	STT_LOPROC, STT_HIPROC, "STT_LOPROC <= value <= STT_HIPROC",
	-1, -1, NULL,
};

IntegerToString st_shndx[] = {
	SHN_UNDEF, "SHN_UNDEF",
	SHN_ABS, "SHN_ABS",
	SHN_COMMON, "SHN_COMMON",
	-1, NULL,
};

RangeToString rst_shndx[] = {
	SHN_LOPROC, SHN_HIPROC, "SHN_LOPROC <= value <= SHN_HIPROC",
	SHN_LORESERVE, SHN_HIRESERVE, "SHN_LORESERVE <= value <= SHN_HIRESERVE",
	-1, -1, NULL,
};

// tables for translating relocation types
IntegerToString r_type_386[] = {
	R_386_NONE, "R_386_NONE",
	R_386_32, "R_386_32",
	R_386_PC32, "R_386_PC32",
	R_386_GOT32, "R_386_GOT32",
	R_386_PLT32, "R_386_PLT32",
	R_386_COPY, "R_386_COPY",
	R_386_GLOB_DAT, "R_386_GLOB_DAT",
	R_386_JMP_SLOT, "R_386_JMP_SLOT",
	R_386_RELATIVE, "R_386_RELATIVE",
	R_386_GOTOFF, "R_386_GOTOFF",
	R_386_GOTPC, "R_386_GOTPC",
	R_386_32PLT, "R_386_32PLT",
	R_386_NUM, "R_386_NUM",
	-1, NULL,
};

IntegerToString r_type_M32[] = {
	R_M32_NONE, "R_M32_NONE",
	R_M32_32, "R_M32_32",
	R_M32_32_S, "R_M32_32_S",
	R_M32_PC32_S, "R_M32_PC32_S",
	R_M32_GOT32_S, "R_M32_GOT32_S",
	R_M32_PLT32_S, "R_M32_PLT32_S",
	R_M32_COPY, "R_M32_COPY",
	R_M32_GLOB_DAT, "R_M32_GLOB_DAT",
	R_M32_JMP_SLOT, "R_M32_JMP_SLOT",
	R_M32_RELATIVE, "R_M32_RELATIVE",
	R_M32_RELATIVE_S, "R_M32_RELATIVE_S",
	R_M32_NUM, "R_M32_NUM",
	-1, NULL,
};

IntegerToString r_type_SPARC[] = {
#ifdef OS5_5
	R_SPARC_NONE, "R_SPARC_NONE",
	R_SPARC_8, "R_SPARC_8",
	R_SPARC_16, "R_SPARC_16",
	R_SPARC_32, "R_SPARC_32",
	R_SPARC_DISP8, "R_SPARC_DISP8",
	R_SPARC_DISP16, "R_SPARC_DISP16",
	R_SPARC_DISP32, "R_SPARC_DISP32",
	R_SPARC_WDISP30, "R_SPARC_WDISP30",
	R_SPARC_WDISP22, "R_SPARC_WDISP22",
	R_SPARC_HI22, "R_SPARC_HI22",
	R_SPARC_22, "R_SPARC_22",
	R_SPARC_13, "R_SPARC_13",
	R_SPARC_LO10, "R_SPARC_LO10",
	R_SPARC_GOT10, "R_SPARC_GOT10",
	R_SPARC_GOT13, "R_SPARC_GOT13",
	R_SPARC_GOT22, "R_SPARC_GOT22",
	R_SPARC_PC10, "R_SPARC_PC10",
	R_SPARC_PC22, "R_SPARC_PC22",
	R_SPARC_WPLT30, "R_SPARC_WPLT30",
	R_SPARC_COPY, "R_SPARC_COPY",
	R_SPARC_GLOB_DAT, "R_SPARC_GLOB_DAT",
	R_SPARC_JMP_SLOT, "R_SPARC_JMP_SLOT",
	R_SPARC_RELATIVE, "R_SPARC_RELATIVE",
	R_SPARC_UA32, "R_SPARC_UA32",
	R_SPARC_PLT32, "R_SPARC_PLT32",
	R_SPARC_HIPLT22, "R_SPARC_HIPLT22",
	R_SPARC_LOPLT10, "R_SPARC_LOPLT10",
	R_SPARC_PCPLT32, "R_SPARC_PCPLT32",
	R_SPARC_PCPLT22, "R_SPARC_PCPLT22",
	R_SPARC_PCPLT10, "R_SPARC_PCPLT10",
	R_SPARC_10, "R_SPARC_10",
	R_SPARC_11, "R_SPARC_11",
	R_SPARC_64, "R_SPARC_64",
	R_SPARC_OLO10, "R_SPARC_OLO10",
	R_SPARC_HH22, "R_SPARC_HH22",
	R_SPARC_HM10, "R_SPARC_HM10",
	R_SPARC_LM22, "R_SPARC_LM22",
	R_SPARC_PC_HH22, "R_SPARC_PC_HH22",
	R_SPARC_PC_HM10, "R_SPARC_PC_HM10",
	R_SPARC_PC_LM22, "R_SPARC_PC_LM22",
	R_SPARC_WDISP16, "R_SPARC_WDISP16",
	R_SPARC_WDISP19, "R_SPARC_WDISP19",
	R_SPARC_GLOB_JMP, "R_SPARC_GLOB_JMP",
	R_SPARC_7, "R_SPARC_7",
	R_SPARC_5, "R_SPARC_5",
	R_SPARC_6, "R_SPARC_6",
	R_SPARC_NUM, "R_SPARC_NUM",
#else
	R_SPARC_NONE, "R_SPARC_NONE",
	R_SPARC_8, "R_SPARC_8",
	R_SPARC_16, "R_SPARC_16",
	R_SPARC_32, "R_SPARC_32",
	R_SPARC_DISP8, "R_SPARC_DISP8",
	R_SPARC_DISP16, "R_SPARC_DISP16",
	R_SPARC_DISP32, "R_SPARC_DISP32",
	R_SPARC_WDISP30, "R_SPARC_WDISP30",
	R_SPARC_WDISP22, "R_SPARC_WDISP22",
	R_SPARC_HI22, "R_SPARC_HI22",
	R_SPARC_22, "R_SPARC_22",
	R_SPARC_13, "R_SPARC_13",
	R_SPARC_LO10, "R_SPARC_LO10",
	R_SPARC_GOT10, "R_SPARC_GOT10",
	R_SPARC_GOT13, "R_SPARC_GOT13",
	R_SPARC_GOT22, "R_SPARC_GOT22",
	R_SPARC_PC10, "R_SPARC_PC10",
	R_SPARC_PC22, "R_SPARC_PC22",
	R_SPARC_WPLT30, "R_SPARC_WPLT30",
	R_SPARC_COPY, "R_SPARC_COPY",
	R_SPARC_GLOB_DAT, "R_SPARC_GLOB_DAT",
	R_SPARC_JMP_SLOT, "R_SPARC_JMP_SLOT",
	R_SPARC_RELATIVE, "R_SPARC_RELATIVE",
	R_SPARC_UA32, "R_SPARC_UA32",
	R_SPARC_PLT32, "R_SPARC_PLT32",
	R_SPARC_HIPLT22, "R_SPARC_HIPLT22",
	R_SPARC_LOPLT10, "R_SPARC_LOPLT10",
	R_SPARC_PCPLT32, "R_SPARC_PCPLT32",
	R_SPARC_PCPLT22, "R_SPARC_PCPLT22",
	R_SPARC_PCPLT10, "R_SPARC_PCPLT10",
	R_SPARC_10, "R_SPARC_10",
	R_SPARC_11, "R_SPARC_11",
	R_SPARC_64, "R_SPARC_64",
	R_SPARC_OLO10, "R_SPARC_OLO10",
	R_SPARC_HH22, "R_SPARC_HH22",
	R_SPARC_HM10, "R_SPARC_HM10",
	R_SPARC_LM22, "R_SPARC_LM22",
	R_SPARC_PC_HH22, "R_SPARC_PC_HH22",
	R_SPARC_PC_HM10, "R_SPARC_PC_HM10",
	R_SPARC_PC_LM22, "R_SPARC_PC_LM22",
	R_SPARC_WDISP16, "R_SPARC_WDISP16",
	R_SPARC_WDISP19, "R_SPARC_WDISP19",
	R_SPARC_GLOB_JMP, "R_SPARC_GLOB_JMP",
	R_SPARC_7, "R_SPARC_7",
	R_SPARC_5, "R_SPARC_5",
	R_SPARC_6, "R_SPARC_6",
	R_SPARC_DISP64, "R_SPARC_DISP64",
	R_SPARC_PLT64, "R_SPARC_PLT64",
	R_SPARC_HIX22, "R_SPARC_HIX22",
	R_SPARC_LOX10, "R_SPARC_LOX10",
	R_SPARC_H44, "R_SPARC_H44",
	R_SPARC_M44, "R_SPARC_M44",
	R_SPARC_L44, "R_SPARC_L44",
	R_SPARC_REGISTER, "R_SPARC_REGISTER",
	R_SPARC_UA64, "R_SPARC_UA64",
	R_SPARC_UA16, "R_SPARC_UA16",
	R_SPARC_NUM, "R_SPARC_NUM",
#endif
	-1, NULL,
};

#ifdef OS5_5
IntegerToString r_type_PPC[] = {
	R_PPC_NONE, "R_PPC_NONE",
	R_PPC_ADDR32, "R_PPC_ADDR32",
	R_PPC_ADDR24, "R_PPC_ADDR24",
	R_PPC_ADDR16, "R_PPC_ADDR16",
	R_PPC_ADDR16_LO, "R_PPC_ADDR16_LO",
	R_PPC_ADDR16_HI, "R_PPC_ADDR16_HI",
	R_PPC_ADDR16_HA, "R_PPC_ADDR16_HA",
	R_PPC_ADDR14, "R_PPC_ADDR14",
	R_PPC_ADDR14_BRTAKEN, "R_PPC_ADDR14_BRTAKEN",
	R_PPC_ADDR14_BRNTAKEN, "R_PPC_ADDR14_BRNTAKEN",
	R_PPC_REL24, "R_PPC_REL24",
	R_PPC_REL14, "R_PPC_REL14",
	R_PPC_REL14_BRTAKEN, "R_PPC_REL14_BRTAKEN",
	R_PPC_REL14_BRNTAKEN, "R_PPC_REL14_BRNTAKEN",
	R_PPC_GOT16, "R_PPC_GOT16",
	R_PPC_GOT16_LO, "R_PPC_GOT16_LO",
	R_PPC_GOT16_HI, "R_PPC_GOT16_HI",
	R_PPC_GOT16_HA, "R_PPC_GOT16_HA",
	R_PPC_PLTREL24, "R_PPC_PLTREL24",
	R_PPC_PLT24, "R_PPC_PLT24",
	R_PPC_COPY, "R_PPC_COPY",
	R_PPC_GLOB_DAT, "R_PPC_GLOB_DAT",
	R_PPC_JMP_SLOT, "R_PPC_JMP_SLOT",
	R_PPC_RELATIVE, "R_PPC_RELATIVE",
	R_PPC_LOCAL24PC, "R_PPC_LOCAL24PC",
	R_PPC_UADDR32, "R_PPC_UADDR32",
	R_PPC_UADDR16, "R_PPC_UADDR16",
	R_PPC_REL32, "R_PPC_REL32",
	R_PPC_PLT32, "R_PPC_PLT32",
	R_PPC_PLTREL32, "R_PPC_PLTREL32",
	R_PPC_PLT16_LO, "R_PPC_PLT16_LO",
	R_PPC_PLT16_HI, "R_PPC_PLT16_HI",
	R_PPC_PLT16_HA, "R_PPC_PLT16_HA",
	R_PPC_SDAREL16, "R_PPC_SDAREL16",
	R_PPC_SDAREL, "R_PPC_SDAREL",
	R_PPC_SECTOFF, "R_PPC_SECTOFF",
	R_PPC_SECTOFF_LO, "R_PPC_SECTOFF_LO",
	R_PPC_SECTOFF_HI, "R_PPC_SECTOFF_HI",
	R_PPC_SECTOFF_HA, "R_PPC_SECTOFF_HA",
	R_PPC_ADDR30, "R_PPC_ADDR30",
	R_PPC_NUM, "R_PPC_NUM",
	-1, NULL,
};
#endif

// tables for translating versioning types
IntegerToString vd_flags[] = {
	VER_FLG_BASE, "VER_FLG_BASE",
	VER_FLG_WEAK, "VER_FLG_WEAK",
	-1, NULL,
};

IntegerToString vn_flags[] = {
	VER_FLG_BASE, "VER_FLG_BASE",
	VER_FLG_WEAK, "VER_FLG_WEAK",
	-1, NULL,
};

IntegerToString vd_version[] = {
	VER_DEF_NONE, "VER_DEF_NONE",
	VER_DEF_CURRENT,"VER_DEF_CURRENT",
	VER_DEF_NUM, "VER_DEF_NUM",
	-1, NULL,
};

IntegerToString vn_version[] = {
	VER_NEED_NONE, "VER_NEED_NONE",
	VER_NEED_CURRENT, "VER_NEED_CURRENT",
	VER_NEED_NUM, "VER_NEED_NUM",
	-1, NULL,
};

// translating dynamic table data
IntegerToString d_tag[] = {
#ifdef OS5_5
	DT_NULL, "DT_NULL",
	DT_NEEDED, "DT_NEEDED",
	DT_PLTRELSZ, "DT_PLTRELSZ",
	DT_PLTGOT, "DT_PLTGOT",
	DT_HASH, "DT_HASH",
	DT_STRTAB, "DT_STRTAB",
	DT_SYMTAB, "DT_SYMTAB",
	DT_RELA, "DT_RELA",
	DT_RELASZ, "DT_RELASZ",
	DT_RELAENT, "DT_RELAENT",
	DT_STRSZ, "DT_STRSZ",
	DT_SYMENT, "DT_SYMENT",
	DT_INIT, "DT_INIT",
	DT_FINI, "DT_FINI",
	DT_SONAME, "DT_SONAME",
	DT_RPATH, "DT_RPATH",
	DT_SYMBOLIC, "DT_SYMBOLIC",
	DT_REL, "DT_REL",
	DT_RELSZ, "DT_RELSZ",
	DT_RELENT, "DT_RELENT",
	DT_PLTREL, "DT_PLTREL",
	DT_DEBUG, "DT_DEBUG",
	DT_TEXTREL, "DT_TEXTREL",
	DT_JMPREL, "DT_JMPREL",
	DT_MAXPOSTAGS, "DT_MAXPOSTAGS",
	DT_VERDEF, "DT_VERDEF",
	DT_VERDEFNUM, "DT_VERDEFNUM",
	DT_VERNEED, "DT_VERNEED",
	DT_VERNEEDNUM, "DT_VERNEEDNUM",
	DT_AUXILIARY, "DT_AUXILIARY",
	DT_USED, "DT_USED",
	DT_FILTER, "DT_FILTER",
#else
	DT_NULL, "DT_NULL",
	DT_NEEDED, "DT_NEEDED",
	DT_PLTRELSZ, "DT_PLTRELSZ",
	DT_PLTGOT, "DT_PLTGOT",
	DT_HASH, "DT_HASH",
	DT_STRTAB, "DT_STRTAB",
	DT_SYMTAB, "DT_SYMTAB",
	DT_RELA, "DT_RELA",
	DT_RELASZ, "DT_RELASZ",
	DT_RELAENT, "DT_RELAENT",
	DT_STRSZ, "DT_STRSZ",
	DT_SYMENT, "DT_SYMENT",
	DT_INIT, "DT_INIT",
	DT_FINI, "DT_FINI",
	DT_SONAME, "DT_SONAME",
	DT_RPATH, "DT_RPATH",
	DT_SYMBOLIC, "DT_SYMBOLIC",
	DT_REL, "DT_REL",
	DT_RELSZ, "DT_RELSZ",
	DT_RELENT, "DT_RELENT",
	DT_PLTREL, "DT_PLTREL",
	DT_DEBUG, "DT_DEBUG",
	DT_TEXTREL, "DT_TEXTREL",
	DT_JMPREL, "DT_JMPREL",
	DT_BIND_NOW, "DT_BIND_NOW",
	DT_INIT_ARRAY, "DT_INIT_ARRAY",
	DT_FINI_ARRAY, "DT_FINI_ARRAY",
	DT_INIT_ARRAYSZ, "DT_INIT_ARRAYSZ",
	DT_FINI_ARRAYSZ, "DT_FINI_ARRAYSZ",
	DT_RUNPATH, "DT_RUNPATH",
	DT_FLAGS, "DT_FLAGS",
	DT_ENCODING, "DT_ENCODING",
	DT_PREINIT_ARRAY, "DT_PREINIT_ARRAY",
	DT_PREINIT_ARRAYSZ, "DT_PREINIT_ARRAYSZ",
	DT_MAXPOSTAGS, "DT_MAXPOSTAGS",
	DT_DEPRECATED_SPARC_REGISTER, "DT_DEPRECATED_SPARC_REGISTER",
	DT_CHECKSUM, "DT_CHECKSUM",
	DT_PLTPADSZ, "DT_PLTPADSZ",
	DT_MOVEENT, "DT_MOVEENT",
	DT_MOVESZ, "DT_MOVESZ",
	DT_FEATURE_1, "DT_FEATURE_1",
	DT_POSFLAG_1, "DT_POSFLAG_1",
	DT_SYMINSZ, "DT_SYMINSZ",
	DT_SYMINENT, "DT_SYMINENT",
	DT_CONFIG, "DT_CONFIG",
	DT_DEPAUDIT, "DT_DEPAUDIT",
	DT_AUDIT, "DT_AUDIT",
	DT_PLTPAD, "DT_PLTPAD",
	DT_MOVETAB, "DT_MOVETAB",
	DT_SYMINFO, "DT_SYMINFO",
	DT_RELACOUNT, "DT_RELACOUNT",
	DT_RELCOUNT, "DT_RELCOUNT",
	DT_FLAGS_1, "DT_FLAGS_1",
	DT_VERDEF, "DT_VERDEF",
	DT_VERDEFNUM, "DT_VERDEFNUM",
	DT_VERNEED, "DT_VERNEED",
	DT_VERNEEDNUM, "DT_VERNEEDNUM",
	DT_AUXILIARY, "DT_AUXILIARY",
	DT_USED, "DT_USED",
	DT_FILTER, "DT_FILTER",
#endif
	-1, NULL,
};

RangeToString rd_tag[] = {
#ifdef OS5_5
	DT_LOPROC, DT_HIPROC, "DT_LOPROC <= value <= DT_HIPROC",
#else
	DT_LOPROC, DT_HIPROC, "DT_LOPROC <= value <= DT_HIPROC",
	DT_LOOS, DT_HIOS, "DT_LOOS <= value <= DT_HIOS",
	DT_VALRNGLO, DT_VALRNGHI, "DT_VALRNGLO <= value <= DT_VALRNGHI",
	DT_ADDRRNGLO, DT_ADDRRNGHI, "DT_ADDRRNGLO <= value <= DT_ADDRRNGHI",
#endif
	-1, -1, NULL,
};

// translating stab table data
IntegerToString n_type[] = {
	N_ABS, "N_ABS",
	N_ALIAS, "N_ALIAS",
	N_BCOMM, "N_BCOMM",
	N_BINCL, "N_BINCL",
	N_BROWS, "N_BROWS",
	N_BSS, "N_BSS",
	N_CMDLINE, "N_CMDLINE",
	N_COMM, "N_COMM",
	N_CONSTRUCT, "N_CONSTRUCT",
	N_DATA, "N_DATA",
	N_DESTRUCT, "N_DESTRUCT",
	N_ECOML, "N_ECOML",
	N_ECOMM, "N_ECOMM",
	N_EINCL, "N_EINCL",
	N_ENDM, "N_ENDM",
	N_ENTRY, "N_ENTRY",
	N_ESYM, "N_ESYM",
	N_EXCL, "N_EXCL",
	N_FN, "N_FN",
	N_FNAME, "N_FNAME",
	N_FUN, "N_FUN",
	N_GSYM, "N_GSYM",
	N_ILDPAD, "N_ILDPAD",
	N_ISYM, "N_ISYM",
	N_LBRAC, "N_LBRAC",
	N_LCSYM, "N_LCSYM",
	N_LENG, "N_LENG",
	N_LSYM, "N_LSYM",
	N_MAIN, "N_MAIN",
	N_OBJ, "N_OBJ",
	N_OPT, "N_OPT",
	N_PATCH, "N_PATCH",
	N_PC, "N_PC",
	N_PSYM, "N_PSYM",
	N_RBRAC, "N_RBRAC",
	N_ROSYM, "N_ROSYM",
	N_RSYM, "N_RSYM",
	N_SINCL, "N_SINCL",
	N_SLINE, "N_SLINE",
	N_SO, "N_SO",
	N_SOL, "N_SOL",
	N_SO_JAVA, "N_SO_JAVA",
	N_SSYM, "N_SSYM",
	N_STSYM, "N_STSYM",
	N_TCOMM, "N_TCOMM",
	N_TEXT, "N_TEXT",
	N_UNDF, "N_UNDF",
	N_USING, "N_USING",
	N_WITH, "N_WITH",
	N_XCOMM, "N_XCOMM",
	N_XLINE, "N_XLINE",
	-1, NULL,
};

// to-string translation functions
const char *
b2s(const IntegerToString *b2stbl, long v)
{
	static char s[BUFSIZ];

	s[0] = '\0';
	if (b2stbl != NULL)
	{
		int first = 1;
		for (int i=0; b2stbl[i].data != NULL; i++)
		{
			if ((b2stbl[i].key&v) != 0)
			{
				if (!first) strcat(s, " ");
				strcat(s, b2stbl[i].data);
				first = 0;
			}
		}
	}
	if (s[0] != '\0')
		return(s);
	else
		return("unknown");
}

const char *
i2s(const IntegerToString *i2stbl, long v)
{
	if (i2stbl != NULL)
	{
		for (int i=0; i2stbl[i].data != NULL; i++)
		{
			if (i2stbl[i].key == v)
				return(i2stbl[i].data);
		}
	}
	return("unknown");
}

const char *
r2s(const RangeToString *r2stbl, long v)
{
	if (r2stbl != NULL)
	{
		for (int i=0; r2stbl[i].data != NULL; i++)
		{
			if (r2stbl[i].lokey <= v && v <= r2stbl[i].hikey)
				return(r2stbl[i].data);
		}
	}
	return("unknown");
}

const char *
ir2s(const IntegerToString *i2stbl, const RangeToString *r2stbl, long v)
{
	if (i2stbl != NULL)
	{
		for (int i=0; i2stbl[i].data != NULL; i++)
		{
			if (i2stbl[i].key == v)
				return(i2stbl[i].data);
		}
	}
	if (r2stbl != NULL)
	{
		for (int i=0; r2stbl[i].data != NULL; i++)
		{
			if (r2stbl[i].lokey <= v && v <= r2stbl[i].hikey)
				return(r2stbl[i].data);
		}
	}
	return("unknown");
}

const char *
s2s(const StringToString *s2stbl, const char *v)
{
	if (s2stbl != NULL)
	{
		for (int i=0; s2stbl[i].data != NULL; i++)
		{
			if (strcmp(s2stbl[i].key, v) == 0)
				return(s2stbl[i].data);
		}
	}
	return("unknown");
}

// from-string translation functions
long
s2b(const IntegerToString *b2stbl, const char *v)
{
	MustBeTrue(b2stbl != NULL && v != NULL);
	long flags = 0;
	tokenize(v, " \t\n");
	for (int it=1; gettoken(it) != NULL; it++)
	{
		for (int i=0; b2stbl[i].data != NULL; i++)
		{
			if (strcmp(b2stbl[i].data, gettoken(it)) == 0)
				flags |= b2stbl[i].key;
		}
	}
	return(flags);
}

long
s2i(const IntegerToString *i2stbl, const char *v)
{
	MustBeTrue(i2stbl != NULL && v != NULL);
	tokenize(v, " \t\n");
	if (gettoken(1) != NULL)
	{
		for (int i=0; i2stbl[i].data != NULL; i++)
		{
			if (strcmp(i2stbl[i].data, gettoken(1)) == 0)
				return(i2stbl[i].key);
		}
	}
	return(NOTOK);
}

