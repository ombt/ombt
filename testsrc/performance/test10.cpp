//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include "system/Debug.h"
#include "performance/NanoSecTime.h"
#include "adt/PriorityQueue_Array.h"

using namespace ombt;

// headers
#include <time.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <climits>

// cheap way
using namespace std;
using namespace ombt;

//
// chess board
//
class ChessBoard
{
public: // data types and constants
    // constants
    static const int null_value = -1;
    static const int el_moves[][2];

    // how much output
    enum VerboseLevel 
    {
        VL_Unknown = -1,
        VL_Off = 0,
        VL_On = 1,
    };

    // data types
    class Square
    {
    public:
        inline Square(int val = null_value, int idx = -1):
            _value(val), _index(idx) { }
        inline Square(const Square &src):
            _value(src._value), 
            _index(src._index) { }
        inline ~Square() { }
        inline Square &operator=(const Square &rhs)
        {
            if (this != &rhs)
            {
                _value = rhs._value;
                _index = rhs._index;
            }
            return *this;
        }
        friend ostream &operator<<(ostream &, const Square &);

        int _value;
        int _index;

        static int _field_width;
    };

    class PQElement
    {
    public:
        // ctors and dtor
        PQElement(int w = INT_MAX, int r=-1, int c=-1):
            _r(r), _c(c), _w(w) { }
        PQElement(const PQElement &src):
            _r(src._r), _c(src._c), _w(src._w) { }
        ~PQElement() { }

        // operators
        inline PQElement &operator=(const PQElement &rhs)
        {
            if (this != &rhs)
            {
                _r = rhs._r;
                _c = rhs._c;
                _w = rhs._w;
            }
            return *this;
        }
#if 1
        inline bool operator<(const PQElement &rhs) const
        {
            return _w < rhs._w;
        }
	inline bool operator<=(const PQElement &rhs) const
	{
            return _w <= rhs._w;
        }
        inline bool operator>(const PQElement &rhs) const
        {
            return _w > rhs._w;
        }
        inline bool operator>=(const PQElement &rhs) const
        {
            return _w >= rhs._w;
        }
        inline bool operator==(const PQElement &rhs) const
        {
            return _w == rhs._w;
        }
        inline bool operator!=(const PQElement &rhs) const
        {
            return _w != rhs._w;
        }
#else
        inline bool operator<(const PQElement &rhs) const
        {
            return _w > rhs._w;
        }
	inline bool operator<=(const PQElement &rhs) const
	{
            return _w >= rhs._w;
        }
        inline bool operator>(const PQElement &rhs) const
        {
            return _w < rhs._w;
        }
        inline bool operator>=(const PQElement &rhs) const
        {
            return _w <= rhs._w;
        }
        inline bool operator==(const PQElement &rhs) const
        {
            return _w == rhs._w;
        }
        inline bool operator!=(const PQElement &rhs) const
        {
            return _w != rhs._w;
        }
#endif

    public:
        int _r, _c, _w;
    };

    // priority type for heuristic search
    typedef PriorityQueue_Array<PQElement> PQType;

    // type for storing allowed moves per key.
    typedef int * AllowedMoves;

public:
    // ctors and dtor
    ChessBoard(int max_row, int max_col);
    ChessBoard(const ChessBoard &);
    ~ChessBoard();
    void reset();

    // assignment
    ChessBoard &operator=(const ChessBoard &);

    // output
    inline void verbose(VerboseLevel v)
    {
        _verbose = v;
    }
    inline VerboseLevel verbose() const
    {
        return _verbose;
    }
    friend ostream &operator<<(ostream &, const ChessBoard &);

