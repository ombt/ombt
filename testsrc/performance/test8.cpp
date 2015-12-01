//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include "system/Debug.h"
#include "performance/NanoSecTime.h"

using namespace ombt;

// headers
#include <time.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <sstream>

// cheap way
using namespace std;

//
// key board
//
class KeyBoard
{
public: // data types and constants
    // constants
    static const int max_row = 4;
    static const int max_col = 5;
    static const char null_key = ' ';
    static const int el_moves[][2];

    // data types
    class Key
    {
    public:
        inline Key(int val = null_key, bool vow = false, int idx = -1):
            _value(val), _vowel(vow), _index(idx) { }
        inline Key(const Key &src):
            _value(src._value), 
            _vowel(src._vowel),
            _index(src._index) { }
        inline ~Key() { }
        inline Key &operator=(const Key &rhs)
        {
            if (this != &rhs)
            {
                _value = rhs._value;
                _vowel = rhs._vowel;
                _index = rhs._index;
            }
            return *this;
        }
        friend ostream &operator<<(ostream &, const Key &);

        char _value; // value of key
        bool _vowel; // is it a vowel?
        int  _index; // index into arrays
    };

    // type for storing allowed moves per key.
    typedef int * AllowedMoves;

public:
    // ctors and dtor
    KeyBoard();
    KeyBoard(const KeyBoard &);
    ~KeyBoard();

    // assignment
    KeyBoard &operator=(const KeyBoard &);

    // output
    friend ostream &operator<<(ostream &, const KeyBoard &);

    // utilities
    inline char key(int r, int c) const
    {
        MustBeTrue((0 <= r) && (r < max_row));
        MustBeTrue((0 <= c) && (c < max_col));
        return _board[r][c]._value;
    }
    inline bool vowel(int r, int c) const
    {
        MustBeTrue((0 <= r) && (r < max_row));
        MustBeTrue((0 <= c) && (c < max_col));
        return _board[r][c]._vowel;
    }

    // generate the allowed L-moves given a key
    void one_vowel_moves(int r, int c, int &one_or_less, int &one_only);
    void zero_vowel_moves(int r, int c, int &zero_only);
    void vowel_moves(int vowels, 
                     AllowedMoves &moves,
                     int r, int c,
                     int &how_many,
                     bool exact);
    bool check_if_allowed(int vowels, int r, int c, bool exact) const;

    // actual crunching routines
    void depth_first(int sequence_number, 
                         int number_of_vowels, 
                         int row, 
                         int col,
                         long &seq_found,
                         long &bail_outs,
                         long &nodes_visited);
    void depth_first_tail_optim(int sequence_number, 
                                    int number_of_vowels, 
                                    int row, 
                                    int col,
                                    long &seq_found,
                                    long &bail_outs,
                                    long &nodes_visited);

protected:
    // internal board representation
    Key _board[max_row][max_col];

    // end-keys after L-step
    int _imax;
    int _key_num;
    AllowedMoves _one_or_less_vowel;
    AllowedMoves _one_vowel;
    AllowedMoves _zero_vowel;
};

// key board class

