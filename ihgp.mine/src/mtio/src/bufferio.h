#ifndef __BUFFERIO_H
#define __BUFFERIO_H
/* header for buffer io functions */

/* required headers */
#include <stdio.h>

/* local definitions */
#undef WRITEONLY
#define WRITEONLY 1
#undef CREATEFILE
#define CREATEFILE 2
#undef CREATEMODE
#define CREATEMODE 0755

/* file descriptor structure for block io */
struct BufferIOFile {
	int ioFd;
	char *ioBuffer;
	long bufferSize;
	long bytesInBuffer;
};

/* function declarations */
extern struct BufferIOFile *bufferOpenFile();
extern int bufferCloseFile();
extern int bufferWrite();

#endif
