// definitions for node class

// headers
#include "queen.h"

// constructors and destructor
NQueenNode::NQueenNode():
	RefCount(),
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children(),
	nqueens(0), queens(0), nrows(0), ncols(0), 
	board(NULL)
{
	// do nothing
}

NQueenNode::NQueenNode(int nq, int nr, int nc):
	RefCount(),
	fvalue(INT_MAX), gvalue(INT_MAX), hvalue(INT_MAX), 
	parent(NULL), children(),
	nqueens(nq), queens(0), 
	nrows(nr), ncols(nc), 
	board(new char [nr*nc])
{
	MustBeTrue(board != NULL);
	for (int i = 0; i < nrows*ncols; i++)
	{
		board[i] = SpaceChar;
	}
}

NQueenNode::NQueenNode(const NQueenNode &n):
	RefCount(),
	fvalue(n.fvalue), gvalue(n.gvalue), hvalue(n.hvalue), 
	parent(NULL), children(),
	nqueens(n.nqueens), queens(n.queens), 
	nrows(n.nrows), ncols(n.ncols), 
	board(new char [n.nrows*n.ncols])
{
	MustBeTrue(board != NULL);
	for (int i = 0; i < nrows*ncols; i++)
	{
		board[i] = n.board[i];
	}
}

NQueenNode::~NQueenNode()
{
	fvalue = INT_MAX;
	gvalue = INT_MAX;
	hvalue = INT_MAX;
	parent = NULL;
	children.clear();
	nqueens = queens = nrows = ncols = 0;
	delete [] board;
	board = NULL;
}

// comparison operators
int
NQueenNode::operator<(const NQueenNode &n) const
{
	return(fvalue < n.fvalue);
}

int
NQueenNode::operator>(const NQueenNode &n) const
{
	return(fvalue > n.fvalue);
}

int
NQueenNode::operator==(const NQueenNode &n) const
{
	for (int ib = 0; ib < nrows*ncols; ib++)
	{
		if (board[ib] != n.board[ib])
			return(0);
	}
	return(1);
}

int
NQueenNode::operator!=(const NQueenNode &n) const
{
	return(!(operator==(n)));
}

// assignment operator
NQueenNode &
NQueenNode::operator=(const NQueenNode &n)
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

		// copy data related to N-queens problem
		queens = n.queens;
		nqueens = n.nqueens;
		nrows = n.nrows;
		nrows = n.ncols;
		delete [] board;
		board = new char [n.nrows*n.ncols];
		MustBeTrue(board != NULL);
		for (int i = 0; i < nrows*ncols; i++)
		{
			board[i] = n.board[i];
		}
	}
	return(*this);
}

// board accessor function
char &
NQueenNode::operator()(int r, int c)
{
	MustBeTrue(0 <= r && r < nrows);
	MustBeTrue(0 <= c && c < ncols);
	return(board[r*nrows+c]);
}

const char &
NQueenNode::operator()(int r, int c) const
{
	MustBeTrue(0 <= r && r < nrows);
	MustBeTrue(0 <= c && c < ncols);
	return(board[r*nrows+c]);
}

// check if the node is a goal.
int
NQueenNode::isGoal() const
{
	if (nqueens == queens)
		return(1);	
	else
		return(0);
}

// accessor functions
int
NQueenNode::getFvalue() const
{
	return(fvalue);
}

int
NQueenNode::getGvalue() const
{
	return(gvalue);
}

int
NQueenNode::getHvalue() const
{
	return(hvalue);
}

NQueenNode *
NQueenNode::getParent() const
{
	return(parent);
}

List<NodePtr<NQueenNode> > *
NQueenNode::getChildren()
{
	return(&children);
}

void
NQueenNode::setHvalue(int h)
{
	hvalue = h;
	return;
}

void
NQueenNode::setFvalue(int f)
{
	fvalue = f;
	return;
}

void
NQueenNode::setGvalue(int g)
{
	gvalue = g;
	return;
}

void
NQueenNode::setParent(NQueenNode *pn)
{
	parent = pn;
	return;
}

