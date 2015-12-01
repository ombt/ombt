//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_STRATEGY_H
#define __OMBT_STRATEGY_H

// strategy used to solve logic problems

// os headers
#include <string>
#include <list>
#include <iostream>

// headers
#include "atomic/BaseObject.h"
#include "atomic/ExtUseCntPtr.h"
#include "errors/Errors.h"
#include "logic/Program.h"
#include "preprocessor/PreProcessor.h"
#include "postprocessor/PostProcessor.h"
#include "knowledgebase/KnowledgeBase.h"
#include "inference/Inference.h"
#include "indexing/Indexing.h"

namespace ombt {

// prover to solve logic problems
class Strategy: public BaseObject
{
public:
    // allocate 
    static Strategy *allocate(const Program &);

    // ctors and dtor
    Strategy();
    Strategy(const Program &);
    Strategy(const Strategy &src);
    virtual ~Strategy();

    // assignment
    Strategy &operator=(const Program &program);
    Strategy &operator=(const Strategy &rhs);

    // misc
    inline void program(const Program &program)
        { program_ = program; }
    inline const Program &program() const
        { return program_; }
    inline bool operator!() const
        { return !error_; }
    inline const Errors &error() const
        { return error_; }

    // inference methods
    virtual int run();

    // print data
    friend std::ostream &operator<<(std::ostream &, const Strategy &);

protected:
    int run(ExtUseCntPtr<KnowledgeBase> &kb,
            ExtUseCntPtr<Indexing> &index,
            ExtUseCntPtr<PreProcessor> &prep,
            ExtUseCntPtr<Inference> &ie,
            ExtUseCntPtr<PostProcessor> &post);

protected:
    // internal data
    Errors error_;
    Program program_;
};

}

#endif

