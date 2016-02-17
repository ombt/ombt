// code for semantic record classes

// headers
#include "semantic.h"

// static variables
int Semantic::Ccount = 0;
int Semantic::Dcount = 0;

// semantic record constructors and destructor
Semantic::Semantic():
	type(Unknown), ctype(NoClause),
	varisused(0), conclusion(0), setofsupport(0), query(0),
	name(), originalname(), value(),
	argnum(0), left(NULL), right(NULL), pargs(NULL)
{
	Ccount++;
}

Semantic::Semantic(const Semantic &src):
	type(src.type), ctype(NoClause),
	varisused(src.varisused), conclusion(src.conclusion), 
	setofsupport(src.setofsupport), query(src.query),
	name(src.name), originalname(src.originalname), value(src.value),
	argnum(src.argnum), left(NULL), right(NULL), pargs(NULL)
{
	Ccount++;
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

Semantic::Semantic(SymbolType t, Semantic *l, Semantic *r, int c):
	type(t), ctype(NoClause),
	varisused(0), conclusion(c), setofsupport(0), query(0),
	name(), originalname(), value(),
	argnum(0), left(l), right(r), pargs(NULL)
{
	Ccount++;
}

Semantic::Semantic(SymbolType t, const String &s, Semantic *r, int c):
	type(t), ctype(NoClause),
	varisused(0), conclusion(c), setofsupport(0), query(0),
	name(s), originalname(s), value(),
	argnum(0), left(NULL), right(r), pargs(NULL)
{
	Ccount++;
}

Semantic::Semantic(SymbolType t, const String &n, const String &v, int c):
	type(t), ctype(NoClause),
	varisused(0), conclusion(c), setofsupport(0), query(0),
	name(n), originalname(n), value(v),
	argnum(0), left(NULL), right(NULL), pargs(NULL)
{
	Ccount++;
}

Semantic::Semantic(SymbolType t, const String &n, 
	List<Semantic *> *pla, int na, int c):
	type(t), ctype(NoClause),
	varisused(0), conclusion(c), setofsupport(0), query(0),
	name(n), originalname(n), value(),
	argnum(na), left(NULL), right(NULL), pargs(pla)
{
	Ccount++;
}

Semantic::Semantic(SymbolType t, const String &s, int c):
	type(t), ctype(NoClause),
	varisused(0), conclusion(c), setofsupport(0), query(0),
	name(s), originalname(s), value(),
	argnum(0), left(NULL), right(NULL), pargs(NULL)
{
	Ccount++;
}

Semantic::Semantic(const String &on, const String &n, SymbolType t, int c):
	type(t), ctype(NoClause),
	varisused(0), conclusion(c), setofsupport(0), query(0),
	name(n), originalname(on), value(),
	argnum(0), left(NULL), right(NULL), pargs(NULL)
{
	Ccount++;
}

Semantic::~Semantic()
{
	Dcount++;
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
	if (pargs != NULL)
	{
		ListIterator<Semantic *> pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			delete pargsIter();
		}
		pargs->clear();
		delete pargs;
		pargs = NULL;
	}
}

// assignment operator
Semantic &
Semantic::operator=(const Semantic &rhs)
{
	if (this != &rhs)
	{
		// release old left/right data or argument lists
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
		if (pargs != NULL)
		{
			ListIterator<Semantic *> pargsIter(*pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				delete pargsIter();
			}
			pargs->clear();
			delete pargs;
			pargs = NULL;
		}

		// copy over left/right data or argument lists
		if (rhs.left != NULL)
		{
			left = new Semantic(*rhs.left);
			MustBeTrue(left != NULL);
		}
		if (rhs.right != NULL)
		{
			right = new Semantic(*rhs.right);
			MustBeTrue(right != NULL);
		}
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

		// copy remaining data
		type = rhs.type;
		ctype = rhs.ctype;
		varisused = rhs.varisused;
		conclusion = rhs.conclusion;
		setofsupport = rhs.setofsupport;
		query = rhs.query;
		name = rhs.name;
		originalname = rhs.originalname;
		value = rhs.value;
		argnum = rhs.argnum;
	}
	return(*this);
}

// typing info
int
Semantic::isPredicate() const
{
	switch (type)
	{
	case PredicateConstant:
	case PredicateFunction:
	case PredicateEqual:
	case LogicalConstant:
		return(1);
	default:
		return(0);
	}
}

