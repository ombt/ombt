// code for semantic record classes

// headers
#include "semantic.h"

// static data
long Semantic::counter = 0;

// function/predicate argument class
Argument::Argument():
	SemanticRep(0), type(Unknown), argnum(0), 
	name(), value(), pargs(NULL)
{
	// nothing to do
}

Argument::Argument(const Argument &src):
	SemanticRep(src), type(src.type), argnum(src.argnum), 
	name(src.name), value(src.value), pargs(NULL)
{
	// check if we have an argument list
	if (src.pargs != NULL)
	{
		// allocate a new list
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);

		// now copy data explicitly since the data are pointers
		ListIterator<Semantic * > pargsIter(*src.pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			Semantic *parg = new Semantic(*pargsIter());
			MustBeTrue(parg != NULL);
			pargs->insertAtEnd(parg);
		}
	}
}

Argument::Argument(Type t, const String &s, int c):
	SemanticRep(c),
	type(t), argnum(0), name(s), value(s), pargs(NULL)
{
	MustBeTrue(type == QuotedString || 
		   type == Constant ||
		   type == Variable ||
		   type == Number);
}

Argument::Argument(Type t, const String &s, List<Semantic *> *pla, 
	int an, int c): 
	SemanticRep(c), type(t), argnum(an), name(s), value(), pargs(pla)
{
	MustBeTrue(type == Function && pla != NULL);
}

Argument::~Argument()
{
	if (type == Function && pargs != NULL)
	{
		ListIterator<Semantic *> pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			if (pargsIter() != NULL)
				delete pargsIter();
		}
		pargs->clear();
		delete pargs;
		pargs = NULL;
	}
}

// duplicate the item
SemanticRep *
Argument::copy() const
{
	SemanticRep *p = new Argument(*this);
	MustBeTrue(p != NULL);
	return(p);
}

// assignment operator
Argument &
Argument::operator=(const Argument &rhs)
{
	MustBeTrue(0);
	if (this != &rhs)
	{
		// copy any semantic data
		SemanticRep::operator=(rhs);

		// copy argument data
		type = rhs.type;
		argnum = rhs.argnum;
		name = rhs.name;
		value = rhs.value;

		// check if we have an existing argument list
		if (pargs != NULL)
		{
			ListIterator<Semantic *> pargsIter(*pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				if (pargsIter() != NULL)
					delete pargsIter();
			}
			pargs->clear();
			delete pargs;
			pargs = NULL;
		}

		// check if we have an argument list
		if (rhs.pargs != NULL)
		{
			// allocate a new list
			pargs = new List<Semantic * >;
			MustBeTrue(pargs != NULL);

			// now copy data explicitly since the data are pointers
			ListIterator<Semantic * > pargsIter(*rhs.pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = new Semantic(*pargsIter());
				MustBeTrue(parg != NULL);
				pargs->insertAtEnd(parg);
			}
		}
	}
	return(*this);
}

// typing info
int
Argument::isPredicate() const
{
	return(0);
}

int
Argument::isExpression() const
{
	return(0);
}

int
Argument::isArgument() const
{
	return(1);
}

// print data
void
Argument::dumpData(ostream &os) const
{
	switch (type)
	{
	case Variable:
		os << name << " ";
		break;
	case Constant:
		os << name << " ";
		break;
	case Function:
	{
		os << name << "( ";
		ListIterator<Semantic *> pargsIter(*pargs);
		for (int first = 1; !pargsIter.done(); pargsIter++)
		{
			if (!first)
				os << ", ";
			os << *pargsIter();
			first = 0;
		}
		os << ") ";
		// os << "[" << pargs << "] ";
		break;
	}
	case QuotedString:
		os << "\"" << value << "\" ";
		break;
	case Number:
		os << value << " ";
		break;
	default:
		MustBeTrue(0);
	}
	return;
}

ostream &
operator<<(ostream &os, const Argument &a)
{
	a.dumpData(os);
	return(os);
}

// semantic record representation constructors and destructor
SemanticRep::SemanticRep(int c):
	conclusion(c)
{
	// nothing to do here
}

SemanticRep::SemanticRep(const SemanticRep &src):
	conclusion(src.conclusion)
{
	// nothing to do here
}

SemanticRep::~SemanticRep()
{
	conclusion = 0;
}

// assignment operator
SemanticRep &
SemanticRep::operator=(const SemanticRep &rhs)
{
	// check for self-assignment
	if (this != &rhs)
		conclusion = rhs.conclusion;
	return(*this);
}

// typing info
int
SemanticRep::isPredicate() const
{
	return(0);
}

int
SemanticRep::isExpression() const
{
	return(0);
}

int
SemanticRep::isArgument() const
{
	return(0);
}

// print data
void
SemanticRep::dumpData(ostream &os) const
{
	os << "conclusion = " << conclusion << endl;
	return;
}

ostream &
operator<<(ostream &os, const SemanticRep &sr)
{
	sr.dumpData(os);
	return(os);
}

// predicate class constructors and destructor
Predicate::Predicate():
	SemanticRep(), argnum(0), type(Unknown), name(), pargs(NULL)
{
	// nothing to do
}

