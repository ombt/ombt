/*
 * Copyright (C) 1995, 1996 Peter Bouthoorn.
 *
 * This software may be freely distributed and modified provided
 * this copyright message is left intact. The copyright message must be
 * included both with this (the original) software and with any modified
 * copies of this software or with any new software based on this software.
 * Furthermore any modified copies of this software must carry prominent
 * notices stating the software was changed and the date of any change.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. In no event
 * will the copyright holder be liable for any damage arising out of
 * the use of this software.
 *
 * As a matter of courtesy, the author requests to be informed about
 * any bugs found in this software and about improvements that may be of
 * general interest.
 *
 * Peter Bouthoorn
 * peter@obelix.icce.rug.nl
 */

#include "8puzzle2.h"

/*
 * The target node is saved because it must be used by totdist() to
 * compute the "difference" of a particulare node and the goal
 * node.
 */
Puzzle::Puzzle(PNode *start, PNode *target)
    :AStar(4, start, target)
{
    goal = target;
}


/*
 * This constructor is used to create the initial board configuration.
 */
PNode::PNode(const char *b, int empty_x, int empty_y)
{
    char
        *p = *board;
    int
        i;

    for (i = 0; i <= 8; i++)
        *(p + i) = *(b + i);

    x = empty_x;
    y = empty_y;
}


/*
 * This constructor initializes a new configuration using another
 * configuration (the parent configuration). First the old board is
 * copied and next the two tiles that are on old_x, old_y and
 * new_x, new_t respectively are swapped.
 */
PNode::PNode(const char *b, int old_x, int old_y, int new_x, int new_y)
{
    char
        *p = *board;
    int
        i;

    for (i = 0; i <= 8; i++)
        *(p + i) = *(b + i);

    board[old_x][old_y] = board[new_x][new_y];
    board[new_x][new_y] = 0;

    x = new_x;
    y = new_y;
}


void PNode::display() const
{
    int
        row,
        col;

    for (row = 0; row < 3; row++)
    {
        for (col = 0; col < 3; col++)
            printf("%d ", board[row][col]);
        putchar('\n');
    }
    putchar('\n');
}


/*
 * operator== determines if two nodes, i.e., two board positions are
 * the same. First, the x- and y-coordinates of the empty tile are
 * compared and next, if necessary, the two boards themselves are
 * compared.
 */
int PNode::operator==(const Node &other) const
{
    const PNode &pnother = (PNode &)other;

    if (x != pnother.x && y != pnother.y)
	return(0);
    return(compare_board(pnother.board));
}


const char (*PNode::get_board() const)[3]
{
    return(board);
}


/*
 * compare_board() compares the current board configuration with
 * another.
 */
int PNode::compare_board(const char bo[3][3]) const
{
    const char
        *p = *board,
        *b = *bo;

    int
        i;

    for (i = 0; i <= 8; i++)
        if (*(p + i) != *(b + i))
            return(0);

    return(1);
}


/*
 * do_operator() applies operator n to the current configuration:
 * the empty tile is moved in the specified direction (by calling one of
 * the do_..() functions) resulting in a new board configuration. 0 is
 * returned if the operator cannot be applied to this board configuration.
 */
Node *PNode::do_operator(int index)
{
    switch(index)
    {
        case 0:
            return(do_left());
        case 1:
            return(do_up());
        case 2:
            return(do_right());
    }
    return(do_down());
}


PNode *PNode::do_left() const
{
    if (!y)
        return(0);

    return(new PNode(*board, x, y, x, y - 1));
}


PNode *PNode::do_right() const
{
    if (y == (2))
        return(0);

    return(new PNode(*board, x, y, x, y + 1));
}


PNode *PNode::do_down() const
{
    if (x == (2))
        return(0);

    return(new PNode(*board, x, y, x + 1, y));
}


PNode *PNode::do_up() const
{
    if (!x)
        return(0);

    return(new PNode(*board, x, y, x - 1, y));
}


/*
 * compute_g() computes the cost of getting from the parent node to
 * this node. In this problem this cost is always 1.
 */
int Puzzle::compute_g(const Node *)
{
    return(1);
}


/*
 * compute_h() computes the heuristic value of the node by calling
 * totdist().
 */
int Puzzle::compute_h(const Node *node)
{
    const PNode *pnode = (PNode *)node;
    return(totdist(pnode->get_board(), goal->get_board()));
}


/*
 * totdist() computes the total or Manhatten distance of the tiles in the 
 * current configuration from their 'home squares' (= the ordering
 * of the tiles in the goal configuration). The Manhatten between
 * two squares S1 and S2 is the distance between S1 and S2 in the
 * horizontal direction plus the distance between S1 and S2 in the
 * vertical direction.
 */
int Puzzle::totdist(const char now[3][3], const char target[3][3])
{
    int nrow, ncol,
        trow, tcol,
        found, tot = 0;

    for (nrow = 0; nrow < 3; nrow++)
    {
        for (ncol = 0; ncol < 3; ncol++)
        {
            found = 0;
            if (now[nrow][ncol] == 0)       // skip empty tile!
                continue;
            for (trow = 0; trow < 3 && !found; trow++)
                for (tcol = 0; tcol < 3 && !found; tcol++)
                    if (now[nrow][ncol] == target[trow][tcol])
                    {
                        tot += abs(ncol - tcol) + abs(nrow - trow);
                        found = 1;
                    }
        }
    }

    return(tot);
}


#ifdef _MSC_VER
int no_mem(size_t size)
{
    fprintf(stderr, "Out of memory\n");
    exit(1);
    return(0);
}
#else
void no_mem()
{
    fprintf(stderr, "Out of memory\n");
    exit(1);
}
#endif


int main()
{
    char
        start[3][3] = {
                        {2, 8, 3},
                        {1, 6, 4},
                        {7, 0, 5},
                       };

    char
        goal[3][3] = {
                        {1, 2, 3},
                        {8, 0, 4},
                        {7, 6, 5},
                    };


    /* Install new handler to catch out of memory errors. */
#ifdef _MSC_VER
    _set_new_handler(no_mem);
#else
    set_new_handler(no_mem);
#endif

    /*
     * Create Puzzle object passing it the start and goal node
     * which are also created at this point.
     */
    Puzzle
        puzzle(new PNode(*start, 2, 1), new PNode(*goal, 1, 1));

    /*
     * To demonstrate how this can be done we call get_sol()
     * and iterate over the solution path and print each element
     * we find instead of just calling display().
     */
    if (puzzle.generate())
    {
        IntrList<Node> *sol = puzzle.get_sol();
        IntrListIterator<Node> iter(*sol);

        for (PNode *p = (PNode *)iter.getfirst(); p; p = (PNode *)iter.getnext())
	    p->display();

        delete sol;
    }
    return(1);
}

