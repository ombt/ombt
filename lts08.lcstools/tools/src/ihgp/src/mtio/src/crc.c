/* crc routines */

/* unix headers */
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* local headers */
#include "crc.h"

/* global variables */
static FILE *crc_fd = 0;
static struct CRC_ENTRY *crc_table[CRCTABLESIZE];

/* clear crc hash table */
void
crc_clear()
{
	int ic;
	for (ic = 0; ic < CRCTABLESIZE; ic++)
	{
		crc_table[ic] = 0;
	}
	return;
}

/* remove trailing white space */
void
crc_truncate(cbuf)
char *cbuf;
{
	for ( ; (*cbuf != ' ' && *cbuf != '\t' && *cbuf != 0); cbuf++) ;
	*cbuf = 0;
	return;
}

/* zap a range of memory */
void
crc_zap(buf, bufsz)
char *buf;
int bufsz;
{
	for ( ; bufsz > 0; bufsz--, buf++)
		*buf = 0;
	return;
}

/* hashing function */
int crc_hash(buf)
char *buf;
{
	register int c;
	register unsigned long sum;
	register char *p;

	for (sum = 0, p = buf; *p != 0; p++)
	{
		c = *p;
		if (sum & 01)
		{
			sum = (sum >> 1) + 0x8000;
		}
		else
		{
			sum >>= 1;
		}
		sum += c;
		sum &= 0xFFFF;
	}

	/* return bucket */
	return((int)(sum % CRCTABLESIZE));
}

/* initialize crc table */
int
crc_open(crc_fname)
char *crc_fname;
{
	int bucket;
	char crc_buf[BUFSIZ];
	struct CRC_ENTRY *crc_record;

	/* check file name */
	if (crc_fname == 0 || *crc_fname == 0)
	{
		ERROR("null filename given.", EINVAL);
		return(NOTOK);
	}

	/* check if crc file exists */
	if (access(crc_fname, 0) != 0)
	{
		ERRORS("CRC file not found.", crc_fname, EINVAL);
		return(NOTOK);
	}

	/* clear crc table */
	crc_clear();

	/* open file for read */
	if ((crc_fd = fopen(crc_fname, "r")) == 0)
	{
		ERRORS("unable to open file.", crc_fname, errno);
		return(NOTOK);
	}

	/* read in read in records from crc file */
	while ( 1 )
	{
		/* read in a record until eof or error */
		fgets(crc_buf, BUFSIZ, crc_fd);
		if (feof(crc_fd) || ferror(crc_fd))
			break;

		/* check for null records and white space */
		if (*crc_buf != 0)
			crc_truncate(crc_buf);
		else
			continue;

		/* allocate a new record */
		crc_record = (struct CRC_ENTRY *) 
				malloc(sizeof(struct CRC_ENTRY));
		crc_zap(crc_record, sizeof(struct CRC_ENTRY));
		strcpy(crc_record->filename, crc_buf);

		/* store name in hash table */
		bucket = crc_hash(crc_buf);
		crc_record->next = crc_table[bucket];
		crc_table[bucket] = crc_record;
	}

	/* close file and return */
	fclose(crc_fd);
	return;
}

/* check if file is in crc table */
int
crc_check(fname)
char *fname;
{
	int bucket;
	struct CRC_ENTRY *pb;

	/* check file name */
	if (fname == 0 || *fname == 0)
	{
		ERROR("null filename given.", EINVAL);
		return(NOTOK);
	}

	/* get bucket */
	bucket = crc_hash(fname);

	/* search link list for a match */
	for (pb = crc_table[bucket]; pb != 0; pb = pb->next)
	{
		/* check if filename is in list */
		if (strcmp(pb->filename, fname) == 0)
		{
			/* found it */
			pb->modified = 1;
			return(OK);
		}
	}

	/* not found */
	return(NOMATCH);
}

/* close crc file. update crc values if file changed */
int
crc_close()
{
	int ib;
	struct CRC_ENTRY *pb;

	/* update crc values for products that changed */
	for (ib = 0; ib < CRCTABLESIZE; ib++)
	{
		for (pb = crc_table[ib]; pb != 0; pb = pb->next)
		{
			if (pb->modified)
				crc_calculate(pb->filename);
		}
	}

	/* all done */
	return(OK);
}

/* execute vfy:file command */
void
crc_calculate(fname)
char *fname;
{
	char buf[BUFSIZ];
	sprintf(buf, "/cft/bin/pdsenv \"VFY:FILE,FN=\\\"%s g\",gen;\\\"\"", fname);
	fprintf(stdout, "Executing ... %s\n", buf);
	system(buf);
	return;
}

/* dump crc file */
void
crc_dump()
{
	int ib;
	struct CRC_ENTRY *pb;

	/* update crc values for products that changed */
	for (ib = 0; ib < CRCTABLESIZE; ib++)
	{
		for (pb = crc_table[ib]; pb != 0; pb = pb->next)
		{
			fprintf(stdout, "%s: %s\n", 
				pb->filename,
				(pb->modified != 0) ? "YES" : "NO");
		}
	}
	return;
}
