// code for literal and term classes

// headers
#include "literal.h"

// static symbols
#ifdef MEMORYLEAK
MemoryLeak Terms::memoryleak(1001, sizeof(Terms));
MemoryLeak Literal::memoryleak(1001, sizeof(Literal));
#endif

// term constructors and destructor
Terms::Terms():
	value(""), inputstring(), type(Unknown), argnum(0), pargs(NULL),
	index(-1)
{
	// get index
	index = nextIndex();
}

Terms::Terms(const Terms &t):
	value(t.value), inputstring(t.inputstring), type(t.type), 
	argnum(t.argnum), pargs(NULL), index(t.index)
{
	// check if we have an argument list
	if (t.pargs != NULL)
	{
		// allocate a new list
		pargs = new List<Terms *>;
		MustBeTrue(pargs != NULL);

		// copy the data explicitly since the data are pointers
		ListIterator<Terms *> pargsIter(*t.pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			Terms *parg = new Terms(*pargsIter());
			MustBeTrue(parg != NULL);
			MustBeTrue(pargs->insertAtEnd(parg) == OK);
		}
	}
}

Terms::Terms(const String &s):
	value(""), inputstring(""), type(Unknown), argnum(0), pargs(NULL),
	index(-1)
{
	index = nextIndex();
	MustBeTrue(parse(s) == OK);
}

