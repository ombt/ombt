//
// problem from getgo.
//

// headers
#include <string.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <sstream>

// cheap way
using namespace std;

// high-res timer
class NanoSecTime
{
public:
    // data types
    enum TimerType
    {
        TT_Realtime = CLOCK_REALTIME,
        TT_Monotonic = CLOCK_MONOTONIC,
        TT_Process_Cputime_Id = CLOCK_PROCESS_CPUTIME_ID,
        TT_Thread_Cputime_Id = CLOCK_THREAD_CPUTIME_ID,
    };

public:
    // ctors and dtor
    NanoSecTime(TimerType tt = TT_Monotonic):
        _timer_type(tt),
        _seconds(0),
        _nanoseconds(0)
    {
        _id = tt;
        assert(clock_gettime(_id, &_tp) == 0);
        _seconds     = _tp.tv_sec;
        _nanoseconds = _tp.tv_nsec;
    }
    NanoSecTime(const NanoSecTime &src):
        _timer_type(src._timer_type),
        _seconds(src._seconds),
        _nanoseconds(src._nanoseconds),
        _id(src._id)
    {
        _tp.tv_sec  = src._tp.tv_sec;
        _tp.tv_nsec = src._tp.tv_nsec;
    }
    ~NanoSecTime() { }

    // assignment
    NanoSecTime &operator=(const NanoSecTime &rhs)
    {
        if (this != &rhs)
        {
            _timer_type  = rhs._timer_type;
            _seconds     = rhs._seconds;
            _nanoseconds = rhs._nanoseconds;
            _id          = rhs._id;
            _tp.tv_sec   = rhs._tp.tv_sec;
            _tp.tv_nsec  = rhs._tp.tv_nsec;
        }
        return *this;
    }

    // basic arithmetic
    NanoSecTime &operator+(const NanoSecTime &rhs)
    {
        long sec  = (_tp.tv_nsec + rhs._tp.tv_nsec)/1000000000;
        long nsec = (_tp.tv_nsec + rhs._tp.tv_nsec)%1000000000;

        _tp.tv_sec += rhs._tp.tv_sec + sec;
        _tp.tv_nsec = nsec;
        assert((_tp.tv_sec >= 0) && (_tp.tv_nsec >= 0));

        _seconds     = _tp.tv_sec;
        _nanoseconds = _tp.tv_nsec;

        return *this;
    }
    NanoSecTime &operator-(const NanoSecTime &rhs)
    {
        long nsec, sec;

        if (_tp.tv_nsec < rhs._tp.tv_nsec)
        {
            --_tp.tv_sec;
            _tp.tv_nsec += 1000000000;
        }

        _tp.tv_nsec -= rhs._tp.tv_nsec;
        _tp.tv_sec  -= rhs._tp.tv_sec;
        assert((_tp.tv_sec >= 0) && (_tp.tv_nsec >= 0));

        _seconds     = _tp.tv_sec;
        _nanoseconds = _tp.tv_nsec;

        return *this;
    }

    // output
    friend ostream &operator<<(ostream &os, const NanoSecTime &o);

public:
    // exposed data
    TimerType _timer_type;
    long      _seconds;
    long      _nanoseconds;

private:
    // internal data
    clockid_t       _id;
    struct timespec _tp;
};

