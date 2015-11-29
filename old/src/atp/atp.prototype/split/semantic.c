// code for semantic record classes
// headers
#include "semantic.h"

// rather than becoming dependent on another header file
extern int makedir(const char *);
extern int removedir(const char *);

// function/predicate argument class
Term::Term():
	SemanticRep(0, 0), type(Unknown), argnum(0), 
	name(), value(), pargs(NULL)
{
	// nothing to do
}

Term::Term(const Term &src):
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

Term::Term(Type t, const String &s, int c):
	SemanticRep(c, 0),
	type(t), argnum(0), name(s), value(s), pargs(NULL)
{
	MustBeTrue(type == QuotedString || 
		   type == Constant ||
		   type == Variable ||
		   type == Number);
}

Term::Term(Type t, const String &s, List<Semantic *> *pla, 
	int an, int c): 
	SemanticRep(c, 0), type(t), argnum(an), name(s), value(), pargs(pla)
{
	MustBeTrue(type == Function && pla != NULL);
}

Term::~Term()
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
Term::copy() const
{
	SemanticRep *p = new Term(*this);
	MustBeTrue(p != NULL);
	return(p);
}

// assignment operator
Term &
Term::operator=(const Term &rhs)
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
Term::isPredicate() const
{
	return(0);
}

int
Term::isExpression() const
{
	return(0);
}

int
Term::isTerm() const
{
	return(1);
}

// print data
void
Term::dumpData(ostream &os) const
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
operator<<(ostream &os, const Term &a)
{
	a.dumpData(os);
	return(os);
}

// semantic record representation constructors and destructor
SemanticRep::SemanticRep(int c, int s):
	conclusion(c), setofsupport(s)
{
	// nothing to do here
}

SemanticRep::SemanticRep(const SemanticRep &src):
	conclusion(src.conclusion), setofsupport(src.setofsupport)
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
	{
		conclusion = rhs.conclusion;
		setofsupport = rhs.setofsupport;
	}
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
SemanticRep::isTerm() const
{
	return(0);
}

