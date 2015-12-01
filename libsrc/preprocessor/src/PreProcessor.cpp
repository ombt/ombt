//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// proprocessor for logic problems

// system includes
#include <string>
#include <list>

// local includes
#include "system/Debug.h"
#include "errors/Errors.h"
#include "hdr/PreProcessor.h"

namespace ombt {

// allocate 
PreProcessor *
PreProcessor::allocate(const Program &program)
{
    return new PreProcessor(program);
}

// ctors and dtor
PreProcessor::PreProcessor():
    error_(),
    program_(),
    BaseObject()
{
}

PreProcessor::PreProcessor(const Program &program): 
    error_(),
    program_(program),
    BaseObject()
{
}

PreProcessor::PreProcessor(const PreProcessor &src): 
    error_(src.error_),
    program_(src.program_),
    BaseObject(src)
{
}

PreProcessor::~PreProcessor()
{
}

// assignment
PreProcessor &
PreProcessor::operator=(const Program &program)
{
    program_ = program;
    return(*this);
}

PreProcessor &
PreProcessor::operator=(const PreProcessor &rhs)
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
operator<<(std::ostream &os, const PreProcessor &p)
{
    return os;
}

}

