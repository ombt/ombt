#ifndef __DATA_H
#define __DATA_H
// data class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations
class DataItem;

// data class definition
class DataItem {
public:
        // constructors and destructor
        DataItem(long d = 0): data(d) {
                // do nothing
        }
        DataItem(const DataItem &d): data(d.data) {
                // do nothing
        }
        virtual ~DataItem() {
                // do nothing
        }

        // assignment and equivalence
        virtual DataItem &operator=(const DataItem &d) {
                if (this != &d) 
                        data = d.data;
                return(*this);
        }
        virtual int operator==(const DataItem &d) const {
                return(data == d.data);
        }
        virtual int operator!=(const DataItem &d) const {
                return(data != d.data);
        }
        virtual int operator<(const DataItem &d) const {
                return(data < d.data);
        }
        virtual int operator>(const DataItem &d) const {
                return(data > d.data);
        }
        virtual int operator<=(const DataItem &d) const {
                return(data <= d.data);
        }
        virtual int operator>=(const DataItem &d) const {
                return(data >= d.data);
        }

        // printing
        virtual void dump(ostream &os = cout) const {
                os << data;
        }
        friend ostream &operator<<(ostream &os, const DataItem &d) {
                d.dump(os);
                return(os);
        }

protected:
        // dummy data
        long data;
};

#endif
