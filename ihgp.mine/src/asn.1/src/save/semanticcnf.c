// code for semantic record classes

// headers
#include "semantic.h"

// convert biconditionals to conditionals, use the following:
//	(A <--> B) is identical to (A --> B) && (B --> A)
int
Semantic::removeBiconditionals()
{
	// check if done
	if (!isExpression())
		return(OK);

	// check left and right sides
	if (left != NULL && left->removeBiconditionals() != OK)
	{
		ERROR("removeBiconditionals failed.", EINVAL);
		return(NOTOK);
	}
	if (right != NULL && right->removeBiconditionals() != OK)
	{
		ERROR("removeBiconditionals failed.", EINVAL);
		return(NOTOK);
	}

	// update current node, if required
	if (type == Biconditional)
	{
		// save left and right records
		Semantic *plsr = left;
		Semantic *pduplsr = new Semantic(*left);
		MustBeTrue(pduplsr != NULL);
		Semantic *prsr = right;
		Semantic *pduprsr = new Semantic(*right);
		MustBeTrue(pduprsr != NULL);

		// create implication records
		left = new Semantic(Conditional, plsr, prsr);
		MustBeTrue(left != NULL);
		right = new Semantic(Conditional, pduprsr, pduplsr);
		MustBeTrue(right != NULL);

		// change operator type to AND
		type = And;
	}

	// all done
	return(OK);
}

// convert implications to ORs, use the following:
//	(A --> B) is identical to (~A || B)
int
Semantic::removeConditionals()
{
	// check if done
	if (!isExpression())
		return(OK);

	// check left and right sides
	if (left != NULL && left->removeConditionals() != OK)
	{
		ERROR("removeConditionals failed.", EINVAL);
		return(NOTOK);
	}
	if (right != NULL && right->removeConditionals() != OK)
	{
		ERROR("removeConditionals failed.", EINVAL);
		return(NOTOK);
	}

	// update current node, if required
	if (type == Conditional)
	{
		// save left record
		Semantic *plsr = left;

		// create NOT record
		left = new Semantic(Negation, NULL, plsr);
		MustBeTrue(left != NULL);

		// change operator type to OR
		type = Or;
	}

	// all done
	return(OK);
}

// remove multiple NOTs, use ~~A <--> A rule.
int
Semantic::removeExtraNots()
{
	// save if a conclusion
	int saveConclusion = conclusion;
	int saveSOS = setofsupport;
	int saveQuery = query;

	// loop and remove extra NOTs.
	Semantic *ps = this;
	while ( 1 )
	{
		// is it a negation record
		if (type != Negation)
			break;

		// check for extra NOT
		if (right->type != Negation)
			break;

		// we have two NOTs. remove them.
		Semantic *psr = right;
		Semantic *psrr = right->right;

		// copy saved data
		type = psrr->type;
		varisused = psrr->varisused;
		conclusion = psrr->conclusion;
		setofsupport = psrr->setofsupport;
		query = psrr->query;
		name = psrr->name;
		originalname = psrr->originalname;
		value = psrr->value;
		argnum = psrr->argnum;
		left = psrr->left;
		right = psrr->right;
		pargs = psrr->pargs;

		// clear links and delete
		psr->left = NULL;
		psr->right = NULL;
		psr->pargs = NULL;
		psrr->left = NULL;
		psrr->right = NULL;
		psrr->pargs = NULL;
		delete psr;
		delete psrr;
	}

	// restore original conclusion status
	conclusion = saveConclusion;
	setofsupport = saveSOS;
	query = saveQuery;

	// all done
	return(OK);
}

