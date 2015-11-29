// definitions for node class

// headers
#include "eight.h"

// constructors and destructor
EightNode::EightNode():
	RefCount(),
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children(),
	blankrow(1), blankcol(1)
{
	// default to a goal node
	board[0][0] = 1;
	board[0][1] = 2;
	board[0][2] = 3;
	board[1][0] = 8;
	board[1][1] = 0;
	board[1][2] = 4;
	board[2][0] = 7;
	board[2][1] = 6;
	board[2][2] = 5;
}

EightNode::EightNode(const char b[3][3]):
	RefCount(),
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children()
{
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			board[r][c] = b[r][c];
			if (board[r][c] == 0)
			{
				blankrow = r;
				blankcol = c;
			}
		}
	}
}

EightNode::EightNode(const EightNode &n):
	RefCount(),
	fvalue(n.fvalue), gvalue(n.gvalue), hvalue(n.hvalue), 
	parent(NULL), children()
{
	blankrow = n.blankrow;
	blankcol = n.blankcol;
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			board[r][c] = n.board[r][c];
		}
	}
}

EightNode::~EightNode()
{
	fvalue = INT_MAX;
	gvalue = INT_MAX;
	hvalue = INT_MAX;
	parent = NULL;
	children.clear();
	blankrow = 1;
	blankcol = 1;
	board[0][0] = 1;
	board[0][1] = 2;
	board[0][2] = 3;
	board[1][0] = 8;
	board[1][1] = 0;
	board[1][2] = 4;
	board[2][0] = 7;
	board[2][1] = 6;
	board[2][2] = 5;
}

// comparison operators
int
EightNode::operator>(const EightNode &n) const
{
	return(fvalue > n.fvalue);
}

int
EightNode::operator<(const EightNode &n) const
{
	return(fvalue < n.fvalue);
}

int
EightNode::operator==(const EightNode &n) const
{
	for (int ir = 0; ir < 3; ir++)
	{
		for (int ic = 0; ic < 3; ic++)
		{
			if (board[ir][ic] != n.board[ir][ic])
				return(0);
		}
	}
	return(1);
}

int
EightNode::operator!=(const EightNode &n) const
{
	return(!(operator==(n)));
}

// assignment operator
EightNode &
EightNode::operator=(const EightNode &n)
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

		// copy data related to 8-puzzle problem
		blankrow = n.blankrow;
		blankcol = n.blankcol;
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				board[r][c] = n.board[r][c];
			}
		}
	}
	return(*this);
}

// board accessor function
char &
EightNode::operator()(int r, int c)
{
	MustBeTrue(0 <= r && r < 3);
	MustBeTrue(0 <= c && c < 3);
	return(board[r][c]);
}

const char &
EightNode::operator()(int r, int c) const
{
	MustBeTrue(0 <= r && r < 3);
	MustBeTrue(0 <= c && c < 3);
	return(board[r][c]);
}

// check if the node is a goal.
int
EightNode::isGoal() const
{
	// not used for this case
	return(0);
}

// accessor functions
int
EightNode::getFvalue() const
{
	return(fvalue);
}

int
EightNode::getGvalue() const
{
	return(gvalue);
}

int
EightNode::getHvalue() const
{
	return(hvalue);
}

void
EightNode::setHvalue(int h)
{
	hvalue = h;
	return;
}

void
EightNode::setFvalue(int f)
{
	fvalue = f;
	return;
}

void
EightNode::setGvalue(int g)
{
	gvalue = g;
	return;
}

EightNode *
EightNode::getParent() const
{
	return(parent);
}

List<NodePtr<EightNode> > *
EightNode::getChildren()
{
	return(&children);
}

void
EightNode::setParent(EightNode *pn)
{
	parent = pn;
	return;
}

// calculate the heuristic value for current node
int
EightNode::calculateGvalue()
{
	gvalue = 0;
	for (EightNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;
	return(gvalue);
}

int
EightNode::heuristic(const EightNode &start, const List<EightNode> &goals)
{
	// set depth in search
#if 0
	if (this != &start)
		gvalue = parent->gvalue + 1;
	else
		gvalue = 0;
#else
	gvalue = 0;
	for (EightNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;
#endif

	// calculate heuristic for all goals node, keep best one
	ListIterator<EightNode> goalsIter(goals);
	for (hvalue = INT_MAX; !goalsIter.done(); goalsIter++)
	{
		int outofplace = 0;
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				if (((board[r][c] != goalsIter()(r, c))) &&
				    (goalsIter()(r, c) != 0))
					outofplace++;
			}
		}
		if (outofplace < hvalue)
			hvalue = outofplace;
	}

	// add up to get total heuristic estimate for this goal
	fvalue = gvalue + hvalue;
	return(OK);
}

