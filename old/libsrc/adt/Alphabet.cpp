// member functions for alphabet and word classes

// alphabet constructors and destructor
template <class DataType>
Alphabet<DataType>::Alphabet(const List<DataType> &al):
	size(0), set(al), orderedset()
{
	ListIterator<DataType> setIter(set);
	for ( ; !setIter.done(); setIter++, size++)
	{
		orderedset.insert(Tuple<DataType, int>(setIter(), size));
	}
}

template <class DataType>
Alphabet<DataType>::Alphabet(const Alphabet<DataType> &a):
	size(a.size), set(a.set), orderedset(a.orderedset)
{
	// do nothing
}

template <class DataType>
Alphabet<DataType>::~Alphabet()
{
	// do nothing
}

// assignment 
template <class DataType>
Alphabet<DataType> &
Alphabet<DataType>::operator=(const Alphabet<DataType> &a)
{
	if (this != &a)
	{
		size = a.size;
		set = a.set;
		orderedset = a.orderedset;
	}
	return(*this);
}

// does alphabet include a code
template <class DataType>
int
Alphabet<DataType>::includes(const DataType &c) const
{
	return(orderedset.includes(Tuple<DataType, int>(c, 0)));
}

// comparison operators
template <class DataType>
int
Alphabet<DataType>::eq(const DataType &l, const DataType &r) const
{
	Tuple<DataType, int> lai(l, 0);
	MustBeTrue(orderedset.retrieve(lai) == OK);
	Tuple<DataType, int> rai(r, 0);
	MustBeTrue(orderedset.retrieve(rai) == OK);
	return(lai.data == rai.data);
}

template <class DataType>
int
Alphabet<DataType>::neq(const DataType &l, const DataType &r) const
{
	Tuple<DataType, int> lai(l, 0);
	MustBeTrue(orderedset.retrieve(lai) == OK);
	Tuple<DataType, int> rai(r, 0);
	MustBeTrue(orderedset.retrieve(rai) == OK);
	return(lai.data != rai.data);
}

template <class DataType>
int
Alphabet<DataType>::lt(const DataType &l, const DataType &r) const
{
	Tuple<DataType, int> lai(l, 0);
	MustBeTrue(orderedset.retrieve(lai) == OK);
	Tuple<DataType, int> rai(r, 0);
	MustBeTrue(orderedset.retrieve(rai) == OK);
	return(lai.data < rai.data);
}

template <class DataType>
int
Alphabet<DataType>::ltoreq(const DataType &l, const DataType &r) const
{
	Tuple<DataType, int> lai(l, 0);
	MustBeTrue(orderedset.retrieve(lai) == OK);
	Tuple<DataType, int> rai(r, 0);
	MustBeTrue(orderedset.retrieve(rai) == OK);
	return(lai.data <= rai.data);
}

template <class DataType>
int
Alphabet<DataType>::gt(const DataType &l, const DataType &r) const
{
	Tuple<DataType, int> lai(l, 0);
	MustBeTrue(orderedset.retrieve(lai) == OK);
	Tuple<DataType, int> rai(r, 0);
	MustBeTrue(orderedset.retrieve(rai) == OK);
	return(lai.data > rai.data);
}

template <class DataType>
int
Alphabet<DataType>::gtoreq(const DataType &l, const DataType &r) const
{
	Tuple<DataType, int> lai(l, 0);
	MustBeTrue(orderedset.retrieve(lai) == OK);
	Tuple<DataType, int> rai(r, 0);
	MustBeTrue(orderedset.retrieve(rai) == OK);
	return(lai.data >= rai.data);
}

#if USEOSTREAM
// print alphabet encoding
template <class DataType>
std::ostream &
operator<<(std::ostream &os, const Alphabet<DataType> &a)
{
	ListIterator<DataType> setIter(a.set);
	os << "alphabet = { ";
	for ( ; !setIter.done(); setIter++)
	{
		os << setIter() << std::endl;
	}
	os << "}" << std::endl;
	return(os);
}
#endif

// alphabet iterator constructors and destructor
template <class DataType>
AlphabetIterator<DataType>::AlphabetIterator(
	const AlphabetIterator<DataType> &ai): 
	alphabet(ai.alphabet), alphabetIter(ai.alphabetIter)
{
	// do nothing
}

template <class DataType>
AlphabetIterator<DataType>::AlphabetIterator(
	const Alphabet<DataType> &a): 
	alphabet(a), alphabetIter(a.set)
{
	// do nothing
}