// push negation as far down as possible, use demorgan's laws
//	~(A && B) identical to (~A || ~B) and
//	~(A || B) identical to (~A && ~B) and
//	~<x>(F(x)) identical to [x](~F(x))
//	~[x](F(x)) identical to <x>(~F(x))
//
int
Semantic::demorgans()
{
	// if not an expression, don't bother.
	if (!isExpression())
		return(OK);

	// remove multiple NOTs, 
	//	use ~~A <--> A and
	//	use ~~~A <--> ~A rules.
	//
	if (removeExtraNots() != OK)
	{
		ERROR("removeExtraNots failed.", EINVAL);
		return(NOTOK);
	}

	// check if we have a negated expression
	if (type != Negation)
	{
		// not a negated expression, check left and right
		if (left != NULL && left->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}
		if (right != NULL && right->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}
		return(OK);
	}

	// we have a negation, check if rhs is a predicate or an expression.
	Semantic *prs = right;
	if (!(prs->isExpression()))
	{
		// what is negated is NOT an expression, just return.
		return(OK);
	}

	// check if expression is an AND, OR, UNIVERSAL, or EXISTENTIAL
	switch (prs->type)
	{
	case Or:
	case And:
	{
		// we have a negated AND or OR expression, apply demorgans laws.
		Semantic *prsls = prs->left;
		Semantic *prsrs = prs->right;

		// create new negation records
		Semantic *pnegls = new Semantic(Negation, NULL, prsls);
		MustBeTrue(pnegls != NULL);
		Semantic *pnegrs = new Semantic(Negation, NULL, prsrs);
		MustBeTrue(pnegrs != NULL);

		// write over negation record with the new record info
		left = pnegls;
		right = pnegrs;
		switch (prs->type)
		{
		case Or:
			type = And;
			break;
		case And:
			type = Or;
			break;
		default:
			ERRORD("unexpected type", prs->type, NOTOK);
			MustBeTrue(0);
			break;
		}

		// delete old right semantic record
		prs->left = NULL;
		prs->right = NULL;
		delete prs;

		// check left and right sides
		if (left != NULL && left->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}
		if (right != NULL && right->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}
		break;
	}
	case Universal:
	case Existential:
	{
		// we have a negated UNIVERSAL or EXISTENTIAL expressions, 
		// ~[] becomes <>~
		// ~<> becones []~
		//
		switch (prs->type)
		{
		case Existential:
			type = Universal;
			break;
		case Universal:
			type = Existential;
			break;
		default:
			ERRORD("unexpected type", prs->type, NOTOK);
			MustBeTrue(0);
			break;
		}
		prs->type = Negation;
		name = prs->name;
		originalname = prs->originalname;
		prs->name = String("");
		prs->originalname = String("");
		varisused = prs->varisused;
		prs->varisused = 0;

		// check left and right sides
		if (left != NULL && left->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}
		if (right != NULL && right->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}
		break;
	}
	default:
	{
		// not any of the above.
		if (left != NULL && left->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}
		if (right != NULL && right->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}
		break;
	}
	}

	// all done
	return(OK);
}

// rename all variable names to unique names.
int
Semantic::renameVariables(List<Symbol> &localscope)
{
	// check of predicate or expression
	if (isExpression())
	{
		// check if we have quantifier
		int popscope = 0;
		if (type == Universal)
		{
			// we have a quantifier, rename variable
			popscope = 1;
			String uname = uniqueName(String("_V"));
			localscope.insertAtFront(
				Symbol(name, uname, UniversalVariable));
			name = uname;
		}
		else if (type == Existential)
		{
			// we have a quantifier, rename variable
			popscope = 1;
			String uname = uniqueName(String("_V"));
			localscope.insertAtFront(
				Symbol(name, uname, ExistentialVariable));
			name = uname;
		}

		// follow left and right branches
		if (left != NULL && left->renameVariables(localscope) != OK) 
		{
			ERROR("renameVariables failed.", EINVAL);
			return(NOTOK);
		}
		if (right != NULL && right->renameVariables(localscope) != OK)
		{
			ERROR("renameVariables failed.", EINVAL);
			return(NOTOK);
		}

		// pop scope variable if required
		if (popscope)
		{
			Symbol tmp;
			MustBeTrue(localscope.removeAtFront(tmp) == OK);
		}
	}
	else if (isPredicate())
	{
		// check if a function
		if ((type == PredicateFunction) || (type == PredicateEqual))
		{
			// we have a function, scan argument list
			ListIterator<Semantic *> pargsIter(*pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->renameVariables(localscope) != OK)
				{
					ERROR("renameVariables failed.", 
						EINVAL);
					return(NOTOK);
				}
			}
		}
	}
	else if (isTerm())
	{
		// check if a variable, function or anything else
		if (type == Variable)
		{
			// find variable in scope
			Symbol usym(name);
			if (localscope.retrieve(usym) == OK)
			{
				name = usym.getUniqueName();
				MustBeTrue(name != String(""));
			}
		}
		else if (type == Function)
		{
			// we have a function, scan argument list
			ListIterator<Semantic *> pargsIter(*pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->renameVariables(localscope) != OK)
				{
					ERROR("renameVariables failed.", 
						EINVAL);
					return(NOTOK);
				}
			}
		}
	}
	else
	{
		ERRORD("unexpected type", type, NOTOK);
		MustBeTrue(0);
	}

	// all done
	return(OK);
}

