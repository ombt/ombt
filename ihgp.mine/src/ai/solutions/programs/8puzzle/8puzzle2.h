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

#include <stdio.h>
#include <new.h>
#include "aigraph.h"


/*
 * This program is as 8puzzle.cc except for the search algorithm used.
 * In this problem we want to use the A* start algorithm and therefore
 * derived class Puzzle from class AStar and PNode from BestNode.
 * Two extra functions must be implemented: compute_g() and compute_h().
 */ 


/*
 * Class PNode defines objects that represent a board configuration.
 * The search graph consists of objects of class Pnode. Because the
 * AStar search algorithm is used PNode must be derived from
 * class BestNode.
 */
class PNode : public BestNode
{
    public:
        PNode(const char *, int empty_x, int empty_y);
        PNode(const char *, int, int, int, int);
        const char (*get_board() const)[3];

       /*
        * The following three functions define the virtual functions
        * that must be implemented.
        */
        int operator==(const Node &) const;
        void display() const;
        Node *do_operator(int);
    private:
        /*
         * The following 4 functions are called to move the empty
         * tile and represent the operators of this problem.
         */
        PNode
            *do_left() const,
            *do_right() const,
            *do_up() const,
            *do_down() const;
            int compare_board(const char [3][3]) const;
        int
            x,			// coordinates of the empty tile
            y;
        char
            board[3][3];	// the board configuration
};


/*
 * Class Puzzle implements the search algorithm. Because the A*
 * algorithm is selected class Puzzle is derived from AStar.
 */
class Puzzle : public AStar
{
    public:
        Puzzle(PNode *start, PNode *target);

	/*
	 * Implementation of virtual functions.
	 */
        int compute_g(const Node *);
        int compute_h(const Node *);
    private:
	/*
	 * totdist() implements the heuristic function, the heuristic
	 * is called the "manhatten distance".
	 */
        int totdist(const char [3][3], const char[3][3]);
	PNode *goal;  // goal node, needed by totdist()
};

