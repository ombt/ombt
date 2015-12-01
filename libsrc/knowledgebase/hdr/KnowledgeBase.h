//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_KNOWLEDGEBASE_H
#define __OMBT_KNOWLEDGEBASE_H

// knowledgebase for logic problems.

// os headers
#include <string>
#include <list>
#include <iostream>

// headers
#include "atomic/BaseObject.h"
#include "atomic/ExtUseCntPtr.h"
#include "errors/Errors.h"
#include "logic/Program.h"

namespace ombt {

// logic program knowledgebase
class KnowledgeBase: public BaseObject
{
public:
    // allocate 
    static KnowledgeBase *allocate(const Program &);

    // ctors and dtor
    KnowledgeBase();
    KnowledgeBase(const Program &program);
    KnowledgeBase(const KnowledgeBase &src);
    virtual ~KnowledgeBase();

    // assignment
    KnowledgeBase &operator=(const Program &program);
    KnowledgeBase &operator=(const KnowledgeBase &rhs);

    // misc
    inline void program(const Program &program)
        { program_ = program; }
    inline const Program &program() const
        { return program_; }
    inline bool operator!() const
        { return !error_; }
    inline const Errors &error() const
        { return error_; }

    // print data
    friend std::ostream &operator<<(std::ostream &, const KnowledgeBase &);

protected:
    // internal data
    Errors error_;
    Program program_;
};

}

#endif