int
Semantic::renameVariables()
{
	List<Symbol> localscope;
	return(renameVariables(localscope));
}

// remove existential quantifier by using skolem functions. all skolem
// functions must be unique. remember that the skolem function is dependent
// on any universal variables that are in scope.
//
int
Semantic::skolemize(List<Symbol> &localscope)
{
	// check if expression or predicate
	if (isExpression())
	{
		// what type is it
		if (type == Universal)
		{
			// store universal variable
			localscope.insertAtEnd(
				Symbol(name, name, originalname,
					UniversalVariable, 0, 0));

			// follow right leg, left leg is null
			if (right->skolemize(localscope) != OK)
			{
				ERROR("skolemize failed.", EINVAL);
				return(NOTOK);
			}

			// remove universal variable
			Symbol tmp;
			localscope.removeAtEnd(tmp);
		}
		else if (type == Existential)
		{
			// store existential variable
			String uname = uniqueName(String("_SK"));
			localscope.insertAtEnd(
				Symbol(name, uname, originalname, 
					ExistentialVariable, 0, 0));

			// follow right leg, left leg is null
			if (right->skolemize(localscope) != OK)
			{
				ERROR("skolemize failed.", EINVAL);
				return(NOTOK);
			}

			// remove existential variable
			Symbol tmp;
			localscope.removeAtEnd(tmp);
		}
		else
		{
			// follow down other expression operators
			if (left != NULL && 
			    left->skolemize(localscope) != OK)
			{
				ERROR("skolemize failed.", EINVAL);
				return(NOTOK);
			}
			if (right != NULL && 
			    right->skolemize(localscope) != OK)
			{
				ERROR("skolemize failed.", EINVAL);
				return(NOTOK);
			}
		}
		
	}
	else if (isPredicate())
	{
		// check for functions
		if ((type == PredicateFunction) || (type == PredicateEqual))
		{
			// cycle thru arguments
			ListIterator<Semantic * > pargsIter(*pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->skolemize(localscope) != OK)
				{
					ERROR("skolemize failed.", EINVAL);
					return(NOTOK);
				}
			}
		}
	}
	else if (isTerm())
	{
		// check type of term
		switch (type)
		{
		case Variable:
		{
			// check if an existential variable
			Symbol qvarsym(name);
			MustBeTrue(localscope.retrieve(qvarsym) == OK);
			if (qvarsym.getType() != ExistentialVariable)
				break;

			// we have an existential variable
			String skolemName(qvarsym.getUniqueName());

			// need to replace this variable with a 
			// skolem function which is dependent on all
			// universal variables in scope at this time.
			//
			List<Semantic * > *npargs = new List<Semantic * >;
			MustBeTrue(npargs != NULL);
			ListIterator<Symbol> scopeIter(localscope);
			int nargs;
			for (nargs = 0; !scopeIter.done(); scopeIter++)
			{
				// get symbol
				Symbol uvar = scopeIter();

				// check if we found the current 
				// symbol. this marks the end of 
				// dependent variables for the 
				// skolem function. all other
				// existential variables are skipped.
				//
				if (uvar.getType() == ExistentialVariable)
				{
					if (uvar == Symbol(name))
						break;
					else
						continue;
				}

				// we have a universal variable in
				// scope
				//
				MustBeTrue(
					uvar.getType() == UniversalVariable);
				Semantic *parg = 
					new Semantic(uvar.getOriginalName(), 
						uvar.getName(), Variable);
				MustBeTrue(parg != NULL);
				npargs->insertAtEnd(parg);
				nargs++;
			}
			if (nargs == 0)
			{
				// skolem constant
				type = Constant;
				name = skolemName;
				pargs = NULL;
				argnum = 0;

				// delete unused argument list
				delete pargs;
			}
			else
			{
				// skolem function
				type = Function;
				name = skolemName;
				pargs = npargs;
				argnum = nargs;
			}
			break;
		}

		case Function:
		{
			// we have a function, scan its arguments
			ListIterator<Semantic *> pargsIter(*pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->skolemize(localscope) != OK)
				{
					ERROR("skolemize failed.", EINVAL);
					return(NOTOK);
				}
			}
			break;
		}
		}
	}
	else
	{
		ERRORD("unexpected type", type, NOTOK);
		MustBeTrue(0);
	}

	// all done
	return(OK);
}

