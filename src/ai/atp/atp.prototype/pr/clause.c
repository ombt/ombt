// member functions for clause class

// required headers
#include "clause.h"

// clause constructors and destructor
Clause::Clause():
	depth(0), number(0), inputstring(""),
	type(Unknown), setOfSupport(0), partOfConclusion(0), 
	positiveClause(), negativeClause()
{
	// do nothing
}

Clause::Clause(const Clause &src):
	depth(src.depth), number(src.number), 
	inputstring(src.inputstring),
	type(src.type), setOfSupport(src.setOfSupport),
	partOfConclusion(src.partOfConclusion),
	positiveClause(src.positiveClause), 
	negativeClause(src.negativeClause)
{
	// do nothing
}

Clause::Clause(const List<String> &list):
	depth(0), number(0), inputstring(""),
	type(Unknown), setOfSupport(0), partOfConclusion(0), 
	positiveClause(), negativeClause()
{
	MustBeTrue(parse(list) == OK);
}

Clause::~Clause()
{
	clear();
}

// parse input string
int
Clause::parse(const List<String> &list)
{
	// set defaults values
	clear();

	// check if list is empty
	if (list.isEmpty())
	{
		// nothing to parse
		type = Empty;
		return(OK);
	}

	// list is not empty, get options.
	ListIterator<String> listIter(list);
	String options = listIter();

	// parse options
	StringMatcher conclusionopt(options, String("CONCLUSION"));
	if (!conclusionopt.done())
		partOfConclusion = 1;
	StringMatcher setofsupportopt(options, String("SET-OF-SUPPORT"));
	if (!setofsupportopt.done())
		setOfSupport = 1;
	
	// scan list of literal strings
	for (listIter++; !listIter.done(); listIter++)
	{
		// insert literal into clause; will set type.
		Literal literal(listIter());
		if (insert(literal) != OK)
			return(NOTOK);
	}

	// convert to a string
	convertToString(inputstring);

	// all done
	return(OK);
}

// assignment 
Clause &
Clause::operator=(const Clause &rhs)
{
	// check of self-assignment
	if (this == &rhs)
		return(*this);

	// clear old data
	clear();

	// copy new data
	depth = rhs.depth;
	number = rhs.number;
	inputstring = rhs.inputstring;
	type = rhs.type;
	setOfSupport = rhs.setOfSupport;
	partOfConclusion = rhs.partOfConclusion;
	positiveClause = rhs.positiveClause;
	negativeClause = rhs.negativeClause;

	// all done
	return(*this);
}

Clause &
Clause::operator=(const List<String> &rhs)
{
	MustBeTrue(parse(rhs) == OK);
	return(*this);
}

// update the clause type
void
Clause::updateType()
{
	if (!negativeClause.isEmpty() && !positiveClause.isEmpty())
		type = Mixed;
	else if (!negativeClause.isEmpty() && positiveClause.isEmpty())
		type = Negative;
	else if (negativeClause.isEmpty() && !positiveClause.isEmpty())
		type = Positive;
	else
		type = Empty;
	return;
}

// comparison operators - can only compare up to the equivalent classes.
// with two clauses (negative and positive) to compare, it becomes a 
// little bit tricky.
//
int
Clause::compare(const BinaryTree<Tuple<Literal, List<Literal> > > &cl1,
	const BinaryTree<Tuple<Literal, List<Literal> > > &cl2) const
{
	// are they the same?
	if (&cl1 == &cl2)
		return(0);

	// compare clauses
	BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > > 
		cl1Iter(cl1), cl2Iter(cl2);
	for ( ; !cl1Iter.done() && !cl2Iter.done(); cl1Iter++, cl2Iter++)
	{
		if (cl1Iter().key < cl2Iter().key)
			return(-1);
		else if (cl1Iter().key > cl2Iter().key)
			return(1);
	}
	if (cl1Iter.done() && !cl2Iter.done())
		return(-1);
	else if (!cl1Iter.done() && cl2Iter.done())
		return(1);
	else 
		return(0);
}

int
Clause::operator==(const Clause &rhs) const
{
	// check of clauses are the same
	if (compare(positiveClause, rhs.positiveClause) != 0)
		return(0);
	else if (compare(negativeClause, rhs.negativeClause) != 0)
		return(0);
	else
		return(1);
}

int
Clause::operator!=(const Clause &rhs) const
{
	// check of clauses are not the same
	if (compare(positiveClause, rhs.positiveClause) != 0)
		return(1);
	else if (compare(negativeClause, rhs.negativeClause) != 0)
		return(1);
	else
		return(0);
}

