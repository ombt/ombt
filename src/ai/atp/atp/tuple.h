#ifndef __TUPLE_H
#define __TUPLE_H
// tuple class definition, combines key and data values.

// headers
#include <stdlib.h>
#include <iostream.h>

// tuple class
template <class KeyType, class DataType> class Tuple {
public:
        // constructors and destructor
        Tuple() {
		// do nothing
	};
        Tuple(const KeyType &k):
		key(k), data() {
		// do nothing
	};
        Tuple(const KeyType &k, const DataType &d):
		key(k), data(d) {
		// do nothing
	};
        Tuple(const Tuple &t):
		key(t.key), data(t.data) {
		// do nothing
	};
        ~Tuple() {
		// do nothing
	};

        // assignment and relational operators
        Tuple &operator=(const Tuple &t) {
		if (this != &t) {
			key = t.key;
			data = t.data;
		}
		return(*this);
	};
        int operator==(const Tuple &t) const {
		return(key == t.key);
	};
        int operator!=(const Tuple &t) const {
		return(key != t.key);
	};
        int operator<(const Tuple &t) const {
		return(key < t.key);
	};
        int operator<=(const Tuple &t) const {
		return(key <= t.key);
	};
        int operator>(const Tuple &t) const {
		return(key > t.key);
	};
        int operator>=(const Tuple &t) const {
		return(key >= t.key);
	};

	// print data
	friend ostream &operator<<(ostream &os, 
		const Tuple<KeyType, DataType> &t) {
		os << "(" << t.key << "," << t.data << ")" << endl;
		return(os);
	};

        // internal data is public
        KeyType key;
        DataType data;
};

#endif
