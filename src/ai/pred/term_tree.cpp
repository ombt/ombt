// code for term class

// headers
#include "term_tree.h"

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
#if 0
	Term t1(*this);
	Term t2(t);
	Substitutions sts;
	if (unify(t1, t2, sts) == OK)
		return(1);
	else
		return(0);
#else
	return(ucompare(*this, t) == 0);
#endif
}

int
Term::operator!=(const Term &t) const
{
#if 0
	Term t1(*this);
	Term t2(t);
	Substitutions sts;
	if (unify(t1, t2, sts) == OK)
		return(0);
	else
		return(1);
#else
	return(ucompare(*this, t) != 0);
#endif
}

int
Term::operator<(const Term &t) const
{
#if 0
	return(compare(value, t.value) < 0);
#else
	return(ucompare(*this, t) < 0);
#endif
}

int
Term::operator<=(const Term &t) const
{
#if 0
	Term t1(*this);
	Term t2(t);
	Substitutions sts;
	if (unify(t1, t2, sts) == OK)
		return(1);
	else
		return(compare(t1, t2) < 0);
#else
	return(ucompare(*this, t) <= 0);
#endif
}

int
Term::operator>(const Term &t) const
{
#if 0
	return(compare(value, t.value) > 0);
#else
	return(compare(*this, t) > 0);
#endif
}

