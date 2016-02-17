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
	partOfConclusion(src.partOfConclusion), terms(src.terms)
{
	// nothing to do
}

Clause::Clause(const Set_List<Term> &alist, int d, int poc):
	depth(d), number(-1), partOfConclusion(poc), terms(alist)
{
	// nothing to do
}

Clause::~Clause()
{
	// nothing to do
}

// assignment operator
Clause &
Clause::operator=(const Clause &rhs)
{
	if (this != &rhs)
	{
		depth = rhs.depth;
		number = rhs.number;
		partOfConclusion = rhs.partOfConclusion;
		terms = rhs.terms;
	}
	return(*this);
}

// comparison operators
int
Clause::operator==(const Clause &rhs) const
{
#if 0
	return(terms == rhs.terms);
#else
	Set_List_Iterator<Term> ti(terms);
	Set_List_Iterator<Term> rhsti(rhs.terms);
	for ( ; !ti.done() && !rhsti.done(); ti++, rhsti++)
	{
		if (ti().predicate() != rhsti().predicate())
			return(0);
	}
	if (ti.done() && rhsti.done())
		return(1);
	else
		return(0);
#endif
}

int
Clause::operator!=(const Clause &rhs) const
{
#if 0
	return(terms != rhs.terms);
#else
	Set_List_Iterator<Term> ti(terms);
	Set_List_Iterator<Term> rhsti(rhs.terms);
	for ( ; !ti.done() && !rhsti.done(); ti++, rhsti++)
	{
		if (ti().predicate() != rhsti().predicate())
			return(1);
	}
	if (ti.done() && rhsti.done())
		return(0);
	else
		return(1);
#endif
}

int
Clause::operator<(const Clause &rhs) const
{
	Set_List_Iterator<Term> ti(terms);
	Set_List_Iterator<Term> rhsti(rhs.terms);
	for ( ; !ti.done() && !rhsti.done(); ti++, rhsti++)
	{
		if (ti().predicate() < rhsti().predicate())
			return(1);
		else if (ti().predicate() > rhsti().predicate())
			return(0);
	}
	if (ti.done() && !rhsti.done())
		return(1);
	else
		return(0);
}

int
Clause::operator>(const Clause &rhs) const
{
	Set_List_Iterator<Term> ti(terms);
	Set_List_Iterator<Term> rhsti(rhs.terms);
	for ( ; !ti.done() && !rhsti.done(); ti++, rhsti++)
	{
		if (ti().predicate() > rhsti().predicate())
			return(1);
		else if (ti().predicate() < rhsti().predicate())
			return(0);
	}
	if (!ti.done() && rhsti.done())
		return(1);
	else
		return(0);
}

int
Clause::operator<=(const Clause &rhs) const
{
	Set_List_Iterator<Term> ti(terms);
	Set_List_Iterator<Term> rhsti(rhs.terms);
	for ( ; !ti.done() && !rhsti.done(); ti++, rhsti++)
	{
		if (ti().predicate() < rhsti().predicate())
			return(1);
		else if (ti().predicate() > rhsti().predicate())
			return(0);
	}
	if (ti.done() && !rhsti.done())
		return(1);
	else if (ti.done() && rhsti.done())
		return(1);
	else
		return(0);
}

int
Clause::operator>=(const Clause &rhs) const
{
	Set_List_Iterator<Term> ti(terms);
	Set_List_Iterator<Term> rhsti(rhs.terms);
	for ( ; !ti.done() && !rhsti.done(); ti++, rhsti++)
	{
		if (ti().predicate() > rhsti().predicate())
			return(1);
		else if (ti().predicate() < rhsti().predicate())
			return(0);
	}
	if (!ti.done() && rhsti.done())
		return(1);
	else if (ti.done() && rhsti.done())
		return(1);
	else
		return(0);
}

// access functions
void
Clause::insert(const Term &term)
{
	terms.insert(term);
}

void
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
	Set_List_Iterator<Term> termsIter(terms);
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
	// clause A subsumes clause B if A's terms
	// are a proper subset of B's terms. we use
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
#if 0
	os << c.terms;
#else
	Set_List_Iterator<Term> cIter(c.terms);
	for (int first = 1; !cIter.done(); cIter++)
	{
		if (first)
		{
			first = 0;
			os << cIter();
		}
		else
			os << "|| " << cIter();
	}
#endif
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

