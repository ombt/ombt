#ifndef __BUFFERIO_H
#define __BUFFERIO_H
// bufferred file IO 

// headers
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// local headers
#include "fileio.h"

// basic file class
class BufferIO {
public:
	// constructors and destructor
	BufferIO(FileIO &fio):
		file(fio), 
		getc_b(0), getc_nr(0), getc_buf(NULL),
		getl_b(0), getl_nr(0), getl_buf(NULL),
		putc_b(0), putc_nw(0), putc_buf(NULL),
		putl_b(0), putl_nw(0), putl_buf(NULL) {
		getc_buf = new unsigned char [BUFSIZ];
		MustBeTrue(getc_buf != NULL);
		getl_buf = new unsigned long [BUFSIZ];
		MustBeTrue(getl_buf != NULL);
		putc_buf = new unsigned char [BUFSIZ];
		MustBeTrue(putc_buf != NULL);
		putl_buf = new unsigned long [BUFSIZ];
		MustBeTrue(putl_buf != NULL);
	}
	~BufferIO() {
		if (getc_buf != NULL)
			delete [] getc_buf;
		getc_buf = NULL;
		if (getl_buf != NULL)
			delete [] getl_buf;
		getl_buf = NULL;
		if (putc_buf != NULL)
			delete [] putc_buf;
		putc_buf = NULL;
		if (putl_buf != NULL)
			delete [] putl_buf;
		putl_buf = NULL;
	}

	// basic operations, forward them
	void open(const char *fnm, int op, int mode = 0) {
		getc_b = getc_nr = 0;
		getl_b = getl_nr = 0;
		putc_b = putc_nw = 0;
		putl_b = putl_nw = 0;
		file.open(fnm, op, mode);
	}
	void open(int op, int mode = 0) {
		getc_b = getc_nr = 0;
		getl_b = getl_nr = 0;
		putc_b = putc_nw = 0;
		putl_b = putl_nw = 0;
		file.open(op, mode);
	}
	void close() {
		file.close();
	}
	void seek(long offset, int whence) {
		file.seek(offset, whence);
	}
	long location() {
		return(file.location());
	}
	void rewind() {
		file.rewind();
	}

	// buffer I/O operations
	int bgetc(unsigned char &uc) const {
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
	BufferIO(const BufferIO &);
	BufferIO &operator=(const BufferIO &);

protected:
	// internal data
	FileIO &file;

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