int
Semantic::skolemize()
{
	// check if expression or predicate
	// if it's a predicate, there is nothing to do
	// since we are at the beginning of the expression,
	// and quantifiers are expressions.
	//
	if (!isExpression())
		return(OK);

	// list of variables in scope
	List<Symbol> localscope;

	// what type is it
	if (type == Universal)
	{
		// store universal variable
		localscope.insertAtEnd(Symbol(name, name, originalname,
					UniversalVariable, 0, 0));

		// follow right leg, left leg is null
		if (right->skolemize(localscope) != OK)
		{
			ERROR("skolemize failed.", EINVAL);
			return(NOTOK);
		}

		// remove universal variable
		Symbol tmp;
		localscope.removeAtEnd(tmp);
	}
	else if (type == Existential)
	{
		// store existential variable
		String uname = uniqueName(String("_SK"));
		localscope.insertAtEnd(Symbol(name, uname, originalname, 
					ExistentialVariable, 0, 0));

		// follow right leg, left leg is null
		if (right->skolemize(localscope) != OK)
		{
			ERROR("skolemize failed.", EINVAL);
			return(NOTOK);
		}

		// remove existential variable
		Symbol tmp;
		localscope.removeAtEnd(tmp);
	}
	else
	{
		// follow down other expression operators
		if (left != NULL && left->skolemize(localscope) != OK)
		{
			ERROR("skolemize failed.", EINVAL);
			return(NOTOK);
		}
		if (right != NULL && right->skolemize(localscope) != OK)
		{
			ERROR("skolemize failed.", EINVAL);
			return(NOTOK);
		}
	}

	// scan one more time to remove existential records
	return(removeExistentials());
}

// remove existential records once skolemizing is done.
int
Semantic::removeExistentials()
{
	// check if an expression
	if (!isExpression())
		return(OK);

	// save if a conclusion
	int saveConclusion = conclusion;
	int saveSOS = setofsupport;
	int saveQuery = query;

	// check if and universal record
	if (type == Existential)
	{
		// save right branch of expression record
		Semantic *prs = right;

		// copy all the data
		type = prs->type;
		varisused = prs->varisused;
		conclusion = prs->conclusion;
		setofsupport = prs->setofsupport;
		query = prs->query;
		name = prs->name;
		originalname = prs->originalname;
		value = prs->value;
		argnum = prs->argnum;
		left = prs->left;
		right = prs->right;
		pargs = prs->pargs;

		// delete old right record
		prs->left = NULL;
		prs->right = NULL;
		prs->pargs = NULL;
		delete prs;
	}

	// reassign since we may have a new expression pointer
	if (isExpression())
	{
		if (type == Existential && removeExistentials() != OK)
		{
			ERROR("removeExistentials failed.", EINVAL);
			return(NOTOK);
		}
		else
		{
			if (left != NULL && left->removeExistentials() != OK)
			{
				ERROR("removeExistentials failed.", EINVAL);
				return(NOTOK);
			}
			if (right != NULL && right->removeExistentials() != OK)
			{
				ERROR("removeExistentials failed.", EINVAL);
				return(NOTOK);
			}
		}
	}

	// restore conclusion status
	conclusion = saveConclusion;
	setofsupport = saveSOS;
	query = saveQuery;

	// all done
	return(OK);
}

