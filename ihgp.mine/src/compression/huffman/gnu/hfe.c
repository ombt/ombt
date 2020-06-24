// program to encode and decode a file using huffman encoding

// headers
#include "huffman.h"

// print help message
void
usage(const char *cmd)
{
	cerr << "usage: " << cmd << " [-?de] [-b bits] infile [outfile]" << endl << endl;
	cerr << "\t-? - print help message" << endl;
	cerr << "\t-d - decode (uncompress) input file" << endl;
	cerr << "\t-e - encode (compress) input file (default)" << endl;
	cerr << "\t-b - bits in decoding index (default=8)" << endl << endl;
	cerr << "infile is the file to encode or decode, and outfile" << endl;
	cerr << "is the name of the output file. if outfile is not" << endl;
	cerr << "give, then '.hf' is appended to the infile, and used" << endl;
	cerr << "as the outfile." << endl;
	return;
}

// main for huffman encoding and decoding
main(int argc, char **argv)
{
	int encode = 1;
	int bits = BitsInByte;

	// get cmd line options	
	for (int c=0; (c=getopt(argc, argv, "?edb:")) != EOF; )
	{
		switch (c)
		{
		case '?':
			// print help msg
			usage(argv[0]);
			return(0);

		case 'e':
			// encode or compress input
			encode = 1;
			break;

		case 'd':
			// decode or uncompress input
			encode = 0;
			break;

		case 'b':
			// bits in decoding index
			bits = atoi(optarg);
			MustBeTrue(bits > 0);
			break;

		default:
			// unknown cmd line option
			ERRORD("invalid command option.", c, EINVAL);
			return(2);
		}
	}

	// catch all exceptions
	try {
		// sanity check
		MustBeTrue(((optind+1) == argc) || ((optind+2) == argc));

		// get input and output files
		char infile[BUFSIZ], outfile[BUFSIZ];
		strcpy(infile, argv[optind]);
		if ((optind+1) < argc)
		{
			strcpy(outfile, argv[optind+1]);
		}
		else
		{
			strcpy(outfile, argv[optind]);
			if (encode)
				strcat(outfile, ".hf");
			else
				strcat(outfile, ".uhf");
		}

		// create huffman encoder/decoder
		Huffman hcodec(infile, outfile);

		// encode or decode a file.
		if (encode)
			hcodec.encode();
		else
			hcodec.decode(bits);
	} catch (const char *pe) {
		ERRORD("exception caught.", pe, errno);
		return(2);
	}

	// all done
	return(0);
}
