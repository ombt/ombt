// problem decomposition class

// headers
#include "problem.h"

// constants
const int MaxSubProblems = 20;

// delete pointers in array of lists of pointers
static void
clear(Array<List<Semantic * > > &a)
{
	ArrayIterator<List<Semantic * > > ai(a);
	for ( ; !ai.done(); ai++)
	{
		ListIterator<Semantic * > li(ai());
		for ( ; !li.done(); li++)
		{
			delete li();
		}
	}
	return;
}

// constructor and destructor
Problem::Problem(const List<Semantic * > &clp, const List<Semantic * > &clx,
		const Semantic *c, const String &rt):
	clpremises(clp), clextraaxioms(clx), conclusion(c),
	andor(), reductiontype(rt), 
	subconclusions(MaxSubProblems), subextraaxioms(MaxSubProblems),
	clsubconclusions(MaxSubProblems), clsubextraaxioms(MaxSubProblems)
{
	// do nothing
}

Problem::~Problem()
{
	clear(subconclusions);
	clear(subextraaxioms);
	clear(clsubconclusions);
	clear(clsubextraaxioms);
}

// split the problem according to reduction type
int
Problem::split()
{
	// split conclusion into subproblems
	if (conclusion->split(reductiontype, andor,
			subconclusions, subextraaxioms) != OK)
	{
		ERROR("split failed.", NOTOK);
		return(NOTOK);
	}

	// all done
	return(OK);
}