// output
ostream &
operator<<(ostream &os, const NanoSecTime &o)
{
    os << "(s;ns)=(" 
       << o._seconds
       << ";"
       << o._nanoseconds
       << ")";
    return os;
}

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
        Key(int val = null_key, bool vow = false, int idx = -1):
            _value(val), _vowel(vow), _index(idx) { }
        Key(const Key &src):
            _value(src._value), 
            _vowel(src._vowel),
            _index(src._index) { }
        ~Key() { }
        Key &operator=(const Key &rhs)
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
        assert((0 <= r) && (r < max_row));
        assert((0 <= c) && (c < max_col));
        return _board[r][c]._value;
    }
    inline bool vowel(int r, int c) const
    {
        assert((0 <= r) && (r < max_row));
        assert((0 <= c) && (c < max_col));
        return _board[r][c]._vowel;
    }

    // test crunching routines
    void all_moves(ostream &os, int r, int c) const;
    void one_vowel_moves(ostream &os, int r, int c);
    void zero_vowel_moves(ostream &os, int r, int c);
    void vowel_moves(ostream &os, 
                     int vowels, 
                     AllowedMoves &moves,
                     int r, int c,
                     int &how_many);
    bool check_if_allowed(int vowels, int r, int c, bool exact = true) const;

    // actual crunching routines
    void brute_force_depth_first(int sequence_number, 
                               int number_of_vowels, 
                               int row, 
                               int col,
                               long &seq_found,
                               long &nodes_visited);
    void smart_depth_first(int sequence_number, 
                         int number_of_vowels, 
                         int row, 
                         int col,
                         long &seq_found,
                         long &nodes_visited);
    void brute_force_breadth_first(int sequence_number, 
                               int number_of_vowels, 
                               int row, 
                               int col,
                               long &seq_found,
                               long &nodes_visited);
    void smart_breadth_first(int sequence_number, 
                         int number_of_vowels, 
                         int row, 
                         int col,
                         long &seq_found,
                         long &nodes_visited);

protected:
    // internal board representation
    Key _board[max_row][max_col];

    // end-keys after L-step
    int _key_num;
    AllowedMoves _zero_vowel;
    AllowedMoves _one_vowel;
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

    int imax = _key_num*(_key_num+1)*2;

    _one_vowel  = new int [imax];
    for (int i=0; i<imax; ++i)
    {
        _one_vowel[i] = -1;
    }
    _zero_vowel  = new int [imax];
    for (int i=0; i<imax; ++i)
    {
        _zero_vowel[i] = -1;
    }
}

KeyBoard::KeyBoard(const KeyBoard &src)
{
    ::memcpy(_board, src._board, sizeof(_board));
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
    }
    return *this;
}

// crunching routines

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
    assert((0<= r) && (r < max_row));
    assert((0<= c) && (c < max_col));

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
                      int &how_many)
{
    // sanity checks
    assert((0 <= r) && (r < max_row));
    assert((0 <= c) && (c < max_col));
    assert(0 <= vowels);

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
            if (check_if_allowed(vowels, new_r, new_c, false))
            {
                os << _board[new_r][new_c] << endl;
                how_many += 1;
                ++mi;
                assert(pm[((mi+1)*2)+0] == -1);
                pm[((mi+1)*2)+0] = new_r;
                assert(pm[((mi+1)*2)+1] == -1);
                pm[((mi+1)*2)+1] = new_c;
            }
        }
    }
}

void
KeyBoard::one_vowel_moves(ostream &os, int r, int c)
{
   int how_many = 0;
   vowel_moves(os, 1, _one_vowel, r, c, how_many);
   os << "Size of one-vowel list of allowed moves: " 
      << how_many << endl;
}

void
KeyBoard::zero_vowel_moves(ostream &os, int r, int c)
{
   int how_many = 0;
   vowel_moves(os, 0, _zero_vowel, r, c, how_many);
   os << "Size of zero-vowel list of allowed moves: " 
      << how_many << endl;
}

// real routines to generate sequences
void
KeyBoard::brute_force_depth_first(int seq_num, 
                                int vowels, 
                                int r, 
                                int c,
                                long &seq_found,
                                long &nodes_visited)
{
    // track the total number of nodes visited
    ++nodes_visited;

    // sanity checks
    if ((r < 0) || (r >= max_row) ||
        (c < 0) || (c >= max_col)) return;

    // is this an acceptable character?
    if (_board[r][c]._value == null_key) return;

    // is this a vowel?
    if (_board[r][c]._vowel) --vowels;

    // is the number of vowels valid?
    if (vowels < 0) return;

    // we found another character
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        // did we fulfill the required number of vowels?
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    for (int move=0; el_moves[move][0] != 0; ++move)
    {
        int new_r = r+el_moves[move][0];
        int new_c = c+el_moves[move][1];
        brute_force_depth_first(seq_num, vowels, new_r, new_c, seq_found, nodes_visited);
    }
}