int
Semantic::isExpression() const
{
	switch (type)
	{
	case Negation:
	case Biconditional:
	case Conditional:
	case Or:
	case And:
	case Universal:
	case Existential:
		return(1);
	default:
		return(0);
	}
}

int
Semantic::isTerm() const
{
	switch (type)
	{
	case Function:
	case Variable:
	case Constant:
	case Number:
	case QuotedString:
		return(1);
	default:
		return(0);
	}
}

// access data bits
int
Semantic::getSOS() const
{
	return(setofsupport);
}

void
Semantic::setSOS(int c)
{
	setofsupport = c;
}

int
Semantic::getConclusion() const
{
	return(conclusion);
}

void
Semantic::setConclusion(int c)
{
	conclusion = c;
}

int
Semantic::getQuery() const
{
	return(query);
}

void
Semantic::setQuery(int q)
{
	query = q;
}

int
Semantic::getCType() const
{
	return(ctype);
}

void
Semantic::setCType(ClauseType ct)
{
	ctype = ct;
}

List<String> 
Semantic::getOptions() const
{
	List<String> lo;
	if (setofsupport)
	{
		MustBeTrue(lo.insertAtEnd("SOS") == OK);
	}
	if (conclusion)
	{
		MustBeTrue(lo.insertAtEnd("CONCLUSION") == OK);
	}
	if (query)
	{
		MustBeTrue(lo.insertAtEnd("QUERY") == OK);
	}
	return(lo);
}

void
Semantic::setOptions(const List<String> &lo)
{
	ListIterator<String> loi(lo);
	for ( ; !loi.done(); loi++)
	{
		if (loi() == "SOS")
			setofsupport = 1;
		else if (loi() == "CONCLUSION")
			conclusion = 1;
		else if (loi() == "QUERY")
			query = 1;
	}
	return;
}

void
Semantic::clearOptions()
{
	setofsupport = 0;
	conclusion = 0;
	query = 0;
	return;
}

// print data
void
Semantic::dumpData(ostream &os) const
{
	if (isPredicate() || isExpression())
	{
		if (setofsupport) os << "SOS ";
		if (conclusion) os << "|- ";
		if (query) os << "?- ";
	}

	if (isExpression())
	{
		os << "( ";
		if (left != NULL)
			os << *left;
	}

	switch (type)
	{
	case Negation:
		os << "~ ";
		break;

	case Biconditional:
		os << "<--> ";
		break;

	case Conditional:
		os << "--> ";
		break;

	case Or:
		os << "|| ";
		break;

	case And:
		os << "&& ";
		break;

	case Universal:
		os << "< ";
		if (name != originalname)
			os << name << "'" << originalname << " ";
		else
			os << name << " ";
		os << ">( ";
		break;

	case Existential:
		os << "[ ";
		if (name != originalname)
			os << name << "'" << originalname << " ";
		else
			os << name << " ";
		os << "]( ";
		break;

	case Variable:
		if (name != originalname)
			os << name << "'" << originalname << " ";
		else
			os << name << " ";
		break;

	case Constant:
		if (name != originalname)
			os << name << "'" << originalname << " ";
		else
			os << name << " ";
		break;

	case Number:
	case LogicalConstant:
	case PredicateConstant:
		os << name << " ";
		break;

	case Function:
	case PredicateFunction:
	{
		if (type == Function && name != originalname)
			os << name << "'" << originalname << " ( ";
		else
			os << name << " ( ";
		ListIterator<Semantic *> pargsIter(*pargs);
		for (int first = 1; !pargsIter.done(); pargsIter++)
		{
			if (!first)
				os << ", ";
			os << *pargsIter();
			first = 0;
		}
		os << ") ";
		break;
	}

	case PredicateEqual:
	{
		ListIterator<Semantic *> pargsIter(*pargs);
		os << "( " << *pargsIter() << "= ";
		pargsIter++;
		os << *pargsIter() << ") ";
		break;
	}

	case QuotedString:
		os << "\"" << value << "\" ";
		break;

	default:
		MustBeTrue(0);
	}

	if (isExpression())
	{
		if (right != NULL)
			os << *right;
		if ((type == Universal) || (type == Existential)) 
			os << ") ) ";
		else
			os << ") ";
	}
	return;
}