// convert to CNF
int
Problem::convert2cnf()
{
	// convert to cnf;
	int i;
	int imax = subconclusions.getSize();
	for (i=0; i<imax; i++)
	{
		if (::convert2cnf(ConclusionClause, 
			subconclusions[i], clsubconclusions[i]) != OK)
		{
			ERROR("convert2cnf failed.", NOTOK);
			return(NOTOK);
		}
	}
	imax = subextraaxioms.getSize();
	for (i=0; i<imax; i++)
	{
		if (::convert2cnf(ExtraAxiomClause, 
			subextraaxioms[i], clsubextraaxioms[i]) != OK)
		{
			ERROR("convert2cnf failed.", NOTOK);
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

// negate split conclusions
int
Problem::negate()
{
	// negated subconclusions for prover
	int imax = subconclusions.getSize();
	for (int i=0; i<imax; i++)
	{
		ListIterator<Semantic * > sci(subconclusions[i]);
		for ( ; !sci.done(); sci++)
		{
			if (sci()->negate() != OK)
			{
				ERROR("negate failed.", NOTOK);
				return(NOTOK);
			}
		}
	}

	// all done
	return(OK);
}

// output operator
static void
printarray(const String &label, const Array<List<Semantic * > > &a)
{
	cout << label << endl;
	ArrayIterator<List<Semantic * > > ai(a);
	for (int p=0; !ai.done(); p++, ai++)
	{
		ListIterator<Semantic * > li(ai());
		for ( ; !li.done(); li++)
		{
			cout << "sub p" << p << ": " << *li() << endl;
		}
	}
	return;
}

ostream &
operator<<(ostream &os, const Problem &p)
{
	os << "Conclusion: " << *p.conclusion << endl;
	os << "And/Or: " << p.andor << endl;
	os << "Reduction: " << p.reductiontype << endl;
	printarray("Subproblem Extra-Axioms:", p.subextraaxioms);
	printarray("Subproblem Extra-Axioms Clauses:", p.clsubextraaxioms);
	printarray("Subproblem Conclusions:", p.subconclusions);
	printarray("Subproblem Conclusions Clauses:", p.clsubconclusions);
	return(os);
}


// write intermediate form for prover
int
Problem::writeAndOr(const String &d) const
{
	String aofnm = d + String("/andor");

	// check if andor file exists
	if (access((const char *)aofnm, F_OK) == OK)
	{
		// remove old file
		unlink((const char *)aofnm);
	}

	// open new andor file
	ofstream aofd((const char *)aofnm);
	if (!aofd)
	{
		ERRORD("unable to open file", aofnm, errno);
		return(NOTOK);
	}

	// write andor record
	aofd << (const char *)andor << endl;

	// close file
	aofd.close();
	
	// all done
	return(OK);
}

static int
writeclause(ofstream &ofd, const List<Semantic * > &l) 
{
	ListIterator<Semantic * > li(l);
	for ( ; !li.done(); li++)
	{
		String clause;
		if (li()->clauseToString(clause) != OK)
		{
			ERRORD("clause-to-string failed", *li(), errno);
			return(NOTOK);
		}
		ofd << (const char *)clause << endl;
	}
	return(OK);
}

Problem::writepfile(const String &pfnm, int i) const
{
	// check if file exists
	if (access((const char *)pfnm, F_OK) == OK)
	{
		// remove old file
		unlink((const char *)pfnm);
	}

	// open new andor file
	ofstream pfd((const char *)pfnm);
	if (!pfd)
	{
		ERRORD("unable to open file", pfnm, errno);
		return(NOTOK);
	}

	// write out file
	pfd << "PREMISES:" << endl;
	if (writeclause(pfd, clpremises) != OK)
	{
		ERROR("writeclause failed for premises.", NOTOK);
		return(NOTOK);
	}
	pfd << "END PREMISES:" << endl;
	pfd << "EXTRA AXIOMS:" << endl;
	if (writeclause(pfd, clextraaxioms) != OK)
	{
		ERROR("writeclause failed for extra axioms.", NOTOK);
		return(NOTOK);
	}
	pfd << "END EXTRA AXIOMS:" << endl;
	pfd << "SUB EXTRA AXIOMS:" << endl;
	if (writeclause(pfd, clsubextraaxioms[i]) != OK)
	{
		ERROR("writeclause failed for sub-extra axioms.", NOTOK);
		return(NOTOK);
	}
	pfd << "END SUB EXTRA AXIOMS:" << endl;
	pfd << "SUB CONCLUSIONS:" << endl;
	if (writeclause(pfd, clsubconclusions[i]) != OK)
	{
		ERROR("writeclause failed for sub-conclusions .", NOTOK);
		return(NOTOK);
	}
	pfd << "END SUB CONCLUSIONS:" << endl;

	// close file
	pfd.close();
	
	// all done
	return(OK);
}

int
Problem::writeOptions(const String &d, const Map<String, String> &opts) const
{
	String optfnm = d + String("/options");

	// check if andor file exists
	if (access((const char *)optfnm, F_OK) == OK)
	{
		// remove old file
		unlink((const char *)optfnm);
	}

	// open new options file
	ofstream optfd((const char *)optfnm);
	if (!optfd)
	{
		ERRORD("unable to open file", optfnm, errno);
		return(NOTOK);
	}

	// write andor record
	optfd << "options {" << endl;
	MapIterator<String, String> oi(opts);
	for ( ; !oi.done(); oi++)
	{
		optfd << (const char *)oi.key() << " = ";
		optfd << (const char *)oi.data() << endl;
	}
	optfd << "}" << endl;

	// close file
	optfd.close();
	
	// all done
	return(OK);
}

int
Problem::writeIR(const Map<String, String> &opts, 
		const String &dnm, const String &pnm) const
{
	// make subdirectory for reduction type and problem
	String pdir = dnm + String("/") + reductiontype + String("/");
	if (makedir(pdir) != OK)
	{
		ERRORD("unable to mkdir", pdir, errno);
		return(NOTOK);
	}
	pdir += pnm + String("/");
	if (makedir(pdir) != OK)
	{
		ERRORD("unable to mkdir", pdir, errno);
		return(NOTOK);
	}

	// write and/or string
	if (writeAndOr(pdir) != OK)
	{
		ERRORD("unable to write andor", pdir, errno);
		return(NOTOK);
	}

	// write prover options 
	if (writeOptions(pdir, opts) != OK)
	{
		ERRORD("unable to write options", pdir, errno);
		return(NOTOK);
	}

	// loop over subproblems, and write oout
	int imax = subconclusions.getSize();
	for (int i = 0; i < imax; i++)
	{
		
		// generate the pfile name
		String pfnm = pdir + String("/p") + itoa(i);

		// write out subproblem
		if (writepfile(pfnm, i) != OK)
		{
			ERRORD("unable to write pfile", pfnm, errno);
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

