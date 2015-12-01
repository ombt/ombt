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
    void all_moves(ostream &os, int r, int c) const;
    void one_vowel_moves(ostream &os, int r, int c);
    void zero_vowel_moves(ostream &os, int r, int c);
    void vowel_moves(ostream &os, 
                     int vowels, 
                     AllowedMoves &moves,
                     int r, int c,
                     int &how_many,
                     bool exact);
    bool check_if_allowed(int vowels, int r, int c, bool exact) const;

    // actual crunching routines
    void brute_force_sequences(ostream &os, 
                               string sequence, // pass by value to isolate
                               int sequence_number, 
                               int number_of_vowels, 
                               int row, 
                               int col,
                               bool verbose,
                               long &seq_found,
                               long &bail_outs,
                               long &nodes_visited);
    void smart_sequences(ostream &os, 
                         string sequence, // pass by value to isolate
                         int sequence_number, 
                         int number_of_vowels, 
                         int row, 
                         int col,
                         bool verbose,
                         long &seq_found,
                         long &bail_outs,
                         long &nodes_visited);
    void smart_sequences_wo_chks(ostream &os, 
                                 string sequence, // pass by value to isolate
                                 int sequence_number, 
                                 int number_of_vowels, 
                                 int row, 
                                 int col,
                                 bool verbose,
                                 long &seq_found,
                                 long &bail_outs,
                                 long &nodes_visited);
    void count_sequences(int sequence_number, 
                         int number_of_vowels, 
                         int row, 
                         int col,
                         long &seq_found,
                         long &bail_outs,
                         long &nodes_visited);
    void count_sequences_tail_optim(int sequence_number, 
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
    AllowedMoves _zero_vowel;
    AllowedMoves _one_vowel;
    AllowedMoves _exact_zero_vowel;
    AllowedMoves _exact_one_vowel;
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

    _exact_one_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _exact_one_vowel[i] = -1;
    }
    _exact_zero_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _exact_zero_vowel[i] = -1;
    }
}