int
Clause::operator<(const Clause &rhs) const
{
	// compare positive clause
	int status = compare(positiveClause, rhs.positiveClause);
	if (status < 0)
		return(1);
	else if (status > 0)
		return(0);

	// compare negative clause
	status = compare(negativeClause, rhs.negativeClause);
	if (status < 0)
		return(1);
	else
		return(0);
}

int
Clause::operator>(const Clause &rhs) const
{
	// compare positive clause
	int status = compare(positiveClause, rhs.positiveClause);
	if (status > 0)
		return(1);
	else if (status < 0)
		return(0);

	// compare negative clause
	status = compare(negativeClause, rhs.negativeClause);
	if (status > 0)
		return(1);
	else
		return(0);
}

int
Clause::operator<=(const Clause &rhs) const
{
	// compare positive clause
	int status = compare(positiveClause, rhs.positiveClause);
	if (status < 0)
		return(1);
	else if (status > 0)
		return(0);

	// compare negative clause
	status = compare(negativeClause, rhs.negativeClause);
	if (status <= 0)
		return(1);
	else
		return(0);
}

int
Clause::operator>=(const Clause &rhs) const
{
	// compare positive clause
	int status = compare(positiveClause, rhs.positiveClause);
	if (status > 0)
		return(1);
	else if (status < 0)
		return(0);

	// compare negative clause
	status = compare(negativeClause, rhs.negativeClause);
	if (status >= 0)
		return(1);
	else
		return(0);
}

// access operations
int
Clause::insert(const Literal &key)
{
	// check if the key is negated
	if (key.isNegated())
		return(insert(key, negativeClause));
	else
		return(insert(key, positiveClause));
}

int
Clause::retrieve(Literal &key, int idx) const
{
	// check if the key is negated
	if (key.isNegated())
		return(retrieve(key, idx, negativeClause));
	else
		return(retrieve(key, idx, positiveClause));
}

int
Clause::update(const Literal &key, int idx)
{
	// check if the key is negated
	if (key.isNegated())
		return(update(key, idx, negativeClause));
	else
		return(update(key, idx, positiveClause));
}

int
Clause::remove(Literal &key, int idx)
{
	// check if the key is negated
	if (key.isNegated())
		return(remove(key, idx, negativeClause));
	else
		return(remove(key, idx, positiveClause));
}

int
Clause::insert(const Literal &key,
	BinaryTree<Tuple<Literal, List<Literal> > > &clause)
{
	// check if a class already exists
	Tuple<Literal, List<Literal> > tuple(key, List<Literal>());
	int status = clause.retrieve(tuple);
	if (status == OK)
	{
		if ((status = tuple.data.insertAtEnd(key)) != OK)
			return(status);
		if ((status = clause.update(tuple)) != OK)
			return(status);
	}
	else if (status == NOMATCH)
	{
		if ((status = tuple.data.insertAtEnd(key)) != OK)
			return(status);
		if ((status = clause.insert(tuple)) != OK)
			return(status);
	}
	else
		return(status);

	// update the type
	updateType();

	// all done
	return(OK);
}

int
Clause::retrieve(Literal &key, int idx,
	const BinaryTree<Tuple<Literal, List<Literal> > > &clause) const
{
	Tuple<Literal, List<Literal> > tuple(key, List<Literal>());
	int status = clause.retrieve(tuple);
	if (status != OK)
		return(status);
	else
		return(tuple.data.retrieveNth(idx, key));
}

int
Clause::update(const Literal &key, int idx,
	BinaryTree<Tuple<Literal, List<Literal> > > &clause)
{
	Tuple<Literal, List<Literal> > tuple(key, List<Literal>());
	int status = clause.retrieve(tuple);
	if (status != OK)
		return(status);
	status = tuple.data.updateNth(idx, key);
	if (status != OK)
		return(status);
	return(clause.update(tuple));
}

int
Clause::remove(Literal &key, int idx,
	BinaryTree<Tuple<Literal, List<Literal> > > &clause)
{
	// delete literal from the clause
	Tuple<Literal, List<Literal> > tuple(key, List<Literal>());
	int status = clause.retrieve(tuple);
	if (status != OK)
		return(status);
	status = tuple.data.removeNth(idx, key);
	if (status != OK)
		return(status);
	if (tuple.data.isEmpty())
	{
		// remove class, we just removed the last member
		status = clause.remove(tuple);
		if (status != OK)
			return(status);
	}

	// update the type
	updateType();

	// all done
	return(OK);
}

