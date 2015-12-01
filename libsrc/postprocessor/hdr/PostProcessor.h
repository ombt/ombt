//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_POST_PROCESSOR_H
#define __OMBT_POST_PROCESSOR_H

// postprocessor for logic problems.

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

// logic program postprocessor
class PostProcessor: public BaseObject
{
public:
    // allocate 
    static PostProcessor *allocate(const Program &);

    // ctors and dtor
    PostProcessor();
    PostProcessor(const Program &program);
    PostProcessor(const PostProcessor &src);
    virtual ~PostProcessor();

    // assignment
    PostProcessor &operator=(const Program &program);
    PostProcessor &operator=(const PostProcessor &rhs);

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
    friend std::ostream &operator<<(std::ostream &, const PostProcessor &);

protected:
    // internal data
    Errors error_;
    Program program_;
};

}

#endif

