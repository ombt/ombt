#ifndef __BLOCKIO_H
#define __BLOCKIO_H
/* header for block io functions */

/* required headers */
#include <stdio.h>

/* local definitions */
#undef READONLY
#define READONLY 0
#undef WRITEONLY
#define WRITEONLY 1
#undef READWRITE
#define READWRITE 2
#undef CREATEFILE 
#define CREATEFILE 3
#undef CREATEMODE
#define CREATEMODE 0755

/* file descriptor structure for block io */
struct BlockIOFile {
	int ioFd;
	char *ioBuffer;
	long blockSize;
};

/* function declarations */
extern struct BlockIOFile *blockOpenFile();
extern int blockCloseFile();
extern int blockRead();
extern int blockWrite();
#ifdef M3B
extern int blockSetIO();
#endif

#endif