// allowed horizontal and vertical deltas
const int KeyBoard::el_moves[][2] = {
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

// ctors and dtor
KeyBoard::KeyBoard()
{
    _key_num = 0;

    // first row
    _board[0][0] = Key('A', true, _key_num);
    _board[0][1] = Key('B', false, ++_key_num);
    _board[0][2] = Key('C', false, ++_key_num);
    _board[0][3] = Key('D', false, ++_key_num);
    _board[0][4] = Key('E', true, ++_key_num);

    // second row
    _board[1][0] = Key('F', false, ++_key_num);
    _board[1][1] = Key('G', false, ++_key_num);
    _board[1][2] = Key('H', false, ++_key_num);
    _board[1][3] = Key('I', true, ++_key_num);
    _board[1][4] = Key('J', false, ++_key_num);

    // third row
    _board[2][0] = Key('K', false, ++_key_num);
    _board[2][1] = Key('L', false, ++_key_num);
    _board[2][2] = Key('M', false, ++_key_num);
    _board[2][3] = Key('N', false, ++_key_num);
    _board[2][4] = Key('O', true, ++_key_num);

    // fourth row
    // _board[3][0] = Key();
    _board[3][1] = Key('1', false, ++_key_num);
    _board[3][2] = Key('2', false, ++_key_num);
    _board[3][3] = Key('3', false, ++_key_num);
    // _board[3][4] = Key();

    // allocate and init allowed move lists
    ++_key_num;

    _imax = _key_num*(_key_num+1)*2;

    _one_or_less_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _one_or_less_vowel[i] = -1;
    }
    _one_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _one_vowel[i] = -1;
    }
    _zero_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _zero_vowel[i] = -1;
    }
}

KeyBoard::KeyBoard(const KeyBoard &src)
{
    ::memcpy(_board, src._board, sizeof(_board));

    _imax = src._imax;
    _key_num = src._key_num;

    _one_or_less_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _one_or_less_vowel[i] = src._one_vowel[i];
    }
    _one_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _one_vowel[i] = src._one_vowel[i];
    }
    _zero_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _zero_vowel[i] = src._zero_vowel[i];
    }
}

KeyBoard::~KeyBoard()
{
}

// assignment operator
KeyBoard &
KeyBoard::operator=(const KeyBoard &rhs)
{
    if (this != &rhs)
    {
        ::memcpy(_board, rhs._board, sizeof(_board));

        _imax = rhs._imax;
        _key_num = rhs._key_num;

        delete [] _one_or_less_vowel;
        _one_or_less_vowel  = new int [_imax];
        for (int i=0; i<_imax; ++i)
        {
            _one_or_less_vowel[i] = rhs._one_or_less_vowel[i];
        }

        delete [] _one_vowel;
        _one_vowel  = new int [_imax];
        for (int i=0; i<_imax; ++i)
        {
            _one_vowel[i] = rhs._one_vowel[i];
        }

        delete [] _zero_vowel;
        _zero_vowel  = new int [_imax];
        for (int i=0; i<_imax; ++i)
        {
            _zero_vowel[i] = rhs._zero_vowel[i];
        }
    }

    return *this;
}

// test if move is legal
bool 
KeyBoard::check_if_allowed(int vowels, int r, int c, bool exact) const
{
    if (((0 <= r) && (r < max_row)) &&
        ((0 <= c) && (c < max_col)) &&
        (_board[r][c]._value != null_key))
    {
        if (_board[r][c]._vowel) --vowels;
        if (exact)
        {
            if (vowels == 0)
                return true;
            else
                return false;
        }
        else
        {
            if (vowels >= 0)
                return true;
            else
                return false;
        }
    }
    else
    {
        return false;
    }
}

// generate all single moves with the given number of vowels
void
KeyBoard::vowel_moves(int vowels, 
                      AllowedMoves &moves,
                      int r, 
                      int c,
                      int &how_many,
                      bool exact)
{
    // sanity checks
    MustBeTrue((0 <= r) && (r < max_row));
    MustBeTrue((0 <= c) && (c < max_col));
    MustBeTrue(0 <= vowels);

    // try moves with the given number of vowels or less 
    if (_board[r][c]._value != null_key)
    {
DUMP2(r,c);
        int index = _board[r][c]._index;
        int *pm = moves + index*(_key_num+1)*2;
        int &mi = (pm[0*2+0]);
DUMP(mi);

        for (int move=0; el_moves[move][0] != 0; ++move)
        {
            int new_r = r+el_moves[move][0];
            int new_c = c+el_moves[move][1];
            if (check_if_allowed(vowels, new_r, new_c, exact))
            {
                how_many += 1;
DUMP(mi);
                ++mi;
DUMP(mi);
                MustBeTrue(pm[((mi+1)*2)+0] == -1);
                pm[((mi+1)*2)+0] = new_r;
                MustBeTrue(pm[((mi+1)*2)+1] == -1);
                pm[((mi+1)*2)+1] = new_c;
            }
        }
DUMP(mi);
    }
}

