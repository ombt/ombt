// definitions for best-first search node class

// headers
#include "bfsnode.h"
#include "trace.h"

#ifdef MEMORYLEAK
// static data
MemoryLeak BFSNode::memoryleak(1001, sizeof(BFSNode));
#endif

// constructors and destructor
BFSNode::BFSNode():
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children(),
	resolved(0), 
	lit1(), lit2(), 
	lit1clause(), lit2clause(), 
	factoredlit1clause(), factoredlit2clause(), 
	factoredlit1subs(), factoredlit2subs(), 
	resolventclause(), depth(0),
	substitutions()
{
	// do nothing
}

BFSNode::BFSNode(const Literal &l1, const Literal &l2,
		const Clause &cl1, const Clause &cl2):
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children(),
	resolved(0), 
	lit1(l1), lit2(l2), 
	lit1clause(cl1), lit2clause(cl2), 
	factoredlit1clause(), factoredlit2clause(), 
	factoredlit1subs(), factoredlit2subs(), 
	resolventclause(), depth(0),
	substitutions()
{
	// do nothing
}

BFSNode::BFSNode(const BFSNode &n):
	fvalue(n.fvalue), gvalue(n.gvalue), hvalue(n.hvalue), 
	parent(NULL), children(),
	resolved(n.resolved), 
	lit1(n.lit1), lit2(n.lit2), 
	lit1clause(n.lit1clause), lit2clause(n.lit2clause), 
	factoredlit1clause(n.factoredlit1clause), 
	factoredlit2clause(n.factoredlit2clause), 
	factoredlit1subs(n.factoredlit1subs), 
	factoredlit2subs(n.factoredlit2subs), 
	resolventclause(n.resolventclause), depth(0),
	substitutions(n.substitutions)
{
	// do nothing
}

BFSNode::~BFSNode()
{
	fvalue = INT_MAX;
	gvalue = INT_MAX;
	hvalue = INT_MAX;
	parent = NULL;
	children.clear();
	resolved = 0;
	lit1clause.clear();
	lit2clause.clear();
	factoredlit1subs.clear();
	factoredlit2subs.clear();
	factoredlit1clause.clear();
	factoredlit2clause.clear();
	resolventclause.clear();
	substitutions.clear();
	depth = 0;
}

// comparison operators
int
BFSNode::operator==(const BFSNode &n) const
{
	return(fvalue == n.fvalue);
}

int
BFSNode::operator!=(const BFSNode &n) const
{
	return(fvalue != n.fvalue);
}

int
BFSNode::operator<(const BFSNode &n) const
{
	return(fvalue < n.fvalue);
}

int
BFSNode::operator<=(const BFSNode &n) const
{
	return(fvalue <= n.fvalue);
}

int
BFSNode::operator>(const BFSNode &n) const
{
	return(fvalue > n.fvalue);
}

int
BFSNode::operator>=(const BFSNode &n) const
{
	return(fvalue >= n.fvalue);
}

// assignment operator
BFSNode &
BFSNode::operator=(const BFSNode &n)
{
	if (this != &n)
	{
		// copy search-related data
		fvalue = n.fvalue;
		gvalue = n.gvalue;
		hvalue = n.hvalue;
		parent = NULL;
		children.clear();

		// copy data related to theorem prover
		resolved = n.resolved;
		lit1 = n.lit1;
		lit2 = n.lit2;
		lit1clause = n.lit1clause;
		lit2clause = n.lit2clause;
		factoredlit1subs = n.factoredlit1subs;
		factoredlit2subs = n.factoredlit2subs;
		factoredlit1clause = n.factoredlit1clause;
		factoredlit2clause = n.factoredlit2clause;
		resolventclause = n.resolventclause;
		substitutions = n.substitutions;

		// reference-counting data
MustBeTrue(0);
		depth = 0;
	}
	return(*this);
}

