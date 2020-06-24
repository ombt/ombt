// headers
#include "folp.h"

// convert statements to conjunctive normal form
int
convert2cnf(Semantic *ps)
{
	// convert biconditionals to implications
	if (ps->removeBiconditionals() != OK)
	{
		ERROR("removeBiconditionals failed.", EINVAL);
		return(NOTOK);
	}

	// convert implications to ORs and NOTs
	if (ps->removeConditionals() != OK)
	{
		ERROR("removeConditionals failed.", EINVAL);
		return(NOTOK);
	}

	// apply demorgan's laws to push negation down
	if (ps->demorgans() != OK)
	{
		ERROR("demorgans failed.", EINVAL);
		return(NOTOK);
	}

	// rename all variables to unique names
	if (ps->renameVariables() != OK)
	{
		ERROR("renameVariables failed.", EINVAL);
		return(NOTOK);
	}

	// remove existential quantifier with skolem functions
	if (ps->skolemize() != OK)
	{
		ERROR("skolemize failed.", EINVAL);
		return(NOTOK);
	}

	// remove universal quantifiers
	if (ps->removeUniversals() != OK)
	{
		ERROR("removeUniversals failed.", EINVAL);
		return(NOTOK);
	}

	// distribution law for ORs and ANDs.
	if (ps->distribution() != OK)
	{
		ERROR("distribution failed.", EINVAL);
		return(NOTOK);
	}

	// all done
	return(OK);
}

int
convert2cnf(ClauseType ctype, 
	List<Semantic * > &list, List<Semantic * > &cllist)
{
	// loop over all expressions and convert
	ListIterator<Semantic * > liter(list);
	for ( ; !liter.done(); liter++)
	{
		// convert to CNF form
		if (convert2cnf(liter()) != OK)
		{
			ERROR("convert2cnf failed.", EINVAL);
			return(NOTOK);
		}

		// get clauses in programs
		if (liter()->getClauses(cllist) != OK)
		{
			ERROR("getClauses failed.", EINVAL);
			return(NOTOK);
		}
	}

	// rename variables in clauses
	ListIterator<Semantic * > cliter(cllist);
	for ( ; !cliter.done(); cliter++)
	{
		// rename all variables to unique names
		if (cliter()->renameAgain() != OK)
		{
			ERROR("renameAgain failed.", EINVAL);
			return(NOTOK);
		}

		// set clause type
		cliter()->setCType(ctype);
	}
	return(OK);
}

int
convert2cnf()
{
	// convert to CNF form
	if (convert2cnf(PremiseClause, premises, clpremises) != OK)
	{
		ERROR("convert2cnf failed.", EINVAL);
		return(NOTOK);
	}
	if (convert2cnf(ExtraAxiomClause, extraaxioms, clextraaxioms) != OK)
	{
		ERROR("convert2cnf failed.", EINVAL);
		return(NOTOK);
	}
	if (convert2cnf(ConclusionClause, conclusions, clconclusions) != OK)
	{
		ERROR("convert2cnf failed.", EINVAL);
		return(NOTOK);
	}

	// all done 
	return(OK);
}