Predicate::Predicate(const Predicate &src):
	SemanticRep(src), argnum(src.argnum), type(src.type),
	name(src.name), value(src.value), pargs(NULL)
{
	// check if we have an argument list
	if (src.pargs != NULL)
	{
		// allocate a new list
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);

		// now copy data explicitly since the data are pointers
		ListIterator<Semantic * > pargsIter(*src.pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			Semantic *parg = new Semantic(*pargsIter());
			MustBeTrue(parg != NULL);
			pargs->insertAtEnd(parg);
		}
	}
}

Predicate::Predicate(Type t, const String &n, const String &v, int c):
	SemanticRep(c), argnum(0), type(t), name(n), value(v), pargs(NULL)
{
	MustBeTrue(type == Constant || type == LogicalConstant);
}

Predicate::Predicate(Type t, const String &n, 
	List<Semantic *> *pla, int na, int c):
	SemanticRep(c), argnum(na), type(t), name(n), value(), pargs(pla)
{
	MustBeTrue(type == Function && pla != NULL);
}

Predicate::~Predicate()
{
	if (type == Function && pargs != NULL)
	{
		ListIterator<Semantic *> pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			if (pargsIter() != NULL)
				delete pargsIter();
		}
		pargs->clear();
		delete pargs;
		pargs = NULL;
	}
}

// duplicate the item
SemanticRep *
Predicate::copy() const
{
	SemanticRep *p = new Predicate(*this);
	MustBeTrue(p != NULL);
	return(p);
}

// assignment operator
Predicate &
Predicate::operator=(const Predicate &rhs)
{
	// check for self-assignment
	if (this != &rhs)
	{
		// copy any semantic data
		SemanticRep::operator=(rhs);

		// copy data specific to atomic record
		argnum = rhs.argnum;
		type = rhs.type;
		name = rhs.name;

		// check if we have an existing argument list
		if (pargs != NULL)
		{
			ListIterator<Semantic *> pargsIter(*pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				if (pargsIter() != NULL)
					delete pargsIter();
			}
			pargs->clear();
			delete pargs;
			pargs = NULL;
		}

		// check if we have an argument list
		if (rhs.pargs != NULL)
		{
			// allocate a new list
			pargs = new List<Semantic * >;
			MustBeTrue(pargs != NULL);

			// now copy data explicitly since the data are pointers
			ListIterator<Semantic * > pargsIter(*rhs.pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = new Semantic(*pargsIter());
				MustBeTrue(parg != NULL);
				pargs->insertAtEnd(parg);
			}
		}
	}
	return(*this);
}

// typing info
int
Predicate::isPredicate() const
{
	return(1);
}

int
Predicate::isExpression() const
{
	return(0);
}

int
Predicate::isArgument() const
{
	return(0);
}

// print data
void
Predicate::dumpData(ostream &os) const
{
	switch (type)
	{
	case LogicalConstant:
		os << value << " ";
		break;

	case Constant:
		os << name << " ";
		break;

	case Function:
	{
		os << name << "( ";
		ListIterator<Semantic *> pargsIter(*pargs);
		for (int first = 1; !pargsIter.done(); pargsIter++)
		{
			if (!first)
				os << ", ";
			os << *pargsIter();
			first = 0;
		}
		os << ") ";
		// os << "[" << pargs << "] ";
		break;
	}

	default:
		MustBeTrue(0);
		break;
	}
	return;
}

ostream &
operator<<(ostream &os, const Predicate &sr)
{
	sr.dumpData(os);
	return(os);
}

// expression class constructors and destructor
Expression::Expression():
	SemanticRep(), type(Unknown), left(NULL), right(NULL),
	name(), varisused(0)
{
	// nothing to do 
}

Expression::Expression(const Expression &src):
	SemanticRep(src), type(src.type), left(NULL), right(NULL),
	name(src.name), varisused(src.varisused)
{
	if (src.left != NULL)
	{
		left = new Semantic(*src.left);
		MustBeTrue(left != NULL);
	}
	if (src.right != NULL)
	{
		right = new Semantic(*src.right);
		MustBeTrue(right != NULL);
	}
}

Expression::Expression(Type t, Semantic *l, Semantic *r, int c):
	SemanticRep(c), type(t), left(l), right(r), name(), varisused(0)
{
	// nothing to do
}

Expression::Expression(Type t, Semantic *r, const String &n, int c):
	SemanticRep(c), type(t), left(NULL), right(r), name(n)
{
	// nothing to do
}

Expression::~Expression()
{
	type = Unknown;
	if (left != NULL) 
	{
		delete left;
		left = NULL;
	}
	if (right != NULL) 
	{
		delete right;
		right = NULL;
	}
}

// duplicate the item
SemanticRep *
Expression::copy() const
{
	SemanticRep *p = new Expression(*this);
	MustBeTrue(p != NULL);
	return(p);
}