void
KeyBoard::smart_depth_first(int seq_num, 
                          int vowels, 
                          int r, 
                          int c,
                          long &seq_found,
                          long &nodes_visited)
{
    // track the total number of nodes visited
    ++nodes_visited;

#if 0
    // sanity checks
    if ((r < 0) || (r >= max_row) ||
        (c < 0) || (c >= max_col)) return;

    // is this an acceptable character?
    if (_board[r][c]._value == null_key) return;
#endif

    // is this a vowel?
    if (_board[r][c]._vowel) --vowels;

    // is the number of vowels valid?
    if (vowels < 0) return;

    // we found another character
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;

    int *pm = NULL;
    if (vowels >= 1)
        pm = _one_vowel + index*(_key_num+1)*2;
    else
        pm = _zero_vowel + index*(_key_num+1)*2;
    int mi = pm[0*2+0];

    int *pmi = NULL;
    for (int i=0; i<=mi; ++i)
    {
        pmi = pm + (i+1)*2;
        smart_depth_first(seq_num, 
                        vowels, 
                        *pmi, 
                        *(pmi+1), 
                        seq_found, 
                        nodes_visited);
    }
}

void
KeyBoard::brute_force_breadth_first(int seq_num, 
                                int vowels, 
                                int r, 
                                int c,
                                long &seq_found,
                                long &nodes_visited)
{
    // track the total number of nodes visited
    ++nodes_visited;

    // sanity checks
    if ((r < 0) || (r >= max_row) ||
        (c < 0) || (c >= max_col)) return;

    // is this an acceptable character?
    if (_board[r][c]._value == null_key) return;

    // is this a vowel?
    if (_board[r][c]._vowel) --vowels;

    // is the number of vowels valid?
    if (vowels < 0) return;

    // we found another character
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        // did we fulfill the required number of vowels?
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    for (int move=0; el_moves[move][0] != 0; ++move)
    {
        int new_r = r+el_moves[move][0];
        int new_c = c+el_moves[move][1];
        brute_force_breadth_first(seq_num, vowels, new_r, new_c, seq_found, nodes_visited);
    }
}

