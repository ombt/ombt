// preprocess source files for atp program

// unix headers
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysent.h>
#include <iostream.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <re_comp.h>
#include <string.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mylist.h"
#include "mystring.h"
#include "tuple.h"

// local definitions
char *pcwd = NULL;
char *IncludeRe = (char *)"^[ \t]*#include";
int verbose = 0;

// macros
#define IFVERBOSE1(msg) \
	if (verbose) \
		cout << msg << endl;
#define IFVERBOSE2(ntabs, msg1, msg2) \
	if (verbose) \
	{ \
		for (int itab = 1; itab <= ntabs; itab++) \
			cout << "\t"; \
		cout << msg1 << msg2 << endl; \
	}
#define ATPTEMP "atp_temp"
#define RMCMD "/usr/bin/rm -rf"

// externals
extern int optind;
extern char *optarg;

// include directories
List<String> includeDirs;

// check if a file is a directory
int
isadir(const char *fname)
{
	struct stat sbuf;
	if (fname == NULL || *fname == 0);
		return(0);
	if (stat(fname, &sbuf) != OK)
		return(0);
	return(S_ISDIR(sbuf.st_mode));
}

// expand a file
int
expandFile(int tabs, const char *ifile, FILE *ofd)
{
	// check arguments
	if (ifile == NULL || *ifile == 0)
	{
		ERROR("invalid input file name.", EINVAL);
		return(NOTOK);
	}
	if (ofd == NULL)
	{
		ERROR("output file descriptor is NULL.", EINVAL);
		return(NOTOK);
	}

	// open input file
	IFVERBOSE2(tabs, "expanding file ... ", ifile);
	FILE *ifd = fopen(ifile, "r");
	if (ifd == NULL)
	{
		ERRORD("unable to open a file", ifile, errno);
		return(NOTOK);
	}

	// include directories base
	ListIterator<String> idIter(includeDirs);

	// scan input file and expand
	char ibuf[BUFSIZ+2];
	while (fgets(ibuf, BUFSIZ, ifd) != NULL)
	{
		// check if an include file
		switch (re_exec(ibuf))
		{
		case 1:
			// we have an include file
			fprintf(ofd, "#\n");
			break;

		case 0:
			// not an include file, just print the line
			fprintf(ofd, ibuf);
			continue;

		case -1:
			// an error
			ERROR("re_exec failed.", errno);
			fclose(ifd);
			return(NOTOK);
		}

		// get current location
		long loc = ftell(ifd);
		if (loc < 0)
		{
			ERRORD("ftell failed.", ifile, -loc);
			fclose(ifd);
			return(NOTOK);
		}

		// get name of input file
		char *pfnm = strtok(ibuf, "\"");
		pfnm = strtok(NULL, "\"");

		// check if file exists
		char fname[BUFSIZ];
		if (*pfnm != '/')
		{
			// a full path was NOT given, look for file
			for (idIter.reset(); !idIter.done(); idIter++)
			{
				// generate file name and check if it exists
				sprintf(fname, "%s/%s", 
					(char *)idIter(), pfnm);
				if (access(fname, R_OK) == 0)
				{
					// check if a directory
					if (isadir(fname))
						continue;
					break;
				}
			}

			// check if a file was found
			if (idIter.done())
			{
				ERRORD("file not found.", pfnm, EINVAL);
				fclose(ifd);
				return(NOTOK);
			}
		}
		else
		{
			// a full path was given
			strcpy(fname, pfnm);
			if (access(fname, R_OK) != 0)
			{
				ERRORD("can not read file.", pfnm, errno);
				fclose(ifd);
				return(NOTOK);
			}
			if (isadir(fname))
			{
				ERRORD("file is a directory.", pfnm, errno);
				fclose(ifd);
				return(NOTOK);
			}
		}
		IFVERBOSE2(tabs+1, "including file ... ", fname);
		
		// close input file
		fclose(ifd);
		ifd = NULL;

		// trace new file
		if (expandFile(tabs+1, fname, ofd) != OK)
		{
			ERROR("expand file failed.", errno);
			return(NOTOK);
		}

		// reopen and position file
		ifd = fopen(ifile, "r");
		if (ifd == NULL)
		{
			ERRORD("unable to re-open a file", ifile, errno);
			return(NOTOK);
		}
		if (fseek(ifd, loc, SEEK_SET) != 0)
		{
			ERRORD("unable to seek in file", ifile, errno);
			fclose(ifd);
			return(NOTOK);
			
		}
	}

	// close input file
	IFVERBOSE2(tabs, "finishing file ... ", ifile);
	if (ifd != NULL)
		fclose(ifd);

	// all done
	return(OK);
}

// usage message
void
usage(const char *cmd)
{
	cout << "usage: " << cmd << " [-?VE] [-t temp_base_directory] " << endl;
	cout << "       " << " [-I include_dir [-I ...] ] file ...";
	cout << endl;
	return;
}