// assignment operator
Expression &
Expression::operator=(const Expression &rhs)
{
	// check for self-assignment
	if (this != &rhs)
	{
		// copy any semantic data
		SemanticRep::operator=(rhs);

		// copy data specific to expression record
		type = rhs.type;
		if (left != NULL)
		{
			delete left;
			left = NULL;
		}
		if (rhs.left != NULL)
		{
			left = new Semantic(*rhs.left);
			MustBeTrue(left != NULL);
		}
		if (right != NULL)
		{
			delete right;
			right = NULL;
		}
		if (rhs.right != NULL)
		{
			right = new Semantic(*rhs.right);
			MustBeTrue(right != NULL);
		}
		name = rhs.name;
		varisused = rhs.varisused;
	}
	return(*this);
}

// typing info
int
Expression::isPredicate() const
{
	return(0);
}

int
Expression::isExpression() const
{
	return(1);
}

int
Expression::isArgument() const
{
	return(0);
}

// print data
void
Expression::dumpData(ostream &os) const
{
	os << "( ";
	if (left != NULL)
		os << *left;
	switch (type)
	{
	case And:
		os << "&& ";
		break;
	case Or:
		os << "|| ";
		break;
	case Conditional:
		os << "--> ";
		break;
	case Biconditional:
		os << "<--> ";
		break;
	case Negation:
		os << "~ ";
		break;
	case Universal:
		os << "< ";
		os << name << " ";
		os << ">( ";
		break;
	case Existential:
		os << "[ ";
		os << name << " ";
		os << "]( ";
		break;
	}
	if (right != NULL)
		os << *right;
	if ((type == Universal) || (type == Existential)) 
		os << ") ) ";
	else
		os << ") ";
	return;
}

ostream &
operator<<(ostream &os, const Expression &sr)
{
	sr.dumpData(os);
	return(os);
}

// semantic record constructors and destructor
Semantic::Semantic():
	prep(NULL)
{
	// nothing to do
}

Semantic::Semantic(const Semantic &src):
	prep(NULL)
{
	MustBeTrue(src.prep != NULL);
	if (src.prep != NULL)
	{
		prep = src.prep->copy();
		MustBeTrue(prep != NULL);
	}
}

Semantic::Semantic(Expression::Type t, Semantic *l, Semantic *r, int c):
	prep(NULL)
{
	prep = new Expression(t, l, r, c);
	MustBeTrue(prep != NULL);
}

Semantic::Semantic(Expression::Type t, const String &s, Semantic *r, int c):
	prep(NULL)
{
	prep = new Expression(t, r, s, c);
	MustBeTrue(prep != NULL);
}

Semantic::Semantic(Predicate::Type t, const String &n, const String &v, int c):
	prep(NULL)
{
	prep = new Predicate(t, n, v, c);
	MustBeTrue(prep != NULL);
}

Semantic::Semantic(Predicate::Type t, const String &n, List<Semantic *> *pla, 
	int na, int c):
	prep(NULL)
{
	prep = new Predicate(t, n, pla, na, c);
	MustBeTrue(prep != NULL);
}

Semantic::Semantic(Argument::Type t, const String &s, int c):
	prep(NULL)
{
	prep = new Argument(t, s, c);
	MustBeTrue(prep != NULL);
}

Semantic::Semantic(Argument::Type t, const String &n, List<Semantic *> *pla, 
	int na, int c):
	prep(NULL)
{
	prep = new Argument(t, n, pla, na, c);
	MustBeTrue(prep != NULL);
}

Semantic::~Semantic()
{
	if (prep != NULL)
	{
		delete prep;
		prep = NULL;
	}
}

// assignment operator
Semantic &
Semantic::operator=(const Semantic &rhs)
{
	if (this != &rhs)
	{
		if (prep != NULL)
		{
			delete prep;
			prep = NULL;
		}
		if (rhs.prep != NULL)
		{
			prep = rhs.prep->copy();
			MustBeTrue(prep != NULL);
		}
	}
	return(*this);
}

// access conclusion data
int
Semantic::getConclusion() const
{
	MustBeTrue(prep != NULL);
	return(prep->conclusion);
}

void
Semantic::setConclusion(int c)
{
	MustBeTrue(prep != NULL);
	prep->conclusion = c;
}

// typing info
int
Semantic::isPredicate() const
{
	return(prep->isPredicate());
}

int
Semantic::isExpression() const
{
	return(prep->isExpression());
}

int
Semantic::isArgument() const
{
	return(prep->isArgument());
}

