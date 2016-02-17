#ifndef __CRC_H
#define __CRC_H
/* header for crc functions */

/* required headers */
#include "returns.h"
#include "debug.h"

/* local definitions */
#define CRCTABLESIZE 197
#define CRCFILENAMESIZE 256
#define CRCFILEPATH "/.crcvalues"

/* crc structure */
struct CRC_ENTRY {
	char filename[CRCFILENAMESIZE];
	struct CRC_ENTRY *next;
	int modified;
};

/* function declarations */
extern void crc_clear();
extern void crc_truncate();
extern void crc_zap();
extern int crc_hash();
extern int crc_open();
extern int crc_check();
extern int crc_close();
extern void crc_dump();
extern void crc_calculate();

#endif
