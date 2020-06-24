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
class File {
public:
	// constructors and destructor
	File():
		fname(NULL), fd(NOTOK) {
		// do nothing
	}
	File(const char *fnm):
		fname(NULL), fd(NOTOK), 
		getc_b(BUFSIZ), getc_nr(BUFSIZ) {
		// sanity checks
		MustBeTrue(fnm != NULL && *fnm != '\0');

		// save file name
		fname = new char [strlen(fnm)+1];
		MustBeTrue(fname != NULL);
		strcpy(fname, fnm);
	}
	~File() {
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
		// previous file must be closed, before opening another
		MustBeTrue(fd == NOTOK);
		if (fname != NULL)
		{
			delete [] fname;
			fname = NULL;
		}

		// sanity checks on new file name
		MustBeTrue(fnm != NULL && *fnm != '\0');

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
		// file name must be known and file must be closed
		// before reopening.
		MustBeTrue(fname != NULL && *fname != '\0');
		MustBeTrue(fd == NOTOK);

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
		MustBeTrue(fname != NULL && *fname != '\0');
		MustBeTrue(fd != NOTOK);
		MustBeTrue(obuf != NULL);

		// write to file
		int nw = NOTOK;
		MustBeTrue((nw = ::write(fd, obuf, nbytes)) != NOTOK);
		return(nw);
	}
	int read(void *ibuf, int nbytes) const {
		// sanity checks
		MustBeTrue(fname != NULL && *fname != '\0');
		MustBeTrue(fd != NOTOK);
		MustBeTrue(ibuf != NULL);

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
	void rewind() {
		// sanity checks
		MustBeTrue(fname != NULL && *fname != '\0');
		MustBeTrue(fd != NOTOK);

		// rewind file to the start
		MustBeTrue(lseek(fd, 0, SEEK_SET) != NOTOK);
	}
	void stat(struct stat *sbuf) const {
		// sanity checks
		MustBeTrue(fname != NULL && *fname != '\0');
		MustBeTrue(fd != NOTOK);
		MustBeTrue(sbuf != NULL);

		// get file data and return it
		MustBeTrue(::fstat(fd, sbuf) != NOTOK);
		return;
	}

	// buffer I/O operations
	int bgetc(unsigned char &c) const {
		// read in a buffer, if needed.
		// don't do sanity checks, we need speed for
		// this function.
		if (++getc_b >= getc_nr)
		{
			MustBeTrue((getc_nr = 
				::read(fd, getc_buf, BUFSIZ)) != NOTOK);
			if (getc_nr == 0) return(getc_nr);
		}

		// return next character in buffer
		c = getc_buf[getc_b];
		return(1);
	}

private:
	// do not allow these for now
	File(const File &);
	File &operator=(const File &);

protected:
	// internal data
	char *fname;
	int fd;

	// transient data for operations
	mutable int getc_b;
	mutable int getc_nr;
	mutable unsigned char getc_buf[BUFSIZ];
};

#endif