template <class DataType>
AlphabetIterator<DataType>::~AlphabetIterator()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
AlphabetIterator<DataType>::reset()
{
	alphabetIter.reset();
	return;
}

// is list empty
template <class DataType>
int
AlphabetIterator<DataType>::done() const
{
	return(alphabetIter.done());
}

// return current data
template <class DataType>
DataType
AlphabetIterator<DataType>::operator()()
{
	return(alphabetIter());
}

// increment to next data item in list
template <class DataType>
int
AlphabetIterator<DataType>::operator++(int)
{
	int status = alphabetIter++;
	return(status);
}

// local definitions
#define DefaultWordSize 128
#define BufSzToStrSz(bs) ((bs)-1)
#define StrSzToBufSz(ss) ((ss)+1)

// word constructors and destructor
template <class DataType>
Word<DataType>::Word(const Alphabet<DataType> &a):
	alphabet(a), size(0), maxsize(DefaultWordSize), 
	buffer(new DataType [DefaultWordSize])
{
	// check if buffer was allocated
        MustBeTrue(buffer != NULL);
}

template <class DataType>
Word<DataType>::Word(const Alphabet<DataType> &a, DataType c):
	alphabet(a), size(1), maxsize(DefaultWordSize), 
	buffer(new DataType [DefaultWordSize])
{
	// check if buffer was allocated
        MustBeTrue(buffer != NULL);

	// store character 
	buffer[0] = c;
}

template <class DataType>
Word<DataType>::Word(const Alphabet<DataType> &a, int wsz, const DataType *pw):
	alphabet(a), size(wsz), maxsize(wsz), 
	buffer(new DataType [wsz])
{
	// check for invalid pointer
	MustBeTrue(buffer != NULL && maxsize > 0);

	// copy over data
	if (pw != NULL)
	{
		for (int i = 0; i < size; i++)
		{
			buffer[i] = pw[i];
		}
	}
}

template <class DataType>
Word<DataType>::Word(const Word<DataType> &w):
	alphabet(w.alphabet), size(w.size), maxsize(w.maxsize), 
	buffer(new DataType [w.maxsize])
{
	// check if buffer was allocated
        MustBeTrue(maxsize > 0 && buffer != NULL);

	// copy string
	wordncpy(buffer, w.buffer, size);
}

template <class DataType>
Word<DataType>::~Word()
{
	// clear everything
	delete [] buffer;
	maxsize = size = 0;
}

// assignment operators
template <class DataType>
Word<DataType> &
Word<DataType>::operator=(const Word<DataType> &rhs)
{
	// check for self-assignment
	if (this == &rhs) return(*this);

	// is buffer long enough
	if (rhs.size > maxsize)
	{
		// delete old buffer and allocate new one
		delete [] buffer;
		maxsize = rhs.maxsize;
        	MustBeTrue(maxsize >= 0);
		buffer = new DataType [maxsize];
        	MustBeTrue(buffer != NULL);
	}

	// copy string
	size = rhs.size;
	wordncpy(buffer, rhs.buffer, size);

	// return modified string
	return(*this);
}

// access operators
template <class DataType>
DataType &
Word<DataType>::operator[](int idx)
{
	MustBeTrue(0 <= idx && idx < maxsize);
	if ((idx+1) > size)
		size = idx+1;
	return(buffer[idx]);
}

template <class DataType>
const DataType &
Word<DataType>::operator[](int idx) const
{
	MustBeTrue(0 <= idx && idx < size);
	return(buffer[idx]);
}

// logical operators
template <class DataType>
int
Word<DataType>::operator==(const Word<DataType> &rhs) const
{
	return(wordcmp(rhs) == 0);
}

template <class DataType>
int
Word<DataType>::operator!=(const Word<DataType> &rhs) const
{
	return(wordcmp(rhs) != 0);
}

template <class DataType>
int
Word<DataType>::operator<(const Word<DataType> &rhs) const
{
	return(wordcmp(rhs) < 0);
}

template <class DataType>
int
Word<DataType>::operator<=(const Word<DataType> &rhs) const
{
	return(wordcmp(rhs) <= 0);
}

template <class DataType>
int
Word<DataType>::operator>(const Word<DataType> &rhs) const
{
	return(wordcmp(rhs) > 0);
}

template <class DataType>
int
Word<DataType>::operator>=(const Word<DataType> &rhs) const
{
	return(wordcmp(rhs) >= 0);
}

