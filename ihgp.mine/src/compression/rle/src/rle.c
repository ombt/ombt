// simple run-length encoding compression. this encoding scheme uses an
// escape value of h'80 to indicate that the following pair of values
// are a runcount, followed by the character to print. if the data value
// h'80 is required in the output, then it is followed by a 0 to flag it.
//

// headers
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// local headers
#include "returns.h"
#include "debug.h"

// globals
int in_b = 0;
int in_nr = 0;
int out_b = 0;
int out_nw = 0;
int compress = 1;
unsigned char inbuf[BUFSIZ];
unsigned char outbuf[BUFSIZ];

// constants
const unsigned char ZeroValue = 0x0;
const unsigned char MaxRunCount = 0x7f;
const unsigned char EscapeValue = 0x80;

// usage message
void
usage(const char *cmd)
{
	cerr << "usage: " << cmd << " [-?ed] [infile] [outfile]\n\n";
	cerr << "\t-? - usage message.\n";
	cerr << "\t-e - encode input file (the default), i.e., compress it.\n";
	cerr << "\t-d - decode input file, i.e., expand it.\n\n";
	cerr << "infile - name of file to encode (compress) or decode (expand).\n";
	cerr << "if an input file is not given, then stdin is read.\n";
	cerr << "outfile - name of output file. if not given, the output goes to\n";
	cerr << "stdout.\n";
	return;
}

// get a word to encode, return the number of bytes packed, 
// or an error.
int
getdata(int infd, unsigned char &data)
{

	// out of data, read a new buffer
	if (in_b >= in_nr)
	{
		in_nr = read(infd, inbuf, BUFSIZ);
		if (in_nr <= 0)
			return(in_nr);
		in_b = 0;
	}

	// return the next character
	data = inbuf[in_b++];
	return(1);
}

// write out data
int
putdata(int outfd, unsigned char data)
{
	// flush buffer if full
	if (out_b >= BUFSIZ)
	{
		out_nw = write(outfd, &outbuf, BUFSIZ);
		if (out_nw != BUFSIZ)
			return(out_nw);
		out_b = 0;
	}

	// return the next character
	outbuf[out_b++] = data;
	return(1);
}

int
flushout(int outfd)
{
	// flush buffer if it contains any data
	if (out_b > 0)
	{
		out_nw = write(outfd, outbuf, out_b);
		if (out_nw != out_b)
			return(out_nw);
		out_b = 0;
	}
	return(1);
}

// run-length encoding scheme
void
rlencode(int infd, int outfd)
{
	// sanity checks
	MustBeTrue(infd >= 0 && outfd >= 0);

	// get input file size, 
	long infsz = 0;
	if (infd != 0)
	{
		struct stat statbuf;
		MustBeTrue(fstat(infd, &statbuf) == OK);
		infsz = statbuf.st_size;
	}

	// read in file and encode
	int status;
	unsigned long bytesread = 0;
	unsigned char runcount = 0;
	unsigned char data1, data2;
	MustBeTrue((status = getdata(infd, data1)) > 0);
	for (bytesread++; (status = getdata(infd, data2)) > 0; bytesread++)
	{
		if (data1 == data2)
		{
			if (data1 == EscapeValue)
			{
				MustBeTrue(putdata(outfd, EscapeValue) == 1);
				MustBeTrue(putdata(outfd, ZeroValue) == 1);
				runcount = 0;
			}
			else if (runcount == MaxRunCount)
			{
				MustBeTrue(putdata(outfd, EscapeValue) == 1);
				MustBeTrue(putdata(outfd, runcount) == 1);
				MustBeTrue(putdata(outfd, data1) == 1);
				runcount = 0;
			}
			else
			{
				runcount++;
			}
		}
		else
		{
			if (data1 == EscapeValue)
			{
				MustBeTrue(putdata(outfd, EscapeValue) == 1);
				MustBeTrue(putdata(outfd, ZeroValue) == 1);
			}
			else if (runcount > 0)
			{
				MustBeTrue(putdata(outfd, EscapeValue) == 1);
				MustBeTrue(putdata(outfd, runcount) == 1);
				MustBeTrue(putdata(outfd, data1) == 1);
			}
			else
			{
				MustBeTrue(putdata(outfd, data1) == 1);
			}
			data1 = data2;
			runcount = 0;
		}
	}
	MustBeTrue(status == 0);
	if (bytesread > 1)
	{
		if (data1 == EscapeValue)
		{
			MustBeTrue(putdata(outfd, EscapeValue) == 1);
			MustBeTrue(putdata(outfd, ZeroValue) == 1);
		}
		else if (runcount > 0)
		{
			MustBeTrue(putdata(outfd, EscapeValue) == 1);
			MustBeTrue(putdata(outfd, runcount) == 1);
			MustBeTrue(putdata(outfd, data1) == 1);
		}
		else
		{
			MustBeTrue(putdata(outfd, data1) == 1);
		}
	}
	else
	{
		// odd ball case ... a one byte file !!!
		MustBeTrue(putdata(outfd, data1) == 1);
	}

	// flush output buffer
	MustBeTrue(flushout(outfd) == 1);

	// get output file size
	long outfsz = 0;
	if (outfd != 1)
	{
		struct stat statbuf;
		MustBeTrue(fstat(outfd, &statbuf) == OK);
		outfsz = statbuf.st_size;
	}

	// compare files, if not stdin and not stdout
	if (infd != 0 && outfd != 1)
	{
		cerr << "Statistics:" << endl;
		cerr << "in file size : " << infsz << endl;
		cerr << "out file size: " << outfsz << endl;
		cerr << "difference   : " << (outfsz-infsz) << endl;
	}

	// close files
	MustBeTrue(infd == 0 || close(infd) == OK);
	MustBeTrue(outfd == 1 || close(outfd) == OK);

	// all done
	return;
}