int
Clause::insertEqSet(const Literal &key, const List<Literal> &data)
{
	int status;

	// insert tuple
	Tuple<Literal, List<Literal> > tuple(key, data);
	if (key.isNegated())
	{
		if ((status = negativeClause.insert(tuple)) != OK)
			return(status);
	}
	else
	{
		if ((status = positiveClause.insert(tuple)) != OK)
			return(status);
	}

	// update the type
	updateType();

	// all done
	return(OK);
}

int
Clause::retrieveEqSet(const Literal &key, List<Literal> &data) const
{
	int status;
	Tuple<Literal, List<Literal> > tuple(key, List<Literal>());
	if (key.isNegated())
		status = negativeClause.retrieve(tuple);
	else
		status = positiveClause.retrieve(tuple);
	if (status == OK)
		data = tuple.data;
	return(status);
}

int
Clause::updateEqSet(const Literal &key, const List<Literal> &data)
{
	Tuple<Literal, List<Literal> > tuple(key, data);
	if (key.isNegated())
		return(negativeClause.update(tuple));
	else
		return(positiveClause.update(tuple));
}

int
Clause::removeEqSet(Literal &key, List<Literal> &data)
{
	int status;
	Tuple<Literal, List<Literal> > tuple(key, List<Literal>());
	if (key.isNegated())
		status = negativeClause.remove(tuple);
	else
		status = positiveClause.remove(tuple);
	if (status == OK)
	{
		updateType();
		data = tuple.data;
	}
	return(status);
}

int
Clause::isInClause(const Literal &key) const
{
	Tuple<Literal, List<Literal> > tuple(key, List<Literal>());
	if (key.isNegated())
		return(negativeClause.retrieve(tuple) == OK);
	else
		return(positiveClause.retrieve(tuple) == OK);
}

int 
Clause::isEmpty() const
{
	return(negativeClause.isEmpty() && positiveClause.isEmpty());
}

// return the number of classes and total members
int
Clause::getTotalMembers() const
{
	int memberno = 0;
	for (ClauseIterator thisIter(*this); !thisIter.done(); thisIter++)
	{
		memberno += thisIter().data.getCount();
	}
	return(memberno);
}

int
Clause::getTotalSets() const
{
	int setno = 0;
	for (ClauseIterator thisIter(*this); !thisIter.done(); thisIter++)
	{
		setno += 1;
	}
	return(setno);
}

int
Clause::getMembersInSet(const Literal &key) const
{
	int status;
	Tuple<Literal, List<Literal> > tuple(key, List<Literal>());
	if (key.isNegated())
		status = negativeClause.retrieve(tuple);
	else
		status = positiveClause.retrieve(tuple);
	if (status == OK)
		return(tuple.data.getCount());
	else
		return(0);
}

// remove all entries in the clause
void
Clause::clear()
{
	// clear types
	depth = number = 0;
	type = Unknown;
	inputstring = String("");
	setOfSupport = 0;
	partOfConclusion = 0;

	// now delete tree nodes
	positiveClause.clear();
	negativeClause.clear();

	// all done
	return;
}

// rename variables in clause
int
Clause::renameVariables()
{
	Clause newcl;
	Map<String, String> nvs;

	// rename variables in positive clause
	BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
		pclIter(positiveClause);
	for ( ; !pclIter.done(); pclIter++)
	{
		ListIterator<Literal> dataIter(pclIter().data);
		for ( ; !dataIter.done(); dataIter++)
		{
			Literal literal(dataIter());
			if (literal.renameVariables(nvs) != OK)
				return(NOTOK);
			if (newcl.insert(literal) != OK)
				return(NOTOK);
		}
	}

	// rename variables in negative clause
	BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
		nclIter(negativeClause);
	for ( ; !nclIter.done(); nclIter++)
	{
		ListIterator<Literal> dataIter(nclIter().data);
		for ( ; !dataIter.done(); dataIter++)
		{
			Literal literal(dataIter());
			if (literal.renameVariables(nvs) != OK)
				return(NOTOK);
			if (newcl.insert(literal) != OK)
				return(NOTOK);
		}
	}

	// overwrite existing clauses
	*this = newcl;

	// all done
	return(OK);
}