    // utilities
    inline int value(int r, int c) const
    {
        MustBeTrue((0 <= r) && (r < _max_row));
        MustBeTrue((0 <= c) && (c < _max_col));
        return _board[r*_max_col+c]._value;
    }
    inline int index(int r, int c) const
    {
        MustBeTrue((0 <= r) && (r < _max_row));
        MustBeTrue((0 <= c) && (c < _max_col));
        return _board[r*_max_col+c]._index;
    }
    inline int &value(int r, int c) 
    {
        MustBeTrue((0 <= r) && (r < _max_row));
        MustBeTrue((0 <= c) && (c < _max_col));
        return _board[r*_max_col+c]._value;
    }
    inline int &index(int r, int c)
    {
        MustBeTrue((0 <= r) && (r < _max_row));
        MustBeTrue((0 <= c) && (c < _max_col));
        return _board[r*_max_col+c]._index;
    }
    inline const Square &operator()(int r, int c) const
    {
        MustBeTrue((0 <= r) && (r < _max_row));
        MustBeTrue((0 <= c) && (c < _max_col));
        return _board[r*_max_col+c];
    }
    inline Square &operator()(int r, int c)
    {
        MustBeTrue((0 <= r) && (r < _max_row));
        MustBeTrue((0 <= c) && (c < _max_col));
        return _board[r*_max_col+c];
    }
    inline const Square &board(int r, int c) const
    {
        MustBeTrue((0 <= r) && (r < _max_row));
        MustBeTrue((0 <= c) && (c < _max_col));
        return _board[r*_max_col+c];
    }
    inline Square &board(int r, int c)
    {
        MustBeTrue((0 <= r) && (r < _max_row));
        MustBeTrue((0 <= c) && (c < _max_col));
        return _board[r*_max_col+c];
    }
    inline int calc_field_width(int n)
    {
        Square::_field_width = 0;
        if (n<=0) n = 1;
        while (n>0)
        {
            Square::_field_width += 1;
            n /= 10;
        }
    }