// print data
void
SemanticRep::dumpData(ostream &os) const
{
	if (setofsupport) os << "SOS ";
	if (conclusion) os << "|- ";
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
	SemanticRep(c, 0), argnum(0), type(t), name(n), value(v), pargs(NULL)
{
	MustBeTrue(type == Constant || type == LogicalConstant);
}

Predicate::Predicate(Type t, const String &n, 
	List<Semantic *> *pla, int na, int c):
	SemanticRep(c, 0), argnum(na), type(t), name(n), value(), pargs(pla)
{
	MustBeTrue((type == Function || type == Equal) && pla != NULL);
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
Predicate::isTerm() const
{
	return(0);
}

// print data
void
Predicate::dumpData(ostream &os) const
{
	SemanticRep::dumpData(os);

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
	case Equal:
	{
		ListIterator<Semantic *> pargsIter(*pargs);
		os << "( " << *pargsIter() << "= ";
		pargsIter++;
		os << *pargsIter() << " ) ";
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
	SemanticRep(c, 0), type(t), left(l), right(r), name(), varisused(0)
{
	// nothing to do
}

Expression::Expression(Type t, Semantic *r, const String &n, int c):
	SemanticRep(c, 0), type(t), left(NULL), right(r), name(n)
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
Expression::isTerm() const
{
	return(0);
}

// print data
void
Expression::dumpData(ostream &os) const
{
	SemanticRep::dumpData(os);

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

Semantic::Semantic(Term::Type t, const String &s, int c):
	prep(NULL)
{
	prep = new Term(t, s, c);
	MustBeTrue(prep != NULL);
}

Semantic::Semantic(Term::Type t, const String &n, List<Semantic *> *pla, 
	int na, int c):
	prep(NULL)
{
	prep = new Term(t, n, pla, na, c);
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

// access data bits
int
Semantic::getSOS() const
{
	MustBeTrue(prep != NULL);
	return(prep->setofsupport);
}

void
Semantic::setSOS(int c)
{
	MustBeTrue(prep != NULL);
	prep->setofsupport = c;
}

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
Semantic::isTerm() const
{
	return(prep->isTerm());
}

// splitting problem functions
int
Semantic::schaum(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// check the type of record
	if (prep->isPredicate())
	{
		return(none(fname, pdir, axioms, "schaum", "p0"));
	}
	else if (prep->isExpression())
	{
		switch (((Expression *)prep)->type)
		{
		case Expression::And:
			return(schaum_and(fname, pdir, axioms));

		case Expression::Or:
			return(schaum_or(fname, pdir, axioms));

		case Expression::Conditional:
			return(schaum_conditional(fname, pdir, axioms));

		case Expression::Biconditional:
			return(schaum_biconditional(fname, pdir, axioms));

		default:
			return(none(fname, pdir, axioms, "schaum", "p0"));
		}
	}
	else if (prep->isTerm())
	{
		return(none(fname, pdir, axioms, "schaum", "p0"));
	}
	else
	{
		MustBeTrue(0);
	}

	// all done
	return(NOTOK);
}

int
Semantic::none(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	return(none(fname, pdir, axioms, "none", "p0"));
}

int
Semantic::bledsoe(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// check the type of record
	if (prep->isPredicate())
	{
		return(none(fname, pdir, axioms, "bledsoe", "p0"));
	}
	else if (prep->isExpression())
	{
		// check if the main operator is a conditional
		switch (((Expression *)prep)->type)
		{
		case Expression::And:
			return(bledsoe_and(fname, pdir, axioms));

		case Expression::Or:
			return(bledsoe_or(fname, pdir, axioms));

		case Expression::Conditional:
			return(bledsoe_conditional(fname, pdir, axioms));

		case Expression::Biconditional:
			return(bledsoe_biconditional(fname, pdir, axioms));

		default:
			return(none(fname, pdir, axioms, "bledsoe", "p0"));
		}
	}
	else if (prep->isTerm())
	{
		return(none(fname, pdir, axioms, "bledsoe", "p0"));
	}
	else
	{
		MustBeTrue(0);
	}

	// all done
	return(OK);
}

// utility functions

//
// split the problem with an AND as follows:
//
// 	axioms;
//	|- A && B;
//
//	becomes:
//
//	AND/OR graph: (p0 && p1) || p2
//
//	p0) axioms, |- A;
//	p1) axioms, |- B;
//	p2) axioms, |- A && B;
//
#define BLEDSOE_AND_ANDOR "(p0 && p1) || p2"

int
Semantic::bledsoe_and(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/bledsoe", pdir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// writen AND/OR graph
	if (writeAndOr(fname, pdir, "bledsoe", BLEDSOE_AND_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// write out file
	if (writeproblem(fname, pdir, ((Expression *)prep)->left, 
			"bledsoe", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}
	if (writeproblem(fname, pdir, ((Expression *)prep)->right, 
			"bledsoe", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// default case
	if (writeproblem(fname, pdir, this, "bledsoe", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

//
// split the problem with an OR as follows:
//
// 	axioms;
//	|- A || B;
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2
//
//	p0) axioms, |- A;
//	p1) axioms, |- B;
//	p2) axioms, |- A || B;
//
#define BLEDSOE_OR_ANDOR "p0 || p1 || p2"

int
Semantic::bledsoe_or(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/bledsoe", pdir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// writen AND/OR graph
	if (writeAndOr(fname, pdir, "bledsoe", BLEDSOE_OR_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// write out file
	if (writeproblem(fname, pdir, ((Expression *)prep)->left, 
			"bledsoe", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}
	if (writeproblem(fname, pdir, ((Expression *)prep)->right, 
			"bledsoe", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// default case
	if (writeproblem(fname, pdir, this, "bledsoe", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

//
// the default case for splitting a conditional is 
// as follows. there are special cases which are 
// handled differently.
//
// 	axioms;
//	|- A --> B;
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2 || p3
//
//	p0) axioms, |- ~A;
//	p1) axioms, |- B;
//	p2) axioms, A |- B;
//	p3) axioms, |- A --> B;
//
// special cases which are handled separately:
//
// 	axioms, |- P --> (A --> B);
//	axioms, |- P --> (A && B);
//	axioms, |- (P || Q) --> A;
//
#define BLEDSOE_CONDITIONAL_ANDOR "p0 || p1 || p2 || p3"

int
Semantic::bledsoe_conditional(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/bledsoe", pdir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// check of conditional has any of the special forms
	// identified by bledsoe.
	//
	Semantic *pel = ((Expression *)prep)->left;
	Semantic *per = ((Expression *)prep)->right;
	if (pel->isExpression())
	{
		// check for an OR
		Expression *pe = (Expression *)(pel->prep);
		if (pe->type == Expression::Or)
			return(bledsoe_conditional_or(fname, pdir, axioms));
	}
	else if (per->isExpression())
	{
		// check for a AND or a CONDITIONAL
		Expression *pe = (Expression *)(per->prep);
		if (pe->type == Expression::And)
			return(bledsoe_conditional_and(fname, pdir, axioms));
		else if (pe->type == Expression::Conditional)
			return(bledsoe_conditional_cond(fname, pdir, axioms));
	}

	// writen AND/OR graph
	if (writeAndOr(fname, pdir, "bledsoe", 
		BLEDSOE_CONDITIONAL_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// get expressions
	pel = new Semantic(*(((Expression *)prep)->left));
	MustBeTrue(pel != NULL);
	per = ((Expression *)prep)->right;

	// create a negation record
	Semantic negatedleft(Expression::Negation, NULL, pel);
	negatedleft.setConclusion(1);
	negatedleft.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&negatedleft, "bledsoe", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy right record
	Semantic right(*per);
	right.setConclusion(1);
	right.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&right, "bledsoe", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// assume A, and try to show B.
	pel = ((Expression *)prep)->left;
	axioms.insertAtEnd(pel);

	// write out files
	if (writeproblem(fname, pdir, 
		&right, "bledsoe", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(pel);

	// default case
	if (writeproblem(fname, pdir, this, "bledsoe", "p3", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

//
// split the problem with a conditional and an OR:
//
// 	axioms;
//	|- (P || Q) --> A
//
//	becomes:
//
//	AND/OR graph: ((p0||p1||p2||p3) && (p4||p5||p6||p7)) || p8
//
//	p0) axioms, |- ~P;
//	p1) axioms, |- A;
//	p2) axioms, P |- A;
//	p3) axioms, |- P --> A;
//	p4) axioms, |- ~Q;
//	p5) axioms, |- A;
//	p6) axioms, Q |- A;
//	p7) axioms, |- Q --> A;
//	p8) axioms, |- (P || Q) --> A;
//
#define BLEDSOE_CONDITIONAL_OR_ANDOR \
	"((p0 || p1 || p2 || p3) && (p4 || p5 || p6 || p7)) || p8"

int
Semantic::bledsoe_conditional_or(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// write AND/OR graph
	if (writeAndOr(fname, pdir, "bledsoe", 
		BLEDSOE_CONDITIONAL_OR_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// get records
	Expression *pe = (Expression *)prep;
	Semantic *pels = pe->left;
	Semantic *pA = pe->right;
	Expression *pelse = (Expression *)(pe->left->prep);
	Semantic *pP = pelse->left;
	Semantic *pQ = pelse->right;

	// create a negation record
	Semantic *pnewP = new Semantic(*pP);
	MustBeTrue(pnewP != NULL);
	Semantic negatedP(Expression::Negation, NULL, pnewP);
	negatedP.setConclusion(1);
	negatedP.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &negatedP, "bledsoe", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy right record
	Semantic newA(*pA);
	newA.setConclusion(1);
	newA.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &newA, "bledsoe", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// assume P, and try to show A.
	axioms.insertAtEnd(pP);

	// write out files
	if (writeproblem(fname, pdir, &newA, "bledsoe", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(pP);

	// create conditional for P --> A
	pnewP = new Semantic(*pP);
	MustBeTrue(pnewP != NULL);
	Semantic *pnewA = new Semantic(*pA);
	MustBeTrue(pnewA != NULL);
	Semantic leftcond(Expression::Conditional, pnewP, pnewA);
	leftcond.setConclusion(1);
	leftcond.setSOS(getSOS());

	// default case
	if (writeproblem(fname, pdir, &leftcond, "bledsoe", "p3", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// create a negation record
	Semantic *pnewQ = new Semantic(*pQ);
	MustBeTrue(pnewQ != NULL);
	Semantic negatedQ(Expression::Negation, NULL, pnewQ);
	negatedQ.setConclusion(1);
	negatedQ.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &negatedQ, "bledsoe", "p4", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// write out files
	if (writeproblem(fname, pdir, &newA, "bledsoe", "p5", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// assume Q, and try to show A.
	axioms.insertAtEnd(pQ);

	// write out files
	if (writeproblem(fname, pdir, &newA, "bledsoe", "p6", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(pQ);

	// create conditional for Q --> A
	pnewQ = new Semantic(*pQ);
	MustBeTrue(pnewQ != NULL);
	pnewA = new Semantic(*pA);
	MustBeTrue(pnewA != NULL);
	Semantic rightcond(Expression::Conditional, pnewQ, pnewA);
	rightcond.setConclusion(1);
	rightcond.setSOS(getSOS());

	// default case
	if (writeproblem(fname, pdir, &rightcond, "bledsoe", "p7", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// default case
	if (writeproblem(fname, pdir, this, "bledsoe", "p8", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

//
// split the problem with a conditional and an OR:
//
// 	axioms;
//	|- P --> (A && B);
//
//	becomes:
//
//	AND/OR graph: ((p0||p1||p2||p3) && (p4||p5||p6||p7)) || p8
//
//	p0) axioms, |- ~P;
//	p1) axioms, |- A;
//	p2) axioms, P |- A;
//	p3) axioms, |- P --> A;
//	p4) axioms, |- ~P;
//	p5) axioms, |- B;
//	p6) axioms, P |- B;
//	p7) axioms, |- P --> B;
//	p8) axioms, |- P --> (A && B);
//

#define BLEDSOE_CONDITIONAL_AND_ANDOR \
	"((p0 || p1 || p2 || p3) && (p4 || p5 || p6 || p7)) || p8"

int
Semantic::bledsoe_conditional_and(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// write AND/OR graph
	if (writeAndOr(fname, pdir, "bledsoe", 
		BLEDSOE_CONDITIONAL_AND_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// get records
	Expression *pe = (Expression *)prep;
	Semantic *pP = pe->left;
	Semantic *pers = pe->right;
	Expression *perse = (Expression *)(pe->right->prep);
	Semantic *pA = perse->left;
	Semantic *pB = perse->right;

	// create a negation record
	Semantic *pnewP = new Semantic(*pP);
	MustBeTrue(pnewP != NULL);
	Semantic negatedP(Expression::Negation, NULL, pnewP);
	negatedP.setConclusion(1);
	negatedP.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &negatedP, "bledsoe", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy right record
	Semantic newA(*pA);
	newA.setConclusion(1);
	newA.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &newA, "bledsoe", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// assume P, and try to show A.
	axioms.insertAtEnd(pP);

	// write out files
	if (writeproblem(fname, pdir, &newA, "bledsoe", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(pP);

	// create conditional for P --> A
	pnewP = new Semantic(*pP);
	MustBeTrue(pnewP != NULL);
	Semantic *pnewA = new Semantic(*pA);
	MustBeTrue(pnewA != NULL);
	Semantic PtoA(Expression::Conditional, pnewP, pnewA);
	PtoA.setConclusion(1);
	PtoA.setSOS(getSOS());

	// write record
	if (writeproblem(fname, pdir, &PtoA, "bledsoe", "p3", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// write out files
	if (writeproblem(fname, pdir, &negatedP, "bledsoe", "p4", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy right record
	Semantic newB(*pB);
	newB.setConclusion(1);
	newB.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &newB, "bledsoe", "p5", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// assume P, and try to show B.
	axioms.insertAtEnd(pP);

	// write out files
	if (writeproblem(fname, pdir, &newB, "bledsoe", "p6", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(pP);

	// create conditional for P --> B
	pnewP = new Semantic(*pP);
	MustBeTrue(pnewP != NULL);
	Semantic *pnewB = new Semantic(*pB);
	MustBeTrue(pnewB != NULL);
	Semantic PtoB(Expression::Conditional, pnewP, pnewB);
	PtoB.setConclusion(1);
	PtoB.setSOS(getSOS());

	// default case
	if (writeproblem(fname, pdir, &PtoB, "bledsoe", "p7", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// default case
	if (writeproblem(fname, pdir, this, "bledsoe", "p8", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

//
// split the problem with a conditional and an conditional:
//
// 	axioms;
//	|- P --> (A --> B)
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2 || p3 || p4 || p5
//
//	p0) axioms, |- ~P;
//	p1) axioms, |- ~A;
//	p2) axioms, |- B;
//	p3) axioms, P && A |- B;
//	p4) axioms, |- (P && A) --> B;
//	p5) axioms, |- P --> (A --> B);
//

#define BLEDSOE_CONDITIONAL_COND_ANDOR \
	"p0 || p1 || p2 || p3 || p4 || p5"

int
Semantic::bledsoe_conditional_cond(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// write AND/OR graph
	if (writeAndOr(fname, pdir, "bledsoe", 
		BLEDSOE_CONDITIONAL_COND_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// get records
	Expression *pe = (Expression *)prep;
	Semantic *pP = pe->left;
	Semantic *pers = pe->right;
	Expression *perse = (Expression *)(pe->right->prep);
	Semantic *pA = perse->left;
	Semantic *pB = perse->right;

	// create a negation record
	Semantic *pnewP = new Semantic(*pP);
	MustBeTrue(pnewP != NULL);
	Semantic negatedP(Expression::Negation, NULL, pnewP);
	negatedP.setConclusion(1);
	negatedP.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &negatedP, "bledsoe", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// create a negation record
	Semantic *pnewA = new Semantic(*pA);
	MustBeTrue(pnewA != NULL);
	Semantic negatedA(Expression::Negation, NULL, pnewA);
	negatedA.setConclusion(1);
	negatedA.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &negatedA, "bledsoe", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy right record
	Semantic newB(*pB);
	newB.setConclusion(1);
	newB.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &newB, "bledsoe", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// assume P && A, and try to show B.
	pnewP = new Semantic(*pP);
	MustBeTrue(pnewP != NULL);
	pnewA = new Semantic(*pA);
	MustBeTrue(pnewA != NULL);
	Semantic PandA(Expression::And, pnewP, pnewA);
	Semantic *pPandA = &PandA;
	axioms.insertAtEnd(pPandA);

	// write out files
	if (writeproblem(fname, pdir, &newB, "bledsoe", "p3", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(pPandA);


	// try to show (P && A) --> B.
	pnewP = new Semantic(*pP);
	MustBeTrue(pnewP != NULL);
	pnewA = new Semantic(*pA);
	MustBeTrue(pnewA != NULL);
	Semantic *pnewPandA = new Semantic(Expression::And, pnewP, pnewA);
	MustBeTrue(pnewPandA != NULL);
	Semantic *pnewB = new Semantic(*pB);
	MustBeTrue(pnewB != NULL);
	Semantic PandAtoB(Expression::Conditional, pnewPandA, pnewB);
	PandAtoB.setConclusion(1);
	PandAtoB.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, &PandAtoB, "bledsoe", "p4", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// default case
	if (writeproblem(fname, pdir, this, "bledsoe", "p5", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

//
// split the problem with a biconditional as follows:
//
// 	axioms;
//	|- A <--> B;
//
//	becomes:
//
//	AND/OR graph: 
//	(p0 && p1) || (p2 && p3) || ((p4 || p5) && (p6 || p7)) || p8
//
//	p0) axioms, |- A --> B;
//	p1) axioms, |- B --> A;
//	p2) axioms, A |- B;
//	p3) axioms, B |- A;
//	p4) axioms, |- ~A;
//	p5) axioms, |- B;
//	p6) axioms, |- ~B;
//	p7) axioms, |- A;
//	p8) axioms, |- A --> B;
//
#define BLEDSOE_BICONDITIONAL_ANDOR \
	"(p0 && p1) || (p2 && p3) || ((p4 || p5) && (p6 || p7)) || p8"

int
Semantic::bledsoe_biconditional(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/bledsoe", pdir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// write AND/OR graph
	if (writeAndOr(fname, pdir, "bledsoe", 
		BLEDSOE_BICONDITIONAL_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// get expressions
	Semantic *pel1 = new Semantic(*(((Expression *)prep)->left));
	MustBeTrue(pel1 != NULL);
	Semantic *pel2 = new Semantic(*(((Expression *)prep)->left));
	MustBeTrue(pel2 != NULL);
	Semantic *per1 = new Semantic(*(((Expression *)prep)->right));
	MustBeTrue(per1 != NULL);
	Semantic *per2 = new Semantic(*(((Expression *)prep)->right));
	MustBeTrue(per2 != NULL);

	// create a conditional expressions
	Semantic conditional1(Expression::Conditional, pel1, per1);
	conditional1.setConclusion(1);
	conditional1.setSOS(getSOS());
	Semantic conditional2(Expression::Conditional, per2, pel2);
	conditional2.setConclusion(1);
	conditional2.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&conditional1, "bledsoe", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}
	if (writeproblem(fname, pdir, 
		&conditional2, "bledsoe", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// get left and right records
	Semantic *pel = ((Expression *)prep)->left;
	Semantic *per = ((Expression *)prep)->right;

	// copy right record
	Semantic right(*per);
	right.setConclusion(1);
	right.setSOS(getSOS());

	// assume A, and try to show B.
	axioms.insertAtEnd(pel);

	// write out files
	if (writeproblem(fname, pdir, 
		&right, "bledsoe", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(pel);

	// copy left record
	Semantic left(*pel);
	left.setConclusion(1);
	left.setSOS(getSOS());

	// assume B, and try to show A.
	axioms.insertAtEnd(per);

	// write out files
	if (writeproblem(fname, pdir, 
		&left, "bledsoe", "p3", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(per);

	// get expressions
	pel = new Semantic(*(((Expression *)prep)->left));
	MustBeTrue(pel != NULL);
	per = ((Expression *)prep)->right;

	// create a negation record
	Semantic negatedleft(Expression::Negation, NULL, pel);
	negatedleft.setConclusion(1);
	negatedleft.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&negatedleft, "bledsoe", "p4", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy right record
	Semantic right2(*per);
	right2.setConclusion(1);
	right2.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&right2, "bledsoe", "p5", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// get expressions
	pel = ((Expression *)prep)->left;
	per = new Semantic(*(((Expression *)prep)->right));
	MustBeTrue(per != NULL);

	// create a negation record
	Semantic negatedright(Expression::Negation, NULL, per);
	negatedright.setConclusion(1);
	negatedright.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&negatedright, "bledsoe", "p6", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy left record
	Semantic left2(*pel);
	left2.setConclusion(1);
	left2.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&left2, "bledsoe", "p7", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// default case
	if (writeproblem(fname, pdir, this, "bledsoe", "p8", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

int
Semantic::none(const char *fname, const char *pdir, List<Semantic * > &axioms,
	const char *strategydir, const char *probdir) const
{
	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/%s", pdir, strategydir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}
	strcat(subpdir, "/");
	strcat(subpdir, probdir);
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// writen AND/OR graph
	if (writeAndOr(fname, pdir, strategydir, probdir) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// write entire problem out to a file
	char cpcmd[BUFSIZ];
	sprintf(cpcmd, "/usr/bin/cp %s %s/axioms_conclusion", fname, subpdir);
	system(cpcmd);

	// all done
	return(OK);
}

//
// split the problem with an AND as follows:
//
// 	axioms;
//	|- A && B;
//
//	becomes:
//
//	AND/OR graph: (p0 && p1) || p2
//
//	p0) axioms, |- A;
//	p1) axioms, |- B;
//	p2) axioms, |- A && B;
//
#define SCHAUM_AND_ANDOR "(p0 && p1) || p2"

int
Semantic::schaum_and(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/schaum", pdir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// writen AND/OR graph
	if (writeAndOr(fname, pdir, "schaum", SCHAUM_AND_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// write out file
	if (writeproblem(fname, pdir, ((Expression *)prep)->left, 
			"schaum", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}
	if (writeproblem(fname, pdir, ((Expression *)prep)->right, 
			"schaum", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// default case
	if (writeproblem(fname, pdir, this, "schaum", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

//
// split the problem with an OR as follows:
//
// 	axioms;
//	|- A || B;
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2
//
//	p0) axioms, |- A;
//	p1) axioms, |- B;
//	p2) axioms, |- A || B;
//
#define SCHAUM_OR_ANDOR "p0 || p1 || p2"

int
Semantic::schaum_or(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/schaum", pdir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// writen AND/OR graph
	if (writeAndOr(fname, pdir, "schaum", SCHAUM_OR_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// write out file
	if (writeproblem(fname, pdir, ((Expression *)prep)->left, 
			"schaum", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}
	if (writeproblem(fname, pdir, ((Expression *)prep)->right, 
			"schaum", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// default case
	if (writeproblem(fname, pdir, this, "schaum", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

//
// split the problem with a conditional as follows:
//
// 	axioms;
//	|- A --> B;
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2 || p3
//
//	p0) axioms, |- ~A;
//	p1) axioms, |- B;
//	p2) axioms, A |- B;
//	p3) axioms, |- A --> B;
//
#define SCHAUM_CONDITIONAL_ANDOR "p0 || p1 || p2 || p3"

int
Semantic::schaum_conditional(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/schaum", pdir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// writen AND/OR graph
	if (writeAndOr(fname, pdir, "schaum", 
		SCHAUM_CONDITIONAL_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// get expressions
	Semantic *pel = new Semantic(*(((Expression *)prep)->left));
	MustBeTrue(pel != NULL);
	Semantic *per = ((Expression *)prep)->right;

	// create a negation record
	Semantic negatedleft(Expression::Negation, NULL, pel);
	negatedleft.setConclusion(1);
	negatedleft.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&negatedleft, "schaum", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy right record
	Semantic right(*per);
	right.setConclusion(1);
	right.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&right, "schaum", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// assume A, and try to show B.
	pel = ((Expression *)prep)->left;
	axioms.insertAtEnd(pel);

	// write out files
	if (writeproblem(fname, pdir, 
		&right, "schaum", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(pel);

	// default case
	if (writeproblem(fname, pdir, this, "schaum", "p3", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

//
// split the problem with a biconditional as follows:
//
// 	axioms;
//	|- A <--> B;
//
//	becomes:
//
//	AND/OR graph: 
//	(p0 && p1) || (p2 && p3) || ((p4 || p5) && (p6 || p7)) || p8
//
//	p0) axioms, |- A --> B;
//	p1) axioms, |- B --> A;
//	p2) axioms, A |- B;
//	p3) axioms, B |- A;
//	p4) axioms, |- ~A;
//	p5) axioms, |- B;
//	p6) axioms, |- ~B;
//	p7) axioms, |- A;
//	p8) axioms, |- A --> B;
//
#define SCHAUM_BICONDITIONAL_ANDOR \
	"(p0 && p1) || (p2 && p3) || ((p4 || p5) && (p6 || p7)) || p8"

int
Semantic::schaum_biconditional(const char *fname, 
	const char *pdir, List<Semantic * > &axioms) const
{
	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/schaum", pdir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// write AND/OR graph
	if (writeAndOr(fname, pdir, "schaum", 
		SCHAUM_BICONDITIONAL_ANDOR) != OK)
	{
		ERROR("writeAndOr failed.", errno);
		return(NOTOK);
	}

	// get expressions
	Semantic *pel1 = new Semantic(*(((Expression *)prep)->left));
	MustBeTrue(pel1 != NULL);
	Semantic *pel2 = new Semantic(*(((Expression *)prep)->left));
	MustBeTrue(pel2 != NULL);
	Semantic *per1 = new Semantic(*(((Expression *)prep)->right));
	MustBeTrue(per1 != NULL);
	Semantic *per2 = new Semantic(*(((Expression *)prep)->right));
	MustBeTrue(per2 != NULL);

	// create a conditional expressions
	Semantic conditional1(Expression::Conditional, pel1, per1);
	conditional1.setConclusion(1);
	conditional1.setSOS(getSOS());
	Semantic conditional2(Expression::Conditional, per2, pel2);
	conditional2.setConclusion(1);
	conditional2.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&conditional1, "schaum", "p0", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}
	if (writeproblem(fname, pdir, 
		&conditional2, "schaum", "p1", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// get left and right records
	Semantic *pel = ((Expression *)prep)->left;
	Semantic *per = ((Expression *)prep)->right;

	// copy right record
	Semantic right(*per);
	right.setConclusion(1);
	right.setSOS(getSOS());

	// assume A, and try to show B.
	axioms.insertAtEnd(pel);

	// write out files
	if (writeproblem(fname, pdir, 
		&right, "schaum", "p2", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(pel);

	// copy left record
	Semantic left(*pel);
	left.setConclusion(1);
	left.setSOS(getSOS());

	// assume B, and try to show A.
	axioms.insertAtEnd(per);

	// write out files
	if (writeproblem(fname, pdir, 
		&left, "schaum", "p3", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// clean up axioms
	axioms.removeAtEnd(per);

	// get expressions
	pel = new Semantic(*(((Expression *)prep)->left));
	MustBeTrue(pel != NULL);
	per = ((Expression *)prep)->right;

	// create a negation record
	Semantic negatedleft(Expression::Negation, NULL, pel);
	negatedleft.setConclusion(1);
	negatedleft.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&negatedleft, "schaum", "p4", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy right record
	Semantic right2(*per);
	right2.setConclusion(1);
	right2.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&right2, "schaum", "p5", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// get expressions
	pel = ((Expression *)prep)->left;
	per = new Semantic(*(((Expression *)prep)->right));
	MustBeTrue(per != NULL);

	// create a negation record
	Semantic negatedright(Expression::Negation, NULL, per);
	negatedright.setConclusion(1);
	negatedright.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&negatedright, "schaum", "p6", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// copy left record
	Semantic left2(*pel);
	left2.setConclusion(1);
	left2.setSOS(getSOS());

	// write out files
	if (writeproblem(fname, pdir, 
		&left2, "schaum", "p7", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// default case
	if (writeproblem(fname, pdir, this, "schaum", "p8", axioms) != OK)
	{
		ERROR("writeproblem failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

int
Semantic::writeproblem(const char *fname, const char *pdir, 
	const Semantic *pconclusion, const char *strategydir, 
	const char *probdir, const List<Semantic * > &axioms) const
{
	// make a copy
	Semantic conclusion(*pconclusion);
	conclusion.setConclusion(1);
	conclusion.setSOS(getSOS());

	// make the directory
	char subpdir[BUFSIZ];
	sprintf(subpdir, "%s/%s/%s", pdir, strategydir, probdir);
	if (removedir(subpdir) != OK)
	{
		ERRORD("unable to remove a directory.", subpdir, errno);
		return(NOTOK);
	}
	if (makedir(subpdir) != OK)
	{
		ERRORD("unable to make directory.", subpdir, errno);
		return(NOTOK);
	}

	// write axioms and conclusion
	char ofile[BUFSIZ];
	sprintf(ofile, "%s/axioms_conclusion", subpdir);
	ofstream fout;
	fout.open(ofile);
	if (!fout)
	{
		ERRORD("unable to open file for write.", ofile, errno);
		return(NOTOK);
	}
	ListIterator<Semantic * > xIter(axioms);
	fout << "argument { " << endl;
	for ( ; !xIter.done(); xIter++)
	{
		fout << *xIter() << ";" << endl;
	}
	fout << conclusion << ";" << endl;
	fout << "}" << endl;
	fout.close();

	// all done
	return(OK);
}

int
Semantic::writeAndOr(const char *fname, const char *pdir, 
	const char *strategydir, const char *andorexpr) const
{
	// create file name
	char andorfname[BUFSIZ];
	sprintf(andorfname, "%s/%s/ANDOR", pdir, strategydir);
	ofstream fout;
	fout.open(andorfname);
	if (!fout)
	{
		ERRORD("unable to open file.", andorfname, errno);
		return(NOTOK);
	}

	// write and/or expression to file.
	fout << andorexpr << endl;

	// close file
	fout.close();

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