// convert term to a string
String
Clause::convertToString(String &s) const
{
	// clear string
	s = String("");

	// convert literals to strings
	BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
		pclIter(positiveClause);
	for ( ; !pclIter.done(); pclIter++)
	{
		ListIterator<Literal> dataIter(pclIter().data);
		for (int first=1; !dataIter.done(); dataIter++)
		{
			if (!first)
				s += String("|| ");
			else 
				first = 0;
			s += (String)dataIter() + String(" ");
		}
	}
	BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
		nclIter(negativeClause);
	for ( ; !nclIter.done(); nclIter++)
	{
		ListIterator<Literal> dataIter(nclIter().data);
		for (int first=1; !dataIter.done(); dataIter++)
		{
			if (!first)
				s += String("|| ");
			else
				first = 0;
			s += (String)dataIter() + String(" ");
		}
	}

	// return the new string
	return(s);
}

String
Clause::operator String() const
{
	String tmp;
	return(convertToString(tmp));
}

String
Clause::operator String()
{
	return(convertToString(inputstring));
}

// get maximal set or literal for a clause
int
Clause::getMaximalSet(Literal &key, List<Literal> &data) const
{
	// check if there any literals
	if (!positiveClause.isEmpty())
	{
		// return first equivalent set
		BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
			pclIter(positiveClause);
		key = pclIter().key;
		data = pclIter().data;
	}
	else if (!negativeClause.isEmpty())
	{
		// return first equivalent set
		BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
			nclIter(negativeClause);
		key = nclIter().key;
		data = nclIter().data;
	}
	else
		return(NOMATCH);

	// all done
	return(OK);
}

int
Clause::getMaximalPositiveSet(Literal &key, List<Literal> &data) const
{
	// check if there any literals
	if (!positiveClause.isEmpty())
	{
		// return first equivalent set
		BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
			pclIter(positiveClause);
		key = pclIter().key;
		data = pclIter().data;
		return(OK);
	}
	else
		return(NOMATCH);
}

int
Clause::getMaximalNegativeSet(Literal &key, List<Literal> &data) const
{
	// check if there any literals
	if (!negativeClause.isEmpty())
	{
		// return first equivalent set
		BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
			nclIter(negativeClause);
		key = nclIter().key;
		data = nclIter().data;
		return(OK);
	}
	else
		return(NOMATCH);
}

int
Clause::getMaximalLiteral(Literal &data) const
{
	// check if there any literals
	if (!positiveClause.isEmpty())
	{
		// return first equivalent set
		BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
			pclIter(positiveClause);
		if (pclIter().data.retrieveAtFront(data) != OK)
			return(NOTOK);
	}
	else if (!negativeClause.isEmpty())
	{
		// return first equivalent set
		BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
			nclIter(negativeClause);
		if (nclIter().data.retrieveAtFront(data) != OK)
			return(NOTOK);
	}
	else
		return(NOMATCH);

	// all done
	return(OK);
}

int
Clause::getMaximalPositiveLiteral(Literal &data) const
{
	// check if there any literals
	if (!positiveClause.isEmpty())
	{
		// return first equivalent set
		BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
			pclIter(positiveClause);
		if (pclIter().data.retrieveAtFront(data) != OK)
			return(NOTOK);
		return(OK);
	}
	else
		return(NOMATCH);
}

int
Clause::getMaximalNegativeLiteral(Literal &data) const
{
	// check if there any literals
	if (!negativeClause.isEmpty())
	{
		// return first equivalent set
		BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > >
			nclIter(negativeClause);
		if (nclIter().data.retrieveAtFront(data) != OK)
			return(NOTOK);
		return(OK);
	}
	else
		return(NOMATCH);
}

// combine two clauses
Clause
operator+(const Clause &cl1, const Clause &cl2)
{
	Clause newcl(cl1);
	ClauseIterator cl2Iter(cl2);
	for ( ; !cl2Iter.done(); cl2Iter++)
	{
		ListIterator<Literal> dataIter(cl2Iter.data());
		for ( ; !dataIter.done(); dataIter++)
		{
			MustBeTrue(newcl.insert(dataIter()) == OK);
		}
	}
DUMP(newcl);
	newcl.updateType();
DUMP(newcl);
	return(newcl);
}