    // check if a move is allowed.
    inline bool check_if_allowed(int r, int c) const
    {
        if (((0 <= r) && (r < _max_row)) &&
            ((0 <= c) && (c < _max_col)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // generate all single moves from a given square
    void knight_moves(ostream &os,
                      AllowedMoves &moves,
                      int r, int c,
                      int &how_many);

    // generate all single moves entire board
    void board_knight_moves(ostream &os, int &how_many);

    // naive depth-first search for solution
    bool basic_depth_first(int marker,
                           int start_row, 
                           int start_col, 
                           int max_visits, 
                           int &number_found,
                           int &bail_outs,
                           int &nodes_visited);

    // precalc depth-first search for solution
    bool precalc_depth_first(int marker,
                             int start_row, 
                             int start_col, 
                             int max_visits, 
                             int &number_found,
                             int &bail_outs,
                             int &nodes_visited);

    // Warnsdorff's heuristic depth-first search for solution
    bool warnsdorff_depth_first(int marker,
                                int start_row, 
                                int start_col, 
                                int max_visits, 
                                int &number_found,
                                int &bail_outs,
                                int &nodes_visited);

private: // not allowed
    ChessBoard();

protected:
    // verbose level
    VerboseLevel _verbose;

    // internal board representation
    int _max_row;
    int _max_col;
    Square *_board;

    // end-keys after L-step
    int _key_num;
    int _imax;
    AllowedMoves _moves;
};

// key board class

// allowed horizontal and vertical deltas
const int ChessBoard::el_moves[][2] = {
    { 2, 1 },
    { 2, -1 },
    { -2, +1 },
    { -2, -1 },
    { +1, +2 },
    { +1, -2 },
    { -1, +2 },
    { -1, -2 },
    { 0, 0 }, // sentinel
};

int ChessBoard::Square::_field_width = 0;

// ctors and dtor
ChessBoard::ChessBoard(int max_row, int max_col):
    _verbose(VL_Off),
    _board(NULL),
    _max_row(max_row),
    _max_col(max_col),
    _key_num(-1),
    _imax(0),
    _moves(NULL)
{
    _board = new Square [_max_row*_max_col];
    MustBeTrue(_board != NULL);

    for (int r=0; r<_max_row; ++r)
    {
        for (int c=0; c<_max_col; ++c)
        {
            ++_key_num;
            _board[r*_max_col+c] = Square(_max_col*_max_row+1, _key_num);
        }
    }

    calc_field_width(_key_num);

    // allocate allowed-moves list
    ++_key_num;
    _imax = _key_num*(_key_num+1)*2;
    _moves = new int [_imax];
    MustBeTrue(_moves != NULL);
    for (int i=0; i<_imax; ++i)
    {
        _moves[i] = -1;
    }
}

ChessBoard::ChessBoard(const ChessBoard &src):
    _verbose(src._verbose),
    _board(NULL),
    _max_row(src._max_row),
    _max_col(src._max_col),
    _key_num(src._key_num),
    _imax(src._imax)
{
    _board = new Square [_max_row*_max_col];
    MustBeTrue(_board != NULL);

    for (int r=0; r<_max_row; ++r)
    {
        for (int c=0; c<_max_col; ++c)
        {
            _board[r*_max_col+c] = src._board[r*_max_col+c];
        }
    }

    _moves = new int [_imax];
    MustBeTrue(_moves != NULL);
    for (int i=0; i<_imax; ++i)
    {
        _moves[i] = src._moves[i];
    }
}

ChessBoard::~ChessBoard()
{
    if (_board != NULL) delete [] _board;
    _board = NULL;
    if (_moves != NULL) delete [] _moves;
    _moves = NULL;
}

void
ChessBoard::reset()
{
    int kn = -1;
    for (int r=0; r<_max_row; ++r)
    {
        for (int c=0; c<_max_col; ++c)
        {
            ++kn;
            _board[r*_max_col+c] = Square(_max_col*_max_row+1, kn);
        }
    }
}

// assignment operator
ChessBoard &
ChessBoard::operator=(const ChessBoard &rhs)
{
    if (this != &rhs)
    {
        if (_board != NULL) delete [] _board;
        if (_moves != NULL) delete [] _moves;

        _verbose = rhs._verbose;
        _max_row = rhs._max_row;
        _max_col = rhs._max_col;
        _key_num = rhs._key_num;
        _imax    = rhs._imax;

        _board = new Square [_max_row*_max_col];
        MustBeTrue(_board != NULL);

        for (int r=0; r<_max_row; ++r)
        {
            for (int c=0; c<_max_col; ++c)
            {
                _board[r*_max_col+c] = rhs._board[r*_max_col+c];
            }
        }

        _moves = new int [_imax];
        MustBeTrue(_moves != NULL);
        for (int i=0; i<_imax; ++i)
        {
            _moves[i] = rhs._moves[i];
        }
    }

    return *this;
}

// generate all single moves from a given square
void
ChessBoard::knight_moves(ostream &os,
                       AllowedMoves &moves,
                       int r, int c,
                       int &how_many)
{
    // sanity checks
    MustBeTrue((0 <= r) && (r < _max_row));
    MustBeTrue((0 <= c) && (c < _max_col));

    // output
    if (_verbose == VL_On) 
    {
        os << endl << "Moves for: " << board(r,c) << endl;
    }

    // check moves from current square
    int index = board(r,c)._index;
    int *pm = moves + index*(_key_num+1)*2;
    int &mi = (pm[0*2+0]);

    for (int move=0; el_moves[move][0] != 0; ++move)
    {
        int new_r = r+el_moves[move][0];
        int new_c = c+el_moves[move][1];
        if (check_if_allowed(new_r, new_c))
        {
            if (_verbose == VL_On) 
            {
                os << board(new_r,new_c) << endl;
            }
            how_many += 1;
            ++mi;
            MustBeTrue(pm[((mi+1)*2)+0] == -1);
            pm[((mi+1)*2)+0] = new_r;
            MustBeTrue(pm[((mi+1)*2)+1] == -1);
            pm[((mi+1)*2)+1] = new_c;
        }
    }
}

// generate all knight moves for a board
void 
ChessBoard::board_knight_moves(ostream &os, int &how_many)
{
    for (int r=0; r<_max_row; ++r)
    {
        for (int c=0; c<_max_col; ++c)
        {
            knight_moves(os, _moves, r, c, how_many);
        }
    }
}

// basic depth-first traversal to find a solution
bool
ChessBoard::basic_depth_first(int marker,
                              int r, int c, 
                              int max_visits, 
                              int &number_found,
                              int &bail_outs,
                              int &nodes_visited)
{
    // count how many nodes are visited
    ++nodes_visited;
    if ((max_visits>0) && (nodes_visited >= max_visits))
    {
        ++bail_outs;
        return true;
    }

    // sanity checks
    if ((r < 0) || (r >= _max_row) ||
        (c < 0) || (c >= _max_col)) 
    {
        ++bail_outs;
        return false;
    }

    // have we visited this one before? if not,
    // then mark it so we do not revisit it.
    int old_marker = value(r,c);
    if (value(r,c) < marker)
    {
        ++bail_outs;
        return false;
    }
    value(r,c) = marker;
    number_found += 1;
    marker += 1;

    // are we done?
    if (number_found == (_max_row*_max_col))
    {
        // we are done!
        return true;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int old_number_found = number_found;
    for (int move=0; el_moves[move][0] != 0; ++move)
    {
        int new_r = r+el_moves[move][0];
        int new_c = c+el_moves[move][1];
        bool done = basic_depth_first(marker,
                                      new_r, new_c, 
                                      max_visits, 
                                      number_found,
                                      bail_outs,
                                      nodes_visited);
        if (done)
        {
            return true;
        }

        number_found = old_number_found;
    }

    // reset to original values since traversal at this node failed.
    value(r,c) = old_marker;
    number_found -= 1;
    bail_outs += 1;

    return false;
}

// precalculate allowed transitions depth-first traversal to find a solution
bool
ChessBoard::precalc_depth_first(int marker,
                                int r, int c, 
                                int max_visits, 
                                int &number_found,
                                int &bail_outs,
                                int &nodes_visited)
{
    // count how many nodes are visited
    ++nodes_visited;
    if ((max_visits>0) && (nodes_visited >= max_visits))
    {
        ++bail_outs;
        return true;
    }

    // sanity checks
    if ((r < 0) || (r >= _max_row) ||
        (c < 0) || (c >= _max_col)) 
    {
        ++bail_outs;
        return false;
    }

    // have we visited this one before? if not,
    // then mark it so we do not revisit it.
    int old_marker = value(r,c);
    if (value(r,c) < marker)
    {
        ++bail_outs;
        return false;
    }
    value(r,c) = marker;
    number_found += 1;
    marker += 1;

    // are we done?
    if (number_found == (_max_row*_max_col))
    {
        // we are done!
        return true;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int old_number_found = number_found;
    int index = board(r,c)._index;
    int *pm = _moves + index*(_key_num+1)*2;
    int mi = pm[0*2+0];
    for (int i=0; i<=mi; ++i)
    {
        int new_r = pm[((i+1)*2)+0];
        int new_c = pm[((i+1)*2)+1];

        bool done = precalc_depth_first(marker,
                                        new_r, new_c, 
                                        max_visits, 
                                        number_found,
                                        bail_outs,
                                        nodes_visited);
        if (done)
        {
            return true;
        }

        number_found = old_number_found;
    }

    // reset to original values since traversal at this node failed.
    value(r,c) = old_marker;
    number_found -= 1;
    bail_outs += 1;

    return false;
}

// warnsdorff heuristic depth-first traversal to find a solution
bool
ChessBoard::warnsdorff_depth_first(int marker,
                                   int r, int c, 
                                   int max_visits, 
                                   int &number_found,
                                   int &bail_outs,
                                   int &nodes_visited)
{
    // count how many nodes are visited
    ++nodes_visited;
    if ((max_visits>0) && (nodes_visited >= max_visits))
    {
        ++bail_outs;
        return true;
    }

    // sanity checks
    if ((r < 0) || (r >= _max_row) ||
        (c < 0) || (c >= _max_col)) 
    {
        ++bail_outs;
        return false;
    }

    // have we visited this one before? if not,
    // then mark it so we do not revisit it.
    int old_marker = value(r,c);
    if (value(r,c) < marker)
    {
        ++bail_outs;
        return false;
    }
    value(r,c) = marker;
    number_found += 1;
    marker += 1;

    // are we done?
    if (number_found == (_max_row*_max_col))
    {
        // we are done!
        return true;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int old_number_found = number_found;
    int index = board(r,c)._index;
    int *pm = _moves + index*(_key_num+1)*2;
    int mi = pm[0*2+0];

    PQType pq;

    for (int i=0; i<=mi; ++i)
    {
        int new_r = pm[((i+1)*2)+0];
        int new_c = pm[((i+1)*2)+1];

        int new_index = board(new_r,new_c)._index;
        int *new_pm = _moves + new_index*(_key_num+1)*2;
        int new_mi = new_pm[0*2+0];

        int new_weight = 0;
        for (int new_i=0; new_i<=new_mi; ++new_i)
        {
            int new_new_r = new_pm[((new_i+1)*2)+0];
            int new_new_c = new_pm[((new_i+1)*2)+1];
            if (value(new_new_r,new_new_c) < marker) continue;
            new_weight += 1;
        }

        if (new_weight > 0)
        {
            pq.enqueue(PQElement(new_weight, new_r, new_c));
        }
    }

    while ( ! pq.isEmpty())
    {
        PQElement pqe;
        pq.dequeue(pqe);

        bool done = warnsdorff_depth_first(marker,
                                           pqe._r, pqe._c, 
                                           max_visits, 
                                           number_found,
                                           bail_outs,
                                           nodes_visited);
        if (done)
        {
            return true;
        }

        number_found = old_number_found;
    }

    // reset to original values since traversal at this node failed.
    value(r,c) = old_marker;
    number_found -= 1;
    bail_outs += 1;

    return false;
}

// output operator
std::ostream &
operator<<(ostream &os, const ChessBoard::Square &o)
{
    os << "(" << setfill('0') << setw(ChessBoard::Square::_field_width) << o._value << ";";
    os << setfill('0') << setw(ChessBoard::Square::_field_width) << o._index << ")";
    return os;
}

std::ostream &
operator<<(ostream &os, const ChessBoard &o)
{
    os << endl;
    for (int r=0; r<o._max_row; ++r)
    {
        os << o(r,0);
        for (int c=1; c<o._max_col; ++c)
        {
	    os << " " << o(r,c);
        }
        os << endl;
    }
    return os;
}

// basic usage
void
usage(const char *cmd)
{
    cout << "usage: " << cmd << " max_rows max_cols start_row start_col max_visits [verbose]" << endl;
}

// main entry
int
main(int argc, char **argv)
{
    NanoSecTime stopwatch;

    cout.imbue(std::locale(""));

    if (argc != 6 && argc != 7)
    {
        usage(argv[0]);
        return 2;
    }

    int max_rows;
    istringstream(argv[1]) >> max_rows;
    MustBeTrue(max_rows > 0);
    cout << endl << "max_rows: " << max_rows << endl;

    int max_cols;
    istringstream(argv[2]) >> max_cols;
    MustBeTrue(max_cols > 0);
    cout << endl << "max_cols: " << max_cols << endl;

    int start_rows;
    istringstream(argv[3]) >> start_rows;
    MustBeTrue(start_rows >= 0);
    cout << endl << "start_rows: " << start_rows << endl;

    int start_cols;
    istringstream(argv[4]) >> start_cols;
    MustBeTrue(start_cols >= 0);
    cout << endl << "start_cols: " << start_cols << endl;

    int max_visits;
    istringstream(argv[5]) >> max_visits;
    MustBeTrue(max_visits != 0);
    cout << endl << "max_visits: " << max_visits << endl;

    bool verbose = false;
    if (argc == 7)
    {
        int dummy;
        istringstream(argv[6]) >> dummy;
        verbose = (dummy == 0) ? false : true;
    }

    // build board and measure time
    stopwatch.start();
    ChessBoard b(max_rows, max_cols);
    if (verbose) 
        b.verbose(ChessBoard::VL_On);
    else
        b.verbose(ChessBoard::VL_Off);
    stopwatch.stop();
    cout << endl << "ChessBoard ctor time: " << stopwatch << endl;

    // generate all possible knight moves
    int how_many = 0;
    stopwatch.start();
    b.board_knight_moves(cout, how_many);
    stopwatch.stop();

    cout << endl << "generate all knight moves time: " << stopwatch << endl;
    cout << endl << "knight moves found: " << how_many << endl;

    // how board looks at the beginning
    cout << endl << "initial board = " << b << endl;

    // look for solution using basic depth-first search
    int visited_marker = 1;
    int nodes_visited = 0;
    int bail_outs = 0;
    int number_found = 0;

    stopwatch.start();
    bool done = b.basic_depth_first(visited_marker,
                                    start_rows, 
                                    start_cols, 
                                    max_visits, 
                                    number_found,
                                    bail_outs,
                                    nodes_visited);
    stopwatch.stop();
    cout << endl << "basic depth first time: " << stopwatch << endl;

    cout << endl << "number needed = " << (max_rows*max_cols) << endl;
    cout << "number found = " << number_found << endl;
    cout << "bail outs = " << bail_outs << endl;
    cout << "max visits = " << max_visits << endl; 
    cout << "nodes visited = " << nodes_visited << endl;;

    if (number_found == (max_rows*max_cols))
        cout << endl << "solution FOUND ..." << endl;
    else
        cout << endl << "solution NOT FOUND ..." << endl;

    cout << endl << "final board = " << b << endl;

    // look for solution using precalc depth-first search
    visited_marker = 1;
    nodes_visited = 0;
    bail_outs = 0;
    number_found = 0;

    // how board looks at the beginning
    b.reset();
    cout << endl << "reset board = " << b << endl;

    stopwatch.start();
    done = b.precalc_depth_first(visited_marker,
                                 start_rows, 
                                 start_cols, 
                                 max_visits, 
                                 number_found,
                                 bail_outs,
                                 nodes_visited);
    stopwatch.stop();
    cout << endl << "precalc depth first time: " << stopwatch << endl;

    cout << endl << "number needed = " << (max_rows*max_cols) << endl;
    cout << "number found = " << number_found << endl;
    cout << "bail outs = " << bail_outs << endl;
    cout << "max visits = " << max_visits << endl; 
    cout << "nodes visited = " << nodes_visited << endl;;

    if (number_found == (max_rows*max_cols))
        cout << endl << "solution FOUND ..." << endl;
    else
        cout << endl << "solution NOT FOUND ..." << endl;

    cout << endl << "final board = " << b << endl;

    // look for solution using warnsdorff heuristic depth-first search
    visited_marker = 1;
    nodes_visited = 0;
    bail_outs = 0;
    number_found = 0;

    // how board looks at the beginning
    b.reset();
    cout << endl << "reset board = " << b << endl;

    stopwatch.start();
    done = b.warnsdorff_depth_first(visited_marker,
                                    start_rows, 
                                    start_cols, 
                                    max_visits, 
                                    number_found,
                                    bail_outs,
                                    nodes_visited);
    stopwatch.stop();
    cout << endl << "warnsdorff depth first time: " << stopwatch << endl;

    cout << endl << "number needed = " << (max_rows*max_cols) << endl;
    cout << "number found = " << number_found << endl;
    cout << "bail outs = " << bail_outs << endl;
    cout << "max visits = " << max_visits << endl; 
    cout << "nodes visited = " << nodes_visited << endl;;

    if (number_found == (max_rows*max_cols))
        cout << endl << "solution FOUND ..." << endl;
    else
        cout << endl << "solution NOT FOUND ..." << endl;

    cout << endl << "final board = " << b << endl;

    return 0;
}