// remove all universals. since all variables are unique, remove 
// the universal quantifiers. it is assumed that all variables are
// universally quantified.
//
int
Semantic::removeUniversals()
{
	// check if an expression
	if (!isExpression())
		return(OK);

	// save if a conclusion
	int saveConclusion = conclusion;
	int saveSOS = setofsupport;
	int saveQuery = query;

	// check if and universal record
	if (type == Universal)
	{
		// save right branch of expression record
		Semantic *prs = right;

		// copy all the data
		type = prs->type;
		varisused = prs->varisused;
		conclusion = prs->conclusion;
		setofsupport = prs->setofsupport;
		query = prs->query;
		name = prs->name;
		originalname = prs->originalname;
		value = prs->value;
		argnum = prs->argnum;
		left = prs->left;
		right = prs->right;
		pargs = prs->pargs;

		// delete old right record
		prs->left = NULL;
		prs->right = NULL;
		prs->pargs = NULL;
		delete prs;
	}

	// reassign since we may have a new expression pointer
	if (isExpression())
	{
		if (type == Universal && removeUniversals() != OK)
		{
			ERROR("removeUniversals failed.", EINVAL);
			return(NOTOK);
		}
		else
		{
			if (left != NULL && left->removeUniversals() != OK)
			{
				ERROR("removeUniversals failed.", EINVAL);
				return(NOTOK);
			}
			if (right != NULL && right->removeUniversals() != OK)
			{
				ERROR("removeUniversals failed.", EINVAL);
				return(NOTOK);
			}
		}
	}

	// restore conclusion status
	conclusion = saveConclusion;
	setofsupport = saveSOS;
	query = saveQuery;
	return(OK);
}

// distribute AND over OR using the rule:
//	(A && B) || C <--> (A || C) && (B || C)
//
int
Semantic::distributeLeftAnd(int &changed)
{
	// check if we have an expression
	if (!isExpression())
		return(OK);

	// update change flag
	changed = 1;

	// left and right semantic/expression records
	Semantic *pls = left;
	Semantic *prs = right;

	// left branch data
	Semantic *plsls = pls->left;
	Semantic *plsrs = pls->right;

	// we have and OR with a left AND. we can distribute the
	// OR using the rule (A&&B)||C = (A||C)&&(B||C)
	//
	// create new right semantic record
	right = new Semantic(Or, plsrs, prs);
	MustBeTrue(right != NULL);

	// update AND right branch
	pls->right = new Semantic(*prs);
	MustBeTrue(pls->right != NULL);

	// update all operations
	type = And;
	pls->type = Or;

	// scan new branches
	if (left != NULL && left->distribution(changed) != OK)
	{
		ERROR("distribution failed.", EINVAL);
		return(NOTOK);
	}
	if (right != NULL && right->distribution(changed) != OK)
	{
		ERROR("distribution failed.", EINVAL);
		return(NOTOK);
	}

	// all done
	return(OK);
}

// distribute OR over ANDs using the rule:
//	A || (B && C) <--> (A || B) && (A || C)
//
int
Semantic::distributeRightAnd(int &changed)
{
	// check if we have an expression
	if (!isExpression())
		return(OK);

	// update change flag
	changed = 1;

	// left and right semantic/expression records
	Semantic *pls = left;
	Semantic *prs = right;

	// right branch data
	Semantic *prsls = prs->left;
	Semantic *prsrs = prs->right;

	// we have an OR with a right AND. we can distribute the
	// OR using the rule A||(B&&C) = (A||B)&&(A||C).
	//
	// create new left semantic record
	left = new Semantic(Or, pls, prsls);
	MustBeTrue(left != NULL);

	// update AND left branch
	prs->left = new Semantic(*pls);
	MustBeTrue(prs->left != NULL);

	// update all operations
	type = And;
	prs->type = Or;

	// scan new branches
	if (left != NULL && left->distribution(changed) != OK)
	{
		ERROR("distribution failed.", EINVAL);
		return(NOTOK);
	}
	if (right != NULL && right->distribution(changed) != OK)
	{
		ERROR("distribution failed.", EINVAL);
		return(NOTOK);
	}

	// all done
	return(OK);
}