// main entry point
main(int argc, char **argv)
{
	// options
	int Eoption = 0;

	// temp directory - default is current directory
#if 0
	char *tmpbase = (char *)"/tmp";
#else
	char *tmpbase = (char *)".";
#endif

	// get command line options
	for (int c = 0; (c = getopt(argc, argv, "?VEt:I:")) != EOF; )
	{
		switch (c)
		{
		case 'E':
			// write expanded file to stdout
			Eoption = 1;
			break;

		case 'V':
			// verbose mode
			verbose = 1;
			break;

		case 't':
			// base directory for temp
			tmpbase = optarg;
			break;

		case 'I':
			// include directories
			if (includeDirs.insertAtEnd(String(optarg)) != OK)
			{
				ERRORD("unable to save an include directory.",
					optarg, EINVAL);
				return(2);
			}
			break;

		case '?':
			usage(argv[0]);
			return(0);

		default:
			ERRORD("invalid command option.", c, EINVAL);
			return(2);
		}
	}

	// default include dir
	if (includeDirs.insertAtEnd(String(".")) != OK)
	{
		ERRORD("unable to save default include directory.",
			".", EINVAL);
		return(2);
	}

	// get current directory
	pcwd = getcwd(NULL, BUFSIZ);
	if (pcwd == NULL)
	{
		ERROR("unable to getcwd", errno);
		return(2);
	}
	
	// check if a temp directory exists
	char tmpdir[BUFSIZ];
#if 0
	sprintf(tmpdir, "%s/%d", tmpbase, getpid());
#else
	sprintf(tmpdir, "%s/%s", tmpbase, ATPTEMP);
#endif
	if (access(tmpdir, R_OK|W_OK|X_OK) != 0)
	{
		// check if directory exists
		if (errno == ENOENT)
		{
			if (mkdir(tmpdir, 0755) != 0)
			{
				ERRORD("mkdir failed.", tmpdir, errno);
				return(2);
			}
		}
		else
		{
			ERRORD("access failed.", tmpdir, errno);
			return(2);
		}
	}

	// regular expression for includes
	if (re_comp(IncludeRe) != NULL)
	{
		ERRORD("RE compile failed.", IncludeRe, errno);
		return(2);
	}

	// save names of generated files
	List<Tuple<String, String> > ofiles;

	// read in files, and expand the includes
	for (int iarg = optind; iarg < argc; iarg++)
	{
		// skip directories
		if (isadir(argv[iarg]))
		{
			IFVERBOSE2(0, "WARNING: skipping directory ", argv[iarg]);
			continue;
		}

		// write output to stdout or temp file
		if (!Eoption)
		{
			// check if the directory exists
			char dfile[BUFSIZ];
			sprintf(dfile, "%s/%s.d", tmpdir, argv[iarg]);
			if (access(dfile, R_OK|W_OK|X_OK) != 0)
			{
				// check if directory exists
				if (errno == ENOENT)
				{
					if (mkdir(dfile, 0755) != 0)
					{
						ERRORD("mkdir failed.", 
							dfile, errno);
						return(2);
					}
				}
				else
				{
					ERRORD("access failed.", 
						dfile, errno);
					return(2);
				}
			}
			else
			{
				// delete old directory and recreate
				char rmcmd[BUFSIZ];
				sprintf(rmcmd, "%s -rf %s\n", RMCMD, dfile);
				system(rmcmd);

				// re-create new directory
				if (mkdir(dfile, 0755) != 0)
				{
					ERRORD("mkdir failed.", dfile, errno);
					return(2);
				}
			}
			
			// generate a temp file name
			char ofile[BUFSIZ];
			sprintf(ofile, "%s/%s.i", dfile, argv[iarg]);
	
			// open output file for write
			FILE *ofd = fopen(ofile, "w");
			if (ofd == NULL)
			{
				ERRORD("unable to open file.", ofile, errno);
				return(2);
			}

			// expand file
			if (expandFile(0, argv[iarg], ofd) != OK)
			{
				ERRORD("expanding a file failed.", 
					argv[iarg], errno);
				fclose(ofd);
				return(2);
			}

			// close output file
			fclose(ofd);
			ofd = NULL;
	
			// store outfile name
			Tuple<String, String> tuple(argv[iarg], ofile);
			if (ofiles.insertAtEnd(tuple) != OK)
			{
				ERRORD("insert failed.", ofile, errno);
				return(2);
			}
		}
		else
		{
			// expand file
			if (expandFile(0, argv[iarg], stdout) != OK)
			{
				ERRORD("expanding a file failed.", 
					argv[iarg], errno);
				return(2);
			}
		}
	}

	// list output files
	if (!Eoption)
	{
		IFVERBOSE1("TEMP FILES ARE: ");
		ListIterator<Tuple<String, String> > ofilesIter(ofiles);
		for (int iprob = 1; !ofilesIter.done(); ofilesIter++, iprob++)
		{
			cout << "Start Of File: " << iprob << endl;
			cout << "Input File: " << ofilesIter().key << endl;
			cout << "Output File: " << ofilesIter().data << endl;
			cout << "End Of File: " << iprob << endl;
		}
	}

	// all done
	return(0);
}
