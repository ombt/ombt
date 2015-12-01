// declare structure for tracking variable names. put it here
// so that semantic structures do not become dependent on globals.
//

// headers
#include "varnames.h"

// structure for tracking variable names
BinaryTree_AVL<Tuple<String, String> > variableNames;

// update function
#if 0
int
updateVariableNames(const Map<String, String> &nvs)
{
	MapIterator<String, String> nvsIter(nvs);
	for ( ; !nvsIter.done(); nvsIter++)
	{
		String oldvarname = nvsIter.key();
		String newvarname = nvsIter.data();
 
		Tuple<String, String> tvar(oldvarname, String(""));
		int status = variableNames.retrieve(tvar);
		if (status == OK)
		{
			// delete old tuple
			if ((status = variableNames.remove(tvar)) != OK)
				return(status);
 
			// insert new tuple
			tvar.key = newvarname;
			if ((status = variableNames.insert(tvar)) != OK)
				return(status);
		}
		else if (status == NOMATCH)
		{
			// insert new tuple
			tvar.key = newvarname;
			tvar.data = oldvarname;
			if ((status = variableNames.insert(tvar)) != OK)
				return(status);
		}
		else
		{
			ERROR("variable names retrieve failed.", errno);
			return(status);
		}
	}

	// all done
	return(OK);
}
#endif

int
updateVariableNames(const String &oldvnm, const String &newvnm)
{
	String oldvarname = oldvnm;
	String newvarname = newvnm;

	Tuple<String, String> tvar(oldvarname, String(""));
	int status = variableNames.retrieve(tvar);
	if (status == OK)
	{
#if 0
		// delete old tuple
		if ((status = variableNames.remove(tvar)) != OK)
			return(status);
#endif

		// insert new tuple
		tvar.key = newvarname;
		if ((status = variableNames.insert(tvar)) != OK)
			return(status);
	}
	else if (status == NOMATCH)
	{
		// insert new tuple
		tvar.key = newvarname;
		tvar.data = oldvarname;
		if ((status = variableNames.insert(tvar)) != OK)
			return(status);
	}
	else
	{
		ERROR("variable names retrieve failed.", errno);
		return(status);
	}

	// all done
	return(OK);
}