// run-length decoding scheme
void
rldecode(int infd, int outfd)
{
	// sanity checks
	MustBeTrue(infd >= 0 && outfd >= 0);

	// read in file and encode
	unsigned char data1, data2, data3;
	while (getdata(infd, data1) > 0)
	{
		if (data1 != EscapeValue)
		{
			// just write the data out
			MustBeTrue(putdata(outfd, data1) == 1);
		}
		else
		{
			// we have a possible runcount and value
			MustBeTrue(getdata(infd, data2) > 0);
			if (data2 == ZeroValue)
			{
				// write out escape value
				MustBeTrue(putdata(outfd, data1) == 1);
			}
			else
			{
				// get data to write
				MustBeTrue(getdata(infd, data3) > 0);
				for (unsigned char i=0; i<=data2; i++)
				{
					MustBeTrue(putdata(outfd, data3) == 1);
				}
			}
		}
	}

	// flush output buffer
	MustBeTrue(flushout(outfd) == 1);

	// close files
	MustBeTrue(infd == 0 || close(infd) == OK);
	MustBeTrue(outfd == 1 || close(outfd) == OK);

	// all done
	return;
}

// start of encoding program
main(int argc, char **argv)
{
	// get cmd line options	
	for (int c=0; (c=getopt(argc, argv, "?ed")) != EOF; )
	{
		switch (c)
		{
		case '?':
			// print help msg
			usage(argv[0]);
			return(0);

		case 'e':
			// encode or compress input
			compress = 1;
			break;

		case 'd':
			// decode or uncompress input
			compress = 0;
			break;

		default:
			// unknown cmd line option
			ERRORD("invalid command option.", c, EINVAL);
			return(2);
		}
	}

	// catch all exceptions
	try {
		// see if any files were given
		int infd = 0; 
		if (optind < argc)
		{
			// open file for read
			MustBeTrue((infd = open(argv[optind], O_RDONLY)) != NOTOK);
		}

		// open output file
		int outfd = 1;
		if ((optind+1) < argc)
		{
			// open file for write
			MustBeTrue((outfd = open(argv[optind+1], 
				O_WRONLY|O_CREAT|O_TRUNC, 0664)) != NOTOK);
		}

		// read file and encode using a run-length encoding scheme.
		if (compress)
			rlencode(infd, outfd);
		else 
			rldecode(infd, outfd);
	} catch (const char *pe) {
		ERRORD("exception caught.", pe, errno);
		return(2);
	}

	// all done
	return(0);
}
