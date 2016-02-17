// definitions for missionary-cannibal problem class

// headers
#include "misscann.h"

// constructors and destructor
NMissCannNode::NMissCannNode():
	RefCount(),
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children(),
	maxNumber(3),
	LHSmissionaries(3), LHScannibals(3),
	RHSmissionaries(0), RHScannibals(0),
	BOATside('L')
{
	MustBeTrue((maxNumber = (LHSmissionaries+RHSmissionaries)) == 
				(LHScannibals+RHScannibals));
	MustBeTrue(maxNumber > 0);
	MustBeTrue(BOATside == 'L' || BOATside == 'R');
}

NMissCannNode::NMissCannNode(
	int lhsnm, int lhsnc, int rhsnm, int rhsnc, char bside):
	RefCount(),
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children(),
	maxNumber(0),
	LHSmissionaries(lhsnm), LHScannibals(lhsnc),
	RHSmissionaries(rhsnm), RHScannibals(rhsnc),
	BOATside(bside)
{
	MustBeTrue((maxNumber = (LHSmissionaries+RHSmissionaries)) == 
				(LHScannibals+RHScannibals));
	MustBeTrue(maxNumber > 0);
	MustBeTrue(BOATside == 'L' || BOATside == 'R');
}

NMissCannNode::NMissCannNode(const NMissCannNode &n):
	RefCount(),
	fvalue(n.fvalue), gvalue(n.gvalue), hvalue(n.hvalue), 
	parent(NULL), children(),
	maxNumber(n.maxNumber),
	LHSmissionaries(n.LHSmissionaries), LHScannibals(n.LHScannibals),
	RHSmissionaries(n.RHSmissionaries), RHScannibals(n.RHScannibals),
	BOATside(n.BOATside)
{
	MustBeTrue((maxNumber = (LHSmissionaries+RHSmissionaries)) == 
				(LHScannibals+RHScannibals));
	MustBeTrue(maxNumber > 0);
	MustBeTrue(BOATside == 'L' || BOATside == 'R');
}

NMissCannNode::~NMissCannNode()
{
	fvalue = INT_MAX;
	gvalue = INT_MAX;
	hvalue = INT_MAX;
	parent = NULL;
	children.clear();
	maxNumber = 0;
	LHSmissionaries = LHScannibals = 0;
	RHSmissionaries = RHScannibals = 0;
	BOATside = 0;
}

// comparison operators
int
NMissCannNode::operator<(const NMissCannNode &n) const
{
	return(fvalue < n.fvalue);
}

int
NMissCannNode::operator>(const NMissCannNode &n) const
{
	return(fvalue > n.fvalue);
}

int
NMissCannNode::operator==(const NMissCannNode &n) const
{
	return((maxNumber == n.maxNumber) &&
		(LHSmissionaries == n.LHSmissionaries) &&
		(RHSmissionaries == n.RHSmissionaries) &&
		(LHScannibals == n.LHScannibals) &&
		(RHScannibals == n.RHScannibals) &&
		(BOATside == n.BOATside));
}

int
NMissCannNode::operator!=(const NMissCannNode &n) const
{
	return(!(operator==(n)));
}

// assignment operator
NMissCannNode &
NMissCannNode::operator=(const NMissCannNode &n)
{
	if (this != &n)
	{
		// copy reference-counting data
		RefCount::operator=(n);

		// copy search-related data
		fvalue = n.fvalue;
		gvalue = n.gvalue;
		hvalue = n.hvalue;
		parent = NULL;
		children.clear();

		// copy data related to missionary-cannibal problem
		maxNumber = n.maxNumber;
		LHSmissionaries = n.LHSmissionaries;
		RHSmissionaries = n.RHSmissionaries;
		LHScannibals = n.LHScannibals;
		RHScannibals = n.RHScannibals;
		BOATside == n.BOATside;
	}
	return(*this);
}

