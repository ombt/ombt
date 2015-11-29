// class functions for the intermediate representation of clauses

// headers
#include "irclause.h"

// constructors and destructor
IRClause::IRClause():
	partOfConclusion(0), terms()
{
	// nothing to do
}

IRClause::IRClause(const IRClause &src):
	partOfConclusion(src.partOfConclusion), terms(src.terms)
{
	// nothing to do
}

IRClause::IRClause(const List<String> &alist, int poc):
	partOfConclusion(poc), terms(alist)
{
	// nothing to do
}

IRClause::~IRClause()
{
	// nothing to do
}

// assignment operator
IRClause &
IRClause::operator=(const IRClause &rhs)
{
	if (this != &rhs)
	{
		partOfConclusion = rhs.partOfConclusion;
		terms = rhs.terms;
	}
	return(*this);
}

// comparison operators
int
IRClause::operator==(const IRClause &rhs) const
{
	ListIterator<String> ti(terms);
	ListIterator<String> rhsti(rhs.terms);
	for ( ; !ti.done() && !rhsti.done(); ti++, rhsti++)
	{
		if (ti() != rhsti())
			return(0);
	}
	if (ti.done() && rhsti.done())
		return(1);
	else
		return(0);
}

int
IRClause::operator!=(const IRClause &rhs) const
{
	ListIterator<String> ti(terms);
	ListIterator<String> rhsti(rhs.terms);
	for ( ; !ti.done() && !rhsti.done(); ti++, rhsti++)
	{
		if (ti() != rhsti())
			return(1);
	}
	if (ti.done() && rhsti.done())
		return(0);
	else
		return(1);
}

// access functions
void
IRClause::insert(const String &term)
{
	terms.insert(term);
}

void
IRClause::remove(String &term)
{
	terms.remove(term);
}

int
IRClause::isInClause(const String &term) const
{
	return(terms.includes(term));
}

void
IRClause::clear()
{
	terms.clear();
}

int
IRClause::isEmpty() const
{
	return(terms.isEmpty());
}

// access conclusion data
int
IRClause::getPartOfConclusion() const
{
	return(partOfConclusion);
}

void
IRClause::setPartOfConclusion(int poc)
{
	partOfConclusion = poc;
}

// print contents of clauses
ostream &
operator<<(ostream &os, const IRClause &c)
{
	ListIterator<String> cIter(c.terms);
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
	return(os);
}

// constructors and destructor for iterator class for clauses class
IRClauseIterator::IRClauseIterator(const IRClause &clause):
	iterator(clause.terms)
{
	// do nothing
}

IRClauseIterator::IRClauseIterator(const IRClauseIterator &clauseIter):
	iterator(clauseIter.iterator)
{
	// do nothing
}

IRClauseIterator::~IRClauseIterator()
{
	// do nothing
}

// reset iterator to start
void
IRClauseIterator::reset()
{
	iterator.reset();
}

// is iterator done?
int
IRClauseIterator::done() const
{
	return(iterator.done());
}

String
IRClauseIterator::operator()()
{
	return(iterator());
}

int
IRClauseIterator::operator++(int)
{
	return(iterator++);
}

