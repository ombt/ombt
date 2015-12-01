//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_PRE_PROCESSOR_H
#define __OMBT_PRE_PROCESSOR_H

// preprocessor for logic problems.

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

// logic program preprocessor
class PreProcessor: public BaseObject
{
public:
    // allocate 
    static PreProcessor *allocate(const Program &);

    // ctors and dtor
    PreProcessor();
    PreProcessor(const Program &program);
    PreProcessor(const PreProcessor &src);
    virtual ~PreProcessor();

    // assignment
    PreProcessor &operator=(const Program &program);
    PreProcessor &operator=(const PreProcessor &rhs);

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
    friend std::ostream &operator<<(std::ostream &, const PreProcessor &);

protected:
    // internal data
    Errors error_;
    Program program_;
};

}

#endif