Terms::~Terms()
{
	if (pargs != NULL)
	{
		ListIterator<Terms *> pargsIter(*pargs);
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
Terms *
Terms::copy() const
{
	Terms *p = new Terms(*this);
	MustBeTrue(p != NULL);
	return(p);
}

// assignment operator
Terms &
Terms::operator=(const Terms &rhs)
{
	if (this != &rhs)
	{
		// copy over data
		value = rhs.value;
		inputstring = rhs.inputstring;
		type = rhs.type;
		argnum = rhs.argnum;
		index = rhs.index;

		// ary there any arguments?
		if (pargs != NULL)
		{
			ListIterator<Terms *> pargsIter(*pargs);
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
			pargs = new List<Terms *>;
			MustBeTrue(pargs != NULL);

			// copy the data explicitly since the data are pointers
			ListIterator<Terms *> pargsIter(*rhs.pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Terms *parg = new Terms(*pargsIter());
				MustBeTrue(parg != NULL);
				MustBeTrue(pargs->insertAtEnd(parg) == OK);
			}
		}
	}
	return(*this);
}

Terms &
Terms::operator=(const String &rhss)
{
	// ary there any arguments?
	if (pargs != NULL)
	{
		ListIterator<Terms *> pargsIter(*pargs);
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

	// get index
	index = nextIndex();

	// all done
	return(*this);
}

// comparison operators
int
Terms::operator==(const Terms &t) const
{
	if (type == t.type && value == t.value && index == t.index)
		return(1);
	else
		return(0);
}

int
Terms::operator!=(const Terms &t) const
{
	if (type != t.type || value != t.value || index != t.index)
		return(1);
	else
		return(0);
}

int
Terms::operator<(const Terms &t) const
{
	if (type < t.type)
		return(1);
	else if (type == t.type && value < t.value)
		return(1);
	else if (type == t.type && value == t.value && index < t.index)
		return(1);
	else
		return(0);
}

int
Terms::operator<=(const Terms &t) const
{
	if (type == t.type && value == t.value && index == t.index)
		return(1);
	else if (type < t.type)
		return(1);
	else if (type == t.type && value < t.value)
		return(1);
	else if (type == t.type && value == t.value && index < t.index)
		return(1);
	else
		return(0);
}

int
Terms::operator>(const Terms &t) const
{
	if (type > t.type)
		return(1);
	else if (type == t.type && value > t.value)
		return(1);
	else if (type == t.type && value == t.value && index > t.index)
		return(1);
	else
		return(0);
}

int
Terms::operator>=(const Terms &t) const
{
	if (type == t.type && value == t.value && index == t.index)
		return(1);
	else if (type > t.type)
		return(1);
	else if (type == t.type && value > t.value)
		return(1);
	else if (type == t.type && value == t.value && index > t.index)
		return(1);
	else
		return(0);
}

// comparison operators
int
Terms::eq(const Terms &t) const
{
	if (type == t.type && value == t.value)
		return(1);
	else
		return(0);
}

int
Terms::unify_eq(const Terms &t) const
{
	// here's the key. assume any variable is a wild-card. it is
	// an automatic match for anything. only unification can
	// determine if two terms are true matches, but for comparisons,
	// assume the any variable is an automatic match with anything
	// else.
	//
	if (type == Variable || t.type == Variable)
		return(1);
	if (type != t.type || value != t.value)
		return(0);
	if (type != Function)
		return(1);
	ListIterator<Terms * > thisIter(*pargs);
	ListIterator<Terms * > tIter(*t.pargs);
	for ( ; !thisIter.done() && !tIter.done(); thisIter++, tIter++)
	{
		if (!(thisIter()->unify_eq(*tIter())))
			return(0);
	}
	return(1);
}

int
Terms::ne(const Terms &t) const
{
	if (type != t.type || value != t.value)
		return(1);
	else
		return(0);
}

int
Terms::unify_ne(const Terms &t) const
{
	return(!unify_eq(t));
}

int
Terms::lt(const Terms &t) const
{
	if (type < t.type)
		return(1);
	else if (type == t.type && value < t.value)
		return(1);
	else
		return(0);
}

int
Terms::le(const Terms &t) const
{
	if (type == t.type && value == t.value)
		return(1);
	else if (type < t.type)
		return(1);
	else if (type == t.type && value < t.value)
		return(1);
	else
		return(0);
}

int
Terms::gt(const Terms &t) const
{
	if (type > t.type)
		return(1);
	else if (type == t.type && value > t.value)
		return(1);
	else
		return(0);
}

int
Terms::ge(const Terms &t) const
{
	if (type == t.type && value == t.value)
		return(1);
	else if (type > t.type)
		return(1);
	else if (type == t.type && value > t.value)
		return(1);
	else
		return(0);
}

// parse a string to populate a term
int
Terms::parse(const String &s)
{
	// store string
	inputstring = s;

	// define the tokenizer
	StringTokens st(s, " \t");

	// call real parser
	return(parse(st));
}

int
Terms::parse(StringTokens &st)
{
	// check if done, it should not be
	if (st.done())
	{
		ERROR("unexpect end-of-string.", EINVAL);
		return(NOTOK);
	}

	// check what we have
	if (st() == String("("))
	{
		// get the function name
		st++;
		if (st.done())
		{
			ERROR("unexpect end-of-string.", EINVAL);
			return(NOTOK);
		}
		value = st();
		type = Function;
		st++;
		if (st.done())
		{
			ERROR("unexpect end-of-string.", EINVAL);
			return(NOTOK);
		}

		// allocate an argument list
		pargs = new List<Terms *>;
		MustBeTrue(pargs != NULL);

		// start scanning the argument list
		for (argnum = 0; !st.done() && st() != String(")"); argnum++)
		{
			Terms *parg = new Terms;
			MustBeTrue(parg != NULL);
			if (parg->parse(st) != OK)
			{
				ERROR("parse failed.", EINVAL);
				return(NOTOK);
			}
			MustBeTrue(pargs->insertAtEnd(parg) == OK);
		}
		if (argnum < 1)
		{
			ERROR("argument number < 1.", EINVAL);
			return(NOTOK);
		}

		// check for end of function
		if (st() != String(")"))
		{
			ERROR("expecting ')' token.", EINVAL);
			return(NOTOK);
		}
		st++;
	}
	else if (st()(0,2) == String("_V"))
	{
		// we have a variable
		type = Variable;
		value = st();
		st++;
	}
	else if (st()(0,3) == String("_RV"))
	{
		// we have a renamed variable, still just a variable
		type = Variable;
		value = st();
		st++;
	}
	else if (st()(0,1) == String("\""))
	{
		// we have a quoted string
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
			ERROR("expecting numeric token.", EINVAL);
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
	else if (st()(0,6) == String("_CONST"))
	{
		// we have a ground constant
		type = Constant;
		value = st();
		st++;
	}
	else if (st()(0,3) == String("_SK"))
	{
		// we have a skolem constant
		type = Constant;
		value = st();
		st++;
	}
	else
	{
		ERROR("unexpected token.", EINVAL);
		return(NOTOK);
	}

	// all done
	return(OK);
}

// occurs check for variables
int
Terms::occurs(const Terms &t) const
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
	else if (type == Function)
	{
		ListIterator<Terms * > pai(*pargs);
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
operator<<(ostream &os, const Terms &t)
{
#if 0
	os << "[" << t.index << "]";
#endif
	switch (t.type)
	{
	case Terms::Constant:
		os << t.value << " ";
		break;

	case Terms::Function:
	{
		os << t.value << "( ";
		ListIterator<Terms *> pargsIter(*t.pargs);
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
	case Terms::Variable:
		os << t.value << " ";
		break;

	case Terms::QuotedString:
		os << "\"" << t.value << "\" ";
		break;

	case Terms::Number:
		os << t.value << " ";
		break;

	default:
		MustBeTrue(0);
	}
	return(os);
}

// rename all variables in the term
int
Terms::renameVariables()
{
	Map<String, String> newvarnames;
	return(renameVariables(newvarnames));
}

int
Terms::renameVariables(Map<String, String> &nvs)
{
	switch (type)
	{
	case Constant:
	case QuotedString:
	case Number:
	case Unknown:
		return(OK);

	case Variable:
		if (!nvs.isInMap(value))
		{
			nvs[value] = uniqueName("_RV");
			if (updateVariableNames(value, nvs[value]) != OK)
			{
				ERROR("update variable names failed.", EINVAL);
				return(NOTOK);
			}
		}
		value = nvs[value];
		return(OK);

	case Function:
	{
		MustBeTrue(pargs != NULL);
		ListIterator<Terms * > pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			MustBeTrue(pargsIter() != NULL);
			if (pargsIter()->renameVariables(nvs) != OK)
			{
				ERROR("renameVariables failed.", EINVAL);
				return(NOTOK);
			}
		}
		return(OK);
	}

	default:
		ERRORD("unexpected type.", type, EINVAL);
		return(NOTOK);
	}
}

// convert term to a string
String
Terms::convertToString(String &s) const
{
	// clear current string
	s = String("");

	// build up the string
	switch (type)
	{
	case Terms::Number:
	case Terms::Variable:
	case Terms::Constant:
		s += value + String(" ");
		break;

	case Terms::QuotedString:
		s += String("\"") + value + String("\" ");
		break;

	case Terms::Function:
	{
		s += String("( ") + value + String(" ");
		ListIterator<Terms * > pargsIter(*pargs);
		for (int first = 1; !pargsIter.done(); pargsIter++)
		{
			if (!first)
				s += String(", ");
			s += (String)*pargsIter();
			first = 0;
		}
		s += String(") ");
		break;
	}

	default:
		MustBeTrue(0);
	}

	// return the new string
	return(s);
}

Terms::operator String() const
{
	String tmp;
	return(convertToString(tmp));
}

Terms::operator String()
{
	return(convertToString(inputstring));
}

// calculate basic statistics for this term
void
Terms::basicStats(int &nconsts, int &nvars, int &nfuncs, int &depth) const
{
	// build up the string
	switch (type)
	{
	case Terms::Number:
	case Terms::Constant:
	case Terms::QuotedString:
		depth += 1;
		nconsts += 1;
		break;

	case Terms::Variable:
		depth += 1;
		nvars += 1;
		break;

	case Terms::Function:
	{
		int savedepth = 0;
		depth += 1;
		nfuncs += 1;
		ListIterator<Terms * > pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			int termdepth = 0;
			pargsIter()->basicStats(nconsts, nvars, nfuncs, termdepth);
			if (termdepth > savedepth)
				savedepth = termdepth;
		}
		depth += savedepth;
		break;
	}

	default:
		MustBeTrue(0);
	}

	// all done
	return;
}

// literal constructors and destructor
Literal::Literal():
	value(""), inputstring(""), type(Unknown), 
	argnum(0), negated(0), pargs(NULL), index(-1)
{
	index = nextIndex();
}

Literal::Literal(const Literal &t):
	value(t.value), inputstring(t.inputstring), type(t.type), 
	argnum(t.argnum), negated(t.negated), pargs(NULL),
	index(t.index)
{
	// check if we have an argument list
	if (t.pargs != NULL)
	{
		// allocate a new list
		pargs = new List<Terms *>;
		MustBeTrue(pargs != NULL);

		// copy the data explicitly since the data are pointers
		ListIterator<Terms *> pargsIter(*t.pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			Terms *parg = new Terms(*pargsIter());
			MustBeTrue(parg != NULL);
			pargs->insertAtEnd(parg);
		}
	}
}

Literal::Literal(const String &s):
	value(""), inputstring(""), type(Unknown), 
	argnum(0), negated(0), pargs(NULL), index(-1)
{
	index = nextIndex();
	MustBeTrue(parse(s) == OK);
}

Literal::~Literal()
{
	if ((type == Function || type == Equal) && pargs != NULL)
	{
		ListIterator<Terms *> pargsIter(*pargs);
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
Literal *
Literal::copy() const
{
	Literal *p = new Literal(*this);
	MustBeTrue(p != NULL);
	return(p);
}

// assignment operator
Literal &
Literal::operator=(const Literal &rhs)
{
	if (this != &rhs)
	{
		// copy over data
		value = rhs.value;
		inputstring = rhs.inputstring;
		type = rhs.type;
		argnum = rhs.argnum;
		negated = rhs.negated;
		index = rhs.index;

		// ary there any arguments?
		if (pargs != NULL)
		{
			ListIterator<Terms *> pargsIter(*pargs);
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
			pargs = new List<Terms *>;
			MustBeTrue(pargs != NULL);

			// copy the data explicitly since the data are pointers
			ListIterator<Terms *> pargsIter(*rhs.pargs);
			for ( ; !pargsIter.done(); pargsIter++)
			{
				Terms *parg = new Terms(*pargsIter());
				MustBeTrue(parg != NULL);
				pargs->insertAtEnd(parg);
			}
		}
	}
	return(*this);
}

Literal &
Literal::operator=(const String &rhss)
{
	// ary there any arguments?
	if (pargs != NULL)
	{
		ListIterator<Terms *> pargsIter(*pargs);
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

	// get index
	index = nextIndex();

	// all done
	return(*this);
}

// comparison operators
int
Literal::operator==(const Literal &t) const
{
	if (negated == t.negated && type == t.type && 
	    value == t.value && index == t.index)
		return(1);
	else
		return(0);
}

int
Literal::operator!=(const Literal &t) const
{
	if (negated != t.negated || type != t.type || 
	    value != t.value || index != t.index)
		return(1);
	else
		return(0);
}

int
Literal::operator<(const Literal &t) const
{
	if (negated < t.negated)
		return(1);
	else if (negated == t.negated && type < t.type)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value < t.value)
		return(1);
	else if (negated == t.negated && type == t.type && 
	 	 value == t.value && index < t.index)
		return(1);
	else
		return(0);
}

int
Literal::operator<=(const Literal &t) const
{
	if (negated == t.negated && type == t.type && 
	    value == t.value && index == t.index)
		return(1);
	else if (negated < t.negated)
		return(1);
	else if (negated == t.negated && type < t.type)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value < t.value)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value == t.value && index < t.index)
		return(1);
	else
		return(0);
}

int
Literal::operator>(const Literal &t) const
{
	if (negated > t.negated)
		return(1);
	else if (negated == t.negated && type > t.type)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value > t.value)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value == t.value && index > t.index)
		return(1);
	else
		return(0);
}

int
Literal::operator>=(const Literal &t) const
{
	if (negated == t.negated && type == t.type && 
	    value == t.value && index == t.index)
		return(1);
	else if (negated > t.negated)
		return(1);
	else if (negated == t.negated && type > t.type)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value > t.value)
		return(1);
	else if (negated == t.negated && type == t.type && 
	 	 value == t.value && index > t.index)
		return(1);
	else
		return(0);
}

// comparison operators
int
Literal::eq(const Literal &t) const
{
	if (negated == t.negated && type == t.type && value == t.value)
		return(1);
	else
		return(0);
}

int
Literal::ne(const Literal &t) const
{
	if (negated != t.negated || type != t.type || value != t.value)
		return(1);
	else
		return(0);
}

int
Literal::unify_eq(const Literal &t) const
{
	if (negated != t.negated || type != t.type || value != t.value)
		return(0);
	if (type != Function && type != Equal)
		return(1);
	ListIterator<Terms * > thisIter(*pargs);
	ListIterator<Terms * > tIter(*t.pargs);
	for ( ; !thisIter.done() && !tIter.done(); thisIter++, tIter++)
	{
		if (!(thisIter()->unify_eq(*tIter())))
			return(0);
	}
	return(1);
}

int
Literal::unify_ne(const Literal &t) const
{
	return(!unify_eq(t));
}

int
Literal::lt(const Literal &t) const
{
	if (negated < t.negated)
		return(1);
	else if (negated == t.negated && type < t.type)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value < t.value)
		return(1);
	else
		return(0);
}

