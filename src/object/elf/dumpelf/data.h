#ifndef __DATA_H
#define __DATA_H
// data structures used in elf program
struct IntToString {
	int key;
	const char *data;
};

struct StringToString {
	const char *key;
	const char *data;
};

struct BitsToString {
	unsigned int key;
	const char *data;
};

// global data
extern IntToString E_Machine[];
extern IntToString Sh_Type[];
extern StringToString Sh_Name[];
extern BitsToString Sh_Flags[];

#endif

