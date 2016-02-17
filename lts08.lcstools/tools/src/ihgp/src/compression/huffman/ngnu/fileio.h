#ifndef __FILEIO_H
#define __FILEIO_H
// basic file IO for data compression

// headers
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// local headers
#include "returns.h"
#include "debug.h"

// basic file class
class FileIO {
public:
	// constructors and destructor
	FileIO():
		fname(NULL), fd(NOTOK) {
		// do nothing
	}
	FileIO(const char *fnm):
		fname(NULL), fd(NOTOK) {
		// sanity checks
		MustBeTrue(fnm != NULL && *fnm != '\0');

		// save file name
		fname = new char [strlen(fnm)+1];
		MustBeTrue(fname != NULL);
		strcpy(fname, fnm);
	}
	~FileIO() {
		// close file, if open
		if (fd != NOTOK)
			::close(fd);
		fd = NOTOK;

		// get rid of saved file name
		if (fname != NULL)
			delete [] fname;
		fname = NULL;
	}

	// basic operations
	void open(const char *fnm, int op, int mode = 0) {
		// sanity checks on new file name
		MustBeTrue(fnm != NULL && *fnm != '\0');

		// close previous file, if any
		if (fd >= 0)
		{
			::close(fd);
			fd = NOTOK;
		}
		if (fname != NULL)
		{
			delete [] fname;
			fname = NULL;
		}

		// save new file name
		fname = new char [strlen(fnm)+1];
		MustBeTrue(fname != NULL);
		strcpy(fname, fnm);

		// open or create file
		if (op&O_CREAT)
		{
			MustBeTrue((fd = ::open(fname, op, mode)) != NOTOK);
		}
		else
		{
			MustBeTrue((fd = ::open(fname, op)) != NOTOK);
		}
		return;
	}
	void open(int op, int mode = 0) {
		// file name must be known.
		MustBeTrue(fname != NULL && *fname != '\0');

		// close file, before reopening.
		if (fd >= 0)
		{
			::close(fd);
			fd = NOTOK;
		}

		// open or create file
		if (op&O_CREAT)
		{
			MustBeTrue((fd = ::open(fname, op, mode)) != NOTOK);
		}
		else
		{
			MustBeTrue((fd = ::open(fname, op)) != NOTOK);
		}
		return;
	}
	void close() {
		// close file, if open; save file name.
		if (fd != NOTOK)
		{
			MustBeTrue(::close(fd) != NOTOK);
			fd = NOTOK;
		}
		return;
	}
	int write(const void *obuf, int nbytes) const {
		// sanity checks
		MustBeTrue(fd != NOTOK);
		MustBeTrue(obuf != NULL && nbytes > 0);

		// write to file
		int nw = NOTOK;
		MustBeTrue((nw = ::write(fd, obuf, nbytes)) != NOTOK);
		return(nw);
	}
	int read(void *ibuf, int nbytes) const {
		// sanity checks
		MustBeTrue(fd != NOTOK);
		MustBeTrue(ibuf != NULL && nbytes > 0);

		// read from file.
		int nr = NOTOK;
		MustBeTrue((nr = ::read(fd, ibuf, nbytes)) != NOTOK);
		return(nr);
	}
	void seek(long offset, int whence) const {
		// sanity checks
		MustBeTrue(whence == SEEK_SET || whence == SEEK_CUR ||
			   whence == SEEK_END);

		// seek in file
		MustBeTrue(::lseek(fd, offset, whence) != NOTOK);
		return;
	}
	long location() {
		// return current location
		return((long)::lseek(fd, 0, SEEK_CUR));
	}
	void rewind() {
		// sanity checks
		MustBeTrue(fd != NOTOK);

		// rewind file to the start
		MustBeTrue(lseek(fd, 0, SEEK_SET) != NOTOK);
	}
	void stat(struct stat &statbuf) const {
		// sanity checks
		MustBeTrue(fd != NOTOK);

		// get file data and return it
		MustBeTrue(::fstat(fd, &statbuf) != NOTOK);
		return;
	}

private:
	// do not allow these for now
	FileIO(const FileIO &);
	FileIO &operator=(const FileIO &);

protected:
	// internal data
	char *fname;
	int fd;
};

#endif


