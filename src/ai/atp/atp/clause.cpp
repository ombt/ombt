// member functions for clause class

// required headers
#include "clause.h"

// static data
#ifdef MEMORYLEAK
MemoryLeak Clause::memoryleak(1001, sizeof(Clause));
#endif

// clause constructors and destructor
Clause::Clause():
	depth(0), number(0), inputstring(""), type(Unknown), 
	setOfSupport(0), partOfConclusion(0), partOfQuery(0),
	answers(), positiveClause(), negativeClause()
{
	// do nothing
}

Clause::Clause(const Clause &src):
	depth(src.depth), number(src.number), 
	inputstring(src.inputstring),
	type(src.type), setOfSupport(src.setOfSupport),
	partOfConclusion(src.partOfConclusion),
	partOfQuery(src.partOfQuery),
	answers(src.answers), 
	positiveClause(src.positiveClause), 
	negativeClause(src.negativeClause)
{
	// do nothing
}

Clause::Clause(const List<String> &list):
	depth(0), number(0), inputstring(""), type(Unknown), 
	setOfSupport(0), partOfConclusion(0), partOfQuery(0),
	answers(), positiveClause(), negativeClause()
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
	StringMatcher queryopt(options, String("QUERY"));
	if (!queryopt.done())
		partOfQuery = 1;
	StringMatcher setofsupportopt(options, String("SET-OF-SUPPORT"));
	if (!setofsupportopt.done())
		setOfSupport = 1;
	
	// scan list of literal strings
	for (listIter++; !listIter.done(); listIter++)
	{
		// insert literal into clause; will set type.
		Literal literal(listIter());
		if (insert(literal) != OK)
		{
			ERROR("insert failed.", errno);
			return(NOTOK);
		}
	}

	// convert to a string
	convertToString(inputstring);

	// add answer literal 
	if (partOfQuery)
	{
		int ivar = 0;
		String answerstring("( ANSWER ");
		StringTokens st(inputstring, " \t");
		for ( ; !st.done(); st++)
		{
			String token = st();
			if (token(0,2) == String("_V"))
			{
				ivar++;
				answerstring += token + String(" ");
			}
			else if (token(0,3) == String("_RV"))
			{
				ivar++;
				answerstring += token + String(" ");
			}
		}
		if (ivar == 0)
		{
			// no variables were found
			answerstring = String("ANSWER");
		}
		else
		{
			// finish answer literal
			answerstring += String(") ");
		}

		// insert answer literal into answer clause
		Literal answerliteral(answerstring);
		if (insertAnswer(answerliteral) != OK)
		{
			ERROR("insertAnswer failed.", errno);
			return(NOTOK);
		}
	}

	// update clause type
	updateType();

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
	partOfQuery = rhs.partOfQuery;
	answers = rhs.answers;
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

