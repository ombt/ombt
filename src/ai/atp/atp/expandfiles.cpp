// expand files for #include directives

// headers
#define _REGEX_RE_COMP
#include <sys/types.h>
#include <regex.h>
#include "pr.h"

// constants
const char *includeRE = "^[ \t]*#include";

// check if a file is a directory
int
isadir(const String &fname)
{
	struct stat sbuf;
	if (fname.strlen() == 0)
		return(0);
	if (stat((const char *)fname, &sbuf) != OK)
		return(0);
	return(S_ISDIR(sbuf.st_mode));
}

// expand a file
int
expandFile(int tabs, const String &ifile, FILE *ofd)
{
	// check arguments
	if (ifile.strlen() == 0)
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
	FILE *ifd = fopen((const char *)ifile, "r");
	if (ifd == NULL)
	{
		ERRORD("unable to open input file", ifile, errno);
		return(NOTOK);
	}

	// include directories base
	ListIterator<String> idIter(includedirs);

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
			fprintf(ofd, "%s", ibuf);
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
		String fname;
		if (*pfnm != '/')
		{
			// a full path was NOT given, look for file
			for (idIter.reset(); !idIter.done(); idIter++)
			{
				// generate file name and check if it exists
				fname = idIter() + String("/") + String(pfnm);
				if (access((const char *)fname, R_OK) == 0)
				{
					// check if a directory
					if (isadir(fname))
						continue;
					else
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
			fname = String(pfnm);
			if (access((const char *)fname, R_OK) != 0)
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
		ifd = fopen((const char *)ifile, "r");
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
	if (ifd != NULL)
		fclose(ifd);

	// all done
	return(OK);
}

// expand files, include any files.
int
expandFiles(const List<String> &inputfiles, 
	List<Tuple<String, String> > &expandedfiles)
{
	// check if anything to run
	if (!phases[EXPANDFILE])
	{
		cout << endl;
		cout << "Skipping expanding files ..." << endl;
		return(OK);
	}

	// clear expanded files list
	expandedfiles.clear();

	// add current directory at the end
	if (includedirs.insertAtEnd(String(".")) != OK)
	{
		ERROR("unable to insert current include directory.", EINVAL);
		return(NOTOK);
	}

	// make temp directory
	tempdir = tempbase + String("/") + String(DEFAULTTEMPDIR);
	if (access((char *)tempdir, R_OK|W_OK|X_OK) != OK)
	{
		if (errno == ENOENT)
		{
			if (mkdir((char *)tempdir, 0755) != 0)
			{
				ERRORD("mkdir failed.", tempdir, errno);
				return(NOTOK);
			}
		}
		else
		{
			ERRORD("access failed.", tempdir, errno);
			return(NOTOK);
		}
	}

	// compile regular expression for includes
	if (re_comp(includeRE) != NULL)
	{
		ERRORD("RE compile failed.", includeRE, errno);
		return(NOTOK);
	}

	// scan and expand input files
	ListIterator<String> ifIter(inputfiles);
	for ( ; !ifIter.done(); ifIter++)
	{
		// verify it is a file
		if (isadir(ifIter()))
		{
			// skip file if it is directory
			if (verbose)
			{
				cout << endl;
				cout << "WARNING: skipping directory ... ";
				cout << ifIter() << endl;
			}
			continue;
		}

		// check if the file exists
		String fname = tempdir + String("/") + ifIter() + String(".i");
		if (access((const char *)fname, F_OK) == 0)
		{
			String rmcmd("/usr/bin/rm -rf ");
			rmcmd += fname;
			system((const char *)rmcmd);
		}

		// new output file
		FILE *ofd = fopen((const char *)fname, "w");
		if (ofd == NULL)
		{
			ERRORD("unable to open file", fname, errno);
			return(NOTOK);
		}

		// expand all include files
		if (expandFile(0, ifIter(), ofd) != OK)
		{
			ERRORD("expand file failed.", ifIter(), errno);
			fclose(ofd);
			return(NOTOK);
		}

		// close output file and save new file name
		fclose(ofd);
		ofd = NULL;
		Tuple<String, String> tuple(ifIter(), fname);
		if (expandedfiles.insertAtEnd(tuple) != OK)
		{
			ERRORD("insert failed", tuple, errno);
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