// check if the node is a goal.
int
BFSNode::isGoal(BinaryTree_AVL<Clause> &slist, BinaryTree_AVL<Clause> &olist)
{
	// better not have been resolved
	MustBeTrue(!resolved);

	// clean up 
	substitutions.clear();

	// check if any of the clauses are empty
	statistics[ResolutionsAttempted] += 1;
	totalstatistics[TotalResolutionsAttempted] += 1;
	if (lit1clause.isEmpty() || lit2clause.isEmpty())
	{
		return(VALID);
	}

	// check if clauses can be resolved
	if (!lit1clause.getSOS() && !lit2clause.getSOS())
	{
		// one clause must be in the set-of-support
		return(NOMATCH);
	}
	if ((lit1clause.getType() == Clause::Negative && 
	     lit2clause.getType() == Clause::Negative) ||
	    (lit1clause.getType() == Clause::Positive && 
	     lit2clause.getType() == Clause::Positive))
	{
		// two positive clauses or two negative clauses
		// can not be resolved.
		return(NOMATCH);
	}

	// check if the new clause exceeds any limits
	int newcldepth = INT_MAX;
	if (lit1clause.getDepth() >= lit2clause.getDepth())
		newcldepth = lit1clause.getDepth()+1;
	else
		newcldepth = lit2clause.getDepth()+1;
	if (newcldepth > maxdepth)
	{
		return(MAXDEPTHEXCEEDED);
	}
	if (nextClause > maxclause)
	{
		return(MAXCLAUSEEXCEEDED);
	}
	if ((lit1clause.getTotalMembers() > maxliterals) ||
	    (lit2clause.getTotalMembers() > maxliterals))
		// maximum literals in clause was exceeded.
	{
		statistics[MaximumLiteralsClausesRejected] += 1;
		totalstatistics[TotalMaximumLiteralsClausesRejected] += 1;
		return(NOMATCH);
	}

	// attempt to resolve two clauses. use A-ordering resolution,
	// try to resolve maximal literals.
	//
	Literal maxlit1;
	if (lit1clause.getMaximalLiteral(maxlit1) != OK)
	{
		ERROR("getMaximalLiteral failed.", errno);
		return(NOTOK);
	}
	MustBeTrue(equal(maxlit1, lit1));
	Literal maxlit2;
	if (lit2clause.getMaximalLiteral(maxlit2) != OK)
	{
		ERROR("getMaximalLiteral failed.", errno);
		return(NOTOK);
	}
	MustBeTrue(equal(maxlit2, lit2));
	if (maxlit1.unify_ne(~maxlit2))
	{
		return(NOMATCH);
	}

	// factor clauses
	factoredlit1clause = lit1clause;
	if (factor(maxlit1, factoredlit1clause, factoredlit1subs) == NOTOK)
	{
		ERROR("factor failed.", errno);
		return(NOTOK);
	}
	factoredlit2clause = lit2clause;
	if (factor(maxlit2, factoredlit2clause, factoredlit2subs) == NOTOK)
	{
		ERROR("factor failed.", errno);
		return(NOTOK);
	}

	// attempt to unify the clauses
	Substitutions subs;
	int status = unify(maxlit1, ~maxlit2, subs);
	switch (status)
	{
	case OK:
		if (verbose)
		{
			cout << endl;
			cout << "unifying subs: " << subs << endl;
			Literal ml1(maxlit1);
			cout << "max literal 1 (before subs): " << ml1 << endl;
			subs.applyTo(ml1);
			cout << "max literal 1 (after subs): " << ml1 << endl;
			Literal ml2(maxlit2);
			cout << "max literal 2 (before subs): " << ml2 << endl;
			subs.applyTo(ml2);
			cout << "max literal 2 (after subs): " << ml2 << endl;
			MustBeTrue(equal(ml1, ~ml2));
		}
		break;
	case NOMATCH:
		return(NOMATCH);
	default:
		ERROR("unify failed.", errno);
		return(status);
	}

	// can resolve, remove literals from clauses
	Clause newcl1(factoredlit1clause);
	if (newcl1.remove(maxlit1) != OK)
	{
		ERROR("remove failed.", errno);
		return(NOTOK);
	}
	if (subs.applyTo(newcl1) != OK)
	{
		ERROR("applyTo failed.", errno);
		return(NOTOK);
	}
	Clause newcl2(factoredlit2clause);
	if (newcl2.remove(maxlit2) != OK)
	{
		ERROR("remove failed.", errno);
		return(NOTOK);
	}
	if (subs.applyTo(newcl2) != OK)
	{
		ERROR("applyTo failed.", errno);
		return(NOTOK);
	}

	// store new clause and update flag
	Clause newcl = newcl1+newcl2;
	if (newcl.renameVariables() != OK)
	{
		ERROR("renameVariables failed.", errno);
		return(NOTOK);
	}
	newcl.setDepth(newcldepth);
	newcl.setNumber(nextClause++);
	resolventclause = newcl;
	substitutions = subs;
	resolved = 1;
	currentBFSDepth = newcl.getDepth();

	// indicate the clauses that were resolved
	statistics[ClausesGenerated] += 1;
	totalstatistics[TotalClausesGenerated] += 1;

	// print out clause data
	cout << "((p1,l1), (p2,l2)) = ((";
	cout << lit1clause.getNumber() << "," 
		<< lit1clause.getTotalMembers() << "),(";
	cout << lit2clause.getNumber() << "," << 
		lit2clause.getTotalMembers() << ")) = ";
	cout << resolventclause << endl;

	// dump memory usage
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// check if the clause is empty
	if (resolventclause.isEmpty())
	{
		return(VALID);
	}

	// check if tautology test should be run
	if (tautologytest)
	{
		Clause clausetotest(resolventclause);
		statistics[AttemptedTautologyTests] += 1;
		totalstatistics[TotalAttemptedTautologyTests] += 1;
		if (tautology(clausetotest) == OK)
		{
			statistics[TautologiesRemoved] += 1;
			totalstatistics[TotalTautologiesRemoved] += 1;
			return(OK);
		}
	}

	// check if subsumption test should be run
	if (subsumptiontest)
	{
		BinaryTree_AVL_Iterator_InOrder<Clause> slistIter(slist);
		for ( ; !slistIter.done(); slistIter++)
		{
			if (slistIter().getDepth() >= 
				resolventclause.getDepth())
				continue;
			statistics[AttemptedSubsumptionTests] += 1;
			totalstatistics[TotalAttemptedSubsumptionTests] += 1;
			Clause cl1(slistIter());
			Clause cl2(resolventclause);
			Substitutions s;
			if (subsumes(cl1, cl2, s) == OK)
			{
				statistics[SubsumedClausesRemoved] += 1;
				totalstatistics[TotalSubsumedClausesRemoved] += 1;
				return(OK);
			}
		}
		BinaryTree_AVL_Iterator_InOrder<Clause> olistIter(olist);
		for ( ; !olistIter.done(); olistIter++)
		{
			if (olistIter().getDepth() >= 
				resolventclause.getDepth())
				continue;
			statistics[AttemptedSubsumptionTests] += 1;
			totalstatistics[TotalAttemptedSubsumptionTests] += 1;
			Clause cl1(olistIter());
			Clause cl2(resolventclause);
			Substitutions s;
			if (subsumes(cl1, cl2, s) == OK)
			{
				statistics[SubsumedClausesRemoved] += 1;
				totalstatistics[TotalSubsumedClausesRemoved] += 1;
				return(OK);
			}
		}
	}

	// insert into SOS list
	if (slist.insert(resolventclause) != OK)
	{
		ERROR("insert of resolvent clause failed.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

// return clauses
String
BFSNode::getClauses() const
{
	return(String(lit1clause) + String(" ") + String(lit2clause));
}

String
BFSNode::getNormalizedClauses() const
{
	long icounter = 0;
	Map<String, String> mapvars;
	String news;
	String s = String(lit1clause) + String(" ") + String(lit2clause);
	StringTokens st(s, " \t");
	for ( ;!st.done(); st++)
	{
		if ((st()(0,2) == String("_V")) ||
		    (st()(0,2) == String("_RV")))
		{
			String oldvar = st();
			if (!mapvars.isInMap(oldvar))
			{
				char buf[BUFSIZ];
				sprintf(buf, "_V%d", ++icounter);
				mapvars[oldvar] = String(buf);
			}
			news = news + String(" ") + mapvars[oldvar];
		}
		else
		{
			news = news + String(" ") + st();
		}
	}
	return(news);
}

// accessor functions
int
BFSNode::getFvalue() const
{
	return(fvalue);
}

int
BFSNode::getGvalue() const
{
	return(gvalue);
}

int
BFSNode::getHvalue() const
{
	return(hvalue);
}

int
BFSNode::getDepth() const
{
	return(depth);
}

Proxy<BFSNode>
BFSNode::getParent() const
{
	return(parent);
}

List<Proxy<BFSNode> > *
BFSNode::getChildren()
{
	return(&children);
}

void
BFSNode::setHvalue(int h)
{
	hvalue = h;
	return;
}

void
BFSNode::setFvalue(int f)
{
	fvalue = f;
	return;
}

void
BFSNode::setGvalue(int g)
{
	gvalue = g;
	return;
}

void
BFSNode::setDepth(int d)
{
	depth = d;
	return;
}

void
BFSNode::setParent(Proxy<BFSNode> &pn)
{
	parent = pn;
	return;
}

// calculate the heuristic value for current node
int
BFSNode::calculateGvalue()
{
	if (lit1clause.getDepth() > lit2clause.getDepth())
		gvalue = lit1clause.getDepth();
	else
		gvalue = lit2clause.getDepth();
	gvalue++;
	return(gvalue);
}

int
BFSNode::heuristic(BinaryTree_AVL<Clause> &slist, BinaryTree_AVL<Clause> &olist)
{
	// initialize values
	fvalue = hvalue = gvalue = 0;

	// calculate distance to root
	if (usedepth)
	{
		// length from start node
		int glength = 0;
		int constant = 0;
		if (lit1clause.getDepth() > lit2clause.getDepth())
			glength = lit1clause.getDepth()+1;
		else
			glength = lit2clause.getDepth()+1;
		getOption("gconst1", 1, constant);
		gvalue += constant*glength;
		getOption("gconst2", 0, constant);
		gvalue += constant*glength*glength;
		getOption("gconst3", 0, constant);
		gvalue += constant*glength*glength*glength;
	}

	// calculate the heuristic estimate for this node. use the 
	// number of literals as the estimate.
	//
	if (unitliteral)
	{
		// length of clauses
		int constant = 0;
		int clength = lit1clause.getTotalMembers() + 
				lit2clause.getTotalMembers();
		getOption("uconst1", 1, constant);
		hvalue += constant*clength;
		getOption("uconst2", 0, constant);
		hvalue += constant*clength*clength;
		getOption("uconst3", 0, constant);
		hvalue += constant*clength*clength*clength;
	}

	// calculate heuristic values using complexity info
	//
	if (complexity)
	{
		// complexity data values
		int c1n1, c1n2, c1n3, c1n4, c1n5, c1n6, c1d;
		int c2n1, c2n2, c2n3, c2n4, c2n5, c2n6, c2d;

		// get basic statistics for each parent clause
		lit1clause.basicStats(c1n1, c1n2, c1n3, c1n4, c1n5, c1n6, c1d);
		lit2clause.basicStats(c2n1, c2n2, c2n3, c2n4, c2n5, c2n6, c2d);

		// get multiplication constants for clauses
		int constant = 0;
		getOption("cconst1", 0, constant);
		hvalue += constant*(c1n1+c2n1);
		getOption("cconst2", 0, constant);
		hvalue += constant*(c1n2+c2n2);
		getOption("cconst3", 0, constant);
		hvalue += constant*(c1n3+c2n3);
		getOption("cconst4", 0, constant);
		hvalue += constant*(c1n4+c2n4);
		getOption("cconst5", 0, constant);
		hvalue += constant*(c1n5+c2n5);
		getOption("cconst6", 0, constant);
		hvalue += constant*(c1n6+c2n6);
		getOption("cconst7", 0, constant);
		hvalue += constant*(c1d+c2d);
	}

	// add up to get total heuristic estimate for this goal
	fvalue = weight*gvalue + (10-weight)*hvalue;
	return(OK);
}

// expand current node. this function populates the children list.
int
BFSNode::expand(BinaryTree_AVL<Clause> &slist, BinaryTree_AVL<Clause> &olist)
{
	// check if node was solveable 
	if (!resolved)
		return(OK);

	// get maximal literal in resolvent clause
	Literal maxlit1;
 	if (resolventclause.getMaximalLiteral(maxlit1) != OK)
	{
		ERROR("getMaximalLiteral failed.", errno);
		return(NOTOK);
	}

	// clear children list 
	children.clear();

	// check if clause is to long
	if (resolventclause.getTotalMembers() > maxliterals)
		return(OK);

	// generate new states for this clause from SOS list
	int newcldepth = INT_MAX;
	BinaryTree_AVL_Iterator_InOrder<Clause> sosIter(slist);
	for (sosIter++ ; !sosIter.done(); sosIter++)
	{
		// generate possible resolution states. skip states
		// exceed the maximum clause depth.
		//
		Literal maxlit2;
		if (sosIter().getMaximalLiteral(maxlit2) != OK)
		{
			ERROR("get maximal literal failed.", errno);
			return(NOTOK);
		}
		if (resolventclause.getDepth() >= sosIter().getDepth())
			newcldepth = resolventclause.getDepth()+1;
		else
			newcldepth = sosIter().getDepth()+1;
		if (newcldepth > maxdepth)
			continue;
		if (sosIter().getTotalMembers() > maxliterals)
		{
			statistics[MaximumLiteralsClausesRejected] += 1;
			totalstatistics[TotalMaximumLiteralsClausesRejected] += 1;
			continue;
		}
		if (maxlit1.unify_ne(~maxlit2))
			continue;
		Proxy<BFSNode> pnew = new BFSNode(
			maxlit1, maxlit2, resolventclause, sosIter());
		MustBeTrue((BFSNode *)pnew != NULL);
		// pnew->parent = this;
		MustBeTrue(children.insertAtFront(pnew) == OK);
	}

	// check if resolvent clause has support
	if (!resolventclause.getSOS())
		return(OK);

	// generate new states for this clause from axiom list
	BinaryTree_AVL_Iterator_InOrder<Clause> axiomIter(olist);
	for ( ; !axiomIter.done(); axiomIter++)
	{
		// generate possible resolution states. skip states
		// exceed the maximum clause depth.
		//
		Literal maxlit2;
		if (axiomIter().getMaximalLiteral(maxlit2) != OK)
		{
			ERROR("get maximal literal failed.", errno);
			return(NOTOK);
		}
		if (resolventclause.getDepth() >= axiomIter().getDepth())
			newcldepth = resolventclause.getDepth()+1;
		else
			newcldepth = axiomIter().getDepth()+1;
		if (newcldepth > maxdepth)
			continue;
		if (axiomIter().getTotalMembers() > maxliterals)
		{
			statistics[MaximumLiteralsClausesRejected] += 1;
			totalstatistics[TotalMaximumLiteralsClausesRejected] += 1;
			continue;
		}
		if (maxlit1.unify_ne(~maxlit2))
			continue;
		Proxy<BFSNode> pnew = new BFSNode(
			maxlit1, maxlit2, resolventclause, axiomIter());
		MustBeTrue((BFSNode *)pnew != NULL);
		// pnew->parent = this;
		MustBeTrue(children.insertAtFront(pnew) == OK);
	}

	// all done
	return(OK);
}

// output function
ostream &
operator<<(ostream &os, const BFSNode &n)
{
	os << endl;
	os << "=================================================" << endl;
	os << "f      = " << n.fvalue << endl;
	os << "g      = " << n.gvalue << endl;
	os << "h      = " << n.hvalue << endl;
	os << "=================================================" << endl;
	os << "resolved = " << n.resolved << endl;
	os << "lit1 = " << n.lit1 << endl;
	os << "lit2 = " << n.lit2 << endl;
	os << "lit1clause = " << n.lit1clause << endl;
	os << "factored lit1clause subs = " << n.factoredlit1subs << endl;
	os << "factored lit1clause = " << n.factoredlit1clause << endl;
	os << "lit2clause = " << n.lit2clause << endl;
	os << "factored lit2clause subs = " << n.factoredlit2subs << endl;
	os << "factored lit2clause = " << n.factoredlit2clause << endl;
	os << "substitutions = " << n.substitutions << endl;
	os << "resolventclause  = " << n.resolventclause << endl;
	os << "=================================================" << endl;
	return(os);
}

void
BFSNode::dumpMinData(ostream &os) const
{
	os << "(p1,d1)+(p2,d2)=(r,d): ";
	os << "(" << lit1clause.getNumber() << ",";
	os << lit1clause.getDepth() << ")+";
	os << "(" << lit2clause.getNumber() << ",";
	os << lit2clause.getDepth() << ") = (";
	os << "(" << resolventclause.getNumber() << ",";
	os << resolventclause.getDepth() << ")" << endl;
	return;
}

#ifdef MEMORYLEAK
// memory allocation and deallocation functions
void *
BFSNode::operator new(size_t size)
{
	BFSNode *p = (BFSNode *) ::new char[size];
	MustBeTrue(memoryleak.insert((unsigned long)p, 1) == OK);
	return(p);
}

void
BFSNode::operator delete(void *p)
{
	unsigned long addr = (unsigned long)p;
	int ucnt = 0;
	MustBeTrue(memoryleak.remove(addr, ucnt) == OK);
	::delete p;
}
#endif