KeyBoard::KeyBoard(const KeyBoard &src)
{
    ::memcpy(_board, src._board, sizeof(_board));

    _imax = src._imax;
    _key_num = src._key_num;

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

    _exact_one_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _exact_one_vowel[i] = src._exact_one_vowel[i];
    }
    _exact_zero_vowel  = new int [_imax];
    for (int i=0; i<_imax; ++i)
    {
        _exact_zero_vowel[i] = src._exact_zero_vowel[i];
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
    
        delete [] _exact_one_vowel;
        _exact_one_vowel  = new int [_imax];
        for (int i=0; i<_imax; ++i)
        {
            _exact_one_vowel[i] = rhs._exact_one_vowel[i];
        }

        delete [] _exact_zero_vowel;
        _exact_zero_vowel  = new int [_imax];
        for (int i=0; i<_imax; ++i)
        {
            _exact_zero_vowel[i] = rhs._exact_zero_vowel[i];
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

// generate all single moves from a given key
void
KeyBoard::all_moves(ostream &os, int r, int c) const
{
    // sanity checks
    MustBeTrue((0<= r) && (r < max_row));
    MustBeTrue((0<= c) && (c < max_col));

    // try all moves
    int count = 0;
    if (_board[r][c]._value != null_key)
    {
        int vowels = 2;
        for (int move=0; el_moves[move][0] != 0; ++move)
        {
            int new_r = r+el_moves[move][0];
            int new_c = c+el_moves[move][1];
            if (check_if_allowed(vowels, new_r, new_c, false))
            {
                os << _board[new_r][new_c] << endl;
                ++count;
            }
        }
    }
    os << "Total allowed moves: " << count << endl;
}

// generate all single moves with the given number of vowels
void
KeyBoard::vowel_moves(ostream &os, 
                      int vowels, 
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
        int index = _board[r][c]._index;
        int *pm = moves + index*(_key_num+1)*2;
        int &mi = (pm[0*2+0]);

        for (int move=0; el_moves[move][0] != 0; ++move)
        {
            int new_r = r+el_moves[move][0];
            int new_c = c+el_moves[move][1];
            if (check_if_allowed(vowels, new_r, new_c, exact))
            {
                os << _board[new_r][new_c] << endl;
                how_many += 1;
                ++mi;
                MustBeTrue(pm[((mi+1)*2)+0] == -1);
                pm[((mi+1)*2)+0] = new_r;
                MustBeTrue(pm[((mi+1)*2)+1] == -1);
                pm[((mi+1)*2)+1] = new_c;
            }
        }
    }
}

// generate moves with one or less vowels
void
KeyBoard::one_vowel_moves(ostream &os, int r, int c)
{
   int how_many = 0;
   vowel_moves(os, 1, _one_vowel, r, c, how_many, false);
   os << "Size of one-vowel list of allowed moves: " 
      << how_many << endl;

   how_many = 0;
   vowel_moves(os, 1, _exact_one_vowel, r, c, how_many, true);
   os << "Size of exact one-vowel list of allowed moves: " 
      << how_many << endl;
}

// generate moves with no vowels
void
KeyBoard::zero_vowel_moves(ostream &os, int r, int c)
{
   int how_many = 0;
   vowel_moves(os, 0, _zero_vowel, r, c, how_many, false);
   os << "Size of zero-vowel list of allowed moves: " 
      << how_many << endl;

   how_many = 0;
   vowel_moves(os, 0, _exact_zero_vowel, r, c, how_many, true);
   os << "Size of exact zero-vowel list of allowed moves: " 
      << how_many << endl;
}

// real routines to generate sequences
void
KeyBoard::brute_force_sequences(ostream &os, 
                                string seq, 
                                int seq_num, 
                                int vowels, 
                                int r, 
                                int c,
                                bool verbose,
                                long &seq_found,
                                long &bail_outs,
                                long &nodes_visited)
{
    // count how many nodes are visited
    ++nodes_visited;

    // sanity checks
    if ((r < 0) || (r >= max_row) ||
        (c < 0) || (c >= max_col)) 
    {
        ++bail_outs;
        return;
    }

    // is this an acceptable character?
    if (_board[r][c]._value == null_key)
    {
        ++bail_outs;
        return;
    }

    // is this a vowel?
    if (_board[r][c]._vowel) --vowels;

    // is the number of vowels valid?
    if (vowels < 0)
    {
        ++bail_outs;
        return;
    }

    // we found another character
    if (verbose) seq += _board[r][c]._value;
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        // did we fulfill the required number of vowels?
        // if (vowels == 0) os << seq << endl;
        if (verbose) os << seq << endl;
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    for (int move=0; el_moves[move][0] != 0; ++move)
    {
        int new_r = r+el_moves[move][0];
        int new_c = c+el_moves[move][1];
        brute_force_sequences(os, 
                              seq, 
                              seq_num, 
                              vowels, 
                              new_r, 
                              new_c, 
                              verbose, 
                              seq_found,
                              bail_outs,
                              nodes_visited);
    }
}

void
KeyBoard::smart_sequences(ostream &os, 
                          string seq, 
                          int seq_num, 
                          int vowels, 
                          int r, 
                          int c,
                          bool verbose,
                          long &seq_found,
                          long &bail_outs,
                          long &nodes_visited)
{
    // count how many nodes are visited
    ++nodes_visited;

    // sanity checks
    if ((r < 0) || (r >= max_row) ||
        (c < 0) || (c >= max_col)) 
    {
        ++bail_outs;
        return;
    }

    // is this an acceptable character?
    if (_board[r][c]._value == null_key) 
    {
        ++bail_outs;
        return;
    }

    // is this a vowel?
    if (_board[r][c]._vowel) --vowels;

    // is the number of vowels valid?
    if (vowels < 0) 
    {
        ++bail_outs;
        return;
    }

    // we found another character
    if (verbose) seq += _board[r][c]._value;
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        if (verbose) os << seq << endl;
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;
    if (vowels >= 1)
    {
        int *pm = _one_vowel + index*(_key_num+1)*2;
        int mi = pm[0*2+0];
        for (int i=0; i<=mi; ++i)
        {
            int new_r = pm[((i+1)*2)+0];
            int new_c = pm[((i+1)*2)+1];
            smart_sequences(os, 
                            seq, 
                            seq_num, 
                            vowels, 
                            new_r, 
                            new_c, 
                            verbose, 
                            seq_found,
                            bail_outs,
                            nodes_visited);
        }
    }
    else
    {
        int *pm = _zero_vowel + index*(_key_num+1)*2;
        int mi = pm[0*2+0];
        for (int i=0; i<=mi; ++i)
        {
            int new_r = pm[((i+1)*2)+0];
            int new_c = pm[((i+1)*2)+1];
            smart_sequences(os, 
                            seq, 
                            seq_num, 
                            vowels, 
                            new_r, 
                            new_c, 
                            verbose, 
                            seq_found,
                            bail_outs,
                            nodes_visited);
        }
    }
}

void
KeyBoard::smart_sequences_wo_chks(ostream &os, 
                                  string seq, 
                                  int seq_num, 
                                  int vowels, 
                                  int r, 
                                  int c,
                                  bool verbose,
                                  long &seq_found,
                                  long &bail_outs,
                                  long &nodes_visited)
{
    // count how many nodes are visited
    ++nodes_visited;

    // is this a vowel?
    if (_board[r][c]._vowel) --vowels;

    // is the number of vowels valid?
    if (vowels < 0) 
    {
        ++bail_outs;
        return;
    }

    // we found another character
    if (verbose) seq += _board[r][c]._value;
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        if (verbose) os << seq << endl;
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;
    if (vowels >= 1)
    {
        register int *pm = _one_vowel + index*(_key_num+1)*2;
        register int mi = pm[0*2+0];
        for (int i=0; i<=mi; ++i)
        {
            register int *pmi = pm + (i+1)*2;
            smart_sequences_wo_chks(os, 
                                    seq, 
                                    seq_num, 
                                    vowels, 
                                    *pmi, 
                                    *(pmi+1), 
                                    verbose, 
                                    seq_found,
                                    bail_outs,
                                    nodes_visited);
        }
    }
    else
    {
        register int *pm = _zero_vowel + index*(_key_num+1)*2;
        register int mi = pm[0*2+0];
        for (int i=0; i<=mi; ++i)
        {
            register int *pmi = pm + (i+1)*2;
            smart_sequences_wo_chks(os, 
                                    seq, 
                                    seq_num, 
                                    vowels, 
                                    *pmi, 
                                    *(pmi+1), 
                                    verbose, 
                                    seq_found,
                                    bail_outs,
                                    nodes_visited);
        }
    }
}

void
KeyBoard::count_sequences(int seq_num, 
                          int vowels, 
                          int r, 
                          int c,
                          long &seq_found,
                          long &bail_outs,
                          long &nodes_visited)
{
    // count how many nodes are visited
    ++nodes_visited;

    // is this a vowel?
    if (_board[r][c]._vowel) --vowels;

    // is the number of vowels valid?
    if (vowels < 0) 
    {
        ++bail_outs;
        return;
    }

    // we found another character
    // is this the end of a sequence?
    if (--seq_num == 0)
    {
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;
    if (vowels >= 1)
    {
        register int *pm = _one_vowel + index*(_key_num+1)*2;
        register int mi = *pm;
        for (int i=0; i<=mi; ++i)
        {
            register int *pmi = pm + (i+1)*2;
            count_sequences(seq_num, 
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
        register int *pm = _zero_vowel + index*(_key_num+1)*2;
        register int mi = *pm;
        for (int i=0; i<=mi; ++i)
        {
            register int *pmi = pm + (i+1)*2;
            count_sequences(seq_num, 
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
KeyBoard::count_sequences_tail_optim(int seq_num, 
                                     int vowels, 
                                     int r, 
                                     int c,
                                     long &seq_found,
                                     long &bail_outs,
                                     long &nodes_visited)
{
    // count how many nodes are visited
    ++nodes_visited;

    // is this a vowel?
    if (_board[r][c]._vowel) --vowels;

    // is the number of vowels valid?
    if (vowels < 0) 
    {
        ++bail_outs;
        return;
    }

    // we found another character
    // is this the end of a sequence?
    if (--seq_num == 0)
    {
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;
    if (seq_num > 1)
    {
        if (vowels >= 1)
        {
            register int *pm = _one_vowel + index*(_key_num+1)*2;
            register int mi = *pm;
            for (int i=0; i<=mi; ++i)
            {
                register int *pmi = pm + (i+1)*2;
                count_sequences_tail_optim(seq_num, 
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
            register int *pm = _zero_vowel + index*(_key_num+1)*2;
            register int mi = *pm;
            for (int i=0; i<=mi; ++i)
            {
                register int *pmi = pm + (i+1)*2;
                count_sequences_tail_optim(seq_num, 
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
        if (vowels == 1)
        {
            register int *pm = _exact_one_vowel + index*(_key_num+1)*2;
            register int mi = *pm;
            for (int i=0; i<=mi; ++i)
            {
                register int *pmi = pm + (i+1)*2;
                count_sequences_tail_optim(seq_num, 
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
            register int *pm = _exact_zero_vowel + index*(_key_num+1)*2;
            register int mi = *pm;
            for (int i=0; i<=mi; ++i)
            {
                register int *pmi = pm + (i+1)*2;
                count_sequences_tail_optim(seq_num, 
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
    cout << endl << "allowed moves per key = " << endl;
    for (int r=0; r<KeyBoard::max_row; ++r)
    {
        for (int c=0; c<KeyBoard::max_col; ++c)
        {
            cout << endl << ">>>> Generate moves for key: " 
                         << b.key(r, c) << " <<<<" << endl;

            cout << endl << ">>>> generate all allowed moves for key: " 
                 << b.key(r, c) << endl;
            b.all_moves(cout, r, c);

            cout << endl << ">>>> generate allowed one vowel moves for key: " 
                 << b.key(r, c) << endl;
            b.one_vowel_moves(cout, r, c);

            cout << endl << ">>>> generate allowed zero vowel moves for key: " 
                 << b.key(r, c) << endl;
            b.zero_vowel_moves(cout, r, c);
        }
    }

    cout << "generate sequences for the given length " 
            "and with the given number of vowels:" << endl;
    cout << "Sequence Length: " << sequence_length << endl;
    cout << "Number of Vowels: " << number_of_vowels << endl;

    cout << endl << "Brute-force crunching ..." << endl;
    NanoSecTime br_s;
    long br_seqs = 0;
    long br_nodes = 0;
    long br_bails = 0;
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            string sequence("");
            b.brute_force_sequences(cout, 
                                    sequence, 
                                    sequence_length, 
                                    number_of_vowels, 
                                    row, 
                                    col,
                                    verbose,
                                    br_seqs,
                                    br_bails,
                                    br_nodes);
        }
    }
    NanoSecTime br_e;
    cout << "Number of sequences found (brute force): " << br_seqs << endl;
    cout << "Number of bails outs (brute force): " << br_bails << endl;
    cout << "Number of nodes visited (brute force): " << br_nodes << endl;
    NanoSecTime br_diff = br_e-br_s;
    cout << "Time (brute force): " << br_diff << endl;

    cout << endl << "Smart crunching ..." << endl;
    NanoSecTime sm_s;
    long sm_seqs = 0;
    long sm_nodes = 0;
    long sm_bails = 0;
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            string sequence("");
            b.smart_sequences(cout, 
                              sequence, 
                              sequence_length, 
                              number_of_vowels, 
                              row, 
                              col,
                              verbose,
                              sm_seqs,
                              sm_bails,
                              sm_nodes);
        }
    }
    NanoSecTime sm_e;
    cout << "Number of sequences found (smart): " << sm_seqs << endl;
    cout << "Number of bail outs (smart): " << sm_bails << endl;
    cout << "Number of nodes visited (smart): " << sm_nodes << endl;
    NanoSecTime sm_diff = sm_e-sm_s;
    cout << "Time (smart): " << sm_diff << endl;

    cout << endl << "Smart crunching without checks ..." << endl;
    NanoSecTime sm_wo_cks_s;
    long sm_wo_cks_seqs = 0;
    long sm_wo_cks_bails = 0;
    long sm_wo_cks_nodes = 0;
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
                string sequence("");
                b.smart_sequences_wo_chks(cout, 
                                          sequence, 
                                          sequence_length, 
                                          number_of_vowels, 
                                          row, 
                                          col,
                                          verbose,
                                          sm_wo_cks_seqs,
                                          sm_wo_cks_bails,
                                          sm_wo_cks_nodes);
            }
        }
    }
    NanoSecTime sm_wo_cks_e;
    cout << "Number of sequences found (smart without checks): " << sm_wo_cks_seqs << endl;
    cout << "Number of bails outs (smart without checks): " << sm_wo_cks_bails << endl;
    cout << "Number of nodes visited (smart without checks): " << sm_wo_cks_nodes << endl;
    NanoSecTime sm_wo_cks_diff = sm_wo_cks_e-sm_wo_cks_s;
    cout << "Time (smart without checks): " << sm_wo_cks_diff << endl;

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
                b.count_sequences(sequence_length, 
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
                b.count_sequences_tail_optim(sequence_length, 
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

#undef CHECKCMP
#define CHECKCMP(EXPR) \
    if (EXPR) \
    { \
        cout << "(" << #EXPR << ") IS TRUE" << endl; \
    }  \
    else \
    { \
        cout << "(" << #EXPR << ") IS FALSE" << endl; \
    }

    cout << endl;

    if (br_diff > sm_diff)
    {
        cout << "Brute > Smart, diff = " << (br_diff-sm_diff) << endl;
    }
    else
    {
        cout << "Brute <= Smart, diff = " << (sm_diff-br_diff) << endl;
    }
    if (sm_diff > sm_wo_cks_diff)
    {
        cout << "Smart > Smart (wo checks), diff = " << (sm_diff-sm_wo_cks_diff) << endl;
    }
    else
    {
        cout << "Smart <= Smart (wo checks), diff = " << (sm_wo_cks_diff-sm_diff) << endl;
    }
    if (!verbose)
    {
        if (sm_diff > cnt_diff)
        {
            cout << "Smart > Count-only, diff = " << (sm_diff-cnt_diff) << endl;
        }
        else
        {
            cout << "Smart <= Count-only, diff = " << (cnt_diff-sm_diff) << endl;
        }
    }

#if 0
    CHECKCMP(br_diff < sm_diff);
    CHECKCMP(br_diff > sm_diff);
    CHECKCMP(br_diff == sm_diff);
    CHECKCMP(br_diff <= sm_diff);
    CHECKCMP(br_diff >= sm_diff);
    CHECKCMP(br_diff != sm_diff);

    CHECKCMP(sm_diff < br_diff);
    CHECKCMP(sm_diff > br_diff);
    CHECKCMP(sm_diff == br_diff);
    CHECKCMP(sm_diff <= br_diff);
    CHECKCMP(sm_diff >= br_diff);
    CHECKCMP(sm_diff != br_diff);

    CHECKCMP(sm_diff < sm_diff);
    CHECKCMP(sm_diff > sm_diff);
    CHECKCMP(sm_diff == sm_diff);
    CHECKCMP(sm_diff <= sm_diff);
    CHECKCMP(sm_diff >= sm_diff);
    CHECKCMP(sm_diff != sm_diff);

    CHECKCMP(sm_diff < sm_wo_cks_diff);
    CHECKCMP(sm_diff > sm_wo_cks_diff);
    CHECKCMP(sm_diff == sm_wo_cks_diff);
    CHECKCMP(sm_diff <= sm_wo_cks_diff);
    CHECKCMP(sm_diff >= sm_wo_cks_diff);
    CHECKCMP(sm_diff != sm_wo_cks_diff);
#endif

    return 0;
}

