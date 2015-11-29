// definitions for node class

// headers
#include "coins.h"

// constructors and destructor
CoinsNode::CoinsNode():
	RefCount(),
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children(),
	number(0), space(0), board(NULL)
{
	// do nothing
}

CoinsNode::CoinsNode(int n):
	RefCount(),
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children(),
	number(n), space(n), board(new char [2*n+2])
{
	MustBeTrue(number > 0);
	MustBeTrue(board != NULL);
	for (int ib = 0; ib < number; ib++)
	{
		board[ib] = 'F';
		board[ib+number+1] = 'P';
	}
	board[space] = ' ';
	board[2*number+1] = 0;
}

CoinsNode::CoinsNode(const CoinsNode &n):
	RefCount(),
	fvalue(n.fvalue), gvalue(n.gvalue), hvalue(n.hvalue), 
	parent(NULL), children(),
	number(n.number), space(n.space),
	board(new char [2*n.number+2])
{
	MustBeTrue(number > 0);
	MustBeTrue(board != NULL);
	strcpy(board, n.board);
}

CoinsNode::~CoinsNode()
{
	fvalue = INT_MAX;
	gvalue = INT_MAX;
	hvalue = INT_MAX;
	parent = NULL;
	children.clear();
	number = space = 0;
}

// comparison operators
int
CoinsNode::operator>(const CoinsNode &n) const
{
	return(fvalue > n.fvalue);
}

int
CoinsNode::operator<(const CoinsNode &n) const
{
	return(fvalue < n.fvalue);
}

int
CoinsNode::operator==(const CoinsNode &n) const
{
	return(strcmp(board, n.board) == 0);
}

int
CoinsNode::operator!=(const CoinsNode &n) const
{
	return(!(operator==(n)));
}

// assignment operator
CoinsNode &
CoinsNode::operator=(const CoinsNode &n)
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

		// copy data related to coins-puzzle problem
		number = n.number;
		space = n.space;
		strcpy(board, n.board);
	}
	return(*this);
}

// board accessor function
char &
CoinsNode::operator[](int idx)
{
	MustBeTrue(0 <= idx && idx < (2*number+1));
	return(board[idx]);
}

const char &
CoinsNode::operator[](int idx) const
{
	MustBeTrue(0 <= idx && idx < (2*number+1));
	return(board[idx]);
}

// check if the node is a goal.
int
CoinsNode::isGoal() const
{
	for (int idx = 0; idx < number; idx++)
	{
		if (board[idx] != 'P') return(0);
	}
	if (board[idx++] != ' ') return(0);
	for ( ; idx < (2*number+1); idx++)
	{
		if (board[idx] != 'F') return(0);
	}
	return(1);
}

// accessor functions
int
CoinsNode::getFvalue() const
{
	return(fvalue);
}

int
CoinsNode::getGvalue() const
{
	return(gvalue);
}

int
CoinsNode::getHvalue() const
{
	return(hvalue);
}

void
CoinsNode::setHvalue(int h)
{
	hvalue = h;
	return;
}

void
CoinsNode::setFvalue(int f)
{
	fvalue = f;
	return;
}

void
CoinsNode::setGvalue(int g)
{
	gvalue = g;
	return;
}

CoinsNode *
CoinsNode::getParent() const
{
	return(parent);
}

List<NodePtr<CoinsNode> > *
CoinsNode::getChildren()
{
	return(&children);
}

void
CoinsNode::setParent(CoinsNode *pn)
{
	parent = pn;
	return;
}

// calculate the heuristic value for current node
int
CoinsNode::calculateGvalue()
{
	gvalue = 0;
	for (CoinsNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;
	return(gvalue);
}

int
CoinsNode::heuristic(const CoinsNode &start, const List<CoinsNode> &goals)
{
	// set depth in search
	gvalue = 0;
	for (CoinsNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;

	// calculate heuristic for current state
	hvalue = 0;

	// add up to get total heuristic estimate for this goal
	fvalue = gvalue + hvalue;
	return(OK);
}

int
CoinsNode::heuristic(const CoinsNode &start)
{
	// set depth in search
	gvalue = 0;
	for (CoinsNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;

	// calculate heuristic for current state
	hvalue = 0;
	for (int ib = 0; ib <= 2*number; ib++)
	{
		switch (board[ib])
		{
		case 'P':
			hvalue += ib;
			break;
		case 'F':	
			hvalue += (2*number-ib);
			break;
		}
	}
	hvalue -= (number-1)*number;

	// add up to get total heuristic estimate for this goal
	fvalue = gvalue + hvalue;
	return(NOTOK);
}

// expand current node. this function MUST populate the
// children list and set all parent pointers to point to
// the current node.
int
CoinsNode::expand()
{
	// generate new children for the
	// current node
	if (((space+1) < (2*number+1)) && (board[space+1] == 'P'))
	{
		// copy current node
		CoinsNode *pnew = new CoinsNode(*this);
		MustBeTrue(pnew != NULL);

		// set parent pointer 
		pnew->parent = this;

		// switch characters
		(*pnew)[space] = 'P';
		(*pnew)[space+1] = ' ';

		// new space location
		pnew->space = space+1;

		// insert into children list
		children.insertAtFront(NodePtr<CoinsNode>(pnew));
	}
	if (((space-1) >= 0) && (board[space-1] == 'F'))
	{
		// copy current node
		CoinsNode *pnew = new CoinsNode(*this);
		MustBeTrue(pnew != NULL);

		// set parent pointer 
		pnew->parent = this;

		// switch characters
		(*pnew)[space] = 'F';
		(*pnew)[space-1] = ' ';

		// new space location
		pnew->space = space-1;

		// insert into children list
		children.insertAtFront(NodePtr<CoinsNode>(pnew));
	}
	if (((space+2) < (2*number+1)) && (board[space+2] == 'P'))
	{
		// copy current node
		CoinsNode *pnew = new CoinsNode(*this);
		MustBeTrue(pnew != NULL);

		// set parent pointer 
		pnew->parent = this;

		// switch characters
		(*pnew)[space] = 'P';
		(*pnew)[space+2] = ' ';

		// new space location
		pnew->space = space+2;

		// insert into children list
		children.insertAtFront(NodePtr<CoinsNode>(pnew));
	}
	if (((space-2) >= 0) && (board[space-2] == 'F'))
	{
		// copy current node
		CoinsNode *pnew = new CoinsNode(*this);
		MustBeTrue(pnew != NULL);

		// set parent pointer 
		pnew->parent = this;

		// switch characters
		(*pnew)[space] = 'F';
		(*pnew)[space-2] = ' ';

		// new space location
		pnew->space = space-2;

		// insert into children list
		children.insertAtFront(NodePtr<CoinsNode>(pnew));
	}
	return(OK);
}

// output function
ostream &
operator<<(ostream &os, const CoinsNode &n)
{
	os << endl;
	os << "=================================================" << endl;
	os << "f = " << n.fvalue << endl;
	os << "g = " << n.gvalue << endl;
	os << "h = " << n.hvalue << endl;
	os << "number = " << n.number << endl;
	os << "space  = " << n.space << endl;
	os << "=================================================" << endl;
	os << "|" << n.board << "|" << endl;
	os << "=================================================" << endl;
	return(os);
}
