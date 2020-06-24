// preprocessor for oty files. removes comments and reads in 
// included files.

// headers
#include "otypp.h"

// constants
const int includesz = 7;
static int verbose = 0;
static int unbuf[BUFSIZ];
static top = -1;

// local states
enum OTYSTATES { 
	OTY_START, 
	OTY_TEXT, 
	OTY_INCLUDE, 
	OTY_COMMENT,
	OTY_C_COMMENT, 
	OTY_DONE
};

// view path table declarations 
ViewPathTable vpathtable;

// usage message
void
usage(const char *cmd)
{
	fprintf(stderr, "usage: %s [-?V] [-I dir1 [-I dir2 [...]]] "
		"[-o outfile] [file1 file2 ...]\n",
		cmd);
	return;
}

// get next character
int
MYgetc(FILE *infd)
{
	if (top >= 0)
		return(unbuf[top--]);
	else
		return(getc(infd));
}

// return a character to input stream
int
MYungetc(int c)
{
	assert(++top < BUFSIZ);
	unbuf[top] = c;
	return(c);
}

// look up viewpath and file file
int
findfile(const char *fn, char *fullfn)
{
	// check arguments
	assert(fn != NULL && *fn != 0);
	assert(fullfn != NULL);

	// check if full path is given
	if (*fn == '/')
	{
		// full path was give, don't search viewpath.
		strcpy(fullfn, fn);

		// does file exist?
		if (access(fullfn, R_OK) == OK)
			return(OK);
		else
			return(NOMATCH);
	}

	// search up viewpath for file
	for (int ip = 0; ip < vpathtable.howmany; ip++)
	{
		// get full path
		sprintf(fullfn, "%s/%s", vpathtable[ip], fn);

		// does file exist?
		if (access(fullfn, R_OK) == OK)
		{
			// found a file
			return(OK);
		}
	}

	// file was not found
	return(NOMATCH);
}

// skip white space
void
skipws(FILE *fd)
{
	for (int c = MYgetc(fd); 
	     ! feof(fd) && (c == ' ' || c == '\t'); 
	     c = MYgetc(fd)) ;
	if ( ! feof(fd)) MYungetc(c);
	return;
}

// read in c-comments
int
readComment(FILE *fd, OTYSTATES &state)
{
	// read until EOF or ending comment
	int c;
	while ( ! feof(fd))
	{
		// search for an asterick
		while ( ! feof(fd) && ((c = MYgetc(fd)) != '*')) ;
		if (c != '*') break;

		// check if it really is the end
		if ((c = MYgetc(fd)) == '/') break;

		// put look-ahead back into stream
		MYungetc(c);
	}

	// set next state
	if (feof(fd))
		state = OTY_DONE;
	else
		state = OTY_START;

	// all done
	return(OK);
}

// read in oty-comments
int
readOtyComment(FILE *fd, OTYSTATES &state)
{
	// read until EOF or newline
	int c;
	while ( ! feof(fd) && ((c = MYgetc(fd)) != '\n')) ;

	// set next state
	if (feof(fd))
		state = OTY_DONE;
	else
	{
		MYungetc(c);
		state = OTY_START;
	}

	// all done
	return(OK);
}

// read include file
int
readInclude(FILE *infd, FILE *outfd, OTYSTATES &state)
{
	// skip any leading white space
	skipws(infd);

	// get file name
	int c;
	int done = 0;
	char includefn[BUFSIZ];
	for (char *pincfn = includefn; ! done && ! feof(infd); )
	{
		// get next character
		c = MYgetc(infd);

		// check for type of character
		switch (c)
		{
		case '\n':
			MYungetc(c);
		case ' ':
		case '\t':
			// all done
			done = 1;
			break;

		case '"':
			continue;

		default:
			// store character
			*pincfn++ = c;
			break;
		}
	}
	*pincfn = 0;

	// read until EOF or newline
	while ( ! feof(infd) && ((c = MYgetc(infd)) != '\n')) ;

	// process new file
	if (process(includefn, outfd) != OK)
	{
		// report error, but don't fail yet.
		ERRORS("WARNING: unable to process include file.", 
			includefn, errno);
	}

	// next state
	if (feof(infd))
		state = OTY_DONE;
	else
		state = OTY_START;

	// all done
	return(OK);
}

// read in text
int
readText(FILE *infd, FILE *outfd, OTYSTATES &state)
{

	// read in text;
	int c;
	while (((c = MYgetc(infd)) != '#') && 
		(c != '/') && (c != EOF))
	{
		putc(c, outfd);
	}
	if (c != EOF)
	{
		MYungetc(c);
		state = OTY_START;
	}
	else
		state = OTY_DONE;
	return(OK);
}

