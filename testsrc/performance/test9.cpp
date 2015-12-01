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
#include <list>

// my adt
#include "adt/Queue_List.h"

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

    // how much output
    enum VerboseLevel 
    {
        VL_Unknown = -1,
        VL_Off = 0,
        VL_On = 1,
    };

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

    // breadth-first queue element data type
    class BFQElement
    {
    public:
        // ctors and dtor
        BFQElement(int r=-1, int c=-1, 
                int seq_num=-1, int nvows=-1):
            _r(r), _c(c), 
            _seq_num(seq_num), _nvows(nvows) { }
        BFQElement(const BFQElement &src):
            _r(src._r), _c(src._c), 
            _seq_num(src._seq_num), _nvows(src._nvows) { }
        ~BFQElement() { }
       
        // assignment
        BFQElement &operator=(const BFQElement &rhs)
        {
            if (this != &rhs)
            {
                _r = rhs._r;
                _c = rhs._c;
                _seq_num = rhs._seq_num;
                _nvows   = rhs._nvows;
            }
            return *this;
        }
    
    public:
        // data
        int _r, _c;
        int _seq_num;
        int _nvows;
    };

    typedef list<BFQElement> BFQueue;
    typedef Queue_List<BFQElement> MyBFQueue;

public:
    // ctors and dtor
    KeyBoard();
    KeyBoard(const KeyBoard &);
    ~KeyBoard();

    // assignment
    KeyBoard &operator=(const KeyBoard &);

    // output
    inline void verbose(VerboseLevel v)
    {
        _verbose = v;
    }
    inline VerboseLevel verbose() const
    {
        return _verbose;
    }
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
    void one_vowel_moves(ostream &os,
                         int r, int c, 
                         int &one_or_less, 
                         int &one_only);
    void zero_vowel_moves(ostream &os,
                          int r, int c, 
                          int &zero_only);
    void vowel_moves(ostream &os,
                     int vowels, 
                     AllowedMoves &moves,
                     int r, int c,
                     int &how_many,
                     bool exact);
    bool check_if_allowed(int vowels, int r, int c, bool exact) const;

    // generate sequences using a basic depth-first search.
    void basic_depth_first(ostream &os, 
                           string sequence, // pass by value to isolate
                           int sequence_number, 
                           int number_of_vowels, 
                           int row, int col,
                           long &seq_found,
                           long &bail_outs,
                           long &nodes_visited);

    // generate sequences using a depth-first search but using
    // preexpanded nodes, that is, knowing all the allowed 
    // one-vowel or zero-vowel single steps.
    void precalc_depth_first(ostream &os, 
                             string sequence, // pass by value to isolate
                             int sequence_number, 
                             int number_of_vowels, 
                             int row, int col,
                             long &seq_found,
                             long &bail_outs,
                             long &nodes_visited);


    // generate sequences using a depth-first search but using
    // preexpanded nodes, that is, knowing all the allowed 
    // one-vowel or zero-vowel single steps. also with all 
    // unnecessary sanity checks removed.
    void precalc_depth_first_wo_chks(ostream &os, 
                                     string sequence, // pass by value to isolate
                                     int sequence_number, 
                                     int number_of_vowels, 
                                     int row, int col,
                                     long &seq_found,
                                     long &bail_outs,
                                     long &nodes_visited);

    // generate sequences using a basic breadth-first search.
    void basic_breadth_first(ostream &os, 
                             string sequence, // pass by value to isolate
                             int sequence_number, 
                             int number_of_vowels, 
                             int row, int col,
                             long &seq_found,
                             long &bail_outs,
                             long &nodes_visited,
                             long &max_queue_size);

    // generate sequences using a precalculated breadth-first search.
    void precalc_breadth_first(ostream &os, 
                               string sequence, // pass by value to isolate
                               int sequence_number, 
                               int number_of_vowels, 
                               int row, int col,
                               long &seq_found,
                               long &bail_outs,
                               long &nodes_visited,
                               long &max_queue_size);

    // generate sequences using a basic breadth-first search.
    void myq_basic_breadth_first(ostream &os, 
                             string sequence, // pass by value to isolate
                             int sequence_number, 
                             int number_of_vowels, 
                             int row, int col,
                             long &seq_found,
                             long &bail_outs,
                             long &nodes_visited,
                             long &max_queue_size);

    // generate sequences using a precalculated breadth-first search.
    void myq_precalc_breadth_first(ostream &os, 
                               string sequence, // pass by value to isolate
                               int sequence_number, 
                               int number_of_vowels, 
                               int row, int col,
                               long &seq_found,
                               long &bail_outs,
                               long &nodes_visited,
                               long &max_queue_size);