// generate moves with one or less vowels
void
KeyBoard::one_vowel_moves(int r, int c, int &one_or_less, int &one_only)
{
DUMP2(r,c);
DUMP(one_or_less);
   vowel_moves(1, _one_or_less_vowel, r, c, one_or_less, false);
DUMP(one_or_less);
DUMP2(r,c);
DUMP(one_only);
   vowel_moves(1, _one_vowel, r, c, one_only, true);
DUMP(one_only);
}

// generate moves with no vowels
void
KeyBoard::zero_vowel_moves(int r, int c, int &zero_only)
{
DUMP2(r,c);
DUMP(zero_only);
   vowel_moves(0, _zero_vowel, r, c, zero_only, true);
DUMP(zero_only);
}

// real routines to generate sequences
void
KeyBoard::depth_first(int seq_num, 
                          int vowels, 
                          int r, 
                          int c,
                          long &seq_found,
                          long &bail_outs,
                          long &nodes_visited)
{
DUMP2(r,c);
    // count how many nodes are visited
DUMP(nodes_visited);
    ++nodes_visited;
DUMP(nodes_visited);

    // is this a vowel?
DUMP(vowels);
    if (_board[r][c]._vowel) --vowels;
DUMP(vowels);

    // is the number of vowels valid?
    if (vowels < 0) 
    {
DUMP(bail_outs);
        ++bail_outs;
DUMP(bail_outs);
        return;
    }

    // we found another character
    // is this the end of a sequence?
DUMP2(seq_num, seq_found);
    if (--seq_num == 0)
    {
        seq_found += 1;
DUMP2(seq_num, seq_found);
        return;
    }
DUMP2(seq_num, seq_found);

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;
DUMP(index);
DUMP2(_board[r][c]._index, _board[r][c]._value);
    if (vowels >= 1)
    {
DUMP(vowels);
        register int *pm = _one_or_less_vowel + index*(_key_num+1)*2;
DUMP(*(_one_or_less_vowel + index*(_key_num+1)*2));
        register int mi = *pm;
DUMP(mi);
        for (int i=0; i<=mi; ++i)
        {
DUMP(vowels);
            register int *pmi = pm + (i+1)*2;
            depth_first(seq_num, 
                            vowels, 
                            *pmi, 
                            *(pmi+1), 
                            seq_found,
                            bail_outs,
                            nodes_visited);
        }
    }
    else
    {
DUMP(vowels);
        register int *pm = _zero_vowel + index*(_key_num+1)*2;
DUMP(*(_zero_vowel + index*(_key_num+1)*2));
        register int mi = *pm;
DUMP(mi);
        for (int i=0; i<=mi; ++i)
        {
DUMP(vowels);
            register int *pmi = pm + (i+1)*2;
            depth_first(seq_num, 
                            vowels, 
                            *pmi, 
                            *(pmi+1), 
                            seq_found,
                            bail_outs,
                            nodes_visited);
        }
    }
}

