/*
 * Tic-Tac-Toe program by Steve Chapel (schapel@cs.ucsb.edu)
 * modified for AIsearch by Peter Bouthoorn (peter@obelix.icce.rug.nl)
 */

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
#include "minimax.h"

#define Squares 9
typedef char Square_Type;
typedef Square_Type Board_Type[Squares];
#define Empty ' '

/*
 * Array that describes all winning board configurations.
 */
#define Possible_Wins 8
const int Three_in_a_Row[Possible_Wins][3] = {
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 6, 7, 8 },
    { 0, 3, 6 },
    { 1, 4, 7 },
    { 2, 5, 8 },
    { 0, 4, 8 },
    { 2, 4, 6 }
};

/*
 * The following array is used to implement the static evaluation
 * function to determine the "goodness" of each move (or rather:
 * of the board configuration resulting from that move). Rows are the
 * number of X's (or O's depending on whose move this is) in a row,
 * columns are the number of O's (or X's if it's O's move). The following
 * information is contained by the array:
 *    get 1000 points for each three X's or in a row.
 *    get  100 points for each two X's and an empty square in a row.
 *    get   10 points for each one X and two empty squares in a row.
 *    lose the same number of points if it is O's and empties in a row.
 * The same applies if it's O's move, reversing the rolls.
 */
const int Heuristic_Array[4][4] = {
    {     0,   -10,  -100, -1000 },
    {    10,     0,     0,     0 },
    {   100,     0,     0,     0 },
    {  1000,     0,     0,     0 }
};

/*
 * Class TicMove represents the moves that can be made on the
 * Tic Tac Toe board.
 */
class TicMove
{
    public:
	TicMove();
        TicMove(Square_Type);
	int operator==(const TicMove &) const;
        Square_Type getmove() const;
        void setmove(Square_Type);
    private:
        Square_Type square;		// square occupied by this move
};

/*
 * Class TicGame represents the game of Tic Tac Toe. It is derived
 * from class Minimax to make use of the minimax algorithm.
 */
class TicGame: public Minimax<TicMove>
{
    public:
	int game_ends() const;
	int evaluate_board(Player) const;
	List<TicMove> *generate_moves(Player);
	void do_move(const TicMove &, Player);
	void undo_move(const TicMove &, Player);
	int legal_move(const TicMove &);
	void init_board();
        void display() const;
    private:
        Square_Type other_player(Square_Type) const;

        Board_Type MyBoard;
};

 