template <class DataType>
int
Word<DataType>::wordcmp(const Word<DataType> &w) const
{
	// check for self comparisons
	if (this == &w)
		return(0);

	// now compare the strings
	int imax = (size < w.size) ? size : w.size;
	for (int i = 0; i < imax; i++)
	{
		// check if equal
		if (buffer[i] != w.buffer[i])
		{
			// not equal, must < or >
			if (alphabet.lt(buffer[i], w.buffer[i]))
				return(-1);
			else
				return(1);
		}
	}

	// one is a prefix of the other or the same
	return(size - w.size);
}

// substring operator
template <class DataType>
Word<DataType>
Word<DataType>::operator()(int start) const
{
	return((*this)(start, maxsize));
}

template <class DataType>
Word<DataType>
Word<DataType>::operator()(int start, int ncpy) const
{
	// check start of substring.
	MustBeTrue(0 <= start && start < size);
	MustBeTrue(0 <= ncpy);

	// calculate length of string to copy
	if ((start+ncpy) > maxsize)
		ncpy = maxsize - start;
	if (ncpy < 0)
		ncpy = 0;

	// copy substring
	Word<DataType> substr(alphabet, ncpy+1);
	wordncpy(substr.buffer, buffer+start, ncpy);
	substr.size = ncpy;

	// return substring
	return(substr);
}

// copy one string to another
template <class DataType>
void 
Word<DataType>::wordncpy(DataType *pwb, const DataType *pwb2, int iwmax) const
{
	for (int iw = 0; iw < iwmax; iw++)
	{
		*(pwb + iw) = *(pwb2 + iw);
	}
	return;
}

// string length
template <class DataType>
int
Word<DataType>::wordlen() const
{
	return(size);
}

// casting string to character array
template <class DataType>
Word<DataType>::operator const DataType *() const
{
	return(buffer);
}

template <class DataType>
Word<DataType>::operator DataType *()
{
	return(buffer);
}

#if USEOSTREAM
// input and output
template <class DataType>
std::ostream &
operator<<(std::ostream &os, const Word<DataType> &s)
{
	MustBeTrue(s.buffer != NULL);
	for (int i = 0; i < s.size; i++)
	{
		os << s.buffer[i];
	}
	return(os);
}
#endif

// concatenation operators
template <class DataType>
Word<DataType> &
Word<DataType>::operator+=(const Word<DataType> &rhs)
{
	// check if current buffer is long enough for new buffer
	int newsize = size + rhs.size;
	if (newsize > maxsize)
	{
		// save the old buffer for now
		DataType *pobuf = buffer;

		// allocate a new buffer
		buffer = new DataType [newsize];
		MustBeTrue(buffer != NULL);

		// copy old buffer
		wordncpy(buffer, pobuf, size);

		// delete old buffer
		delete [] pobuf;

		// set new maxsize
		maxsize = newsize;
	}

	// copy buffer at the end of the current buffer
	wordncpy(buffer+size, rhs.buffer, rhs.size);

	// new size of string
	size = newsize;

	// return new string
	return(*this);
}

template <class DataType>
Word<DataType>
Word<DataType>::operator+(const Word<DataType> &rhs) const
{
	return(Word<DataType>(*this) += rhs);
}

// word iterator constructors and destructor
template <class DataType>
WordIterator<DataType>::WordIterator(const WordIterator<DataType> &wi): 
	word(wi.word), current(wi.current)
{
	// do nothing
}

template <class DataType>
WordIterator<DataType>::WordIterator(const Word<DataType> &w): 
	word(w), current(0)
{
	// do nothing
}

template <class DataType>
WordIterator<DataType>::~WordIterator()
{
	current = 0;
}

// reset iterator to beginning
template <class DataType>
void
WordIterator<DataType>::reset()
{
	current = 0;
	return;
}

// is list empty
template <class DataType>
int
WordIterator<DataType>::done() const
{
	return(current >= word.size);
}

// return current data
template <class DataType>
DataType
WordIterator<DataType>::operator()()
{
	return(word.buffer[current]);
}

// increment to next data item in list
template <class DataType>
int
WordIterator<DataType>::operator++(int)
{
	if (current < word.size)
	{
		current++;
		return(OK);
	}
	else
		return(NOTOK);
}

// get current index
template <class DataType>
int
WordIterator<DataType>::getIndex() const
{
	return(current);
}