// convert biconditionals to conditionals, use the following:
//	(A <--> B) is identical to (A --> B) && (B --> A)
int
Semantic::removeBiconditionals()
{
	// check if done
	if (!isExpression())
		return(OK);

	// check left and right sides
	Expression *pe = (Expression *)prep;
	if (pe->left != NULL && pe->left->removeBiconditionals() != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->removeBiconditionals() != OK)
		return(NOTOK);

	// update current node, if required
	if (pe->type == Expression::Biconditional)
	{
		// save left and right records
		Semantic *plsr = pe->left;
		Semantic *pduplsr = new Semantic(*pe->left);
		MustBeTrue(pduplsr != NULL);
		Semantic *prsr = pe->right;
		Semantic *pduprsr = new Semantic(*pe->right);
		MustBeTrue(pduprsr != NULL);

		// create implication records
		pe->left = 
			new Semantic(Expression::Conditional, plsr, prsr);
		MustBeTrue(pe->left != NULL);
		pe->right = 
			new Semantic(Expression::Conditional, pduprsr, pduplsr);
		MustBeTrue(pe->right != NULL);

		// change operator type to AND
		pe->type = Expression::And;
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
	Expression *pe = (Expression *)prep;
	if (pe->left != NULL && pe->left->removeConditionals() != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->removeConditionals() != OK)
		return(NOTOK);

	// update current node, if required
	if (pe->type == Expression::Conditional)
	{
		// save left record
		Semantic *plsr = pe->left;

		// create NOT record
		pe->left = new Semantic(Expression::Negation, NULL, plsr);
		MustBeTrue(pe->left != NULL);

		// change operator type to OR
		pe->type = Expression::Or;
	}

	// all done
	return(OK);
}

// remove multiple NOTs, use ~~A <--> A rule.
int
Semantic::removeExtraNots()
{
	// save if a conclusion
	int saveConclusion = prep->conclusion;

	// loop and remove extra NOTs.
	Semantic *ps = this;
	while ( 1 )
	{
		// check we have an expression
		if (!(ps->isExpression()))
			break;

		// get expression record and check for NOT
		Expression *pe = (Expression *)ps->prep;
		if (pe->type != Expression::Negation)
			break;

		// check for extra NOT
		Semantic *ps2 = pe->right;
		if (!(ps2->isExpression()))
			break;
		Expression *pe2 = (Expression *)ps2->prep;
		if (pe2->type != Expression::Negation)
			break;

		// we have two NOTs. remove them.
		Semantic *psaves = pe2->right;
		pe2->right = NULL;
		delete pe;

		// copy saved data
		ps->prep = psaves->prep;
		psaves->prep = NULL;
		delete psaves;
	}

	// restore original conclusion status
	prep->conclusion = saveConclusion;

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
	// check if a predicate or an expression
	if (!isExpression())
		return(OK);

	// remove multiple NOTs, 
	//	use ~~A <--> A and
	//	use ~~~A <--> ~A rules.
	//
	if (removeExtraNots() != OK)
		return(NOTOK);

	// check if a predicate or an expression. this second check
	// is required since it is possible that when all extra
	// negations are removed, only a predicate is left.
	//
	if (!isExpression())
		return(OK);

	// check if we have a negated expression
	Expression *pe = (Expression *)prep;
	if (pe->type != Expression::Negation)
	{
		// not a negated expression, check left and right
		if (pe->left != NULL && pe->left->demorgans() != OK)
			return(NOTOK);
		if (pe->right != NULL && pe->right->demorgans() != OK)
			return(NOTOK);
		return(OK);
	}

	// we have a negation, check if rhs is a predicate or an expression.
	Semantic *prs = pe->right;
	if (!(prs->isExpression()))
	{
		// what is negated is NOT an expression, just return.
		return(OK);
	}

	// check if expression is an AND, OR, UNIVERSAL, or EXISTENTIAL
	Expression *pre = (Expression *)prs->prep;
	switch (pre->type)
	{
	case Expression::Or:
	case Expression::And:
	{
		// we have a negated AND or OR expression, apply demorgans laws.
		Semantic *prels = pre->left;
		Semantic *prers = pre->right;

		// create new negation records
		Semantic *pnegls = 
			new Semantic(Expression::Negation, NULL, prels);
		MustBeTrue(pnegls != NULL);
		Semantic *pnegrs = 
			new Semantic(Expression::Negation, NULL, prers);
		MustBeTrue(pnegrs != NULL);

		// write over negation record with the new record info
		pe->left = pnegls;
		pe->right = pnegrs;
		switch (pre->type)
		{
		case Expression::Or:
			pe->type = Expression::And;
			break;
		case Expression::And:
			pe->type = Expression::Or;
			break;
		default:
			MustBeTrue(0);
			break;
		}

		// delete old right semantic record
		pre->left = NULL;
		pre->right = NULL;
		delete prs;

		// check left and right sides
		if (pe->left != NULL && pe->left->demorgans() != OK)
			return(NOTOK);
		if (pe->right != NULL && pe->right->demorgans() != OK)
			return(NOTOK);
		break;
	}
	case Expression::Universal:
	case Expression::Existential:
	{
		// we have a negated UNIVERSAL or EXISTENTIAL expression, 
		// apply demorgans laws.
		Semantic *prers = pre->right;

		// create new negation records
		Semantic *pnegrs = 
			new Semantic(Expression::Negation, NULL, prers);
		MustBeTrue(pnegrs != NULL);

		// write over negation record with the new record info
		pe->left = NULL;
		pe->right = pnegrs;
		pe->name = pre->name;
		pe->varisused = pre->varisused;
		switch (pre->type)
		{
		case Expression::Universal:
			pe->type = Expression::Existential;
			break;
		case Expression::Existential:
			pe->type = Expression::Universal;
			break;
		default:
			MustBeTrue(0);
			break;
		}

		// delete old right semantic record
		pre->left = NULL;
		pre->right = NULL;
		delete prs;

		// check left and right sides
		if (pe->left != NULL && pe->left->demorgans() != OK)
			return(NOTOK);
		if (pe->right != NULL && pe->right->demorgans() != OK)
			return(NOTOK);
	}
		break;
	default:
	{
		// not any of the above.
		if (pre->left != NULL && pre->left->demorgans() != OK)
			return(NOTOK);
		if (pre->right != NULL && pre->right->demorgans() != OK)
			return(NOTOK);
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
		// get expression record
		Expression *pe = (Expression *)prep;

		// check if we have quantifier
		int popscope = 0;
		if (pe->type == Expression::Universal)
		{
			// we have a quantifier, rename variable
			popscope = 1;
			String uname = uniqueName(String("_Q"));
			localscope.insertAtFront(
				Symbol(pe->name, uname, 
				Symbol::UniversalVariable));

			// change name in semantic record
			pe->name = uname;
		}
		else if (pe->type == Expression::Existential)
		{
			// we have a quantifier, rename variable
			popscope = 1;
			String uname = uniqueName(String("_Q"));
			localscope.insertAtFront(
				Symbol(pe->name, uname, 
				Symbol::ExistentialVariable));

			// change name in semantic record
			pe->name = uname;
		}

		// follow left and right branches
		if (pe->left != NULL && 
		    pe->left->renameVariables(localscope) != OK) 
			return(NOTOK);
		if (pe->right != NULL && 
		    pe->right->renameVariables(localscope) != OK)
			return(NOTOK);

		// pop scope variable if required
		if (popscope)
		{
			Symbol tmp;
			MustBeTrue(localscope.removeAtFront(tmp) == OK);
		}
	}
	else if (isPredicate())
	{
		// get predicate record
		Predicate *pp = (Predicate *)prep;

		// check if a function
		if (pp->type == Predicate::Function)
		{
			// we have a function, scan argument list
			ListIterator<Semantic *> pargsIter(*pp->pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->renameVariables(localscope) != OK)
					return(NOTOK);
			}
		}
	}
	else if (isArgument())
	{
		// check if a variable, function or anything else
		Argument *pa = (Argument *)prep;
		if (pa->type == Argument::Variable)
		{
			// find variable in scope
			Symbol usym(pa->name);
			if (localscope.retrieve(usym) == OK)
			{
				pa->name = usym.getUniqueName();
				MustBeTrue(pa->name != String(""));
			}
		}
		else if (pa->type == Argument::Function)
		{
			// we have a function, scan argument list
			ListIterator<Semantic *> pargsIter(*pa->pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->renameVariables(localscope) != OK)
					return(NOTOK);
			}
		}
	}
	else
	{
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
		// get expression
		Expression *pe = (Expression *)prep;

		// what type is it
		if (pe->type == Expression::Universal)
		{
			// store universal variable
			localscope.insertAtEnd(Symbol(pe->name, 
				Symbol::UniversalVariable));

			// follow right leg, left leg is null
			MustBeTrue(pe->left == NULL && pe->right != NULL);
			if (pe->right->skolemize(localscope) != OK)
				return(NOTOK);

			// remove universal variable
			Symbol tmp;
			localscope.removeAtEnd(tmp);
		}
		else if (pe->type == Expression::Existential)
		{
			// store existential variable
			String uname = uniqueName(String("_SK"));
			localscope.insertAtEnd(Symbol(pe->name, uname,
				Symbol::ExistentialVariable));

			// follow right leg, left leg is null
			MustBeTrue(pe->left == NULL && pe->right != NULL);
			if (pe->right->skolemize(localscope) != OK)
				return(NOTOK);

			// remove existential variable
			Symbol tmp;
			localscope.removeAtEnd(tmp);
		}
		else
		{
			// follow down other expression operators
			if (pe->left != NULL && 
			    pe->left->skolemize(localscope) != OK)
				return(NOTOK);
			if (pe->right != NULL && 
			    pe->right->skolemize(localscope) != OK)
				return(NOTOK);
		}
		
	}
	else if (isPredicate())
	{
		// get predicate
		Predicate *pp = (Predicate *)prep;

		// check for functions
		if (pp->type == Predicate::Function)
		{
			// cycle thru arguments
			ListIterator<Semantic * > pargsIter(*pp->pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->skolemize(localscope) != OK)
					return(NOTOK);
			}
		}
	}
	else if (isArgument())
	{
		// check type of argument
		Argument *pa = (Argument *)prep;
		switch (pa->type)
		{
		case Argument::Variable:
		{
			// check if an existential variable
			Symbol qvarsym(pa->name);
			if(localscope.retrieve(qvarsym) != OK)
				break;
			if (qvarsym.getType() != Symbol::ExistentialVariable)
				break;

			// we have an existential variable
			String skolemName(qvarsym.getUniqueName());

			// need to replace this variable with a 
			// skolem function which is dependent on all
			// universal variables in scope at this time.
			//
			List<Semantic * > *pargs = new List<Semantic * >;
			MustBeTrue(pargs != NULL);
			ListIterator<Symbol> scopeIter(localscope);
			for (int nargs = 0; !scopeIter.done(); scopeIter++)
			{
				// get symbol
				Symbol uvar = scopeIter();

				// check if we found the current 
				// symbol. this marks the end of 
				// dependent variables for the 
				// skolem function. all other
				// existential variables are skipped.
				//
				if (uvar.getType() == 
					Symbol::ExistentialVariable)
				{
					if (uvar == Symbol(pa->name))
						break;
					else
						continue;
				}

				// we have a universal variable in
				// scope
				//
				Semantic *parg = new Semantic(
					Argument::Variable, uvar.getName());
				MustBeTrue(parg != NULL);
				pargs->insertAtEnd(parg);
				nargs++;
			}
			if (nargs == 0)
			{
				// skolem constant
				pa->type = Argument::Constant;
				pa->name = skolemName;
				pa->pargs = NULL;
				pa->argnum = 0;

				// delete unused argument list
				delete pargs;
			}
			else
			{
				// skolem function
				pa->type = Argument::Function;
				pa->name = skolemName;
				pa->pargs = pargs;
				pa->argnum = nargs;
			}
			break;
		}

		case Argument::Function:
		{
			// we have a function, scan its arguments
			ListIterator<Semantic *> pargsIter(*pa->pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->skolemize(localscope) != OK)
					return(NOTOK);
			}
			break;
		}
		}
	}
	else
	{
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

	// get expression
	Expression *pe = (Expression *)prep;

	// what type is it
	if (pe->type == Expression::Universal)
	{
		// store universal variable
		localscope.insertAtEnd(Symbol(pe->name, 
			Symbol::UniversalVariable));

		// follow right leg, left leg is null
		MustBeTrue(pe->left == NULL && pe->right != NULL);
		if (pe->right->skolemize(localscope) != OK)
			return(NOTOK);

		// remove universal variable
		Symbol tmp;
		localscope.removeAtEnd(tmp);
	}
	else if (pe->type == Expression::Existential)
	{
		// store existential variable
		String uname = uniqueName(String("_SK"));
		localscope.insertAtEnd(Symbol(pe->name, uname,
			Symbol::ExistentialVariable));

		// follow right leg, left leg is null
		MustBeTrue(pe->left == NULL && pe->right != NULL);
		if (pe->right->skolemize(localscope) != OK)
			return(NOTOK);

		// remove existential variable
		Symbol tmp(pe->name);
		localscope.removeAtEnd(tmp);
	}
	else
	{
		// follow down other expression operators
		if (pe->left != NULL && pe->left->skolemize(localscope) != OK)
			return(NOTOK);
		if (pe->right != NULL && pe->right->skolemize(localscope) != OK)
			return(NOTOK);
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
	int saveConclusion = prep->conclusion;

	// check if and universal record
	Expression *pe = (Expression *)prep;
	if (pe->type == Expression::Existential)
	{
		// save right branch of expression record
		Semantic *prs = pe->right;
		SemanticRep *prp = prs->prep;

		// remove old records
		pe->left = NULL;
		pe->right = NULL;
		delete pe;
		prs->prep = NULL;
		delete prs;

		// set up new links
		prep = prp;
	}

	// reassign since we may have a new expression pointer
	if (isExpression())
	{
		pe = (Expression *)prep;
		if (pe->type == Expression::Existential &&
		    removeExistentials() != OK)
		{
			return(NOTOK);
		}
		else
		{
			if (pe->left != NULL && 
			    pe->left->removeExistentials() != OK)
				return(NOTOK);
			if (pe->right != NULL && 
			    pe->right->removeExistentials() != OK)
				return(NOTOK);
		}
	}

	// restore conclusion status
	prep->conclusion = saveConclusion;

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
	int saveConclusion = prep->conclusion;

	// check if and universal record
	Expression *pe = (Expression *)prep;
	if (pe->type == Expression::Universal)
	{
		// save right branch of expression record
		Semantic *prs = pe->right;
		SemanticRep *prp = prs->prep;

		// remove old records
		pe->left = NULL;
		pe->right = NULL;
		delete pe;
		prs->prep = NULL;
		delete prs;

		// set up new links
		prep = prp;
	}

	// reassign since we may have a new expression pointer
	if (isExpression())
	{
		pe = (Expression *)prep;
		if (pe->type == Expression::Universal &&
		    removeUniversals() != OK)
		{
			return(NOTOK);
		}
		else
		{
			if (pe->left != NULL && 
			    pe->left->removeUniversals() != OK)
				return(NOTOK);
			if (pe->right != NULL && 
			    pe->right->removeUniversals() != OK)
				return(NOTOK);
		}
	}

	// restore conclusion status
	prep->conclusion = saveConclusion;

	// all done
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

	// expression record
	Expression *pe = (Expression *)prep;

	// left and right semantic/expression records
	Semantic *pls = pe->left;
	Expression *ple = (Expression *)pls->prep;
	Semantic *prs = pe->right;
	Expression *pre = (Expression *)prs->prep;

	// left branch data
	Semantic *plels = ple->left;
	Expression *plele = (Expression *)plels->prep;
	Semantic *plers = ple->right;
	Expression *plere = (Expression *)plers->prep;

	// we have and OR with a left AND. we can distribute the
	// OR using the rule (A&&B)||C = (A||C)&&(B||C)
	//
	// create new right semantic record
	pe->right = new Semantic(Expression::Or, plers, prs);
	MustBeTrue(pe->right != NULL);

	// update AND right branch
	ple->right = new Semantic(*prs);
	MustBeTrue(ple->right != NULL);

	// update all operations
	pe->type = Expression::And;
	ple->type = Expression::Or;

	// scan new branches
	if (pe->left != NULL && pe->left->distribution(changed) != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->distribution(changed) != OK)
		return(NOTOK);

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

	// expression record
	Expression *pe = (Expression *)prep;

	// left and right semantic/expression records
	Semantic *pls = pe->left;
	Expression *ple = (Expression *)pls->prep;
	Semantic *prs = pe->right;
	Expression *pre = (Expression *)prs->prep;

	// right branch data
	Semantic *prels = pre->left;
	Expression *prele = (Expression *)prels->prep;
	Semantic *prers = pre->right;
	Expression *prere = (Expression *)prers->prep;

	// we have an OR with a right AND. we can distribute the
	// OR using the rule A||(B&&C) = (A||B)&&(A||C).
	//
	// create new left semantic record
	pe->left = new Semantic(Expression::Or, pls, prels);
	MustBeTrue(pe->left != NULL);

	// update AND left branch
	pre->left = new Semantic(*pls);
	MustBeTrue(pre->left != NULL);

	// update all operations
	pe->type = Expression::And;
	pre->type = Expression::Or;

	// scan new branches
	if (pe->left != NULL && pe->left->distribution(changed) != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->distribution(changed) != OK)
		return(NOTOK);

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
	Expression *pe = (Expression *)prep;
	if (pe->type != Expression::Or)
	{
		// we don't have an OR, check left and right
		if (pe->left != NULL && 
		    pe->left->distribution(changed) != OK)
			return(NOTOK);
		if (pe->right != NULL && 
		    pe->right->distribution(changed) != OK)
			return(NOTOK);
		return(OK);
	}

	// we have an OR, check if left is an AND.
	Semantic *pls = pe->left;
	if (pls->isExpression())
	{
		Expression *ple = (Expression *)pls->prep;
		if (ple->type == Expression::And)
			return(distributeLeftAnd(changed));
	}

	// we have an OR, check if right is an AND.
	Semantic *prs = pe->right;
	if (prs->isExpression())
	{
		Expression *pre = (Expression *)prs->prep;
		if (pre->type == Expression::And)
			return(distributeRightAnd(changed));
	}

	// check both sides
	if (pe->left != NULL && pe->left->distribution(changed) != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->distribution(changed) != OK)
		return(NOTOK);

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
			return(NOTOK);
	}
	return(OK);
}


