// code for term class

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
