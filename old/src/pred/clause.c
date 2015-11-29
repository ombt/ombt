// functions for clause class

// headers
#include "clause.h"

// constructors and destructor
Clause::Clause():
	depth(-1), number(-1), partOfConclusion(0), terms()
{
	// nothing to do
}

Clause::Clause(const Clause &src):
	depth(src.depth), number(src.number), 
	partOfConclusion(src.partOfConclusion), terms()
{
	ListIterator<Term *> srcti(src.terms);
	for ( ; !srcti.done(); srcti++)
	{
		Term *newterm = new Term(*srcti());
		MustBeTrue(newterm != NULL);
		MustBeTrue(terms.insertAtEnd(newterm) == OK);
	}
}

Clause::Clause(const List<Term> &alist, int d, int poc):
	depth(d), number(-1), partOfConclusion(poc), terms()
{
	ListIterator<Term> alisti(alist);
	for ( ; !alist.done(); alist++)
	{
		Term *newterm = new Term(alisti());
		MustBeTrue(newterm != NULL);
		MustBeTrue(terms.insertAtEnd(newterm) == OK);
	}
}

Clause::~Clause()
{
	ListIterator<Term *> termsi(terms);
	for ( ; !termsi.done(); termsi++)
	{
		delete termsi();
	}
	terms.clear();
}

// assignment operator
Clause &
Clause::operator=(const Clause &rhs)
{
	if (this != &rhs)
	{
		// delete old terms
		ListIterator<Term *> termsi(terms);
		for ( ; !termsi.done(); termsi++)
		{
			delete termsi();
		}
		terms.clear();

		// copy date
		depth = rhs.depth;
		number = rhs.number;
		partOfConclusion = rhs.partOfConclusion;

		// copy terms
		ListIterator<Term *> rhsti(rhs.terms);
		for ( ; !rhsti.done(); rhsti++)
		{
			Term *newterm = new Term(*rhsti());
			MustBeTrue(newterm != NULL);
			MustBeTrue(terms.insertAtEnd(newterm) == OK);
		}
	}
	return(*this);
}

// comparison operators
#if 0
int
Clause::operator==(const Clause &rhs) const
{
	return(terms == rhs.terms);
}

int
Clause::operator!=(const Clause &rhs) const
{
	return(terms != rhs.terms);
}

int
Clause::operator<(const Clause &rhs) const
{
	ListIterator<Term> ai(terms);
	ListIterator<Term> rhsai(rhs.terms);
	for ( ; !ai.done() && !rhsai.done(); ai++, rhsai++)
	{
		if (ai() < rhsai())
			return(1);
		else if (ai() > rhsai())
			return(0);
	}
	if (ai.done() && !rhsai.done())
		return(1);
	else
		return(0);
}

int
Clause::operator>(const Clause &rhs) const
{
	ListIterator<Term> ai(terms);
	ListIterator<Term> rhsai(rhs.terms);
	for ( ; !ai.done() && !rhsai.done(); ai++, rhsai++)
	{
		if (ai() > rhsai())
			return(1);
		else if (ai() < rhsai())
			return(0);
	}
	if (!ai.done() && rhsai.done())
		return(1);
	else
		return(0);
}

int
Clause::operator<=(const Clause &rhs) const
{
	ListIterator<Term> ai(terms);
	ListIterator<Term> rhsai(rhs.terms);
	for ( ; !ai.done() && !rhsai.done(); ai++, rhsai++)
	{
		if (ai() < rhsai())
			return(1);
		else if (ai() > rhsai())
			return(0);
	}
	if (ai.done() && !rhsai.done())
		return(1);
	else if (ai.done() && rhsai.done())
		return(1);
	else
		return(0);
}

int
Clause::operator>=(const Clause &rhs) const
{
	ListIterator<Term> ai(terms);
	ListIterator<Term> rhsai(rhs.terms);
	for ( ; !ai.done() && !rhsai.done(); ai++, rhsai++)
	{
		if (ai() > rhsai())
			return(1);
		else if (ai() < rhsai())
			return(0);
	}
	if (!ai.done() && rhsai.done())
		return(1);
	else if (ai.done() && rhsai.done())
		return(1);
	else
		return(0);
}
#endif