ostream &
operator<<(ostream &os, const Semantic &sr)
{
	sr.dumpData(os);
	return(os);
}

// traverse syntax tree and print 
String 
Semantic::options() const 
{
	if (!conclusion && !query && !setofsupport)
		return(String(""));
	String opts("{ ");
	if (conclusion) opts += String("|- ");
	if (query) opts += String("?- ");
	if (setofsupport) opts += String("SOS ");
	opts += String("} ");
	return(opts);
}

void
Semantic::preOrder(ostream &os) const
{
	if (isTerm())
	{
		os << *this;
	}
	else if (isPredicate())
	{
		os << options() << *this;
	}
	else if (isExpression())
	{
		os << options();
		switch (type)
		{
		case And:
			os << "[ && ] ";
			break;
		case Or:
			os << "[ || ] ";
			break;
		case Conditional:
			os << "[ --> ] ";
			break;
		case Biconditional:
			os << "[ <--> ] ";
			break;
		case Negation:
			os << "[ ~ ] ";
			break;
		case Universal:
			if (name != originalname)
				os << "[ < " << name << "'" << originalname << " > ] ";
			else
				os << "[ < " << name << " > ] ";
			break;
		case Existential:
			if (name != originalname)
				os << "[ [ " << name << "'" << originalname << " ] ] ";
			else
				os << "[ [ " << name << " ] ] ";
			break;
		}
		if (left != NULL)
		{
			os << "[ ";
			left->preOrder(os);
			os << "] ";
		}
		if (right != NULL)
		{
			os << "[ ";
			right->preOrder(os);
			os << "] ";
		}
	}
	return;
}

void
Semantic::inOrder(ostream &os) const
{
	if (isTerm())
	{
		os << *this;
	}
	else if (isPredicate())
	{
		os << options() << *this;
	}
	else if (isExpression())
	{
		if (left != NULL)
		{
			os << "[ ";
			left->inOrder(os);
			os << "] ";
		}
		os << options();
		switch (type)
		{
		case And:
			os << "[ && ] ";
			break;
		case Or:
			os << "[ || ] ";
			break;
		case Conditional:
			os << "[ --> ] ";
			break;
		case Biconditional:
			os << "[ <--> ] ";
			break;
		case Negation:
			os << "[ ~ ] ";
			break;
		case Universal:
			if (name != originalname)
				os << "[ < " << name << "'" << originalname << " > ] ";
			else
				os << "[ < " << name << " > ] ";
			break;
		case Existential:
			if (name != originalname)
				os << "[ [ " << name << "'" << originalname << " ] ] ";
			else
				os << "[ [ " << name << " ] ] ";
			break;
		}
		if (right != NULL)
		{
			os << "[ ";
			right->inOrder(os);
			os << "] ";
		}
	}
	return;
}

void
Semantic::postOrder(ostream &os) const
{
	if (isTerm())
	{
		os << *this;
	}
	else if (isPredicate())
	{
		os << options() << *this;
	}
	else if (isExpression())
	{
		if (left != NULL)
		{
			os << "[ ";
			left->inOrder(os);
			os << "] ";
		}
		if (right != NULL)
		{
			os << "[ ";
			right->inOrder(os);
			os << "] ";
		}
		os << options();
		switch (type)
		{
		case And:
			os << "[ && ] ";
			break;
		case Or:
			os << "[ || ] ";
			break;
		case Conditional:
			os << "[ --> ] ";
			break;
		case Biconditional:
			os << "[ <--> ] ";
			break;
		case Negation:
			os << "[ ~ ] ";
			break;
		case Universal:
			if (name != originalname)
				os << "[ < " << name << "'" << originalname << " > ] ";
			else
				os << "[ < " << name << " > ] ";
			break;
		case Existential:
			if (name != originalname)
				os << "[ [ " << name << "'" << originalname << " ] ] ";
			else
				os << "[ [ " << name << " ] ] ";
			break;
		}
	}
	return;
}

// negate term
int
Semantic::negate()
{
	// only negate if expression or predicate
	if (!isExpression() && !isPredicate())
		return(NOTOK);

	// copy the current record
	Semantic *p = new Semantic(*this);
	MustBeTrue(p != NULL);

	// clear some flags in new record
	p->setofsupport = 0;
	p->query = 0;
	p->conclusion = 0;


	// convert the current record to a negation
	type = Negation;
	varisused = 0;
	name = String("");
	originalname = String("");
	value = String("");
	argnum = 0;
	if (left != NULL)
		delete left;
	left = NULL;
	if (right != NULL)
		delete right;
	right = p;
	if (pargs != NULL)
	{
		ListIterator<Semantic *> pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			delete pargsIter();
		}
		pargs->clear();
		delete pargs;
		pargs = NULL;
	}
	pargs = NULL;

	// all done
	return(OK);
}