int
Literal::le(const Literal &t) const
{
	if (negated == t.negated && type == t.type && 
	    value == t.value)
		return(1);
	else if (negated < t.negated)
		return(1);
	else if (negated == t.negated && type < t.type)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value < t.value)
		return(1);
	else
		return(0);
}

int
Literal::gt(const Literal &t) const
{
	if (negated > t.negated)
		return(1);
	else if (negated == t.negated && type > t.type)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value > t.value)
		return(1);
	else
		return(0);
}

int
Literal::ge(const Literal &t) const
{
	if (negated == t.negated && type == t.type && 
	    value == t.value)
		return(1);
	else if (negated > t.negated)
		return(1);
	else if (negated == t.negated && type > t.type)
		return(1);
	else if (negated == t.negated && type == t.type && 
		 value > t.value)
		return(1);
	else
		return(0);
}

// negation functions
int
Literal::isNegated() const
{
	return(negated);
}

Literal
Literal::operator~() const
{
	Literal nt(*this);
	if (nt.negated)
		nt.negated = 0;
	else
		nt.negated = 1;
	return(nt);
}

// parse a string to populate a term
int
Literal::parse(const String &s)
{
	// save string
	inputstring = s;

	// define the tokenizer
	StringTokens st(s, " \t");

	// call real parser
	return(parse(st));
}

