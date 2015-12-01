// code for semantic record classes
// headers
#include "semantic.h"

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


