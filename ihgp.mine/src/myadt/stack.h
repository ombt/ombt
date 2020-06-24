#ifndef __STACK_H
#define __STACK_H
// stack class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"
#include "list.h"

// forward declarations
class Stack;

// stack class
class Stack {
public:
        // constructors and destructor
        Stack();
        Stack(const Stack &);
        virtual ~Stack();

        // assignment
        virtual Stack &operator=(const Stack &);

        // stack operations
        friend int makeNull(Stack &);
        friend int push(Stack &, DataItem *);
        friend int pop(Stack &, DataItem *&);
        friend int top(const Stack &, DataItem *&);
        friend int isEmpty(const Stack &);

        // print data
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const Stack &);

protected:
        // data
        List stack;
};


#endif