int
Literal::parse(StringTokens &st)
{
	// check if done, it should not be
	if (st.done())
		return(OK);

	// is the literal negated?
	if (st() == String("~"))
	{
		// set negation flag
		negated = 1;

		// advance to next token
		st++;
		if (st.done()) 
		{
			ERROR("unexpected end-of-string.", EINVAL);
			return(NOTOK);
		}
	}

	// check what we have
	if (st() == String("("))
	{
		// get the function name
		st++;
		if (st.done())
		{
			ERROR("unexpected end-of-string.", EINVAL);
			return(NOTOK);
		}
		value = st();

		// get the type of function
		if (value == String("="))
			type = Equal;
		else
			type = Function;
		st++;
		if (st.done())
		{
			ERROR("unexpected end-of-string.", EINVAL);
			return(NOTOK);
		}

		// allocate an argument list
		pargs = new List<Terms * >;
		MustBeTrue(pargs != NULL);

		// start scanning the argument list
		for (argnum = 0; !st.done() && st() != String(")"); argnum++)
		{
			Terms *parg = new Terms;
			MustBeTrue(parg != NULL);
			if (parg->parse(st) != OK)
			{
				ERROR("parse failed.", EINVAL);
				return(NOTOK);
			}
			MustBeTrue(pargs->insertAtEnd(parg) == OK);
		}
		if (argnum < 1)
		{
			ERROR("argument number < 1.", EINVAL);
			return(NOTOK);
		}

		// check for end of function
		if (st() != String(")"))
		{
			ERROR("expecting ')' token.", EINVAL);
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
	else if (String("A") <= st()(0,1) && st()(0,1) <= String("Z"))
	{
		// we have a predicate constant
		type = Constant;
		value = st();
		st++;
	}
	else
	{
		ERROR("unexpected token.", EINVAL);
		return(NOTOK);
	}

	// all done
	return(OK);
}

// print data
ostream &
operator<<(ostream &os, const Literal &t)
{
#if 0
	// print index
	os << "[" << t.index << "]";
#endif

	// any negation ???
	if (t.negated)
		os << "~ ";

	switch (t.type)
	{
	case Literal::Constant:
		os << t.value << " ";
		break;

	case Literal::Equal:
	{
		ListIterator<Terms *> pargsIter(*t.pargs);
		os << *pargsIter() << " ";
		os << t.value << " ";
		pargsIter++;
		os << *pargsIter() << " ";
		break;
	}

	case Literal::Function:
	{
		os << t.value << "( ";
		ListIterator<Terms *> pargsIter(*t.pargs);
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

	case Literal::LogicalConstant:
		os << t.value << " ";
		break;

	case Literal::Unknown:
		os << t.value << " ";
		break;

	default:
		MustBeTrue(0);
	}
	return(os);
}

// rename all variables in the term
int
Literal::renameVariables()
{
	Map<String, String> newvarnames;
	return(renameVariables(newvarnames));
}

int
Literal::renameVariables(Map<String, String> &nvs)
{
	switch (type)
	{
	case Constant:
	case LogicalConstant:
	case Unknown:
		return(OK);

	case Equal:
	case Function:
	{
		MustBeTrue(pargs != NULL);
		ListIterator<Terms * > pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			MustBeTrue(pargsIter() != NULL);
			if (pargsIter()->renameVariables(nvs) != OK)
			{
				ERROR("renameVariables failed.", EINVAL);
				return(NOTOK);
			}
		}
		return(OK);
	}

	default:
		ERRORD("unexpected type.", type, EINVAL);
		return(NOTOK);
	}
}

// convert literal to a string
String
Literal::convertToString(String &s) const
{
	// clear current string
	s = String("");

	// any negation ???
	if (negated)
		s += String("~ ");

	// build up the string
	switch (type)
	{
	case Literal::Constant:
	case Literal::LogicalConstant:
		s += value + String(" ");
		break;

	case Literal::Equal:
	case Literal::Function:
	{
		s += String("( ") + value + String(" ");
		ListIterator<Terms * > pargsIter(*pargs);
		for (int first = 1; !pargsIter.done(); pargsIter++)
		{
			if (!first)
				s += String(", ");
			s += (String)*pargsIter();
			first = 0;
		}
		s += String(") ");
		break;
	}

	default:
		MustBeTrue(0);
	}

	// return the new string
	return(s);
}

Literal::operator String()
{
	return(convertToString(inputstring));
}

Literal::operator String() const
{
	String tmp;
	return(convertToString(tmp));
}

// calculate basic statistics for this literal
void
Literal::basicStats(int &npconsts, int &npeqs, int &npfuncs,
	int &nconsts, int &nvars, int &nfuncs, int &depth) const
{
	// any negation ???
	if (negated)
		depth += 1;

	// build up the string
	switch (type)
	{
	case Literal::Constant:
	case Literal::LogicalConstant:
		depth += 1;
		npconsts += 1;
		break;

	case Literal::Equal:
		npeqs += 1;

	case Literal::Function:
	{
		int savedepth = 0;
		depth += 1;
		npfuncs += 1;
		ListIterator<Terms * > pargsIter(*pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			int termdepth = 0;
			pargsIter()->basicStats(nconsts, nvars, nfuncs, termdepth);
			if (termdepth > savedepth)
				savedepth = termdepth;
		}
		depth += savedepth;
		break;
	}

	default:
		MustBeTrue(0);
	}

	// all done
	return;
}

#ifdef MEMORYLEAK
// memory allocation and deallocation functions
void *
Terms::operator new(size_t size)
{
	Terms *p = (Terms *) ::new char[size];
	MustBeTrue(memoryleak.insert((unsigned long)p, 1) == OK);
	return(p);
}

void
Terms::operator delete(void *p)
{
	unsigned long addr = (unsigned long)p;
	int ucnt = 0;
	MustBeTrue(memoryleak.remove(addr, ucnt) == OK);
	::delete p;
}

void *
Literal::operator new(size_t size)
{
	Literal *p = (Literal *) ::new char[size];
	MustBeTrue(memoryleak.insert((unsigned long)p, 1) == OK);
	return(p);
}

void
Literal::operator delete(void *p)
{
	unsigned long addr = (unsigned long)p;
	int ucnt = 0;
	MustBeTrue(memoryleak.remove(addr, ucnt) == OK);
	::delete p;
}
#endif

