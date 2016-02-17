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
		getc_b(BUFSIZ), getc_nr(BUFSIZ),
		getl_b(BUFSIZ), getl_nr(BUFSIZ),
		putc_b(0), putc_nw(BUFSIZ),
		putl_b(0), putl_nw(BUFSIZ) {
		// sanity checks
		MustBeTrue(fnm != NULL && *fnm != '\0');
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
	long location() {
		// return current location
		return((long)::lseek(fd, 0, SEEK_CUR));
	}
	void rewind() {
		// sanity checks
		MustBeTrue(fname != NULL && *fname != '\0');
		MustBeTrue(fd != NOTOK);

		// rewind file to the start
		MustBeTrue(lseek(fd, 0, SEEK_SET) != NOTOK);

		// reset counters
		getc_b = BUFSIZ;
		getc_nr = BUFSIZ;
		getl_b = BUFSIZ;
		getl_nr = BUFSIZ;
	}
	void stat(struct stat &statbuf) const {
		// sanity checks
		MustBeTrue(fname != NULL && *fname != '\0');
		MustBeTrue(fd != NOTOK);

		// get file data and return it
		MustBeTrue(::fstat(fd, &statbuf) != NOTOK);
		return;
	}

	// buffer I/O operations
	int bgetc(unsigned char &uc) const {
		// read in a buffer, if needed.
		// don't do sanity checks, we need speed for
		// this function.
		if (++getc_b >= getc_nr)
		{
			MustBeTrue((getc_nr = 
				::read(fd, getc_buf, BUFSIZ)) != NOTOK);
			if (getc_nr == 0) return(getc_nr);
			getc_b = 0;
		}

		// return next character in buffer
		uc = getc_buf[getc_b];
		return(1);
	}
	int bputc(unsigned char uc) const {
		// flush buffer if full
		if (putc_b >= BUFSIZ)
		{
			MustBeTrue((putc_nw = ::write(
				fd, putc_buf, BUFSIZ)) == BUFSIZ);
			putc_b = 0;
		}

		// return next character in buffer
		putc_buf[putc_b++] = uc;
		return(1);
	}
	int flush_bputc()
	{
		// flush buffer if it contains any data
		if (putc_b > 0)
		{
			MustBeTrue((putc_nw = 
				::write(fd, putc_buf, putc_b)) == putc_b);
			putc_b = 0;
		}
		return(1);
	}
	int bgetl(unsigned long &c) const {
		// read in a buffer, if needed.
		// don't do sanity checks, we need speed for
		// this function.
		if (++getl_b >= getl_nr)
		{
			MustBeTrue((getl_nr = 
				::read(fd, getl_buf, BUFSIZ)) != NOTOK);
			if (getl_nr == 0) return(getl_nr);
			getl_b = 0;
		}

		// return next character in buffer
		c = getl_buf[getl_b];
		return(1);
	}
	int bputl(unsigned long ul) const {
		// flush buffer if full
		if (putl_b >= BUFSIZ)
		{
			MustBeTrue((putl_nw = ::write(fd, putl_buf, 
				BUFSIZ*sizeof(unsigned long))) == 
				BUFSIZ*sizeof(unsigned long));
			putl_b = 0;
		}

		// return next character in buffer
		putl_buf[putl_b++] = ul;
		return(1);
	}
	int flush_bputl()
	{
		// flush buffer if it contains any data
		if (putl_b > 0)
		{
			MustBeTrue((putl_nw = ::write(fd, putl_buf, 
				putl_b*sizeof(unsigned long))) == 
				putl_b*sizeof(unsigned long));
			putl_b = 0;
		}
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
	mutable int getl_b;
	mutable int getl_nr;
	mutable unsigned long getl_buf[BUFSIZ];
	mutable int putc_b;
	mutable int putc_nw;
	mutable unsigned char putc_buf[BUFSIZ];
	mutable int putl_b;
	mutable int putl_nw;
	mutable unsigned long putl_buf[BUFSIZ];
};

#endif