// check if the node is a goal.
int
NMissCannNode::isGoal() const
{
	return((LHSmissionaries == 0) &&
		(RHSmissionaries == maxNumber) &&
		(LHScannibals == 0) &&
		(RHScannibals == maxNumber) &&
		(BOATside == 'R'));
}

// accessor functions
int
NMissCannNode::getFvalue() const
{
	return(fvalue);
}

int
NMissCannNode::getGvalue() const
{
	return(gvalue);
}

int
NMissCannNode::getHvalue() const
{
	return(hvalue);
}

NMissCannNode *
NMissCannNode::getParent() const
{
	return(parent);
}

List<NodePtr<NMissCannNode> > *
NMissCannNode::getChildren()
{
	return(&children);
}

void
NMissCannNode::setHvalue(int h)
{
	hvalue = h;
	return;
}

void
NMissCannNode::setFvalue(int f)
{
	fvalue = f;
	return;
}

void
NMissCannNode::setGvalue(int g)
{
	gvalue = g;
	return;
}

void
NMissCannNode::setParent(NMissCannNode *pn)
{
	parent = pn;
	return;
}

// calculate the heuristic value for current node
int
NMissCannNode::calculateGvalue()
{
	gvalue = 0;
	for (NMissCannNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;
	return(gvalue);
}

int
NMissCannNode::heuristic(const NMissCannNode &start, const List<NMissCannNode> &goals)
{
	// calculate estimate to get to start node
	gvalue = 0;
	for (NMissCannNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;

	// calculate the heuristic estimate for this node
	hvalue = 0;

	// add up to get total heuristic estimate for this goal
	fvalue = gvalue + hvalue;
	return(OK);
}

int
NMissCannNode::heuristic(const NMissCannNode &start)
{
	// calculate estimate to get to start node
	gvalue = 0;
	for (NMissCannNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;

	// calculate the heuristic estimate for this node
	hvalue = 0;

	// add up to get total heuristic estimate for this goal
	fvalue = gvalue + hvalue;
	return(OK);
}

// expand current node. this function MUST populate the
// children list and set all parent pointers to point to
// the current node.
int
NMissCannNode::expand()
{
	int NewLHSmissionaries, NewRHSmissionaries;
	int NewLHScannibals, NewRHScannibals;
	char NewBOATside;

	// which side is the boat?
	if (BOATside == 'L')
	{
		// check for (M, L->R)
		if (LHSmissionaries >= 1)
		{
			// possible new values
			NewBOATside = 'R';
			NewLHScannibals = LHScannibals;
			NewRHScannibals = RHScannibals;
			NewLHSmissionaries = LHSmissionaries - 1;
			NewRHSmissionaries = RHSmissionaries + 1;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}

		// check for (MM, L->R)
		if (LHSmissionaries >= 2)
		{
			// possible new values
			NewBOATside = 'R';
			NewLHScannibals = LHScannibals;
			NewRHScannibals = RHScannibals;
			NewLHSmissionaries = LHSmissionaries - 2;
			NewRHSmissionaries = RHSmissionaries + 2;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}

		// check for (C, L->R)
		if (LHScannibals >= 1)
		{
			NewBOATside = 'R';
			NewLHScannibals = LHScannibals - 1;
			NewRHScannibals = RHScannibals + 1;
			NewLHSmissionaries = LHSmissionaries;
			NewRHSmissionaries = RHSmissionaries;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}

		// check for (CC, L->R)
		if (LHScannibals >= 2)
		{
			// possible new values
			NewBOATside = 'R';
			NewLHScannibals = LHScannibals - 2;
			NewRHScannibals = RHScannibals + 2;
			NewLHSmissionaries = LHSmissionaries;
			NewRHSmissionaries = RHSmissionaries;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}

		// check for (MC, L->R)
		if (LHSmissionaries >= 1 && LHScannibals >= 1)
		{
			// possible new values
			NewBOATside = 'R';
			NewLHScannibals = LHScannibals - 1;
			NewRHScannibals = RHScannibals + 1;
			NewLHSmissionaries = LHSmissionaries - 1;
			NewRHSmissionaries = RHSmissionaries + 1;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}
	}
	else if (BOATside == 'R')
	{
		// check for (M, R->L)
		if (RHSmissionaries >= 1)
		{
			// possible new values
			NewBOATside = 'L';
			NewLHScannibals = LHScannibals;
			NewRHScannibals = RHScannibals;
			NewLHSmissionaries = LHSmissionaries + 1;
			NewRHSmissionaries = RHSmissionaries - 1;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}

		// check for (MM, R->L)
		if (RHSmissionaries >= 2)
		{
			// possible new values
			NewBOATside = 'L';
			NewLHScannibals = LHScannibals;
			NewRHScannibals = RHScannibals;
			NewLHSmissionaries = LHSmissionaries + 2;
			NewRHSmissionaries = RHSmissionaries - 2;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}

		// check for (C, R->L)
		if (RHScannibals >= 1)
		{
			NewBOATside = 'L';
			NewLHScannibals = LHScannibals + 1;
			NewRHScannibals = RHScannibals - 1;
			NewLHSmissionaries = LHSmissionaries;
			NewRHSmissionaries = RHSmissionaries;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}

		// check for (CC, R->L)
		if (RHScannibals >= 2)
		{
			// possible new values
			NewBOATside = 'L';
			NewLHScannibals = LHScannibals + 2;
			NewRHScannibals = RHScannibals - 2;
			NewLHSmissionaries = LHSmissionaries;
			NewRHSmissionaries = RHSmissionaries;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}

		// check for (MC, R->L)
		if (RHSmissionaries >= 1 && RHScannibals >= 1)
		{
			// possible new values
			NewBOATside = 'L';
			NewLHScannibals = LHScannibals + 1;
			NewRHScannibals = RHScannibals - 1;
			NewLHSmissionaries = LHSmissionaries + 1;
			NewRHSmissionaries = RHSmissionaries - 1;

			// check if legal
			if (!(((NewLHSmissionaries > 0) && 
				(NewLHSmissionaries < NewLHScannibals)) ||
			     ((NewRHSmissionaries > 0) && 
				(NewRHSmissionaries < NewRHScannibals))))
			{
				// create a new node
				NMissCannNode *pnew = new NMissCannNode(
					NewLHSmissionaries, NewLHScannibals,
					NewRHSmissionaries, NewRHScannibals,
					NewBOATside);
				MustBeTrue(pnew != NULL);

				// set parent pointer 
				pnew->parent = this;
	
				// insert into children list
				children.insertAtFront(
					NodePtr<NMissCannNode>(pnew));
			}
		}
	}
	else
	{
		// invalid value
		return(NOTOK);
	}
	return(OK);
}

// output function
ostream &
operator<<(ostream &os, const NMissCannNode &n)
{
#if 0
	os << endl;
	os << "=================================================" << endl;
	os << "f      = " << n.fvalue << endl;
	os << "g      = " << n.gvalue << endl;
	os << "h      = " << n.hvalue << endl;
	os << "=================================================" << endl;
	os << "LHS Missionaries = " << n.LHSmissionaries << endl;;
	os << "LHS Cannibals    = " << n.LHScannibals << endl;;
	os << "RHS Missionaries = " << n.RHSmissionaries << endl;;
	os << "RHS Cannibals    = " << n.RHScannibals << endl;;
	os << "Boat Side        = " << n.BOATside << endl;
	os << "=================================================" << endl;
#else
	os << "(f,g,h,lhsm,lhsc,rhsm,rhsc,bside) = (";
	os << n.fvalue << ",";
	os << n.gvalue << ",";
	os << n.hvalue << ",";
	os << n.LHSmissionaries << ",";
	os << n.LHScannibals << ",";
	os << n.RHSmissionaries << ",";
	os << n.RHScannibals << ",";
	os << n.BOATside << ")" << endl;
#endif
	return(os);
}