int
Term::operator>=(const Term &t) const
{
#if 0
	Term t1(*this);
	Term t2(t);
	Substitutions sts;
	if (unify(t1, t2, sts) == OK)
		return(1);
	else
		return(compare(t1, t2) > 0);
#else
	return(ucompare(*this, t) >= 0);
#endif
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
		return(0);
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
		if (argnum <= 0)
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
		// we have a quoted string
		type = QuotedString;
		value = st()(0, st().strlen()-2);
#if 0
		// check for ending double quote
		if (st()(st().strlen()-1, 1) != String("\""))
			return(NOTOK);
#endif
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

// check if two terms are equal
int
Term::equals(const Term &t) const
{
	if (type == t.type && value == t.value)
		return(1);
	else
		return(0);
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

// substitution constructors and destructor
Substitution::Substitution():
	variable(""), term()
{
	// nothing to do
}

Substitution::Substitution(const Substitution &s):
	variable(s.variable), term(s.term)
{
	// nothing to do
}

Substitution::Substitution(const Term &t, const String &v):
	variable(v), term(t)
{
	// nothing to do
}

Substitution::~Substitution()
{
	// nothing to do
}

// assignment operator
Substitution &
Substitution::operator=(const Substitution &rhs)
{
	if (this != &rhs)
	{
		variable = rhs.variable;
		term = rhs.term;
	}
	return(*this);
}

// comparison operators
int
Substitution::operator==(const Substitution &s) const
{
	return(variable == s.variable);
}

int
Substitution::operator!=(const Substitution &s) const
{
	return(variable != s.variable);
}

int
Substitution::operator<(const Substitution &s) const
{
	return(variable < s.variable);
}

int
Substitution::operator<=(const Substitution &s) const
{
	return(variable <= s.variable);
}

int
Substitution::operator>(const Substitution &s) const
{
	return(variable > s.variable);
}

int
Substitution::operator>=(const Substitution &s) const
{
	return(variable >= s.variable);
}

// composition, combine two substitutions
Substitution
Substitution::operator*(const Substitution &s) const
{
	Substitution composition(*this);
	MustBeTrue(s.applyTo(composition.term) == OK);
	return(composition);
}

int
Substitution::applyTo(Term &t) const
{
	// search for the substitution variable in the term
	if (t.type == Term::Variable)
	{
		// check if the variables are the same
		if (t.value == variable)
			t = term;
	}
	else if (t.type == Term::PredicateFunction || t.type == Term::Function)
	{
		ListIterator<Term *> pargsIter(*t.pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			if (pargsIter() == NULL)
				continue;
			if (applyTo(*pargsIter()) != OK)
				return(NOTOK);
		}
	}
	return(OK);
}

// print data
ostream &
operator<<(ostream &os, const Substitution &s)
{
	os << "{ " << s.term << "/" << s.variable << " }";
	return(os);
}

// substitution constructors and destructor
Substitutions::Substitutions():
	psubs(NULL)
{
	psubs = new List<Substitution *>;
	MustBeTrue(psubs != NULL);
}

Substitutions::Substitutions(const Substitutions &s):
	psubs(NULL)
{
	psubs = new List<Substitution *>;
	MustBeTrue(psubs != NULL);
	ListIterator<Substitution *> psubsIter(*s.psubs);
	for ( ; !psubsIter.done(); psubsIter++)
	{
		if (psubsIter() == NULL)
			continue;
		Substitution *psub = new Substitution(*psubsIter());
		MustBeTrue(psub != NULL);
		MustBeTrue(psubs->insertAtEnd(psub) == OK);
	}
}

Substitutions::~Substitutions()
{
	ListIterator<Substitution *> psubsIter(*psubs);
	for ( ; !psubsIter.done(); psubsIter++)
	{
		if (psubsIter() != NULL)
			delete psubsIter();
	}
	psubs->clear();
	delete psubs;
	psubs = NULL;
}

// assignment operator
Substitutions &
Substitutions::operator=(const Substitutions &rhs)
{
	if (this != &rhs)
	{
		// delete current data
		ListIterator<Substitution *> psubsIter(*psubs);
		for ( ; !psubsIter.done(); psubsIter++)
		{
			if (psubsIter() != NULL)
				delete psubsIter();
		}
		psubs->clear();

		// insert new data
		ListIterator<Substitution *> prhssubsIter(*rhs.psubs);
		for ( ; !prhssubsIter.done(); prhssubsIter++)
		{
			if (prhssubsIter() == NULL)
				continue;
			Substitution *psub = new Substitution(*prhssubsIter());
			MustBeTrue(psub != NULL);
			MustBeTrue(psubs->insertAtEnd(psub) == OK);
		}
	}
	return(*this);
}

// comparison operators
int
Substitutions::operator==(const Substitutions &s) const
{
	ListIterator<Substitution *> psi(*psubs);
	ListIterator<Substitution *> pssi(*s.psubs);
	for ( ; !psi.done() && !pssi.done(); psi++, pssi++)
	{
		if (psi()->variable != pssi()->variable)
			return(0);
	}
	if (!psi.done() || !pssi.done())
		return(0);
	else
		return(1);
}

int
Substitutions::operator!=(const Substitutions &s) const
{
	ListIterator<Substitution *> psi(*psubs);
	ListIterator<Substitution *> pssi(*s.psubs);
	for ( ; !psi.done() && !pssi.done(); psi++, pssi++)
	{
		if (psi()->variable != pssi()->variable)
			return(1);
	}
	if (!psi.done() || !pssi.done())
		return(1);
	else
		return(0);
}

int
Substitutions::operator<(const Substitutions &s) const
{
	ListIterator<Substitution *> psi(*psubs);
	ListIterator<Substitution *> pssi(*s.psubs);
	for ( ; !psi.done() && !pssi.done(); psi++, pssi++)
	{
		if (psi()->variable < pssi()->variable)
			return(1);
		else if (psi()->variable > pssi()->variable)
			return(0);
	}
	if (psi.done() && !pssi.done())
		return(1);
	else
		return(0);
}

int
Substitutions::operator>(const Substitutions &s) const
{
	ListIterator<Substitution *> psi(*psubs);
	ListIterator<Substitution *> pssi(*s.psubs);
	for ( ; !psi.done() && !pssi.done(); psi++, pssi++)
	{
		if (psi()->variable > pssi()->variable)
			return(1);
		else if (psi()->variable < pssi()->variable)
			return(0);
	}
	if (!psi.done() && pssi.done())
		return(1);
	else
		return(0);
}

int
Substitutions::operator<=(const Substitutions &s) const
{
	ListIterator<Substitution *> psi(*psubs);
	ListIterator<Substitution *> pssi(*s.psubs);
	for ( ; !psi.done() && !pssi.done(); psi++, pssi++)
	{
		if (psi()->variable < pssi()->variable)
			return(1);
		else if (psi()->variable > pssi()->variable)
			return(0);
	}
	if (psi.done() && !pssi.done())
		return(1);
	else if (psi.done() && pssi.done())
		return(1);
	else
		return(0);
}

int
Substitutions::operator>=(const Substitutions &s) const
{
	ListIterator<Substitution *> psi(*psubs);
	ListIterator<Substitution *> pssi(*s.psubs);
	for ( ; !psi.done() && !pssi.done(); psi++, pssi++)
	{
		if (psi()->variable > pssi()->variable)
			return(1);
		else if (psi()->variable < pssi()->variable)
			return(0);
	}
	if (!psi.done() && pssi.done())
		return(1);
	else if (psi.done() && pssi.done())
		return(1);
	else
		return(0);
}

// insert a substitution in the list
int
Substitutions::insert(const Substitution &s)
{
	Substitution *psub = new Substitution(s);
	MustBeTrue(psub != NULL);
	return(psubs->insertAtEnd(psub));
}

void
Substitutions::clear()
{
	// delete current data
	ListIterator<Substitution *> psubsIter(*psubs);
	for ( ; !psubsIter.done(); psubsIter++)
	{
		if (psubsIter() != NULL)
			delete psubsIter();
	}
	psubs->clear();
	return;
}

int
Substitutions::isEmpty() const
{
	return(psubs->isEmpty());
}

// compositions, combine two substitutions
Substitutions
Substitutions::operator*(const Substitutions &s) const
{
	// composition 
	Substitutions composition;

	// iterators
	ListIterator<Substitution *> ti(*psubs);
	ListIterator<Substitution *> si(*s.psubs);

	// apply substitutions
	for ( ; !ti.done(); ti++)
	{
		Substitution newsub = *ti();
		for (si.reset(); !si.done(); si++)
		{
			newsub = newsub*(*si());
		}
		if (!newsub.isDegenerate())
		{
			MustBeTrue(composition.insert(newsub) == OK);
		}
	}

	// copy any substitutions not in the original
	for (si.reset(); !si.done(); si++)
	{
		int found = 0;
		for (ti.reset() ; !ti.done(); ti++)
		{
			if (ti()->variable == si()->variable)
			{
				found = 1;
				break;
			}
		}
		if (!found)
		{
			MustBeTrue(composition.insert(*si()) == OK);
		}
	}
	return(composition);
}

Substitutions
Substitutions::operator*(const Substitution &s) const
{
	// composition 
	Substitutions composition;

	// iterators
	ListIterator<Substitution *> ti(*psubs);

	// apply substitutions
	for ( ; !ti.done(); ti++)
	{
		Substitution newsub = (*ti())*s;
		if (!newsub.isDegenerate())
		{
			MustBeTrue(composition.insert(newsub) == OK);
		}
	}

	// copy any substitutions not in the original
	int found = 0;
	for (ti.reset() ; !ti.done(); ti++)
	{
		if (ti()->variable == s.variable)
		{
			found = 1;
			break;
		}
	}
	if (!found)
	{
		MustBeTrue(composition.insert(s) == OK);
	}
	return(composition);
}

int
Substitutions::applyTo(Term &t) const
{
	ListIterator<Substitution *> psi(*psubs);
	for ( ; !psi.done(); psi++)
	{
		if (psi()->applyTo(t) != OK)
			return(NOTOK);
	}
	return(OK);
}

// check if variable is also the term
int
Substitution::isDegenerate() const
{
	if ((term.type == Term::Variable) &&
	    (term.value == variable))
		return(1);
	else
		return(0);
}

// print data
ostream &
operator<<(ostream &os, const Substitutions &s)
{
	if (s.psubs != NULL)
	{
		os << "{ ";
		ListIterator<Substitution *> psubsIter(*s.psubs);
		for ( ; !psubsIter.done(); psubsIter++)
		{
			if (psubsIter() != NULL)
				os << *psubsIter() << " , ";
		}
		os << " }";
	}
	return(os);
}

// code for unification. the algorithm is from the book Artificial Intelligence
// by E. Rich and K. Knight (2nd edition) on pages 152, 153. unify two term, 
// if possible
int
unify(Term &t1, Term &t2, Substitutions &s)
{
	// check if a variable or constant
	if ((t1.type != Term::PredicateFunction && 
	     t1.type != Term::Function) ||
	    (t2.type != Term::PredicateFunction && 
	     t2.type != Term::Function))
	{
		// type better be known
		MustBeTrue((t1.type != Term::Unknown) && 
			   (t2.type != Term::Unknown));

		// check if they are identical
		if ((t1.type == t2.type) && (t1.value == t2.value))
		{
			// no substitutions required
			s.clear();
		}
		else if (t1.type == Term::Variable)
		{
			// t1 is a variable, check if t1 occurs in t2
			if (t2.occurs(t1))
			{
				// not unifiable
				s.clear();
				return(NOMATCH);
			}

			// return { t2 / t1 }
			s.insert(Substitution(t2, t1.value));
		}
		else if (t2.type == Term::Variable)
		{
			// t2 is a variable, check if t2 occurs in t1
			if (t1.occurs(t2))
			{
				// not unifiable
				s.clear();
				return(NOMATCH);
			}

			// return { t1 / t2 }
			s.insert(Substitution(t1, t2.value));
		}
		else
		{
			// not unifiable
			s.clear();
			return(NOMATCH);
		}
		return(OK);
	}

	// symbol name must the same
	if (!t1.equals(t2))
	{
		// not unifiable
		s.clear();
		return(NOMATCH);
	}

	// check number arguments
	MustBeTrue(t1.argnum > 0 && t2.argnum > 0);
	if (t1.argnum != t2.argnum)
	{
		// not unifiable
		s.clear();
		return(NOMATCH);
	}

	// unify function arguments
	ListIterator<Term *> t1ai(*t1.pargs);
	ListIterator<Term *> t2ai(*t2.pargs);

	// cycle through the arguments
	Substitutions s2;
	for ( ; !t1ai.done() && !t2ai.done(); t1ai++, t2ai++)
	{
		// clear old substitutions
		s2.clear();

		// attempt to unify the arguments
		int status = unify(*t1ai(), *t2ai(), s2);
		if (status != OK)
		{
			// not unifiable
			s.clear();
			return(status);
		}

		// check if any substitutions were generated
		if (s2.isEmpty()) continue;

		// apply substitution to remaining arguments
		ListIterator<Term *> restoft1ai(t1ai);
		ListIterator<Term *> restoft2ai(t2ai);
		for (restoft1ai++, restoft2ai++;
		     !restoft1ai.done() && !restoft2ai.done();
		     restoft1ai++, restoft2ai++)
		{
			if (s2.applyTo(*restoft1ai()) != OK)
			{
				// not unifiable
				s.clear();
				return(NOMATCH);
			}
			if (s2.applyTo(*restoft2ai()) != OK)
			{
				// not unifiable
				s.clear();
				return(NOMATCH);
			}
		}

		// save substitution
		s = s*s2;
	}

	// both iterators should be done
	if (!t1ai.done() || !t2ai.done())
	{
		// not unifiable
		s.clear();
		return(NOMATCH);
	}

	// unified
	return(OK);
}

// compare two term, impose an order base on syntax
int
compare(const Term &t1, const Term &t2)
{
	// type better be known
	MustBeTrue((t1.type != Term::Unknown) && (t2.type != Term::Unknown));

	// first, use type for sorting
	if (int(t1.type) < int(t2.type))
		return(-1);
	else if (int(t1.type) > int(t2.type))
		return(1);

	// we have the same type of terms. check if they are functions
	if ((t1.type != Term::PredicateFunction && 
	     t1.type != Term::Function))
	{
		String t1tmp;
		String t2tmp;

		// check if negated
		if (t1.isNegated())
			t1tmp = t1.value(1);
		else
			t1tmp = t1.value;
		if (t2.isNegated())
			t2tmp = t2.value(1);
		else
			t2tmp = t2.value;

		// compare unnegated names
		if (t1tmp < t2tmp)
			return(-1);
		else if (t1tmp > t2tmp)
			return(1);
		else
			return(0);
	}

	// check if the function names are same
	String t1tmp;
	String t2tmp;
	if (t1.isNegated())
		t1tmp = t1.value(1);
	else
		t1tmp = t1.value;
	if (t2.isNegated())
		t2tmp = t2.value(1);
	else
		t2tmp = t2.value;

	// compare unnegated names
	if (t1tmp < t2tmp)
		return(-1);
	else if (t1tmp > t2tmp)
		return(1);

	// compare the number of function arguments
	MustBeTrue(t1.argnum == t2.argnum && t1.argnum > 0);

	// cycle thru the arguments
	ListIterator<Term *> t1ai(*t1.pargs);
	ListIterator<Term *> t2ai(*t2.pargs);
	for ( ; !t1ai.done() && !t2ai.done(); t1ai++, t2ai++)
	{
		// compare the arguments
		int status = compare(*t1ai(), *t2ai());
		if (status != 0) 
			return(status);
	}

	// both iterators should be done
	MustBeTrue(t1ai.done() && t2ai.done());

	// the same
	return(0);
}

// code for unification. the algorithm is from the book Artificial Intelligence
// by E. Rich and K. Knight (2nd edition) on pages 152, 153.  unify two term, 
// if possible.
int
ucompare(const Term &t1, const Term &t2)
{
	Term t1tmp(t1);
	Term t2tmp(t2);
	Substitutions sts;
	return(CompareAndUnify(t1tmp, t2tmp, sts));
}

int
CompareAndUnify(Term &t1, Term &t2, Substitutions &s)
{
	// check if a variable or constant
	if ((t1.type != Term::PredicateFunction && 
	     t1.type != Term::Function) ||
	    (t2.type != Term::PredicateFunction && 
	     t2.type != Term::Function))
	{
		// type better be known
		MustBeTrue((t1.type != Term::Unknown) && 
			   (t2.type != Term::Unknown));

		// check if they are identical
		if ((t1.type == t2.type) && (t1.value == t2.value))
		{
			// no substitutions required
			s.clear();
		}
		else if (t1.type == Term::Variable)
		{
			// t1 is a variable, check if t1 occurs in t2
			if (t2.occurs(t1))
			{
				// not unifiable
				s.clear();

				// t1 is a variable and t2 contains
				// t1 within it. this means that t2
				// is a function. if t2 was a variable,
				// then it would have been caught 
				// above.
				//
				MustBeTrue((t2.type == Term::Function) ||
					(t2.type == Term::PredicateFunction));
				if (t1.type < t2.type)
					return(-1);
				else if (t1.type > t2.type)
					return(1);
				else
					MustBeTrue(0);
			}

			// return { t2 / t1 }
			s.insert(Substitution(t2, t1.value));
		}
		else if (t2.type == Term::Variable)
		{
			// t2 is a variable, check if t2 occurs in t1
			if (t1.occurs(t2))
			{
				// not unifiable
				s.clear();

				// t2 is a variable and t1 contains
				// t2 within it. this means that t2
				// is a function. if t2 was a variable,
				// then it would have been caught 
				// above.
				//
				MustBeTrue((t1.type == Term::Function) ||
					(t1.type == Term::PredicateFunction));
				if (t1.type < t2.type)
					return(-1);
				else if (t1.type > t2.type)
					return(1);
				else
					MustBeTrue(0);
			}

			// return { t1 / t2 }
			s.insert(Substitution(t1, t2.value));
		}
		else
		{
			// not unifiable
			s.clear();

			// compare type, and names 
			if (t1.type < t2.type)
				return(-1);
			else if (t1.type > t2.type)
				return(1);
			else if (t1.value < t2.value)
				return(-1);
			else if (t1.value > t2.value)
				return(1);
			else
				MustBeTrue(0);
		}
		return(0);
	}

	// symbol name and type must the same
	if ((t1.type != t2.type) || (t1.value != t2.value))
	{
		// not unifiable
		s.clear();

		// determine which is greater
		if (t1.type < t2.type)
			return(-1);
		else if (t1.type > t2.type)
			return(1);
		else if (t1.value < t2.value)
			return(-1);
		else if (t1.value > t2.value)
			return(1);
		else
			MustBeTrue(0);
	}

	// check number arguments
	MustBeTrue(t1.argnum == t2.argnum);
	MustBeTrue(t1.argnum > 0 && t2.argnum > 0);

	// unify function arguments
	ListIterator<Term *> t1ai(*t1.pargs);
	ListIterator<Term *> t2ai(*t2.pargs);

	// cycle through the arguments
	Substitutions s2;
	for ( ; !t1ai.done() && !t2ai.done(); t1ai++, t2ai++)
	{
		// clear old substitutions
		s2.clear();

		// attempt to unify the arguments
		int status = CompareAndUnify(*t1ai(), *t2ai(), s2);
		if (status != 0)
		{
			// not unifiable
			s.clear();
			return(status);
		}

		// check if any substitutions were generated
		if (s2.isEmpty()) continue;

		// apply substitution to remaining arguments
		ListIterator<Term *> restoft1ai(t1ai);
		ListIterator<Term *> restoft2ai(t2ai);
		for (restoft1ai++, restoft2ai++;
		     !restoft1ai.done() && !restoft2ai.done();
		     restoft1ai++, restoft2ai++)
		{
			MustBeTrue(s2.applyTo(*restoft1ai()) == OK);
			MustBeTrue(s2.applyTo(*restoft2ai()) == OK);
		}

		// save substitution
		s = s*s2;
	}

	// both iterators should be done
	MustBeTrue(t1ai.done() && t2ai.done());

	// unified
	return(0);
}

