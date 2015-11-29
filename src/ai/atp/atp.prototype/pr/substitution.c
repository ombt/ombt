// code for substitution classes

// headers
#include "substitution.h"

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

Substitution::Substitution(const Terms &t, const String &v):
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
Substitution::applyTo(Literal &l) const
{
	// check if a function. if not, nothing to do
	if (l.type != Literal::Function)
	{
		return(OK);
	}

	// apply substitution to each parameter
	ListIterator<Terms * > lai(*l.pargs);
	for ( ; !lai.done(); lai++)
	{
		if (applyTo(*lai()) != OK)
		{
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

int
Substitution::applyTo(Clause &cl) const
{
	Clause newcl;
	ClauseIterator clIter(cl);
	for ( ; !clIter.done(); clIter++)
	{
		ListIterator<Literal> dataIter(clIter.data());
		for ( ; !dataIter.done(); dataIter++)
		{
			Literal literal(dataIter());
			if (applyTo(literal) != OK)
				return(NOTOK);
			if (newcl.insert(literal) != OK)
				return(NOTOK);
		}
	}
	cl = newcl;

	// all done
	return(OK);
}

int
Substitution::applyTo(Terms &t) const
{
	// search for the substitution variable in the term
	if (t.type == Terms::Variable)
	{
		// check if the variables are the same
		if (t.value == variable)
		{
			t = term;
		}
	}
	else if (t.type == Terms::Function)
	{
		ListIterator<Terms * > pargsIter(*t.pargs);
		for ( ; !pargsIter.done(); pargsIter++)
		{
			if (pargsIter() == NULL)
				continue;
			if (applyTo(*pargsIter()) != OK)
			{
				return(NOTOK);
			}
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
	subs()
{
	// do nothing
}

Substitutions::Substitutions(const Substitutions &s):
	subs(s.subs)
{
	// do nothing
}

Substitutions::~Substitutions()
{
	// do nothing
}

// assignment operator
Substitutions &
Substitutions::operator=(const Substitutions &rhs)
{
	if (this != &rhs)
		subs = rhs.subs;
	return(*this);
}

// comparison operators
int
Substitutions::operator==(const Substitutions &s) const
{
	ListIterator<Substitution> si(subs);
	ListIterator<Substitution> ssi(s.subs);
	for ( ; !si.done() && !ssi.done(); si++, ssi++)
	{
		if (si().variable != ssi().variable)
			return(0);
	}
	if (!si.done() || !ssi.done())
		return(0);
	else
		return(1);
}

int
Substitutions::operator!=(const Substitutions &s) const
{
	ListIterator<Substitution> si(subs);
	ListIterator<Substitution> ssi(s.subs);
	for ( ; !si.done() && !ssi.done(); si++, ssi++)
	{
		if (si().variable != ssi().variable)
			return(1);
	}
	if (!si.done() || !ssi.done())
		return(1);
	else
		return(0);
}

int
Substitutions::operator<(const Substitutions &s) const
{
	ListIterator<Substitution> si(subs);
	ListIterator<Substitution> ssi(s.subs);
	for ( ; !si.done() && !ssi.done(); si++, ssi++)
	{
		if (si().variable < ssi().variable)
			return(1);
		else if (si().variable > ssi().variable)
			return(0);
	}
	if (si.done() && !ssi.done())
		return(1);
	else
		return(0);
}

int
Substitutions::operator>(const Substitutions &s) const
{
	ListIterator<Substitution> si(subs);
	ListIterator<Substitution> ssi(s.subs);
	for ( ; !si.done() && !ssi.done(); si++, ssi++)
	{
		if (si().variable > ssi().variable)
			return(1);
		else if (si().variable < ssi().variable)
			return(0);
	}
	if (!si.done() && ssi.done())
		return(1);
	else
		return(0);
}

int
Substitutions::operator<=(const Substitutions &s) const
{
	ListIterator<Substitution> si(subs);
	ListIterator<Substitution> ssi(s.subs);
	for ( ; !si.done() && !ssi.done(); si++, ssi++)
	{
		if (si().variable < ssi().variable)
			return(1);
		else if (si().variable > ssi().variable)
			return(0);
	}
	if (si.done() && !ssi.done())
		return(1);
	else if (si.done() && ssi.done())
		return(1);
	else
		return(0);
}

int
Substitutions::operator>=(const Substitutions &s) const
{
	ListIterator<Substitution> si(subs);
	ListIterator<Substitution> ssi(s.subs);
	for ( ; !si.done() && !ssi.done(); si++, ssi++)
	{
		if (si().variable > ssi().variable)
			return(1);
		else if (si().variable < ssi().variable)
			return(0);
	}
	if (!si.done() && ssi.done())
		return(1);
	else if (si.done() && ssi.done())
		return(1);
	else
		return(0);
}

// insert a substitution in the list
int
Substitutions::insert(const Substitution &s)
{
	return(subs.insertAtEnd(s));
}

void
Substitutions::clear()
{
	subs.clear();
	return;
}

int
Substitutions::isEmpty() const
{
	return(subs.isEmpty());
}

// compositions, combine two substitutions
Substitutions
Substitutions::operator*(const Substitutions &s) const
{
	// composition 
	Substitutions composition;

	// iterators
	ListIterator<Substitution> ti(subs);
	ListIterator<Substitution> si(s.subs);

	// apply substitutions
	for ( ; !ti.done(); ti++)
	{
		Substitution newsub = ti();
		for (si.reset(); !si.done(); si++)
		{
			newsub = newsub*si();
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
			if (ti().variable == si().variable)
			{
				found = 1;
				break;
			}
		}
		if (!found)
		{
			MustBeTrue(composition.insert(si()) == OK);
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
	ListIterator<Substitution> ti(subs);

	// apply substitutions
	for ( ; !ti.done(); ti++)
	{
		Substitution newsub = ti()*s;
		if (!newsub.isDegenerate())
		{
			MustBeTrue(composition.insert(newsub) == OK);
		}
	}

	// copy any substitutions not in the original
	int found = 0;
	for (ti.reset() ; !ti.done(); ti++)
	{
		if (ti().variable == s.variable)
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
Substitutions::applyTo(Literal &l) const
{
	ListIterator<Substitution> si(subs);
	for ( ; !si.done(); si++)
	{
		if (si().applyTo(l) != OK)
			return(NOTOK);
	}
	return(OK);
}

int
Substitutions::applyTo(Terms &t) const
{
	ListIterator<Substitution> si(subs);
	for ( ; !si.done(); si++)
	{
		if (si().applyTo(t) != OK)
			return(NOTOK);
	}
	return(OK);
}

int
Substitutions::applyTo(Clause &cl) const
{
	ListIterator<Substitution> si(subs);
	for ( ; !si.done(); si++)
	{
		if (si().applyTo(cl) != OK)
			return(NOTOK);
	}
	return(OK);
}

// check if variable is also the term
int
Substitution::isDegenerate() const
{
	if ((term.type == Terms::Variable) &&
	    (term.value == variable))
		return(1);
	else
		return(0);
}

// print data
ostream &
operator<<(ostream &os, const Substitutions &s)
{
	os << "{ ";
	ListIterator<Substitution> subsIter(s.subs);
	for ( ; !subsIter.done(); subsIter++)
	{
		os << subsIter() << " , ";
	}
	os << " }";
	return(os);
}