void
KeyBoard::depth_first_tail_optim(int seq_num, 
                                     int vowels, 
                                     int r, 
                                     int c,
                                     long &seq_found,
                                     long &bail_outs,
                                     long &nodes_visited)
{
DUMP2(r,c);
    // count how many nodes are visited
DUMP(nodes_visited);
    ++nodes_visited;
DUMP(nodes_visited);

    // is this a vowel?
DUMP(vowels);
    if (_board[r][c]._vowel) --vowels;
DUMP(vowels);

    // is the number of vowels valid?
    if (vowels < 0) 
    {
DUMP(bail_outs);
        ++bail_outs;
DUMP(bail_outs);
        return;
    }

    // we found another character
    // is this the end of a sequence?
DUMP2(seq_num, seq_found);
    if (--seq_num == 0)
    {
        seq_found += 1;
DUMP2(seq_num, seq_found);
        return;
    }
DUMP2(seq_num, seq_found);

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;
DUMP(index);
DUMP2(_board[r][c]._index, _board[r][c]._value);
    if (seq_num > 1)
    {
TRACE();
        if (vowels >= 1)
        {
DUMP(vowels);
            register int *pm = _one_or_less_vowel + index*(_key_num+1)*2;
DUMP(*(_one_or_less_vowel + index*(_key_num+1)*2));
            register int mi = *pm;
DUMP(mi);
            for (int i=0; i<=mi; ++i)
            {
DUMP(vowels);
                register int *pmi = pm + (i+1)*2;
                depth_first_tail_optim(seq_num, 
                                           vowels, 
                                           *pmi, 
                                           *(pmi+1), 
                                           seq_found,
                                           bail_outs,
                                           nodes_visited);
            }
        }
        else
        {
DUMP(vowels);
            register int *pm = _zero_vowel + index*(_key_num+1)*2;
DUMP(*(_zero_vowel + index*(_key_num+1)*2));
            register int mi = *pm;
DUMP(mi);
            for (int i=0; i<=mi; ++i)
            {
DUMP(vowels);
                register int *pmi = pm + (i+1)*2;
                depth_first_tail_optim(seq_num, 
                                           vowels, 
                                           *pmi, 
                                           *(pmi+1), 
                                           seq_found,
                                           bail_outs,
                                           nodes_visited);
            }
        }
    }
    else
    {
TRACE();
        if (vowels == 1)
        {
DUMP(vowels);
            register int *pm = _one_vowel + index*(_key_num+1)*2;
DUMP(*(_one_vowel + index*(_key_num+1)*2));
            register int mi = *pm;
DUMP(mi);
            for (int i=0; i<=mi; ++i)
            {
DUMP(vowels);
                register int *pmi = pm + (i+1)*2;
                depth_first_tail_optim(seq_num, 
                                           vowels, 
                                           *pmi, 
                                           *(pmi+1), 
                                           seq_found,
                                           bail_outs,
                                           nodes_visited);
            }
        }
        else if (vowels == 0)
        {
DUMP(vowels);
            register int *pm = _zero_vowel + index*(_key_num+1)*2;
DUMP(*(_zero_vowel + index*(_key_num+1)*2));
            register int mi = *pm;
DUMP(mi);
            for (int i=0; i<=mi; ++i)
            {
DUMP(vowels);
                register int *pmi = pm + (i+1)*2;
                depth_first_tail_optim(seq_num, 
                                           vowels, 
                                           *pmi, 
                                           *(pmi+1), 
                                           seq_found,
                                           bail_outs,
                                           nodes_visited);
            }
        }
        else
        {
DUMP(vowels);
            ++bail_outs;
            return;
        }
    }
}

// output operator
std::ostream &
operator<<(ostream &os, const KeyBoard::Key &o)
{
    os << "(" << o._value << ";";
    if (o._vowel)
    {
        os << "T;";
    }
    else
    {
        os << "F;";
    }
    os << o._index << ")";
    return os;
}

std::ostream &
operator<<(ostream &os, const KeyBoard &o)
{
    os << endl;
    for (int r=0; r<KeyBoard::max_row; ++r)
    {
        os << o._board[r][0];
        for (int c=1; c<KeyBoard::max_col; ++c)
        {
	    os << " " << o._board[r][c];
        }
        os << endl;
    }
    return os;
}

// basic usage
void
usage(const char *cmd)
{
    cout << "usage: " << cmd << " sequence_length number_of_vowels [verbose]" << endl;
}