// figure next state
int
readStart(FILE *infd, FILE *outfd, OTYSTATES &state)
{
	char buf[16];
	char *p, *pbuf;

	// get next character
	int c = MYgetc(infd);

	// determine state using look-ahead
	switch (c)
	{
	case EOF:
		// end of file
		state = OTY_DONE;
		break;

	case '/':
		// check for c-comments
		c = MYgetc(infd);
		if (c == '*')
		{
			// c-comment
			state = OTY_C_COMMENT;
		}
		else
		{
			// text
			state = OTY_TEXT;
			putc('/', outfd);
			MYungetc(c);
		}
		break;

	case '#':
		// skip whitespace
		skipws(infd);

		// check for comment or include
		pbuf = buf;
		for (p = "include";
		    (*p != 0) && ((*pbuf = MYgetc(infd)) == *p);
		     p++, pbuf++) ;

		// check where if failed
		if (*p == 0)
		{
			// we have an include
			state = OTY_INCLUDE;
		}
		else
		{
			// we have a comment
			for ( ; pbuf >= buf; pbuf--)
			{
				MYungetc(*pbuf);
			}
			state = OTY_COMMENT;
		}
		break;

	default:
		// text
		MYungetc(c);
		state = OTY_TEXT;
		break;
	}
	return(OK);
}

// process file. remove comments and read include files.
int
process(char *infname, FILE *outfd)
{
	// check arguments
	assert(outfd != NULL);
	assert(infname != NULL && *infname != 0);

	// print name of file
	VERBOSE(stdout, "otypp: processing file %s ...\n", infname);

	// check if stdin is being read
	FILE *infd = NULL;
	if (strcmp(infname, "/dev/tty") != 0)
	{
		// find file for open
		char filename[BUFSIZ];
		if (findfile(infname, filename) != OK)
		{
			ERRORS("unable to find file.", infname, errno);
			return(NOTOK);
		}

		// open input file
		if ((infd = fopen(filename, "r")) == NULL)
		{
			ERRORS("unable to open file.", filename, errno);
			return(NOTOK);
		}
	}
	else
	{
		// read in stdin
		infd = stdin;
	}

	// start reading in and writing out
	OTYSTATES state = OTY_START;
	while (state != OTY_DONE)
	{
		// what state are we in?
		switch (state)
		{
		case OTY_START:
			// determine next state
			readStart(infd, outfd, state);
			break;

		case OTY_TEXT:
			// read in text
			readText(infd, outfd, state);
			break;

		case OTY_C_COMMENT:
			// read in c-comment
			readComment(infd, state);
			break;

		case OTY_COMMENT:
			// read in c-comment
			readOtyComment(infd, state);
			break;

		case OTY_INCLUDE:
			// read in include file
			readInclude(infd, outfd, state);
			break;

		case OTY_DONE:
			// nothing to do
			break;

		default:
			assert(state == OTY_START ||
				state == OTY_TEXT ||
				state == OTY_INCLUDE ||
				state == OTY_COMMENT ||
				state == OTY_C_COMMENT ||
				state == OTY_DONE);
		}
	}

	// all done
	if (infd != stdin) fclose(infd);
	return(OK);
}

// add viewpath to viewpath table
int
addVPATH()
{
	// get viewpath from environment
	char *pvpath = getenv("VPATH");
	if (pvpath == NULL)
	{
		// nothing to add
		return(OK);
	}

	// store viewpath locally so we can search it
	char vpath[BUFSIZ];
	strcpy(vpath, pvpath);

	// add paths to viewpath table
	for (char *path = strtok(vpath, ":"); 
	     path != NULL; path = strtok(NULL, ":"))
	{
		if (*path != 0)
			vpathtable.insert(path);
		else
			vpathtable.insert(".");
	}

	// all done
	return(OK);
}

// main entry point
main(int argc, char **argv)
{
	char *outfile = NULL;

	// insert current directory in viewpath as a default.
	vpathtable.insert(".");

	// get options
	for (int c = 0; (c = getopt(argc, argv, "?Vo:I:")) != EOF; )
	{
		switch (c)
		{
		case 'o':
			// output file
			outfile = optarg;
			break;

		case 'V':
			// turn on verbose flag
			verbose = 1;
			break;

		case 'I':
			// additional directory to search
			vpathtable.insert(optarg);
			break;

		case '?':
			// usage message
			usage(argv[0]);
			exit(0);

		default:
			// invalid option
			ERROR("invalid option.", EINVAL);
			usage(argv[0]);
			exit(2);
		}
	}

	// add VPATH directories to vpath table
	if (addVPATH() != OK)
	{
		ERROR("unable to add VPATH to vpath table.", errno);
		exit(2);
	}

	// determine where to read and where to write
	FILE *outfd = NULL;
	if (outfile != NULL)
	{
		// open file for write
		if ((outfd = fopen(outfile, "w+")) == NULL)
		{
			ERRORS("unable to open file.", outfile, errno);
			exit(2);
		}
	}
	else
	{
		// write to stdout
		outfd = stdout;
	}

	// now start reading in files
	if (optind >= argc)
	{
		// process file, read in from stdin
		if (process("/dev/tty", outfd) != OK)
		{
			ERROR("process failed reading stdin.", errno);
			fclose(outfd);
			exit(2);
		}
	}
	else
	{
		// loop thru file and read in
		for ( ; optind < argc; optind++)
		{
			// process file
			if (process(argv[optind], outfd) != OK)
			{
				ERRORS("process failed reading file.", 
					argv[optind] , errno);
				fclose(outfd);
				exit(2);
			}
		}
	}

	// close output
	fclose(outfd);

	// all done
	return(0);
}

