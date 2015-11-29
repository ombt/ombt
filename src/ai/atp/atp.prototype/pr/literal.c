// code for literal and term classes

// headers
#include "literal.h"

// term constructors and destructor
Terms::Terms():
	value(""), inputstring(), type(Unknown), argnum(0), pargs(NULL)
{
	// nothing to do
}

Terms::Terms(const Terms &t):
	value(t.value), inputstring(t.inputstring), type(t.type), 
	argnum(t.argnum), pargs(NULL)
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
	value(""), inputstring(""), type(Unknown), argnum(0), pargs(NULL)
{
	MustBeTrue(parse(s) == OK);
}

Terms::~Terms()
{
	if (type == Function && pargs != NULL)
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
	return(*this);
}

// comparison operators
int
Terms::operator==(const Terms &t) const
{
	if (type == t.type && value == t.value)
		return(1);
	else
		return(0);
}

int
Terms::operator!=(const Terms &t) const
{
	if (type != t.type || value != t.value)
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
	else
		return(0);
}

int
Terms::operator<=(const Terms &t) const
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
Terms::operator>(const Terms &t) const
{
	if (type > t.type)
		return(1);
	else if (type == t.type && value > t.value)
		return(1);
	else
		return(0);
}

int
Terms::operator>=(const Terms &t) const
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
		return(NOTOK);
	}

	// check what we have
	if (st() == String("("))
	{
		// get the function name
		st++;
		if (st.done())
			return(NOTOK);
		value = st();
		type = Function;
		st++;
		if (st.done())
			return(NOTOK);

		// allocate an argument list
		pargs = new List<Terms *>;
		MustBeTrue(pargs != NULL);

		// start scanning the argument list
		for (argnum = 0; !st.done() && st() != String(")"); argnum++)
		{
			Terms *parg = new Terms;
			MustBeTrue(parg != NULL);
			if (parg->parse(st) != OK)
				return(NOTOK);
			MustBeTrue(pargs->insertAtEnd(parg) == OK);
		}
		if (argnum < 1)
			return(NOTOK);

		// check for end of function
		if (st() != String(")"))
			return(NOTOK);
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
			return(NOTOK);
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
			nvs[value] = uniqueName("_RV");
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
				return(NOTOK);
			
		}
		return(OK);
	}

	default:
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

String
Terms::operator String() const
{
	String tmp;
	return(convertToString(tmp));
}

String
Terms::operator String()
{
	return(convertToString(inputstring));
}

// literal constructors and destructor
Literal::Literal():
	value(""), inputstring(""), type(Unknown), 
	argnum(0), negated(0), pargs(NULL)
{
	// nothing to do
}

Literal::Literal(const Literal &t):
	value(t.value), inputstring(t.inputstring), type(t.type), 
	argnum(t.argnum), negated(t.negated), pargs(NULL)
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
	argnum(0), negated(0), pargs(NULL)
{
	MustBeTrue(parse(s) == OK);
}

Literal::~Literal()
{
	if (type == Function && pargs != NULL)
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
	return(*this);
}

// comparison operators
int
Literal::operator==(const Literal &t) const
{
	if (negated == t.negated && type == t.type && value == t.value)
		return(1);
	else
		return(0);
}

int
Literal::operator!=(const Literal &t) const
{
	if (negated != t.negated || type != t.type || value != t.value)
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
	else if (negated == t.negated && type == t.type && value < t.value)
		return(1);
	else
		return(0);
}

int
Literal::operator<=(const Literal &t) const
{
	if (negated == t.negated && type == t.type && value == t.value)
		return(1);
	else if (negated < t.negated)
		return(1);
	else if (negated == t.negated && type < t.type)
		return(1);
	else if (negated == t.negated && type == t.type && value < t.value)
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
	else if (negated == t.negated && type == t.type && value > t.value)
		return(1);
	else
		return(0);
}

int
Literal::operator>=(const Literal &t) const
{
	if (negated == t.negated && type == t.type && value == t.value)
		return(1);
	else if (negated > t.negated)
		return(1);
	else if (negated == t.negated && type > t.type)
		return(1);
	else if (negated == t.negated && type == t.type && value > t.value)
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
		return(NOTOK);

	// is the literal negated?
	if (st() == String("~"))
	{
		// set negation flag
		negated = 1;

		// advance to next token
		st++;
		if (st.done()) 
			return(NOTOK);
	}

	// check what we have
	if (st() == String("("))
	{
		// get the function name
		st++;
		if (st.done())
			return(NOTOK);
		value = st();

		// get the type of function
		if (value == String("="))
			type = Equal;
		else
			type = Function;
		st++;
		if (st.done())
			return(NOTOK);

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
				return(NOTOK);
			}
			MustBeTrue(pargs->insertAtEnd(parg) == OK);
		}
		if (argnum < 1)
			return(NOTOK);

		// check for end of function
		if (st() != String(")"))
			return(NOTOK);
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
		return(NOTOK);
	}

	// all done
	return(OK);
}

// print data
ostream &
operator<<(ostream &os, const Literal &t)
{
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
				return(NOTOK);
			
		}
		return(OK);
	}

	default:
		return(NOTOK);
	}
}

// convert literal to a string
String
Literal::convertToString(String &s) const
{
	// clear current string
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

String
Literal::operator String()
{
	return(convertToString(inputstring));
}

String
Literal::operator String() const
{
	String tmp;
	return(convertToString(tmp));
}