protected:
    // verbose level
    VerboseLevel _verbose;

    // internal board representation
    Key _board[max_row][max_col];

    // breadth-first queue
    BFQueue _queue;
    MyBFQueue _my_queue;

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
    _queue.clear();

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
KeyBoard::vowel_moves(ostream &os,
                      int vowels, 
                      AllowedMoves &moves,
                      int r, int c,
                      int &how_many,
                      bool exact)
{
    // sanity checks
    MustBeTrue((0 <= r) && (r < max_row));
    MustBeTrue((0 <= c) && (c < max_col));
    MustBeTrue(0 <= vowels);

    // output
    if (_verbose == VL_On) 
    {
        os << endl << "Moves for: " << _board[r][c] << endl;
    }

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
                if (_verbose == VL_On) 
                {
                    os << _board[new_r][new_c] << endl;
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
}

// generate moves with one or less vowels
void
KeyBoard::one_vowel_moves(ostream &os,
                          int r, int c, 
                          int &one_or_less, 
                          int &one_only)
{
    if (_verbose == VL_On)
    {
        int how_many = 0;
        vowel_moves(os, 1, _one_or_less_vowel, r, c, how_many, false);
        os << "Size of one-or-less-vowel list of allowed moves: " 
           << how_many << endl;
        one_or_less += how_many;

        how_many = 0;
        vowel_moves(os, 1, _one_vowel, r, c, one_only, true);
        os << "Size of exact one-vowel list of allowed moves: " 
           << how_many << endl;
        one_only += how_many;
    }
    else
    {
        vowel_moves(os, 1, _one_or_less_vowel, r, c, one_or_less, false);
        vowel_moves(os, 1, _one_vowel, r, c, one_only, true);
    }
}

// generate moves with no vowels
void
KeyBoard::zero_vowel_moves(ostream &os,
                           int r, int c, 
                           int &zero_only)
{
    if (_verbose == VL_On)
    {
        int how_many = 0;
        vowel_moves(os, 0, _zero_vowel, r, c, how_many, true);
        os << "Size of exact zero-vowel list of allowed moves: " 
           << how_many << endl;
        zero_only += how_many;
    }
    else
    {
        vowel_moves(os, 0, _zero_vowel, r, c, zero_only, true);
    }
}

// generate sequences using depth-first traversal with no optimizations
void
KeyBoard::basic_depth_first(ostream &os, 
                                string seq, 
                                int seq_num, 
                                int vowels, 
                                int r, int c,
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
    if (_verbose == VL_On) 
    {
        seq += _board[r][c]._value;
    }
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        // did we fulfill the required number of vowels?
        if (_verbose == VL_On) 
        {
            os << seq << endl;
        }
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    for (int move=0; el_moves[move][0] != 0; ++move)
    {
        int new_r = r+el_moves[move][0];
        int new_c = c+el_moves[move][1];
        basic_depth_first(os, 
                          seq, 
                          seq_num, 
                          vowels, 
                          new_r, new_c, 
                          seq_found,
                          bail_outs,
                          nodes_visited);
    }
}

// generate sequences using precalculate expanded nodes and 
// a depth-first search.
void
KeyBoard::precalc_depth_first(ostream &os, 
                              string seq, 
                              int seq_num, 
                              int vowels, 
                              int r, int c,
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
    if (_verbose == VL_On) 
    {
        seq += _board[r][c]._value;
    }
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        if (_verbose == VL_On) 
        {
            os << seq << endl;
        }
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;
    if (vowels >= 1)
    {
        int *pm = _one_or_less_vowel + index*(_key_num+1)*2;
        int mi = pm[0*2+0];
        for (int i=0; i<=mi; ++i)
        {
            int new_r = pm[((i+1)*2)+0];
            int new_c = pm[((i+1)*2)+1];
            precalc_depth_first(os, 
                                seq, 
                                seq_num, 
                                vowels, 
                                new_r, new_c, 
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
            precalc_depth_first(os, 
                                seq, 
                                seq_num, 
                                vowels, 
                                new_r, new_c, 
                                seq_found,
                                bail_outs,
                                nodes_visited);
        }
    }
}

// generate sequences using precalculate expanded nodes and 
// a depth-first search. unnecessary sanity-checks removed.
void
KeyBoard::precalc_depth_first_wo_chks(ostream &os, 
                                      string seq, 
                                      int seq_num, 
                                      int vowels, 
                                      int r, int c,
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
    if (_verbose == VL_On) 
    {
        seq += _board[r][c]._value;
    }
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        if (_verbose == VL_On) 
        {
            os << seq << endl;
        }
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;
    if (vowels >= 1)
    {
        register int *pm = _one_or_less_vowel + index*(_key_num+1)*2;
        register int mi = pm[0*2+0];
        for (int i=0; i<=mi; ++i)
        {
            register int *pmi = pm + (i+1)*2;
            precalc_depth_first_wo_chks(os, 
                                        seq, 
                                        seq_num, 
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
        register int mi = pm[0*2+0];
        for (int i=0; i<=mi; ++i)
        {
            register int *pmi = pm + (i+1)*2;
            precalc_depth_first_wo_chks(os, 
                                        seq, 
                                        seq_num, 
                                        vowels, 
                                        *pmi, 
                                        *(pmi+1), 
                                        seq_found,
                                        bail_outs,
                                        nodes_visited);
        }
    }
}

// generate sequences using breadth-first traversal with no optimizations
void
KeyBoard::basic_breadth_first(ostream &os, 
                              string seq, 
                              int seq_num, 
                              int vowels, 
                              int r, int c,
                              long &seq_found,
                              long &bail_outs,
                              long &nodes_visited,
                              long &max_queue_size)
{
    // place first element in queue
    _queue.clear();
    _queue.push_back(BFQElement(r, c, seq_num, vowels));

    // track queue sizes
    long current_queue_size = _queue.size();

    // start the traversal
    for ( ; ! _queue.empty(); --current_queue_size, _queue.pop_front())
    {
        // get the next queue element
        BFQElement &e = _queue.front();
        if (current_queue_size > max_queue_size)
        {
            max_queue_size = current_queue_size;
        }

        // count how many nodes are visited
        ++nodes_visited;
    
        // sanity checks
        if ((e._r < 0) || (e._r >= max_row) ||
            (e._c < 0) || (e._c >= max_col)) 
        {
            ++bail_outs;
            continue;
        }
    
        // is this an acceptable character?
        if (_board[e._r][e._c]._value == null_key)
        {
            ++bail_outs;
            continue;
        }
    
        // is this a vowel?
        if (_board[e._r][e._c]._vowel) --e._nvows;
    
        // is the number of vowels valid?
        if (e._nvows < 0)
        {
            ++bail_outs;
            continue;
        }
    
        // we found another character
        if (_verbose == VL_On) 
        {
            seq += _board[e._r][e._c]._value;
        }
        --e._seq_num;
    
        // is this the end of a sequence?
        if (e._seq_num == 0)
        {
            if (_verbose == VL_On) 
            {
                os << seq.substr(0, seq_num) << endl;
            }
            seq_found += 1;
            if (_verbose == VL_On) 
            {
                seq = seq.substr(0,seq_num-1);
            }
            continue;
        }

        // not the end of a sequence. check next L-step, that is,
        // expand the horizon to the next step.
        for (int move=0; el_moves[move][0] != 0; ++move)
        {
            ++current_queue_size;
            int new_r = e._r+el_moves[move][0];
            int new_c = e._c+el_moves[move][1];
            _queue.push_back(BFQElement(new_r, new_c, e._seq_num, e._nvows));
        }

        // reset parent string if in verbose mode
        if (_verbose == VL_On) 
        {
            seq = seq.substr(0,seq_num-1);
        }
    }
}

// generate sequences using breadth-first traversal with precalculated
// transitions.
void
KeyBoard::precalc_breadth_first(ostream &os, 
                                string seq, 
                                int seq_num, 
                                int vowels, 
                                int r, int c,
                                long &seq_found,
                                long &bail_outs,
                                long &nodes_visited,
                                long &max_queue_size)
{
    // place first element in queue
    _queue.clear();
    _queue.push_back(BFQElement(r, c, seq_num, vowels));

    // track queue sizes
    long current_queue_size = _queue.size();

    // start the traversal
    for ( ; ! _queue.empty(); --current_queue_size, _queue.pop_front())
    {
        // get the next queue element
        BFQElement &e = _queue.front();
        if (current_queue_size > max_queue_size)
        {
            max_queue_size = current_queue_size;
        }

        // count how many nodes are visited
        ++nodes_visited;
    
        // sanity checks
        if ((e._r < 0) || (e._r >= max_row) ||
            (e._c < 0) || (e._c >= max_col)) 
        {
            ++bail_outs;
            continue;
        }
    
        // is this an acceptable character?
        if (_board[e._r][e._c]._value == null_key)
        {
            ++bail_outs;
            continue;
        }
    
        // is this a vowel?
        if (_board[e._r][e._c]._vowel) --e._nvows;
    
        // is the number of vowels valid?
        if (e._nvows < 0)
        {
            ++bail_outs;
            continue;
        }
    
        // we found another character
        if (_verbose == VL_On) 
        {
            seq += _board[e._r][e._c]._value;
        }
        --e._seq_num;
    
        // is this the end of a sequence?
        if (e._seq_num == 0)
        {
            if (_verbose == VL_On) 
            {
                os << seq.substr(0, seq_num) << endl;
            }
            seq_found += 1;
            if (_verbose == VL_On) 
            {
                seq = seq.substr(0,seq_num-1);
            }
            continue;
        }

        // not the end of a sequence. check next L-step, that is,
        // expand the horizon to the next step.
        int index = _board[e._r][e._c]._index;
        if (e._nvows >= 1)
        {
            int *pm = _one_or_less_vowel + index*(_key_num+1)*2;
            int mi = pm[0*2+0];
            for (int i=0; i<=mi; ++i)
            {
                ++current_queue_size;
                int new_r = pm[((i+1)*2)+0];
                int new_c = pm[((i+1)*2)+1];
                _queue.push_back(BFQElement(new_r, new_c, 
                                            e._seq_num, 
                                            e._nvows));
            }
        }
        else
        {
            int *pm = _zero_vowel + index*(_key_num+1)*2;
            int mi = pm[0*2+0];
            for (int i=0; i<=mi; ++i)
            {
                ++current_queue_size;
                int new_r = pm[((i+1)*2)+0];
                int new_c = pm[((i+1)*2)+1];
                _queue.push_back(BFQElement(new_r, new_c, 
                                            e._seq_num, 
                                            e._nvows));
            }
        }

        // reset parent string if in verbose mode
        if (_verbose == VL_On) 
        {
            seq = seq.substr(0,seq_num-1);
        }
    }
}

// generate sequences using breadth-first traversal with no optimizations
void
KeyBoard::myq_basic_breadth_first(ostream &os, 
                              string seq, 
                              int seq_num, 
                              int vowels, 
                              int r, int c,
                              long &seq_found,
                              long &bail_outs,
                              long &nodes_visited,
                              long &max_queue_size)
{
    // place first element in queue
    _my_queue.clear();
    _my_queue.enqueue(BFQElement(r, c, seq_num, vowels));

    // start the traversal
    while ( ! _my_queue.isEmpty())
    {
        // get the next queue element
        if (_my_queue.getCount() > max_queue_size)
        {
            max_queue_size = _my_queue.getCount();
        }
        BFQElement e; 
        _my_queue.dequeue(e);

        // count how many nodes are visited
        ++nodes_visited;
    
        // sanity checks
        if ((e._r < 0) || (e._r >= max_row) ||
            (e._c < 0) || (e._c >= max_col)) 
        {
            ++bail_outs;
            continue;
        }
    
        // is this an acceptable character?
        if (_board[e._r][e._c]._value == null_key)
        {
            ++bail_outs;
            continue;
        }
    
        // is this a vowel?
        if (_board[e._r][e._c]._vowel) --e._nvows;
    
        // is the number of vowels valid?
        if (e._nvows < 0)
        {
            ++bail_outs;
            continue;
        }
    
        // we found another character
        if (_verbose == VL_On) 
        {
            seq += _board[e._r][e._c]._value;
        }
        --e._seq_num;
    
        // is this the end of a sequence?
        if (e._seq_num == 0)
        {
            if (_verbose == VL_On) 
            {
                os << seq.substr(0, seq_num) << endl;
            }
            seq_found += 1;
            if (_verbose == VL_On) 
            {
                seq = seq.substr(0,seq_num-1);
            }
            continue;
        }

        // not the end of a sequence. check next L-step, that is,
        // expand the horizon to the next step.
        for (int move=0; el_moves[move][0] != 0; ++move)
        {
            int new_r = e._r+el_moves[move][0];
            int new_c = e._c+el_moves[move][1];
            _my_queue.enqueue(BFQElement(new_r, new_c, 
                                      e._seq_num, 
                                      e._nvows));
        }

        // reset parent string if in verbose mode
        if (_verbose == VL_On) 
        {
            seq = seq.substr(0,seq_num-1);
        }
    }
}

// generate sequences using breadth-first traversal with precalculated
// transitions.
void
KeyBoard::myq_precalc_breadth_first(ostream &os, 
                                string seq, 
                                int seq_num, 
                                int vowels, 
                                int r, int c,
                                long &seq_found,
                                long &bail_outs,
                                long &nodes_visited,
                                long &max_queue_size)
{
    // place first element in queue
    _my_queue.clear();
    _my_queue.enqueue(BFQElement(r, c, seq_num, vowels));

    // start the traversal
    while ( ! _my_queue.isEmpty())
    {
        // get the next queue element
        if (_my_queue.getCount() > max_queue_size)
        {
            max_queue_size = _my_queue.getCount();
        }
        BFQElement e; 
        _my_queue.dequeue(e);

        // count how many nodes are visited
        ++nodes_visited;
    
        // sanity checks
        if ((e._r < 0) || (e._r >= max_row) ||
            (e._c < 0) || (e._c >= max_col)) 
        {
            ++bail_outs;
            continue;
        }
    
        // is this an acceptable character?
        if (_board[e._r][e._c]._value == null_key)
        {
            ++bail_outs;
            continue;
        }
    
        // is this a vowel?
        if (_board[e._r][e._c]._vowel) --e._nvows;
    
        // is the number of vowels valid?
        if (e._nvows < 0)
        {
            ++bail_outs;
            continue;
        }
    
        // we found another character
        if (_verbose == VL_On) 
        {
            seq += _board[e._r][e._c]._value;
        }
        --e._seq_num;
    
        // is this the end of a sequence?
        if (e._seq_num == 0)
        {
            if (_verbose == VL_On) 
            {
                os << seq.substr(0, seq_num) << endl;
            }
            seq_found += 1;
            if (_verbose == VL_On) 
            {
                seq = seq.substr(0,seq_num-1);
            }
            continue;
        }

        // not the end of a sequence. check next L-step, that is,
        // expand the horizon to the next step.
        int index = _board[e._r][e._c]._index;
        if (e._nvows >= 1)
        {
            int *pm = _one_or_less_vowel + index*(_key_num+1)*2;
            int mi = pm[0*2+0];
            for (int i=0; i<=mi; ++i)
            {
                int new_r = pm[((i+1)*2)+0];
                int new_c = pm[((i+1)*2)+1];
                _my_queue.enqueue(BFQElement(new_r, new_c, 
                                            e._seq_num, 
                                            e._nvows));
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
                _my_queue.enqueue(BFQElement(new_r, new_c, 
                                            e._seq_num, 
                                            e._nvows));
            }
        }

        // reset parent string if in verbose mode
        if (_verbose == VL_On) 
        {
            seq = seq.substr(0,seq_num-1);
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
    if (verbose) 
        b.verbose(KeyBoard::VL_On);
    else
        b.verbose(KeyBoard::VL_Off);
    stopwatch.stop();
    cout << endl << "KeyBoard ctor time: " << stopwatch << endl;

    cout << endl << "initial board = " << b << endl;

    // build L-move transitions for each key.
    int one_or_less = 0;
    int one_only = 0;
    int zero_only = 0;
    stopwatch.start();
    for (int r=0; r<KeyBoard::max_row; ++r)
    {
        for (int c=0; c<KeyBoard::max_col; ++c)
        {
            b.one_vowel_moves(cout, r, c, one_or_less, one_only);
            b.zero_vowel_moves(cout, r, c, zero_only);
        }
    }
    stopwatch.stop();

    cout << endl << "Generate allowed moves time: " << stopwatch << endl;
    cout << "Generated One-or-Less moves: " << one_or_less << endl;
    cout << "Generated One-only moves: " << one_only << endl;
    cout << "Generated Zero-only moves: " << zero_only << endl;

    cout << endl << "Basic depth-first traversal..." << endl;
    long bdf_seqs = 0;
    long bdf_nodes = 0;
    long bdf_bails = 0;
    stopwatch.start();
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            string sequence("");
            b.basic_depth_first(cout, 
                                sequence, 
                                sequence_length, 
                                number_of_vowels, 
                                row, col,
                                bdf_seqs,
                                bdf_bails,
                                bdf_nodes);
        }
    }
    stopwatch.stop();

    cout << "Time (basic depth-first): " << stopwatch << endl;
    cout << "Number of sequences found (basic depth-first): " << bdf_seqs << endl;
    cout << "Number of bails outs (basic depth-first): " << bdf_bails << endl;
    cout << "Number of nodes visited (basic depth-first): " << bdf_nodes << endl;

    cout << endl << "Precalc depth-first traversal ..." << endl;
    long pdf_seqs = 0;
    long pdf_nodes = 0;
    long pdf_bails = 0;
    stopwatch.start();
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            string sequence("");
            b.precalc_depth_first(cout, 
                                  sequence, 
                                  sequence_length, 
                                  number_of_vowels, 
                                  row, col,
                                  pdf_seqs,
                                  pdf_bails,
                                  pdf_nodes);
        }
    }
    stopwatch.stop();

    cout << "Time (precalc depth-first): " << stopwatch << endl;
    cout << "Number of sequences found (precalc depth-first): " << pdf_seqs << endl;
    cout << "Number of bails outs (precalc depth-first): " << pdf_bails << endl;
    cout << "Number of nodes visited (precalc depth-first): " << pdf_nodes << endl;

    cout << endl << "Precalc with no-checks depth-first traversal ..." << endl;
    long pdf_wo_cks_seqs = 0;
    long pdf_wo_cks_bails = 0;
    long pdf_wo_cks_nodes = 0;
    stopwatch.start();
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
                b.precalc_depth_first_wo_chks(cout, 
                                              sequence, 
                                              sequence_length, 
                                              number_of_vowels, 
                                              row, col,
                                              pdf_wo_cks_seqs,
                                              pdf_wo_cks_bails,
                                              pdf_wo_cks_nodes);
            }
        }
    }
    stopwatch.stop();

    cout << "Time (precalc no-chks depth-first): " << stopwatch << endl;
    cout << "Number of sequences found (precalc no-chks depth-first): " << pdf_wo_cks_seqs << endl;
    cout << "Number of bails outs (precalc no-chks depth-first): " << pdf_wo_cks_bails << endl;
    cout << "Number of nodes visited (precalc no-chks depth-first): " << pdf_wo_cks_nodes << endl;

    cout << endl << "Basic breadth-first traversal..." << endl;
    long bbf_maxq = 0;
    long bbf_seqs = 0;
    long bbf_nodes = 0;
    long bbf_bails = 0;
    stopwatch.start();
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            string sequence("");
            b.basic_breadth_first(cout, 
                                sequence, 
                                sequence_length, 
                                number_of_vowels, 
                                row, col,
                                bbf_seqs,
                                bbf_bails,
                                bbf_nodes,
                                bbf_maxq);
        }
    }
    stopwatch.stop();

    cout << "Time (basic breadth-first): " << stopwatch << endl;
    cout << "Number of sequences found (basic breadth-first): " << bbf_seqs << endl;
    cout << "Number of bails outs (basic breadth-first): " << bbf_bails << endl;
    cout << "Number of nodes visited (basic breadth-first): " << bbf_nodes << endl;
    cout << "Max queue size (basic breadth-first): " << bbf_maxq << endl;

    cout << endl << "Precalc breadth-first traversal..." << endl;
    long pbf_maxq = 0;
    long pbf_seqs = 0;
    long pbf_nodes = 0;
    long pbf_bails = 0;
    stopwatch.start();
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            string sequence("");
            b.precalc_breadth_first(cout, 
                                    sequence, 
                                    sequence_length, 
                                    number_of_vowels, 
                                    row, col,
                                    pbf_seqs,
                                    pbf_bails,
                                    pbf_nodes,
                                    pbf_maxq);
        }
    }
    stopwatch.stop();

    cout << "Time (precalc breadth-first): " << stopwatch << endl;
    cout << "Number of sequences found (precalc breadth-first): " << pbf_seqs << endl;
    cout << "Number of bails outs (precalc breadth-first): " << pbf_bails << endl;
    cout << "Number of nodes visited (precalc breadth-first): " << pbf_nodes << endl;
    cout << "Max queue size (precalc breadth-first): " << pbf_maxq << endl;

    cout << endl << "My-Q Basic breadth-first traversal..." << endl;
    long myq_bbf_maxq = 0;
    long myq_bbf_seqs = 0;
    long myq_bbf_nodes = 0;
    long myq_bbf_bails = 0;
    stopwatch.start();
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            string sequence("");
            b.basic_breadth_first(cout, 
                                sequence, 
                                sequence_length, 
                                number_of_vowels, 
                                row, col,
                                myq_bbf_seqs,
                                myq_bbf_bails,
                                myq_bbf_nodes,
                                myq_bbf_maxq);
        }
    }
    stopwatch.stop();

    cout << "Time (my-q basic breadth-first): " << stopwatch << endl;
    cout << "Number of sequences found (my-q basic breadth-first): " << myq_bbf_seqs << endl;
    cout << "Number of bails outs (my-q basic breadth-first): " << myq_bbf_bails << endl;
    cout << "Number of nodes visited (my-q basic breadth-first): " << myq_bbf_nodes << endl;
    cout << "Max queue size (my-q basic breadth-first): " << myq_bbf_maxq << endl;

    cout << endl << "My-Q Precalc breadth-first traversal..." << endl;
    long myq_pbf_maxq = 0;
    long myq_pbf_seqs = 0;
    long myq_pbf_nodes = 0;
    long myq_pbf_bails = 0;
    stopwatch.start();
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            string sequence("");
            b.precalc_breadth_first(cout, 
                                    sequence, 
                                    sequence_length, 
                                    number_of_vowels, 
                                    row, col,
                                    myq_pbf_seqs,
                                    myq_pbf_bails,
                                    myq_pbf_nodes,
                                    myq_pbf_maxq);
        }
    }
    stopwatch.stop();

    cout << "Time (my-q precalc breadth-first): " << stopwatch << endl;
    cout << "Number of sequences found (my-q precalc breadth-first): " << myq_pbf_seqs << endl;
    cout << "Number of bails outs (my-q precalc breadth-first): " << myq_pbf_bails << endl;
    cout << "Number of nodes visited (my-q precalc breadth-first): " << myq_pbf_nodes << endl;
    cout << "Max queue size (my-q precalc breadth-first): " << myq_pbf_maxq << endl;

    return 0;
}