void
KeyBoard::smart_breadth_first(int seq_num, 
                          int vowels, 
                          int r, 
                          int c,
                          long &seq_found,
                          long &nodes_visited)
{
    // track the total number of nodes visited
    ++nodes_visited;

#if 0
    // sanity checks
    if ((r < 0) || (r >= max_row) ||
        (c < 0) || (c >= max_col)) return;

    // is this an acceptable character?
    if (_board[r][c]._value == null_key) return;
#endif

    // is this a vowel?
    if (_board[r][c]._vowel) --vowels;

    // is the number of vowels valid?
    if (vowels < 0) return;

    // we found another character
    --seq_num;

    // is this the end of a sequence?
    if (seq_num == 0)
    {
        seq_found += 1;
        return;
    }

    // not the end of a sequence. check next L-step, that is,
    // expand the horizon to the next step.
    int index = _board[r][c]._index;

    int *pm = NULL;
    if (vowels >= 1)
        pm = _one_vowel + index*(_key_num+1)*2;
    else
        pm = _zero_vowel + index*(_key_num+1)*2;
    int mi = pm[0*2+0];

    int *pmi = NULL;
    for (int i=0; i<=mi; ++i)
    {
        pmi = pm + (i+1)*2;
        smart_breadth_first(seq_num, 
                        vowels, 
                        *pmi, 
                        *(pmi+1), 
                        seq_found, 
                        nodes_visited);
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
    cout.imbue(locale(""));

    if (argc != 3 && argc != 4)
    {
        usage(argv[0]);
        return 2;
    }

    int sequence_length;
    istringstream(argv[1]) >> sequence_length;
    assert(sequence_length > 0);
    cout << endl << "Sequence Length: " << sequence_length << endl;

    int number_of_vowels;
    istringstream(argv[2]) >> number_of_vowels;
    assert(number_of_vowels >= 0);
    cout << endl << "Number of Vowels: " << number_of_vowels << endl;

    bool verbose = false;
    if (argc == 4)
    {
        int dummy;
        istringstream(argv[3]) >> dummy;
        verbose = (dummy == 0) ? false : true;
    }

    KeyBoard b;

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

    cout << endl << "Brute-force depth-first crunching ..." << endl;
    NanoSecTime br_s;
    long brute_force_sequences_found = 0;
    long brute_force_nodes_visited = 0;
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            b.brute_force_depth_first(sequence_length, 
                                    number_of_vowels, 
                                    row, 
                                    col,
                                    brute_force_sequences_found,
                                    brute_force_nodes_visited);
        }
    }
    NanoSecTime br_e;
    cout << "Number of sequences found (brute force depth-first): " << brute_force_sequences_found << endl;
    cout << "Time (brute force depth-first): " << (br_e-br_s) << endl;
    cout << "Nodes visited (brute force depth-first): " << brute_force_nodes_visited << endl;
    cout << "Real Nodes per Nodes visited (brute force depth-first): " << double(brute_force_nodes_visited)/double(brute_force_sequences_found) << endl;

    cout << endl << "Smart depth-first crunching ..." << endl;
    NanoSecTime sm_s;
    long smart_sequences_found = 0;
    long smart_nodes_visited = 0;
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            // is this an acceptable character?
            if (b.key(row,col) == KeyBoard::null_key) continue;

            // generate sequences
            b.smart_depth_first(sequence_length, 
                              number_of_vowels, 
                              row, 
                              col,
                              smart_sequences_found,
                              smart_nodes_visited);
        }
    }
    NanoSecTime sm_e;
    cout << "Number of sequences found (smart depth-first): " << smart_sequences_found << endl;
    cout << "Time (smart depth-first): " << (sm_e-sm_s) << endl;
    cout << "Nodes visited (smart depth-first): " << smart_nodes_visited << endl;
    cout << "Real Nodes per Nodes visited (smart depth-first): " << double(smart_nodes_visited)/double(smart_sequences_found) << endl;

    cout << endl << "Brute-force breadth-first crunching ..." << endl;
    NanoSecTime br_br_s;
    long br_brute_force_sequences_found = 0;
    long br_brute_force_nodes_visited = 0;
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            b.brute_force_breadth_first(sequence_length, 
                                    number_of_vowels, 
                                    row, 
                                    col,
                                    br_brute_force_sequences_found,
                                    br_brute_force_nodes_visited);
        }
    }
    NanoSecTime br_br_e;
    cout << "Number of sequences found (brute force breadth-first): " << br_brute_force_sequences_found << endl;
    cout << "Time (brute force breadth-first): " << (br_br_e-br_br_s) << endl;
    cout << "Nodes visited (brute force breadth-first): " << br_brute_force_nodes_visited << endl;
    cout << "Real Nodes per Nodes visited (brute force breadth-first): " << double(br_brute_force_nodes_visited)/double(br_brute_force_sequences_found) << endl;

    cout << endl << "Smart breadth-first crunching ..." << endl;
    NanoSecTime br_sm_s;
    long br_smart_sequences_found = 0;
    long br_smart_nodes_visited = 0;
    for (int row=0; row<KeyBoard::max_row; ++row)
    {
        for (int col=0; col<KeyBoard::max_col; ++col)
        {
            // is this an acceptable character?
            if (b.key(row,col) == KeyBoard::null_key) continue;

            // generate sequences
            b.smart_breadth_first(sequence_length, 
                              number_of_vowels, 
                              row, 
                              col,
                              br_smart_sequences_found,
                              br_smart_nodes_visited);
        }
    }
    NanoSecTime br_sm_e;
    cout << "Number of sequences found (smart breadth-first): " << br_smart_sequences_found << endl;
    cout << "Time (smart breadth-first): " << (br_sm_e-br_sm_s) << endl;
    cout << "Nodes visited (smart breadth-first): " << br_smart_nodes_visited << endl;
    cout << "Real Nodes per Nodes visited (smart breadth-first): " << double(br_smart_nodes_visited)/double(br_smart_sequences_found) << endl;

    return 0;
}

