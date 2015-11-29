// required headers
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// other headers
#include "split.h"

// find last occurrence of a character in a string
char *
findlast(char *pstr, int c)
{
	if (pstr == NULL)
		return(NULL);
	char *pc = pstr;
	for ( ; *pstr; pstr++)
	{
		if (*pstr == c)
			pc = pstr;
	}
	if (*pc == c)
		return(pc);
	else
		return(NULL);
}

// schaum's outline heuristics
int
schaum(List<Semantic * > &axioms, List<Semantic * > &concl)
{
	// get conclusion
	Semantic *psem = NULL;
	if (concl.getCount() != 1)
	{
		ERRORD("schaum: more than one conclusion found.", 
			concl.getCount(), EINVAL);
		return(NOTOK);
	}
	if (concl.retrieveAtFront(psem) != OK)
	{
		ERROR("schaum: no conclusion found.", EINVAL);
		return(NOTOK);
	}

	// split the problem
	return(psem->schaum(filename, problemdir, axioms));
}

// no heuristics, just resolution to entire problem
int
none(List<Semantic * > &axioms, List<Semantic * > &concl)
{
	// get conclusion
	Semantic *psem = NULL;
	if (concl.getCount() != 1)
	{
		ERRORD("none: more than one conclusion found.", 
			concl.getCount(), EINVAL);
		return(NOTOK);
	}
	if (concl.retrieveAtFront(psem) != OK)
	{
		ERROR("none: no conclusion found.", EINVAL);
		return(NOTOK);
	}

	// split the problem
	return(psem->none(filename, problemdir, axioms));
}

// bledsoe's heuristics
int
bledsoe(List<Semantic * > &axioms, List<Semantic * > &concl)
{
	// get conclusion
	Semantic *psem = NULL;
	if (concl.getCount() != 1)
	{
		ERRORD("bledsoe: more than one conclusion found.", 
			concl.getCount(), EINVAL);
		return(NOTOK);
	}
	if (concl.retrieveAtFront(psem) != OK)
	{
		ERROR("bledsoe: no conclusion found.", EINVAL);
		return(NOTOK);
	}

	// split the problem
	return(psem->bledsoe(filename, problemdir, axioms));
}

// create subproblem directories for each heuristic algorithm used for
// splitting a problem.
int
createSubProblems()
{
	// separate into axioms and conclusion
	List<Semantic * > axioms;
	List<Semantic * > conclusions;
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		if (!ptreesIter()->getConclusion())
		{
			if (axioms.insertAtEnd(ptreesIter()) != OK)
			{
				ERROR("inserting axioms failed.", errno);
				return(NOTOK);
			}
		}
		else
			break;
	}
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		if (conclusions.insertAtEnd(ptreesIter()) != OK)
		{
			ERROR("inserting conclusions failed.", errno);
			return(NOTOK);
		}
	}

	// split problems into subproblems
	ListIterator<String> stIter(strategylist);
	for ( ; !stIter.done(); stIter++)
	{
		if ((String("none") == stIter()) && 
		    (none(axioms, conclusions) != OK))
		{
			ERROR("none strategy failed.", errno);
			return(NOTOK);
		}
		else if ((String("bledsoe") == stIter()) && 
			 (bledsoe(axioms, conclusions) != OK))
		{
			ERROR("bledsoe strategy failed.", errno);
			return(NOTOK);
		}
		else if ((String("schaum") == stIter()) && 
			 (schaum(axioms, conclusions) != OK))
		{
			ERROR("schaum strategy failed.", errno);
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

// make a directory
int
makedir(const char *pdir)
{
	if (access(pdir, R_OK|W_OK|X_OK) != 0)
	{
		// check if directory exists
		if (errno == ENOENT)
		{
			if (mkdir(pdir, 0755) != 0)
			{
				ERRORD("mkdir failed.", pdir, errno);
				return(NOTOK);
			}
		}
		else
		{
			ERRORD("access failed.", pdir, errno);
			return(NOTOK);
		}
	}
	return(OK);
}

// remve a directory
int
removedir(const char *pdir)
{
	// check if the directory exists
	if (access(pdir, R_OK|W_OK|X_OK) == 0)
	{
		// it exists, remove it.
		char rmcmd[BUFSIZ];
		sprintf(rmcmd, "%s %s\n", RMCMD, pdir);
		system(rmcmd);
	}
	return(OK);
}