// convert semantic record to a string. this 
// function assumes that the record is in CNF form.
// if it finds an unexpected type, it returns failure.
//
int
Semantic::cltostr(String &tmpcl) const
{
	// determine type
	switch (type)
	{
	case Or:
	{
		String lcl, rcl;
		if (left->cltostr(lcl) != OK)
		{
			ERROR("cltostr failed.", NOTOK);
			return(NOTOK);
		}
		if (right->cltostr(rcl) != OK)
		{
			ERROR("cltostr failed.", NOTOK);
			return(NOTOK);
		}
		tmpcl += lcl + String("|| ") + rcl;
		break;
	}
	case Negation:
	{
		String rcl;
		if (right->cltostr(rcl) != OK)
		{
			ERROR("cltostr failed.", NOTOK);
			return(NOTOK);
		}
		tmpcl += String("~ ") + rcl;
		break;
	}
	case PredicateFunction:
	{
		tmpcl += String("( ") + name + String(" ");
		ListIterator<Semantic * > ai(*pargs);
		for ( ; !ai.done(); ai++)
		{
			String arg;
			if (ai()->cltostr(arg) != OK)
			{
				ERROR("cltostr failed.", NOTOK);
				return(NOTOK);
			}
			tmpcl += arg + String(" ");
		}
		tmpcl += String(") ");
		break;
	}
	case PredicateConstant:
	{
		tmpcl += name + String(" ");
		break;
	}
	case PredicateEqual:
	{
		tmpcl += String("( = ");
		ListIterator<Semantic * > ai(*pargs);
		for ( ; !ai.done(); ai++)
		{
			String arg;
			if (ai()->cltostr(arg) != OK)
			{
				ERROR("cltostr failed.", NOTOK);
				return(NOTOK);
			}
			tmpcl += arg + String(" ");
		}
		tmpcl += String(") ");
		break;
	}
	case LogicalConstant:
	{
		tmpcl += value + String(" ");
		break;
	}
	case Variable:
	case UniversalVariable:
	{
		if (name != originalname)
			tmpcl += name + String("'") + originalname;
		else
			tmpcl += name;
		tmpcl += String(" ");
		break;
	}
	case Function:
	{
		tmpcl += String("( ");
		if (name != originalname)
			tmpcl += name + String("'") + originalname;
		else
			tmpcl += name;
		tmpcl += String(" ");
		ListIterator<Semantic * > ai(*pargs);
		for ( ; !ai.done(); ai++)
		{
			String arg;
			if (ai()->cltostr(arg) != OK)
			{
				ERROR("cltostr failed.", NOTOK);
				return(NOTOK);
			}
			tmpcl += arg + String(" ");
		}
		tmpcl += String(") ");
		break;
	}
	case QuotedString:
	{
		tmpcl += String("\"") + name + String("\" ");
		break;
	}
	case Constant:
	{
		if (name != originalname)
			tmpcl += name + String("'") + originalname;
		else
			tmpcl += name;
		tmpcl += String(" ");
		break;
	}
	case Number:
	{
		tmpcl += name + String(" ");
		break;
	}
	default:
		ERRORD("unexpected type.", type, EINVAL);
		return(NOTOK);
	}

	// all done
	return(OK);
}

int
Semantic::clauseToString(String &clause) const
{
	String tmpcl;

	// check type
	if (!isExpression() && !isPredicate())
	{
		ERRORD("clauseToString failed; not a clause.", type, NOTOK);
		return(NOTOK);
	}

	// get options
	if (setofsupport)
		tmpcl += String("SOS ");
	if (conclusion)
		tmpcl += String("|- ");
	if (query)
		tmpcl += String("?- ");

	// convert to a string
	if (cltostr(tmpcl) != OK)
	{
		ERROR("cltostr failed.", NOTOK);
		return(NOTOK);
	}

	// copy over clause
	clause = tmpcl;

	// all done
	return(OK);
}
