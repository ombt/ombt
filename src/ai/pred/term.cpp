// code for term class

// headers
#include "term.h"

// term constructors and destructor
Term::Term():
	value(""), type(Unknown), argnum(0), pargs(NULL)
{
	// nothing to do
}

Term::Term(const Term &t):
	value(t.value), type(t.type), argnum(t.argnum), pargs(NULL)
{
	// check if we have an argument list
	if (t.pargs != NULL)
	{
		// allocate a new list
		pargs = new List<Term *>;
		MustBeTrue(pargs != NULL);

		// copy the data explicitly since the data are pointers
		ListIterator<Term *> pargsIter(*t.pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			Term *parg = new Term(*pargsIter());
			MustBeTrue(parg != NULL);
			pargs->insertAtEnd(parg);
		}
	}
}

Term::Term(const String &s):
	value(""), type(Unknown), argnum(0), pargs(NULL)
{
	MustBeTrue(parse(s) == OK);
}

Term::~Term()
{
	if ((type == PredicateFunction || type == Function) &&
	    (pargs != NULL))
	{
		ListIterator<Term *> pargsIter(*pargs);
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
Term *
Term::copy() const
{
	Term *p = new Term(*this);
	MustBeTrue(p != NULL);
	return(p);
}

// assignment operator
Term &
Term::operator=(const Term &rhs)
{
	if (this != &rhs)
	{
		// copy over data
		value = rhs.value;
		type = rhs.type;
		argnum = rhs.argnum;

		// ary there any arguments?
		if (pargs != NULL)
		{
			ListIterator<Term *> pargsIter(*pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				if (pargsIter() != NULL)
					delete pargsIter();
			}
			pargs->clear();
			delete pargs;
			pargs = NULL;
		}

		// is there a new list?
		if (rhs.pargs != NULL)
		{
			// allocate a new list
			pargs = new List<Term *>;
			MustBeTrue(pargs != NULL);

			// copy the data explicitly since the data are pointers
			ListIterator<Term *> pargsIter(*rhs.pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Term *parg = new Term(*pargsIter());
				MustBeTrue(parg != NULL);
				pargs->insertAtEnd(parg);
			}
		}
	}
	return(*this);
}

Term &
Term::operator=(const String &rhss)
{
	// ary there any arguments?
	if (pargs != NULL)
	{
		ListIterator<Term *> pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			if (pargsIter() != NULL)
				delete pargsIter();
		}
		pargs->clear();
		delete pargs;
		pargs = NULL;
	}

	// parse string
	MustBeTrue(parse(rhss) == OK);
	return(*this);
}

// comparison operators
int
Term::operator==(const Term &t) const
{
	if (unify(t) == OK)
		return(1);
	else
		return(0);
}

int
Term::operator!=(const Term &t) const
{
	if (unify(t) == OK)
		return(0);
	else
		return(1);
}

int
Term::operator<(const Term &t) const
{
	return(value < t.value);
}

int
Term::operator<=(const Term &t) const
{
	if (value < t.value)
		return(1);
	else if (unify(t) == OK)
		return(1);
	else
		return(0);
}

int
Term::operator>(const Term &t) const
{
	return(value > t.value);
}

int
Term::operator>=(const Term &t) const
{
	if (value > t.value)
		return(1);
	else if (unify(t) == OK)
		return(1);
	else
		return(0);
}

// negation functions
int
Term::isNegated() const
{
	switch (type)
	{
	case PredicateFunction:
	case PredicateConstant:
		if (value(0,1) == String("~"))
			return(1);
		else
			return(0);

	case LogicalConstant:
		return(0);

	default:
		MustBeTrue(0);
	}
}

Term
Term::operator~() const
{
	Term nt(*this);
	if (nt.value(0, 1) == String("~"))
		nt.value = nt.value(1);
	else
		nt.value = String("~") + nt.value;
	return(nt);
}

// parse a string to populate a term
int
Term::parse(const String &s)
{
	// define the tokenizer
	StringTokens st(s, " \t");

	// call real parser
	return(parse(st));
}

int
Term::parse(StringTokens &st)
{
	// check if done, it should not be
	if (st.done())
	{
		return(NOTOK);
	}

	// check what we have
	if (st() == String("("))
	{
		// get the function name
		st++;
		if (st.done())
		{
			return(NOTOK);
		}
		value = st();

		// get the type of function
		if (st()(0, 1) == String("~"))
		{
			// check if a negated predicate function
			if (String("A") <= st()(1,1) && 
			    st()(1,1) <= String("Z"))
				type = PredicateFunction;
			else
				return(NOTOK);
		}
		else if (String("A") <= st()(0,1) && st()(0,1) <= String("Z"))
		{
			type = PredicateFunction;
		}
		else
		{
			type = Function;
		}
		st++;
		if (st.done())
		{
			return(NOTOK);
		}

		// allocate an argument list
		pargs = new List<Term *>;
		MustBeTrue(pargs != NULL);

		// start scanning the argument list
		for (argnum = 0; !st.done() && st() != String(")"); argnum++)
		{
			Term *parg = new Term;
			MustBeTrue(parg != NULL);
			if (parg->parse(st) != OK)
			{
				return(NOTOK);
			}
			MustBeTrue(pargs->insertAtEnd(parg) == OK);
		}
		if (argnum < 1)
			return(NOTOK);

		// check for end of function
		if (st() != String(")"))
		{
			return(NOTOK);
		}
		st++;
	}
	else if ((st() == String("True")) || (st() == String("False")))
	{
		// we have a logical constant
		type = LogicalConstant;
		value = st();
		st++;
	}
	else if (st()(0, 1) == String("~"))
	{
		// we have a negated predicate constant
		if (String("A") <= st()(1,1) && st()(1,1) <= String("Z"))
			type = PredicateConstant;
		else
			return(NOTOK);
		value = st();
		st++;
	}
	else if (String("A") <= st()(0,1) && st()(0,1) <= String("Z"))
	{
		// we have a predicate constant
		type = PredicateConstant;
		value = st();
		st++;
	}
	else if (st()(0,2) == String("_V"))
	{
		// we have a variable
		type = Variable;
		value = st();
		st++;
	}
	else if (st()(0,1) == String("\""))
	{
		// we have a logical constant
		type = QuotedString;
		value = st()(0, st().strlen()-2);
		st++;
	}
	else if (String("0") <= st()(0,1) && st()(0,1) <= String("9"))
	{
		// we may have a number, assume numbers are delimiters
		StringTokens stnum(st(), "0123456789");
		if (!stnum.done())
		{
			return(NOTOK);
		}
		type = Number;
		value = st();
		st++;
	}
	else if (String("a") <= st()(0,1) && st()(0,1) <= String("z"))
	{
		// we have a constant
		type = Constant;
		value = st();
		st++;
	}
	else
	{
		return(NOTOK);
	}

	// all done
	return(OK);
}

// unify two terms, if possible
int
Term::unify(const Term &t) const
{
	// TEMPORARY: for now, do this
	if (type == t.type && value == t.value)
		return(OK);
	else
		return(NOTOK);
}

// does a variable appear in a term
int
Term::occurs(const Term &t) const
{
	// argument must be a variable
	MustBeTrue(t.type == Variable);

	// check if it occurs
	if (type == Variable)
	{
		if (value == t.value)
			return(1);
		else 
			return(0);
	}
	else if ((type == PredicateFunction) ||
		 (type == Function))
	{
		ListIterator<Term *> pai(*pargs);
		for ( ; !pai.done(); pai++)
		{
			if (pai()->occurs(t))
				return(1);
		}
		return(0);
	}
	else
	{
		return(0);
	}
}

// print data
ostream &
operator<<(ostream &os, const Term &t)
{
	switch (t.type)
	{
	case Term::PredicateConstant:
		os << t.value << " ";
		break;

	case Term::PredicateFunction:
	{
		os << t.value << "( ";
		ListIterator<Term *> pargsIter(*t.pargs);
		for (int first = 1; !pargsIter.done(); pargsIter++)
		{
			if (!first)
				os << ", ";
			os << *pargsIter();
			first = 0;
		}
		os << ") ";
		// os << ") [" << t.argnum << "] ";
		break;
	}
	case Term::Constant:
		os << t.value << " ";
		break;

	case Term::Function:
	{
		os << t.value << "( ";
		ListIterator<Term *> pargsIter(*t.pargs);
		for (int first = 1; !pargsIter.done(); pargsIter++)
		{
			if (!first)
				os << ", ";
			os << *pargsIter();
			first = 0;
		}
		os << ") ";
		// os << ") [" << t.argnum << "] ";
		break;
	}
	case Term::Variable:
		os << t.value << " ";
		break;

	case Term::QuotedString:
		os << "\"" << t.value << "\" ";
		break;

	case Term::Number:
		os << t.value << " ";
		break;

	case Term::LogicalConstant:
		os << t.value << " ";
		break;

	default:
		MustBeTrue(0);
	}
	return(os);
}