int
EightNode::heuristic(const EightNode &start)
{
	fvalue = gvalue = hvalue = INT_MAX;
	return(NOTOK);
}

// expand current node. this function MUST populate the
// children list and set all parent pointers to point to
// the current node.
int
EightNode::expand()
{
	// generate new children for the
	// current node
	if ((blankrow-1) >= 0)
	{
		// copy current node
		EightNode *pnew = new EightNode(*this);
		MustBeTrue(pnew != NULL);

		// set parent pointer 
		pnew->parent = this;

		// new blank location
		pnew->blankrow = blankrow-1;
		pnew->blankcol = blankcol;

		// switch tiles
		int tmp = 
			(*pnew)(blankrow, blankcol);
		(*pnew)(blankrow, blankcol) = 
			(*pnew)(pnew->blankrow, pnew->blankcol);
		(*pnew)(pnew->blankrow, pnew->blankcol) =
			tmp;

		// insert into children list
		children.insertAtFront(NodePtr<EightNode>(pnew));
	}
	if ((blankrow+1) < 3)
	{
		// copy current node
		EightNode *pnew = new EightNode(*this);
		MustBeTrue(pnew != NULL);

		// set parent pointer 
		pnew->parent = this;

		// new blank location
		pnew->blankrow = blankrow+1;
		pnew->blankcol = blankcol;

		// switch tiles
		int tmp = 
			(*pnew)(blankrow, blankcol);
		(*pnew)(blankrow, blankcol) = 
			(*pnew)(pnew->blankrow, pnew->blankcol);
		(*pnew)(pnew->blankrow, pnew->blankcol) =
			tmp;

		// insert into children list
		children.insertAtFront(NodePtr<EightNode>(pnew));
	}
	if ((blankcol-1) >= 0)
	{
		// copy current node
		EightNode *pnew = new EightNode(*this);
		MustBeTrue(pnew != NULL);

		// set parent pointer 
		pnew->parent = this;

		// new blank location
		pnew->blankrow = blankrow;
		pnew->blankcol = blankcol-1;

		// switch tiles
		int tmp = 
			(*pnew)(blankrow, blankcol);
		(*pnew)(blankrow, blankcol) = 
			(*pnew)(pnew->blankrow, pnew->blankcol);
		(*pnew)(pnew->blankrow, pnew->blankcol) =
			tmp;

		// insert into children list
		children.insertAtFront(NodePtr<EightNode>(pnew));
	}
	if ((blankcol+1) < 3)
	{
		// copy current node
		EightNode *pnew = new EightNode(*this);
		MustBeTrue(pnew != NULL);

		// set parent pointer 
		pnew->parent = this;

		// new blank location
		pnew->blankrow = blankrow;
		pnew->blankcol = blankcol+1;

		// switch tiles
		int tmp = 
			(*pnew)(blankrow, blankcol);
		(*pnew)(blankrow, blankcol) = 
			(*pnew)(pnew->blankrow, pnew->blankcol);
		(*pnew)(pnew->blankrow, pnew->blankcol) =
			tmp;

		// insert into children list
		children.insertAtFront(NodePtr<EightNode>(pnew));
	}
	return(OK);
}

// output function
ostream &
operator<<(ostream &os, const EightNode &n)
{
	os << endl;
	os << "=================================================" << endl;
	os << "f = " << n.fvalue << endl;
	os << "g = " << n.gvalue << endl;
	os << "h = " << n.hvalue << endl;
	os << "blankrow = " << n.blankrow << endl;
	os << "blankcol = " << n.blankcol << endl;
	os << "=================================================" << endl;
	for (int ir = 0; ir < 3; ir++)
	{
		os << "|";
		for (int ic = 0; ic < 3; ic++)
		{
			os << char(0x30+n(ir, ic));
		}
		os << "|" << endl;
	}
	os << "=================================================" << endl;
	return(os);
}