// main entry
int
main(int argc, char **argv)
{
    NanoSecTime stopwatch;

    cout.imbue(std::locale(""));

    if (argc != 3 && argc != 4)
    {
        usage(argv[0]);
        return 2;
    }

    int sequence_length;
    istringstream(argv[1]) >> sequence_length;
    MustBeTrue(sequence_length > 0);
    cout << endl << "Sequence Length: " << sequence_length << endl;

    int number_of_vowels;
    istringstream(argv[2]) >> number_of_vowels;
    MustBeTrue(number_of_vowels >= 0);
    cout << endl << "Number of Vowels: " << number_of_vowels << endl;

    bool verbose = false;
    if (argc == 4)
    {
        int dummy;
        istringstream(argv[3]) >> dummy;
        verbose = (dummy == 0) ? false : true;
    }


    // build board and measure time
    stopwatch.start();
    KeyBoard b;
    stopwatch.stop();
    cout << endl << "KeyBoard ctor time: " << stopwatch << endl;

    cout << endl << "initial board = " << b << endl;

    // test routines for checking basic ideas.
    int one_or_less = 0;
    int one_only = 0;
    int zero_only = 0;
    stopwatch.start();
    for (int r=0; r<KeyBoard::max_row; ++r)
    {
        for (int c=0; c<KeyBoard::max_col; ++c)
        {
            b.one_vowel_moves(r, c, one_or_less, one_only);
            b.zero_vowel_moves(r, c, zero_only);
        }
    }
    stopwatch.stop();
    cout << endl << "Generate allowed moves time: " << stopwatch << endl;
    cout << "Generated One-or-Less moves: " << one_or_less << endl;
    cout << "Generated One-only moves: " << one_only << endl;
    cout << "Generated Zero-only moves: " << zero_only << endl;

    cout << endl << "generate sequences for the given length " 
            "and with the given number of vowels:" << endl;

    cout << endl << "Count only ..." << endl;
    NanoSecTime cnt_s;
    long cnt_seqs = 0;
    long cnt_bails = 0;
    long cnt_nodes = 0;
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            // since most checks are removed from
            // sequence function and it assume *only*
            // legitimate moves are made, we need to remove
            // the two blank, ie, non-keys.
            if (b.key(row,col) != KeyBoard::null_key)
            {
                b.depth_first(sequence_length, 
                                  number_of_vowels, 
                                  row, 
                                  col,
                                  cnt_seqs,
                                  cnt_bails,
                                  cnt_nodes);
            }
        }
    }
    NanoSecTime cnt_e;
    cout << "Number of sequences found (count only): " << cnt_seqs << endl;
    cout << "Number of bail outs (count only): " << cnt_bails << endl;
    cout << "Number of nodes visited (count only): " << cnt_nodes << endl;
    NanoSecTime cnt_diff = cnt_e-cnt_s;
    cout << "Time (count only): " << cnt_diff << endl;

    cout << endl << "Count tail optim only ..." << endl;
    NanoSecTime cnt_to_s;
    long cnt_to_seqs = 0;
    long cnt_to_bails = 0;
    long cnt_to_nodes = 0;
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            // since most checks are removed from
            // sequence function and it assume *only*
            // legitimate moves are made, we need to remove
            // the two blank, ie, non-keys.
            if (b.key(row,col) != KeyBoard::null_key)
            {
                b.depth_first_tail_optim(sequence_length, 
                                         number_of_vowels, 
                                         row, 
                                         col,
                                         cnt_to_seqs,
                                         cnt_to_bails,
                                         cnt_to_nodes);
            }
        }
    }
    NanoSecTime cnt_to_e;
    cout << "Number of sequences found (count tail optim only): " << cnt_to_seqs << endl;
    cout << "Number of bail outs (count tail optim only): " << cnt_to_bails << endl;
    cout << "Number of nodes visited (count tail optim only): " << cnt_to_nodes << endl;
    NanoSecTime cnt_to_diff = cnt_to_e-cnt_to_s;
    cout << "Time (count tail optim only): " << cnt_to_diff << endl;

    return 0;
}