// print clause literals
ostream &
operator<<(ostream &os, const Clause &c)
{
	os << "Type: ";
	switch (c.type)
	{
	case Clause::Positive:
		os << "Positive";
		break;
	case Clause::Negative:
		os << "Negative";
		break;
	case Clause::Mixed:
		os << "Mixed";
		break;
	case Clause::Unknown:
		os << "Unknown";
		break;
	case Clause::Empty:
		os << "Empty";
		break;
	default:
		MustBeTrue(0);
		break;

	}
	os << endl;
	os << "Clause Depth: " << c.depth << endl;
	os << "Clause Number: " << c.number << endl;
	if (c.setOfSupport)
		os << "Set-of-Support: Yes" << endl;
	else
		os << "Set-of-Support: No" << endl;
	if (c.partOfConclusion)
		os << "Part-of-Conclusion: Yes" << endl;
	else
		os << "Part-of-Conclusion: No" << endl;

	// os << "Clause: " << c.clause << endl;
	ClauseIterator cIter(c);
	for ( ; !cIter.done(); cIter++)
	{
		Literal key = cIter.key();
		List<Literal> data = cIter.data();
		os << "equivalence class key literal: " << key << endl;
		ListIterator<Literal> dataIter(data);
		for ( ; !dataIter.done(); dataIter++)
		{
			os << "equivalence class data literal: " << dataIter() << endl;
		}
	}
	return(os);
}

// ordered set iterator constructors and destructor
ClauseIterator::ClauseIterator(const ClauseIterator &src): 
		pclIter(src.pclIter), nclIter(src.nclIter)
{
	// do nothing
}

ClauseIterator::ClauseIterator(const Clause &c): 
		pclIter(c.positiveClause), nclIter(c.negativeClause)
{
	// do nothing
}

ClauseIterator::~ClauseIterator()
{
	// do nothing
}

// reset iterator to beginning
void
ClauseIterator::reset()
{
	pclIter.reset();
	nclIter.reset();
	return;
}

// is list empty
int
ClauseIterator::done() const
{
	return(pclIter.done() && nclIter.done());
}

// return current data
Tuple<Literal, List<Literal> >
ClauseIterator::operator()()
{
	if (!pclIter.done())
		return(pclIter());
	else if (!nclIter.done())
		return(nclIter());
	else
		MustBeTrue(0);
}

Literal
ClauseIterator::key()
{
	if (!pclIter.done())
		return(pclIter().key);
	else if (!nclIter.done())
		return(nclIter().key);
	else
		MustBeTrue(0);
}

List<Literal>
ClauseIterator::data()
{
	if (!pclIter.done())
		return(pclIter().data);
	else if (!nclIter.done())
		return(nclIter().data);
	else
		MustBeTrue(0);
}

// increment to next data item in list
int
ClauseIterator::operator++(int)
{
	if (!pclIter.done())
		return(pclIter++);
	else if (!nclIter.done())
		return(nclIter++);
	else
		MustBeTrue(0);
}

// clause iterator constructors and destructor
PositiveClauseIterator::PositiveClauseIterator(
		const PositiveClauseIterator &src): 
		clauseIter(src.clauseIter)
{
	// do nothing
}

PositiveClauseIterator::PositiveClauseIterator(const Clause &c): 
		clauseIter(c.positiveClause)
{
	// do nothing
}

PositiveClauseIterator::~PositiveClauseIterator()
{
	// do nothing
}

// reset iterator to beginning
void
PositiveClauseIterator::reset()
{
	clauseIter.reset();
	return;
}

// is list empty
int
PositiveClauseIterator::done() const
{
	return(clauseIter.done());
}

// return current data
Tuple<Literal, List<Literal> >
PositiveClauseIterator::operator()()
{
	return(clauseIter());
}

Literal
PositiveClauseIterator::key()
{
	return(clauseIter().key);
}

List<Literal>
PositiveClauseIterator::data()
{
	return(clauseIter().data);
}

// increment to next data item in list
int
PositiveClauseIterator::operator++(int)
{
	return(clauseIter++);
}

// clause iterator constructors and destructor
NegativeClauseIterator::NegativeClauseIterator(
		const NegativeClauseIterator &src): 
		clauseIter(src.clauseIter)
{
	// do nothing
}

NegativeClauseIterator::NegativeClauseIterator(const Clause &c): 
		clauseIter(c.negativeClause)
{
	// do nothing
}

NegativeClauseIterator::~NegativeClauseIterator()
{
	// do nothing
}

// reset iterator to beginning
void
NegativeClauseIterator::reset()
{
	clauseIter.reset();
	return;
}

// is list empty
int
NegativeClauseIterator::done() const
{
	return(clauseIter.done());
}

// return current data
Tuple<Literal, List<Literal> >
NegativeClauseIterator::operator()()
{
	return(clauseIter());
}

Literal
NegativeClauseIterator::key()
{
	return(clauseIter().key);
}

List<Literal>
NegativeClauseIterator::data()
{
	return(clauseIter().data);
}

// increment to next data item in list
int
NegativeClauseIterator::operator++(int)
{
	return(clauseIter++);
}