// convert program to conjunctive normal form
// distribute OR over ANDs using the rules:
//	A || (B && C) <--> (A || B) && (A || C)
//	(A && B) || C <--> (A || C) && (B || C)
//
int
Semantic::distribution(int &changed)
{
	// check if we have an expression
	if (!isExpression())
		return(OK);

	// check type of expression
	if (type != Or)
	{
		// we don't have an OR, check left and right
		if (left != NULL && left->distribution(changed) != OK)
		{
			ERROR("distribution failed.", EINVAL);
			return(NOTOK);
		}
		if (right != NULL && right->distribution(changed) != OK)
		{
			ERROR("distribution failed.", EINVAL);
			return(NOTOK);
		}
		return(OK);
	}

	// we have an OR, check if left is an AND.
	if (left->isExpression() && left->type == And)
		return(distributeLeftAnd(changed));

	// we have an OR, check if right is an AND.
	if (right->isExpression() && right->type == And)
		return(distributeRightAnd(changed));

	// check both sides
	if (left != NULL && left->distribution(changed) != OK)
	{
		ERROR("distribution failed.", EINVAL);
		return(NOTOK);
	}
	if (right != NULL && right->distribution(changed) != OK)
	{
		ERROR("distribution failed.", EINVAL);
		return(NOTOK);
	}

	// all done
	return(OK);
}

int
Semantic::distribution()
{
	for (int changed = 1; changed ; )
	{
		changed = 0;
		if (distribution(changed) != OK)
		{
			ERROR("distribution failed.", EINVAL);
			return(NOTOK);
		}
	}
	return(OK);
}

// get clauses from CNF statement
int
Semantic::getClauses(List<Semantic *> &clist)
{
	return(getClauses(clist, conclusion, setofsupport, query));
}
int
Semantic::getClauses(List<Semantic *> &clist, int con, int sos, int qry)
{
	// check if expression is an AND
	if (type != And)
	{
		Semantic *pnew = new Semantic(*this);
		MustBeTrue(pnew != NULL);
		pnew->conclusion = con;
		pnew->setofsupport = sos;
		pnew->query = qry;
		clist.insertAtEnd(pnew);
		return(OK);
	}

	// keep tracing down the ANDs
	if (left->getClauses(clist, con, sos, qry) != OK)
	{
		ERROR("getClauses failed.", EINVAL);
		return(NOTOK);
	}
	if (right->getClauses(clist, con, sos, qry) != OK)
	{
		ERROR("getClauses failed.", EINVAL);
		return(NOTOK);
	}

	// all done
	return(OK);
}

// rename all variables to unique names.
int
Semantic::renameAgain(List<Symbol> &localscope)
{
	// check of predicate or expression
	if (isExpression())
	{
		// clauses, can not be an AND
		MustBeTrue(type != And)

		// follow left and right branches
		if (left != NULL && left->renameAgain(localscope) != OK) 
		{
			ERROR("renameAgain failed.", EINVAL);
			return(NOTOK);
		}
		if (right != NULL && right->renameAgain(localscope) != OK)
		{
			ERROR("renameAgain failed.", EINVAL);
			return(NOTOK);
		}
	}
	else if (type == PredicateFunction || type == PredicateEqual)
	{
		// we have a function, scan argument list
		ListIterator<Semantic *> pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			Semantic *parg = pargsIter();
			if (parg != NULL && 
			    parg->renameAgain(localscope) != OK)
			{
				ERROR("renameAgain failed.", EINVAL);
				return(NOTOK);
			}
		}
	}
	else if (type == Variable)
	{
		// rename variable
		Symbol varsym(name);
		if (localscope.retrieve(varsym) == OK)
		{
			name = varsym.getUniqueName();
		}
		else
		{
			String uname = uniqueName(String("_V"));
			Symbol varsym(name, uname, Variable);
			name = uname;
			localscope.insertAtEnd(varsym);
		}
	}
	else if (type == Function)
	{
		// we have a function, scan argument list
		ListIterator<Semantic *> pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			Semantic *parg = pargsIter();
			if (parg != NULL && 
			    parg->renameAgain(localscope) != OK)
			{
				ERROR("renameAgain failed.", EINVAL);
				return(NOTOK);
			}
		}
	}

	// all done
	return(OK);
}

int
Semantic::renameAgain()
{
	List<Symbol> localscope;
	return(renameAgain(localscope));
}

