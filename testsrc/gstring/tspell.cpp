//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// spell program

// unix headers
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// local headers
#include <gstring/gstring.h>
#include <gstring/trie.h>

using namespace ombt;
using namespace std;

// input operator for a character generic string
istream &
operator>>(istream &is, GString<char> &gs)
{
	char buffer[BUFSIZ];
	is >> buffer;
	gs = GString<char>(strlen(buffer), buffer);
	return(is);
}

#define DictFile "words"

void
usage(const char *cmd)
{
	cerr << "usage: " << cmd << " [-d dictionary_file] words ..." << endl;
	return;
}

int
main(int argc, char **argv)
{
	// check if a dictionary file was given
	int iarg = 1;
	char *dictfile = (char *)DictFile;
	if (argc <= 1)
	{
		ERROR("no arguments were given.", EINVAL);
		usage(argv[0]);
		return(2);
	}
	if (strcmp("-d", argv[iarg]) == 0)
	{
		iarg++;
		dictfile = argv[iarg];
		iarg++;
	}

	// get size of file
	struct stat sbuf;
	if (stat(dictfile, &sbuf) != 0)
	{
		ERRORD("unable to stat file", dictfile, errno);
		return(2);
	}

	// allocate a buffer to read in dictionary file
	char *dfbuf = new char [sbuf.st_size+1];
	MustBeTrue(dfbuf != NULL);

	// read in dictionary file
	int dfd = open(dictfile, O_RDONLY);
	if (dfd == -1)
	{
		ERRORD("unable to open file", dictfile, errno);
		return(2);
	}
	if (read(dfd, dfbuf, sbuf.st_size) != sbuf.st_size)
	{
		ERRORD("unable to read file", dictfile, errno);
		return(2);
	}
	close(dfd);

	// actual dictionary
	Trie<char> dictionary;

	// tokenize buffer and store in dictionary
	char *pbuf = dfbuf;
	for (char *ptoken = NULL; 
	    (ptoken = strtok(pbuf, "\n\t ")) != NULL; pbuf = NULL)
	{
		// read in a word
		GString<char> word(strlen(ptoken), ptoken);

		// store word a trie
		MustBeTrue(dictionary.insert(word) == OK);
	}

	// check if words are in the dictionary
	for ( ; iarg < argc; iarg++)
	{
		GString<char> word(strlen(argv[iarg]), argv[iarg]);
		if (dictionary.includes(word))
			cout << word << " is spelled correctly." << endl;
		else
			cout << word << " is spelled incorrectly." << endl;
	}

	// all done
	return(0);
}
	