// calculate the heuristic value for current node
int
NQueenNode::calculateGvalue()
{
	gvalue = 0;
	for (NQueenNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;
	return(gvalue);
}

int
NQueenNode::heuristic(const NQueenNode &start, const List<NQueenNode> &goals)
{
	fvalue = gvalue = hvalue = INT_MAX;
	return(OK);
}

int
NQueenNode::heuristic(const NQueenNode &start)
{
#if 0
	if (this != &start)
		gvalue = parent->gvalue + 1;
	else
		gvalue = 0;
#else
	gvalue = 0;
	for (NQueenNode *ppar = parent; ppar != NULL; 
		gvalue++, ppar = ppar->parent) ;
#endif

	// calculate the heuristic estimate for this node
	int numberOfSpaces = 0;
	for (int ib = 0; ib < nrows*ncols; ib++)
	{
		if (board[ib] == SpaceChar) numberOfSpaces++;
	}
	hvalue = numberOfSpaces;

	// check if the number of spaces is enough to store
	// the remaining required queens.
	if ((nqueens-queens) > numberOfSpaces)
		hvalue = INT_MAX/2;

	// add up to get total heuristic estimate for this goal
	fvalue = gvalue + hvalue;
	return(OK);
}

// expand current node. this function MUST populate the
// children list and set all parent pointers to point to
// the current node.
int
NQueenNode::expand()
{
	// generate new children for the
	// current node
	for (int r = 0; r < nrows; r++)
	{
		for (int c = 0; c < ncols; c++)
		{
			// check if space is attacked
			if ((*this)(r, c) != SpaceChar)
				continue;

			// copy current node
			NQueenNode *pnew = new NQueenNode(*this);
			MustBeTrue(pnew != NULL);

			// set parent pointer 
			pnew->parent = this;

			// place new queen 
			(*pnew)(r, c) = QueenChar;
			pnew->queens++;
			
			// update attacked row squares
			for (int ic = 0; ic < ncols; ic++)
			{
				if ((*pnew)(r, ic) == SpaceChar)
					(*pnew)(r, ic) = AttackedChar;
			}

			// update attacked column squares
			for (int ir = 0; ir < nrows; ir++)
			{
				if ((*pnew)(ir, c) == SpaceChar)
					(*pnew)(ir, c) = AttackedChar;
			}

			// update attacked diagonal squares
			for (ir = r, ic = c; 
				ir >= 0 && ic >= 0; ir--, ic--)
			{
				if ((*pnew)(ir, ic) == SpaceChar)
					(*pnew)(ir, ic) = AttackedChar;
			}
			for (ir = r, ic = c; 
				ir < nrows && ic < ncols; ir++, ic++)
			{
				if ((*pnew)(ir, ic) == SpaceChar)
					(*pnew)(ir, ic) = AttackedChar;
			}
			for (ir = r, ic = c; 
				ir < nrows && ic >= 0; ir++, ic--)
			{
				if ((*pnew)(ir, ic) == SpaceChar)
					(*pnew)(ir, ic) = AttackedChar;
			}
			for (ir = r, ic = c; 
				ir >= 0 && ic < ncols; ir--, ic++)
			{
				if ((*pnew)(ir, ic) == SpaceChar)
					(*pnew)(ir, ic) = AttackedChar;
			}

			// insert into children list
			children.insertAtFront(NodePtr<NQueenNode>(pnew));
		}
	}
	return(OK);
}

// output function
ostream &
operator<<(ostream &os, const NQueenNode &n)
{
	os << endl;
	os << "=================================================" << endl;
	os << "f      = " << n.fvalue << endl;
	os << "g      = " << n.gvalue << endl;
	os << "h      = " << n.hvalue << endl;
	os << "queens = " << n.queens << endl;
	os << "=================================================" << endl;
	for (int ir = 0; ir < n.nrows; ir++)
	{
		os << "|";
		for (int ic = 0; ic < n.ncols; ic++)
		{
			os << n(ir, ic);
		}
		os << "|" << endl;
	}
	os << "=================================================" << endl;
	return(os);
}

