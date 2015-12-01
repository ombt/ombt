//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// strategy for logic problems

// system includes
#include <string>
#include <list>

// local includes
#include "system/Debug.h"
#include "hdr/Strategy.h"

namespace ombt {

// allocate 
Strategy *
Strategy::allocate(const Program &program)
{
    return new Strategy(program);
}

// ctors and dtor
Strategy::Strategy():
    error_(),
    program_(),
    BaseObject()
{
}

Strategy::Strategy(const Program &program): 
    error_(),
    program_(program),
    BaseObject()
{
}

Strategy::Strategy(const Strategy &src): 
    error_(src.error_),
    program_(src.program_),
    BaseObject(src)
{
}

Strategy::~Strategy()
{
}

// assignment
Strategy &
Strategy::operator=(const Program &program)
{
    program_ = program;
    return(*this);
}

Strategy &
Strategy::operator=(const Strategy &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        error_ = rhs.error_;
        program_ = rhs.program_;
    }
    return(*this);
}

// proof strategy functions
int
Strategy::run()
{
    ExtUseCntPtr<PreProcessor> prep(PreProcessor::allocate(program_));
    ExtUseCntPtr<KnowledgeBase> kb(KnowledgeBase::allocate(program_));
    ExtUseCntPtr<Indexing> index(Indexing::allocate(program_));
    ExtUseCntPtr<Inference> ie(Inference::allocate(program_));
    ExtUseCntPtr<PostProcessor> postp(PostProcessor::allocate(program_));
    return run(kb, index, prep, ie, postp);
}

int 
Strategy::run(ExtUseCntPtr<KnowledgeBase> &kb,
              ExtUseCntPtr<Indexing> &index,
              ExtUseCntPtr<PreProcessor> &prep,
              ExtUseCntPtr<Inference> &ie,
              ExtUseCntPtr<PostProcessor> &postp)
{
    return error_.ok();
}

// output operators
std::ostream &
operator<<(std::ostream &os, const Strategy &p)
{
    return os;
}

}

