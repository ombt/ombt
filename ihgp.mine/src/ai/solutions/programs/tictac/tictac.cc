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

#include "tictac.h"

TicMove::TicMove()
{
    square = -1;
}

TicMove::TicMove(Square_Type move)
{
    square = move;
}

Square_Type TicMove::getmove() const
{
    return(square);
}

void TicMove::setmove(Square_Type move)
{
    square = move;
}

int TicMove::operator==(const TicMove &other) const
{
    return(square == other.square);
}

/*
 * game_ends() determines if the current board configuration is a final
 * configuration. If either player has won, return the winner. If the game
 * ends in a draw, return 'T'. If the game is not yet over, return 0.
 */
int TicGame::game_ends() const
{
    int i;

    /* Has either player one? */
    for (i = 0; i < Possible_Wins; i++)
    {
	Square_Type possible_winner;

        possible_winner = MyBoard[Three_in_a_Row[i][0]];
        if (possible_winner != Empty &&
            possible_winner == MyBoard[Three_in_a_Row[i][1]] &&
            possible_winner == MyBoard[Three_in_a_Row[i][2]])
                return(possible_winner);
    }

    /* Any empty squares left on the board? If so, the game is not over. */
    for (i = 0; i < Squares; i++)
        if (MyBoard[i] == Empty)
            return(0);

    /* Otherwise it's a draw. */
    return('T');
}

/*
 * evaluate_board() evaluates the current board position from the
 * correct point of view.
 * Important: this function assumes player one is always 'X' and player
 * two is always 'O'.
 */
int TicGame::evaluate_board(Player pl) const
{
    int val = 0;
    Square_Type player;

    player = (pl == PlayerOne) ? 'X' : 'O';

    for (int i = 0; i < Possible_Wins; i++)
    {
	int j, players = 0, others = 0;

	for (j = 0; j < 3; j++)
	{
	    Square_Type piece = MyBoard[Three_in_a_Row[i][j]];
            if (piece == player)
		players++;
	    else if (piece == other_player(player))
		others++;
	}
	val += Heuristic_Array[players][others];
    }
    return(val);
}

/*
 * generate_moves() generates all moves that can be made from
 * the current board configuration.
 */
List<TicMove> *TicGame::generate_moves(Player)
{
    List<TicMove> *ret = new List<TicMove>;

    for (Square_Type i = 0; i < Squares; i++)
	if (MyBoard[i] == Empty)
	    ret->addtotail(TicMove(i));

    if (ret->is_empty())
    {
	delete(ret);
	return(0);
    }
    return(ret);
}

/*
 * do_move() makes the specified move on the current board configuration.
 */
void TicGame::do_move(const TicMove &move, Player pl)
{
    Square_Type player;
    Square_Type sq;

    player = (pl == PlayerOne) ? 'X' : 'O';
    sq = move.getmove();

    MyBoard[sq] = player;
}

/*
 * undo_move() undoes the specified move from the current board configuration.
 */
void TicGame::undo_move(const TicMove &move, Player)
{
    Square_Type sq = move.getmove();

    MyBoard[sq] = Empty;
}

int TicGame::legal_move(const TicMove &move)
{
    if (move.getmove() < 0 || move.getmove() > 8)
	return(0);
    return(MyBoard[move.getmove()] == Empty);
}

Square_Type TicGame::other_player(Square_Type player) const
{
    return(player == 'X' ? 'O' : 'X');
}

void TicGame::init_board()
{
    for (int i = 0; i < Squares; i++)
	MyBoard[i] = Empty;
}

void TicGame::display() const
{
    for (int i = 0; i < Squares; i += 3)
    {
        if (i > 0)
	    printf("---+---+---\n");
        printf(" %c | %c | %c \n", MyBoard[i], MyBoard[i + 1], MyBoard[i + 2]);
    }
    printf("\n");
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
    int depth;
    char answer[80];
    TicGame game;
    int winner, computer, human, tomove;

    /* Install new handler to catch out of memory errors. */
#ifdef _MSC_VER
    _set_new_handler(no_mem);
#else
    set_new_handler(no_mem);
#endif

    printf("Do you want alpha-beta cut offs? ");
    gets(answer);
    if (answer[0] != 'y' && answer[0] != 'Y')
	game.do_cutoff(0);

    printf("Depth of search? ");
    gets(answer);
    depth = atoi(answer);

    /* The player that is to move first must always be 'X'. */
    printf("Do you want to move first? ");
    gets(answer);
    if (answer[0] == 'y' || answer[0] == 'Y')
    {
	tomove = human = 'X';
	computer = 'O';
    }
    else
    {
	tomove = computer = 'X';
	human = 'O';
    }

    /* Initialize and display board. */
    game.init_board();
    game.display();

    /* Play until the game ends. */
    while (!(winner = game.game_ends()))
    {
        TicMove move;
	int val;

	if (tomove == human)
	{
	    do
	    {
	        printf("What is your move? ");
	        gets(answer);
	        move = (Square_Type)atoi(answer) - 1;
	    }
	    while (!game.legal_move(move));
	}
	else	/* Computer is to move: call minimax. */
	    val = game.do_minimax(computer == 'X' ? PlayerOne : PlayerTwo,
				  depth, &move);

	game.do_move(move, tomove == 'X' ? PlayerOne : PlayerTwo);
	if (tomove == computer)
	{
	    printf("My move is: %d\n", move.getmove() + 1);
	    printf("evaluation of this move: %d\n", val);
	}
	game.display();
	tomove = tomove == human ? computer : human;
    }
    if (winner == 'T')
	puts("It's a tie.");
    else if (winner == computer)
	puts("I win.");
    else
	puts("You win.");
    return(1);
}
