#ifndef __TWOKEYS_H
#define __TWOKEYS_H
// two keys class definition, combines two key.

// headers
#include <stdlib.h>
#include <iostream>

// two keys class
template <class KeyType1, class KeyType2> class TwoKeys {
public:
        // constructors and destructor
        TwoKeys() {
		// do nothing
	};
        TwoKeys(const KeyType1 &k1, const KeyType2 &k2):
		key1(k1), key2(k2) {
		// do nothing
	};
        TwoKeys(const TwoKeys &t):
		key1(t.key1), key2(t.key2) {
		// do nothing
	};
        ~TwoKeys() {
		// do nothing
	};

        // assignment and relational operators
        TwoKeys &operator=(const TwoKeys &t) {
		if (this != &t) {
			key1 = t.key1;
			key2 = t.key2;
		}
		return(*this);
	};
        int operator==(const TwoKeys &t) const {
		if ((key1 == t.key1) && (key2 == t.key2))
			return(1);
		else
			return(0);
	};
        int operator!=(const TwoKeys &t) const {
		if ((key1 != t.key1) || (key2 != t.key2))
			return(1);
		else
			return(0);
	};
        int operator<(const TwoKeys &t) const {
		if (key1 < t.key1)
			return(1);
		else if ((key1 == t.key1) && (key2 < t.key2))
			return(1);
		else
			return(0);
	};
        int operator<=(const TwoKeys &t) const {
		if ((key1 == t.key1) && (key2 == t.key2))
			return(1);
		else if (key1 < t.key1)
			return(1);
		else if ((key1 == t.key1) && (key2 < t.key2))
			return(1);
		else
			return(0);
	};
        int operator>(const TwoKeys &t) const {
		if (key1 > t.key1)
			return(1);
		else if ((key1 == t.key1) && (key2 > t.key2))
			return(1);
		else
			return(0);
	};
        int operator>=(const TwoKeys &t) const {
		if ((key1 == t.key1) && (key2 == t.key2))
			return(1);
		else if (key1 . t.key1)
			return(1);
		else if ((key1 == t.key1) && (key2 > t.key2))
			return(1);
		else
			return(0);
	};

#if USEOSTREAM
	// print data
	friend ostream &operator<<(ostream &os, 
		const TwoKeys<KeyType1, KeyType2> &t) {
		os << "(" << t.key1 << "," << t.key2 << ")" << endl;
		return(os);
	};
#endif

        // internal data is public
        KeyType1 key1;
        KeyType2 key2;
};

#endif
