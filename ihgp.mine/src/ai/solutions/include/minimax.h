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

#ifndef _MINIMAX_H_
#define _MINIMAX_H_

#include <stdio.h>
#include "xlist.h"

#define Infinity 10000

enum Player { PlayerOne, PlayerTwo };

/*
 * class Minimax implements the minimax algorithm, including alpha-
 * beta cutoffs. The use of alpha-beta cutoffs can be disabled if
 * this is desired.
 *
 * Application-level classes derived from this class must implement
 * the following functions:
 *
 * - game_ends():      decides whether the game has ended, i.e. whether the
 *                     current board position is a final board configuration,
 *                     1: yes, 0: no.
 * - evaluate_board(): the static evaluation function which determines
 *                     the value of the current board congiguration from
 *                     the perspective of the player that is about to move.
 *                     Note: the value returned by this function must never
 *                     exceed neither Infinity nor -Infinity.
 * - generate_moves(): returns a list of moves that can be made starting
 *                     from the current board position.
 * - do_move():        make a move on the current board configuration.
 * - undo_move():      undo a move on the current board configuration.
 *
 * Also, because a number of objects of type T are created by this program
 * the application must define a constructor for T with no arguments. T is
 * the class defined by the application that represents the moves of the
 * game.
 */
template <class T>
class Minimax
{
    public:
	Minimax();
	virtual ~Minimax();
        virtual int game_ends() const = 0;
        virtual int evaluate_board(Player) const = 0;
        virtual List<T> *generate_moves(Player) = 0;
        virtual void do_move(const T &, Player) = 0;
        virtual void undo_move(const T &, Player) = 0;

	void do_cutoff(int);		// enable or disable alpha-beta cutoffs
	Player other(Player);		// other player to move

	int do_minimax(Player, int maxdepth, T *answer,
		       int alpha = -Infinity, int beta = Infinity);
    private:
	int do_alphabeta;		// alpha-beta cutoffs: on or off
};

template <class T>
inline Minimax<T>::Minimax()
{
    do_alphabeta = 1;
}

template <class T>
inline Minimax<T>::~Minimax()
{
}

template <class T>
inline void Minimax<T>::do_cutoff(int c)
{
    do_alphabeta = c;
}

template <class T>
inline Player Minimax<T>::other(Player pl)
{
    return(pl == PlayerOne ? PlayerTwo: PlayerOne);
}

template <class T>
int Minimax<T>::do_minimax(Player pl, int depth, T *answer, int alpha, int beta)
{
    List<T> *succs;
    T bestmove, child;

    /*
     * Recursion ends if the depthbound has been reached or if
     * the game ends.
     */
    if (!depth || game_ends())
	return(evaluate_board(pl));

    /* Generate successors of current position. */
    succs = generate_moves(pl);

    /* Recursion also ends if no successors were generated. */
    if (!succs || succs->is_empty())
	return(evaluate_board(pl));

    /*
     * Alpha is used to keep track of the best value we can find.
     * When alpha-beta cutoffs are not used initialize alpha to
     * an absolute low.
     */
    if (!do_alphabeta)
	alpha = -Infinity;

    /*
     * Call do_minimax() recursively on each successor to determine
     * its value, keeping track of the best value and node that are found.
     */
    for (int i = succs->getcount(); i; i--)
    {
        int newval;

	child = succs->gethead();
	succs->remove_head();

	/*
         * Make the move on the board, call minimax on the new board
	 * configuration to determine the value of this move and
         * undo the move.
	 */
	do_move(child, pl);
	newval = -do_minimax(other(pl), depth - 1, answer, -beta, -alpha);
	undo_move(child, pl);

	/* Keep track of the best value and the best move. */
	if (newval > alpha)
	{
	    alpha = newval;
	    bestmove = child;
	}

	/* Here's where the alpha-beta cut offs come in to play. */
        if (do_alphabeta)
	    if (alpha >= beta)
	    {
		bestmove = child;
		break;
	    }
    }

    delete succs;
    *answer = bestmove;
    return(alpha);
}

#endif