// get clauses from CNF statement
int
Semantic::getClauses(List<Semantic *> &clist)
{
	return(getClauses(clist, prep->conclusion));
}
int
Semantic::getClauses(List<Semantic *> &clist, int concluded)
{
	// check if an expression
	if (!isExpression())
	{
		Semantic *pnew = new Semantic(*this);
		MustBeTrue(pnew != NULL);
		pnew->prep->conclusion = concluded;
		clist.insertAtEnd(pnew);
		return(OK);
	}

	// check if expression is an AND
	Expression *pe = (Expression *)prep;
	if (pe->type != Expression::And)
	{
		Semantic *pnew = new Semantic(*this);
		MustBeTrue(pnew != NULL);
		pnew->prep->conclusion = concluded;
		clist.insertAtEnd(pnew);
		return(OK);
	}

	// keep tracing down the ANDs
	if (pe->left != NULL && pe->left->getClauses(clist, concluded) != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->getClauses(clist, concluded) != OK)
		return(NOTOK);

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
		// get expression record
		Expression *pe = (Expression *)prep;
		MustBeTrue(pe->type != Expression::And)

		// follow left and right branches
		if (pe->left != NULL && 
		    pe->left->renameAgain(localscope) != OK) 
			return(NOTOK);
		if (pe->right != NULL && 
		    pe->right->renameAgain(localscope) != OK)
			return(NOTOK);
	}
	else if (isPredicate())
	{
		// get predicate record
		Predicate *pp = (Predicate *)prep;

		// check if a function
		if (pp->type == Predicate::Function)
		{
			// we have a function, scan argument list
			ListIterator<Semantic *> pargsIter(*pp->pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->renameAgain(localscope) != OK)
					return(NOTOK);
			}
		}
	}
	else if (isArgument())
	{
		// check if a variable, function or anything else
		Argument *pa = (Argument *)prep;
		if (pa->type == Argument::Variable)
		{
			// rename variable
			Symbol varsym(pa->name);
			if (localscope.retrieve(varsym) == OK)
			{
				pa->name = varsym.getUniqueName();
			}
			else
			{
				String uname = uniqueName(String("_V"));
				Symbol varsym(pa->name, 
					uname, Symbol::Variable);
				pa->name = uname;
				localscope.insertAtEnd(varsym);
			}
		}
		else if (pa->type == Argument::Function)
		{
			// we have a function, scan argument list
			ListIterator<Semantic *> pargsIter(*pa->pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->renameAgain(localscope) != OK)
					return(NOTOK);
			}
		}
	}
	else
	{
		MustBeTrue(0);
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

// get clauses from CNF statement in a list form
int
Semantic::getClausesAsLists(IRClause &clause, String &term, int isnegated)
{
	// check type of record
	if (isExpression())
	{
		// get expression record
		Expression *pe = (Expression *)prep;

		// must be a negation
		MustBeTrue(pe->type == Expression::Negation)

		// process the rest of the record
		MustBeTrue(pe->left == NULL && pe->right != NULL);
		if (pe->right->getClausesAsLists(clause, term, 1) != OK)
			return(NOTOK);
	}
	else if (isPredicate())
	{
		// get predicate record
		Predicate *pp = (Predicate *)prep;

		// check type of record
		if (pp->type == Predicate::Function)
		{
			// start a new list
			term += String("( ");

			// check if negated
			if (isnegated)
				term += String("~");

			// add function name
			term += pp->name + String(" ");

			// scan arguments
			ListIterator<Semantic * > pargsIter(*pp->pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->getClausesAsLists(clause, term, 0) != OK)
					return(NOTOK);
			}

			// close list
			term += String(") ");
		}
		else if (pp->type == Predicate::Constant)
		{
			// check if negated
			if (isnegated)
				term += String("~");

			// add constant name
			term += pp->name + String(" ");
		}
		else if (pp->type == Predicate::LogicalConstant)
		{
			// check if negated
			if (isnegated)
			{
				if (pp->value == String("True"))
					term += String("False ");
				else
					term += String("True ");

			}
			else
				term += pp->value + String(" ");
		}
		else
		{
			MustBeTrue(0);
		}

	}
	else if (isArgument())
	{
		// get argument record
		Argument *pa = (Argument *)prep;

		// check type of record
		if (pa->type == Argument::Function)
		{
			// start a new list
			term += String("( ");

			// add function name
			term += pa->name + String(" ");

			// scan arguments
			ListIterator<Semantic * > pargsIter(*pa->pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Semantic *parg = pargsIter();
				if (parg != NULL && 
				    parg->getClausesAsLists(clause, term, 0) != OK)
					return(NOTOK);
			}

			// close list
			term += String(") ");
		}
		else if (pa->type == Argument::Variable)
		{
			// add variable name
			term += pa->name + String(" ");
		}
		else if (pa->type == Argument::Constant)
		{
			// add constant name
			term += pa->name + String(" ");
		}
		else if (pa->type == Argument::Number)
		{
			// add number value
			term += pa->value + String(" ");
		}
		else if (pa->type == Argument::QuotedString)
		{
			// add quoted string
			term += String(" \"") + pa->value + String("\" ");
		}
		else
		{
			MustBeTrue(0);
		}
	}
	else
	{
		MustBeTrue(0);
	}

	// all done
	return(OK);
}