// access functions
int
Clause::insert(const Term &term)
{
	// insert the new term
	if (terms.isEmpty())
	{
		// copy term, and insert it
		Term *pterm = new Term(term);
		MustBeTrue(pterm != NULL);
		if (terms.insertAtEnd(pterm) != OK)
		{
			delete pterm;
			return(NOTOK);
		}
		return(OK);
	}

	// check if the term already exist, use unification
	Substitution sts;
	ListIterator<Term *) termsi(terms);
	for ( ; !termsi.done(); termsi++)
	{
		Term t1(*termsi());
		Term t2(term);
		sts.clear();
		if (unify(term1, term2, sts) == OK)
		{
			sts.applyTo(*termsi());
		}
	}
}

int
Clause::remove(Term &term)
{
	terms.remove(term);
}

int
Clause::isInClause(const Term &term) const
{
	return(terms.isMember(term));
}

void
Clause::clear()
{
	terms.clear();
}

int
Clause::isEmpty() const
{
	return(terms.isEmpty());
}

int
Clause::isATautology() const
{
	ListIterator<Term> termsIter(terms);
	for ( ; !termsIter.done(); termsIter++)
	{
		if (terms.isMember(~termsIter()))
			return(1);
	}
	return(0);
}

int
Clause::isPartOfConclusion() const
{
	return(partOfConclusion != 0);
}

int
Clause::subsumes(const Clause &c) const
{
	// clause A subsumes clause B if A's atoms
	// are a proper subset of B's atoms. we use
	// proper subset since we don't want to
	// replace identical clauses. we always 
	// want a clause replacd by a shorter clause.
	//
	return(terms.isProperSubsetOf(c.terms));
}

// access conclusion data
int
Clause::getPartOfConclusion() const
{
	return(partOfConclusion);
}

void
Clause::setPartOfConclusion(int poc)
{
	partOfConclusion = poc;
}

// access depth or number data
int
Clause::getDepth() const
{
	return(depth);
}

void
Clause::setDepth(int d)
{
	depth = d;
}

int
Clause::getNumber() const
{
	return(number);
}

void
Clause::setNumber(int n)
{
	number = n;
}

// set operations
Clause &
Clause::operator|=(const Clause &rhs)
{
	terms |= rhs.terms;
	return(*this);
}

Clause
Clause::operator|(const Clause &rhs) const
{
	return(Clause(*this) |= rhs);
}

Clause &
Clause::operator&=(const Clause &rhs)
{
	terms &= rhs.terms;
	return(*this);
}

Clause
Clause::operator&(const Clause &rhs) const
{
	return(Clause(*this) &= rhs);
}

Clause &
Clause::operator-=(const Clause &rhs)
{
	terms -= rhs.terms;
	return(*this);
}

Clause
Clause::operator-(const Clause &rhs) const
{
	return(Clause(*this) -= rhs);
}

// print contents of clauses
ostream &
operator<<(ostream &os, const Clause &c)
{
	os << c.terms;
	return(os);
}

// constructors and destructor for iterator class for clauses class
ClauseIterator::ClauseIterator(const Clause &clause):
	iterator(clause.terms)
{
	// do nothing
}

ClauseIterator::ClauseIterator(const ClauseIterator &clauseIter):
	iterator(clauseIter.iterator)
{
	// do nothing
}

ClauseIterator::~ClauseIterator()
{
	// do nothing
}

// reset iterator to start
void
ClauseIterator::reset()
{
	iterator.reset();
}

// is iterator done?
int
ClauseIterator::done() const
{
	return(iterator.done());
}

Term
ClauseIterator::operator()()
{
	return(iterator());
}

int
ClauseIterator::operator++(int)
{
	return(iterator++);
}

