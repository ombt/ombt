//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// postprocessor for logic problems

// system includes
#include <string>
#include <list>

// local includes
#include "system/Debug.h"
#include "errors/Errors.h"
#include "hdr/PostProcessor.h"

namespace ombt {

// allocate 
PostProcessor *
PostProcessor::allocate(const Program &program)
{
    return new PostProcessor(program);
}

// ctors and dtor
PostProcessor::PostProcessor():
    error_(),
    program_(),
    BaseObject()
{
}

PostProcessor::PostProcessor(const Program &program): 
    error_(),
    program_(program),
    BaseObject()
{
}

PostProcessor::PostProcessor(const PostProcessor &src): 
    error_(src.error_),
    program_(src.program_),
    BaseObject(src)
{
}

PostProcessor::~PostProcessor()
{
}

// assignment
PostProcessor &
PostProcessor::operator=(const Program &program)
{
    program_ = program;
    return(*this);
}

PostProcessor &
PostProcessor::operator=(const PostProcessor &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        error_ = rhs.error_;
        program_ = rhs.program_;
    }
    return(*this);
}

// output operators
std::ostream &
operator<<(std::ostream &os, const PostProcessor &p)
{
    return os;
}

}