int
Semantic::getClausesAsLists(IRClause &clause)
{
	// check type of record
	if (isExpression())
	{
		// get expression record
		Expression *pe = (Expression *)prep;
	
		// check if an OR expression
		if (pe->type == Expression::Or)
		{
			// check left branch of the OR
			if (pe->left != NULL && 
			    pe->left->getClausesAsLists(clause) != OK)
				return(NOTOK);

			// check right branch of the OR
			if (pe->right != NULL && 
			    pe->right->getClausesAsLists(clause) != OK)
				return(NOTOK);
		}
		else if (pe->type == Expression::Negation)
		{
			// trace down the negation record
			String term;
			if (getClausesAsLists(clause, term, 1) != OK)
				return(NOTOK);

			// store term in set
			clause.insert(term);
		}
		else
		{
			MustBeTrue(0);
		}
	}
	else if (isPredicate())
	{
		// trace predicate record
		String term;
		if (getClausesAsLists(clause, term, 0) != OK)
			return(NOTOK);

		// store term in clause
		clause.insert(term);
	}
	else
	{
		MustBeTrue(0);
	}

	// all done
	return(OK);
}

int
Semantic::getClausesAsLists(List<IRClause> &clist)
{
	// generate clause in list format
	IRClause clause;
	if (getClausesAsLists(clause) != OK)
		return(NOTOK);

	// set part of a conclusion flag
	clause.setPartOfConclusion(prep->conclusion);

	// store clause
	if (!clause.isEmpty())
		clist.insertAtEnd(clause);

	// all done
	return(OK);
}

// print data
void
Semantic::dumpData(ostream &os) const
{
	if (prep != NULL)
		os << *prep;
	return;
}

ostream &
operator<<(ostream &os, const Semantic &sr)
{
	sr.dumpData(os);
	return(os);
}

// function to generate unique names
String
Semantic::uniqueName(const String &prefix)
{
	char buf[BUFSIZ];
	sprintf(buf, "%d", ++counter);
	return(prefix + String(buf));
}

