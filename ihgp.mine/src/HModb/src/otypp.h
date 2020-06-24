#ifndef __OTYPP_H
#define __OTYPP_H

// unix headers
#include <sysent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

// local headers
#include "returns.h"
#include "debug.h"

// local definitions
#define VERBOSE if (verbose) fprintf

// view path table definitions 
struct ViewPathTable {
	// constructor and destructor
	ViewPathTable() {
		howmany = 0;
		for (int i=0; i<BUFSIZ; i++)
			list[i] = NULL;
	}
	~ViewPathTable() {
		for (int i=0; i<howmany; i++)
			delete [] list[i];
	}

	// utility functions
	void insert(char *path) {
		assert(howmany < BUFSIZ && path != NULL);
		list[howmany] = new char [strlen(path)+1];
		assert(list[howmany] != NULL);
		strcpy(list[howmany++], path);
		return;
	}
	const char *operator[](int idx) {
		assert(howmany > 0);
		assert(0 <= idx && idx < howmany);
		return(list[idx]);
	}

	// data
	int howmany;
	char *list[BUFSIZ];
};

// declarations
extern void usage(const char *);
extern int process(char *, FILE *);
extern int findfile(const char *, char *);
extern int addVPATH();
extern void skipws(FILE *);
extern int MYgetc(FILE *);
extern int readComment(FILE *);
extern int readOtyComment(FILE *);
extern int readInclude(FILE *, FILE *);

#endif