// comparison operators, with two clauses (negative and positive) to 
// compare, it becomes a little bit tricky.
//
int
Clause::compare(const BinaryTree_AVL<Literal> &cl1, 
	const BinaryTree_AVL<Literal> &cl2) const
{
	// are they the same?
	if (&cl1 == &cl2)
		return(0);

	// compare clauses
	BinaryTree_AVL_Iterator_InOrder<Literal> cl1Iter(cl1), cl2Iter(cl2);
	for ( ; !cl1Iter.done() && !cl2Iter.done(); cl1Iter++, cl2Iter++)
	{
		if (cl1Iter() < cl2Iter())
			return(-1);
		else if (cl1Iter() > cl2Iter())
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
	int status;
	if (key.isNegated())
		status = negativeClause.insert(key);
	else
		status = positiveClause.insert(key);
	if (status == OK)
		updateType();
	return(status);
}

int
Clause::retrieve(Literal &key) const
{
	// check if the key is negated
	if (key.isNegated())
		return(negativeClause.retrieve(key));
	else
		return(positiveClause.retrieve(key));
}

int
Clause::update(const Literal &key)
{
	// check if the key is negated
	int status;
	if (key.isNegated())
		status = negativeClause.update(key);
	else
		status = positiveClause.update(key);
	if (status == OK)
		updateType();
	return(status);
}

int
Clause::remove(Literal &key)
{
	// check if the key is negated
	int status;
	if (key.isNegated())
		status = negativeClause.remove(key);
	else
		status = positiveClause.remove(key);
	if (status == OK)
		updateType();
	return(status);
}

int
Clause::insertAnswer(const Literal &key)
{
	ListIterator<Literal> ansIter(answers);
	for ( ; !ansIter.done(); ansIter++)
	{
		if (equal(ansIter(), key))
			return(OK);
	}
	return(answers.insertAtEnd(key));
}

int
Clause::isInClause(const Literal &key) const
{
	if (key.isNegated())
		return(negativeClause.retrieve(Literal(key)) == OK);
	else
		return(positiveClause.retrieve(Literal(key)) == OK);
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
		memberno += 1;
	}
	return(memberno);
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
	partOfQuery = 0;

	// now delete tree nodes
	answers.clear();
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
	BinaryTree_AVL_Iterator_InOrder<Literal> pclIter(positiveClause);
	for ( ; !pclIter.done(); pclIter++)
	{
		Literal literal(pclIter());
		if (literal.renameVariables(nvs) != OK)
		{
			ERROR("renameVariables failed.", errno);
			return(NOTOK);
		}
		if (newcl.insert(literal) != OK)
		{
			ERROR("insert failed.", errno);
			return(NOTOK);
		}
	}

	// rename variables in negative clause
	BinaryTree_AVL_Iterator_InOrder<Literal> nclIter(negativeClause);
	for ( ; !nclIter.done(); nclIter++)
	{
		Literal literal(nclIter());
		if (literal.renameVariables(nvs) != OK)
		{
			ERROR("renameVariables failed.", errno);
			return(NOTOK);
		}
		if (newcl.insert(literal) != OK)
		{
			ERROR("insert failed.", errno);
			return(NOTOK);
		}
	}

	// rename variables in answers clause
	ListIterator<Literal> ansIter(answers);
	for ( ; !ansIter.done(); ansIter++)
	{
		Literal literal(ansIter());
		if (literal.renameVariables(nvs) != OK)
		{
			ERROR("renameVariables failed.", errno);
			return(NOTOK);
		}
		if (newcl.insertAnswer(literal) != OK)
		{
			ERROR("insert failed.", errno);
			return(NOTOK);
		}
	}

#if 0
	// update table of renamed variables
	if (updateVariableNames(nvs) != OK)
		return(NOTOK);
#endif

	// overwrite existing clauses
	newcl.setDepth(getDepth());
	newcl.setNumber(getNumber());
	newcl.setConclusion(getConclusion());
	newcl.setQuery(getQuery());
	newcl.setSOS(getSOS());
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
	BinaryTree_AVL_Iterator_InOrder<Literal> pclIter(positiveClause);
	for (int first=1; !pclIter.done(); pclIter++)
	{
		if (!first)
			s += String("|| ");
		else 
			first = 0;
		s += (String)pclIter() + String(" ");
	}
	BinaryTree_AVL_Iterator_InOrder<Literal> nclIter(negativeClause);
#ifdef SC42
	for (first=1 ; !nclIter.done(); nclIter++)
#else
	for (int first=1 ; !nclIter.done(); nclIter++)
#endif
	{
		if (!first)
			s += String("|| ");
		else
			first = 0;
		s += (String)nclIter() + String(" ");
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

// get maximal literal for a clause
int
Clause::getMaximalLiteral(Literal &data) const
{
	// check if there any literals
	if (!positiveClause.isEmpty())
	{
		// return first item
		BinaryTree_AVL_Iterator_InOrder<Literal> pclIter(positiveClause);
		data = pclIter();
	}
	else if (!negativeClause.isEmpty())
	{
		// return first item
		BinaryTree_AVL_Iterator_InOrder<Literal> nclIter(negativeClause);
		data = nclIter();
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
		// return first item
		BinaryTree_AVL_Iterator_InOrder<Literal> pclIter(positiveClause);
		data = pclIter();
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
		// return first item
		BinaryTree_AVL_Iterator_InOrder<Literal> nclIter(negativeClause);
		data = nclIter();
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
		Literal cl2lit(cl2Iter());
		cl2lit.setIndex(::nextIndex());
		MustBeTrue(newcl.insert(cl2lit) == OK);
	}
	ClauseAnswersIterator cl2ansIter(cl2);
	for ( ; !cl2ansIter.done(); cl2ansIter++)
	{
		Literal cl2anslit(cl2ansIter());
		cl2anslit.setIndex(::nextIndex());
		MustBeTrue(newcl.insertAnswer(cl2anslit) == OK);
	}
	if (cl1.getSOS() || cl2.getSOS())
		newcl.setSOS(1);
	if (cl1.getConclusion() || cl2.getConclusion())
		newcl.setConclusion(1);
	if (cl1.getQuery() || cl2.getQuery())
		newcl.setQuery(1);
	newcl.updateType();
	return(newcl);
}

// print clause literals
ostream &
operator<<(ostream &os, const Clause &c)
{
	os << "(T,CL,CD,CN,SOS,POC,POQ)=(";
	switch (c.type)
	{
	case Clause::Positive:
		os << "Pos,";
		break;
	case Clause::Negative:
		os << "Neg,";
		break;
	case Clause::Mixed:
		os << "Mix,";
		break;
	case Clause::Unknown:
		os << "Unk,";
		break;
	case Clause::Empty:
		os << "Empty,";
		break;
	default:
		MustBeTrue(0);
		break;

	}
	os << c.getTotalMembers() << ",";
	os << c.depth << ",";
	os << c.number << ",";
	if (c.setOfSupport)
		os << "T,";
	else
		os << "F,";
	if (c.partOfConclusion)
		os << "T,";
	else
		os << "F,";
	if (c.partOfQuery)
		os << "T): ";
	else
		os << "F): ";

	int first = 1;
	ClauseIterator clIter(c);
	for ( ; !clIter.done(); clIter++)
	{
		if (!first)
			os << "|| ";;
		os << clIter();
		first = 0;
	}
	ClauseAnswersIterator clansIter(c);
	for ( ; !clansIter.done(); clansIter++)
	{
		if (!first)
			os << "|| ";;
		os << clansIter();
		first = 0;
	}
	return(os);
}

// get basic statistics for the clause
void
Clause::basicStats(int &npconsts, int &npeqs, int &npfuncs,
		int &nconsts, int &nvars, int &nfuncs, int &ndepth) const
{
	// initialize the variables
	npconsts = npeqs = npfuncs = 0;
	nconsts = nvars = nfuncs = 0;
	ndepth = 0;

	// get basic stats
	int savedepth = 0;
	BinaryTree_AVL_Iterator_InOrder<Literal> pclIter(positiveClause);
	for ( ; !pclIter.done(); pclIter++)
	{
		int termdepth = 0;
		pclIter().basicStats(npconsts, npeqs, npfuncs,
				nconsts, nvars, nfuncs, termdepth);
		if (termdepth > savedepth)
			savedepth = termdepth;
	}
	BinaryTree_AVL_Iterator_InOrder<Literal> nclIter(negativeClause);
	for ( ; !nclIter.done(); nclIter++)
	{
		int termdepth = 0;
		nclIter().basicStats(npconsts, npeqs, npfuncs,
				nconsts, nvars, nfuncs, termdepth);
		if (termdepth > savedepth)
			savedepth = termdepth;
	}
	ndepth = savedepth;

	// all done
	return;
}

#ifdef MEMORYLEAK
// memory allocation and deallocation functions
void *
Clause::operator new(size_t size)
{
	Clause *p = (Clause *) ::new char[size];
	MustBeTrue(memoryleak.insert((unsigned long)p, 1) == OK);
	return(p);
}

void
Clause::operator delete(void *p)
{
	unsigned long addr = (unsigned long)p;
	int ucnt = 0;
	MustBeTrue(memoryleak.remove(addr, ucnt) == OK);
	::delete p;
}
#endif

// clause iterator constructors and destructor
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
Literal
ClauseIterator::operator()()
{
	if (!pclIter.done())
		return(pclIter());
	else if (!nclIter.done())
		return(nclIter());
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
Literal
PositiveClauseIterator::operator()()
{
	return(clauseIter());
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
Literal
NegativeClauseIterator::operator()()
{
	return(clauseIter());
}

// increment to next data item in list
int
NegativeClauseIterator::operator++(int)
{
	return(clauseIter++);
}

// clause answers iterator constructors and destructor
ClauseAnswersIterator::ClauseAnswersIterator(const ClauseAnswersIterator &src): 
		ansIter(src.ansIter)
{
	// do nothing
}

ClauseAnswersIterator::ClauseAnswersIterator(const Clause &c): 
		ansIter(c.answers)
{
	// do nothing
}

ClauseAnswersIterator::~ClauseAnswersIterator()
{
	// do nothing
}

// reset iterator to beginning
void
ClauseAnswersIterator::reset()
{
	ansIter.reset();
	return;
}

// is list empty
int
ClauseAnswersIterator::done() const
{
	return(ansIter.done());
}

// return current data
Literal
ClauseAnswersIterator::operator()()
{
	return(ansIter());
}

// increment to next data item in list
int
ClauseAnswersIterator::operator++(int)
{
	return(ansIter++);
}

