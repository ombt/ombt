// functions for clause class

// headers
#include "clause.h"

// constructors and destructor
Clause::Clause():
	depth(-1), number(-1), partOfConclusion(0), atoms()
{
	// nothing to do
}

Clause::Clause(const Clause &src):
	depth(src.depth), number(src.number), 
	partOfConclusion(src.partOfConclusion), atoms(src.atoms)
{
	// nothing to do
}

Clause::Clause(const Set_List<Atom> &alist, int d, int poc):
	depth(d), number(-1), partOfConclusion(poc), atoms(alist)
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
		atoms = rhs.atoms;
	}
	return(*this);
}

// comparison operators
int
Clause::operator==(const Clause &rhs) const
{
	return(atoms == rhs.atoms);
}

int
Clause::operator!=(const Clause &rhs) const
{
	return(atoms != rhs.atoms);
}

int
Clause::operator<(const Clause &rhs) const
{
	Set_List_Iterator<Atom> ai(atoms);
	Set_List_Iterator<Atom> rhsai(rhs.atoms);
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
	Set_List_Iterator<Atom> ai(atoms);
	Set_List_Iterator<Atom> rhsai(rhs.atoms);
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
	Set_List_Iterator<Atom> ai(atoms);
	Set_List_Iterator<Atom> rhsai(rhs.atoms);
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
	Set_List_Iterator<Atom> ai(atoms);
	Set_List_Iterator<Atom> rhsai(rhs.atoms);
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

// access functions
void
Clause::insert(const Atom &atom)
{
	atoms.insert(atom);
}

void
Clause::remove(Atom &atom)
{
	atoms.remove(atom);
}

int
Clause::isInClause(const Atom &atom) const
{
	return(atoms.isMember(atom));
}

void
Clause::clear()
{
	atoms.clear();
}

int
Clause::isEmpty() const
{
	return(atoms.isEmpty());
}

int
Clause::isATautology() const
{
	Set_List_Iterator<Atom> atomsIter(atoms);
	for ( ; !atomsIter.done(); atomsIter++)
	{
		if (atoms.isMember(~atomsIter()))
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
	return(atoms.isProperSubsetOf(c.atoms));
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
	atoms |= rhs.atoms;
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
	atoms &= rhs.atoms;
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
	atoms -= rhs.atoms;
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
	os << c.atoms;
	return(os);
}

// constructors and destructor for iterator class for clauses class
ClauseIterator::ClauseIterator(const Clause &clause):
	iterator(clause.atoms)
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

Atom
ClauseIterator::operator()()
{
	return(iterator());
}

int
ClauseIterator::operator++(int)
{
	return(iterator++);
}

// constructors and destructor length sorted clause
LClause::LClause():
	Clause()
{
	// nothing to do
}

LClause::LClause(const Clause &src):
	Clause(src)
{
	// nothing to do
}

LClause::LClause(const LClause &src):
	Clause(src)
{
	// nothing to do
}

LClause::~LClause()
{
	// nothing to do
}

// assignment operator
LClause &
LClause::operator=(const LClause &rhs)
{
	Clause::operator=(rhs);
	return(*this);
}

// comparison operators
int
LClause::operator==(const Clause &rhs) const
{
	ClauseIterator thisIter(*this);
	for (int thislen = 0; !thisIter.done(); thislen++, thisIter++);
	ClauseIterator rhsIter(rhs);
	for (int rhslen = 0; !rhsIter.done(); rhslen++, rhsIter++);
	return(thislen == rhslen);
}

int
LClause::operator!=(const Clause &rhs) const
{
	ClauseIterator thisIter(*this);
	for (int thislen = 0; !thisIter.done(); thislen++, thisIter++);
	ClauseIterator rhsIter(rhs);
	for (int rhslen = 0; !rhsIter.done(); rhslen++, rhsIter++);
	return(thislen != rhslen);
}

int
LClause::operator<(const Clause &rhs) const
{
	ClauseIterator thisIter(*this);
	for (int thislen = 0; !thisIter.done(); thislen++, thisIter++);
	ClauseIterator rhsIter(rhs);
	for (int rhslen = 0; !rhsIter.done(); rhslen++, rhsIter++);
	return(thislen < rhslen);
}

int
LClause::operator>(const Clause &rhs) const
{
	ClauseIterator thisIter(*this);
	for (int thislen = 0; !thisIter.done(); thislen++, thisIter++);
	ClauseIterator rhsIter(rhs);
	for (int rhslen = 0; !rhsIter.done(); rhslen++, rhsIter++);
	return(thislen > rhslen);
}

int
LClause::operator<=(const Clause &rhs) const
{
	ClauseIterator thisIter(*this);
	for (int thislen = 0; !thisIter.done(); thislen++, thisIter++);
	ClauseIterator rhsIter(rhs);
	for (int rhslen = 0; !rhsIter.done(); rhslen++, rhsIter++);
	return(thislen <= rhslen);
}

int
LClause::operator>=(const Clause &rhs) const
{
	ClauseIterator thisIter(*this);
	for (int thislen = 0; !thisIter.done(); thislen++, thisIter++);
	ClauseIterator rhsIter(rhs);
	for (int rhslen = 0; !rhsIter.done(); rhslen++, rhsIter++);
	return(thislen >= rhslen);
}

