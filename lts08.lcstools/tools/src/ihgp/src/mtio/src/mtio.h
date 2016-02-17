#ifndef __MTIO_H
#define __MTIO_H
/* header for program to read/write a list of files to a device */

/* definitions */
#define DEFAULTBLOCKSIZE (6*1024)
#define MAXFILEPATHSIZE 256
#define PRINTNEWLINE() fprintf(stderr, "\n")
#define SLASH '/'
#define DEFAULTMODE 0755

/* list of files structure */
struct ListOfFiles {
	long numberOfFiles;
	char (*sourceFiles)[MAXFILEPATHSIZE];
	char (*destinationFiles)[MAXFILEPATHSIZE];
};

/* file structure */
struct FileHeader {
	long numberOfFiles;
	long useHashSums;
};

/* record structure */
struct RecordHeader {
	char fromFilePath[MAXFILEPATHSIZE];
	char toFilePath[MAXFILEPATHSIZE];
	long sizeInBytes;
	long sizeInBlocks;
};

/* hash sum structure */
struct HashSumRecord {
	unsigned short hashSum;
};

#endif
