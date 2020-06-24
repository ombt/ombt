// member functions for stack class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "stack.h"

// constructors and destructor
Stack::Stack(): stack() 
{
        // do nothing
}

Stack::Stack(const Stack &s): stack(s.stack) 
{
        // do nothing
}

Stack::~Stack() 
{
        // do nothing
}

// assignment
Stack &
Stack::operator=(const Stack &s) 
{
        if (this != &s)
                stack = s.stack;
        return(*this);
}

// stack operations
int 
makeNull(Stack &s) 
{
        return(makeNull(s.stack));
}
int 
push(Stack &s, DataItem *d) 
{
        return(insert(s.stack, d, STARTOFLIST));
}

int 
pop(Stack &s, DataItem *&d) 
{
        return(retrieveAndRemove(s.stack, d, STARTOFLIST));
}

int 
top(const Stack &s, DataItem *&d) 
{
        return(retrieve(s.stack, d, STARTOFLIST));
}

int 
isEmpty(const Stack &s) 
{
        return(isEmpty(s.stack));
}

// print data
void 
Stack::dump(ostream &os) const
{
        stack.dump(os);
}

ostream &
operator<<(ostream &os, const Stack &s) 
{
        s.dump(os);
        return(os);
}
