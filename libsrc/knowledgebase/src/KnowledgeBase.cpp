//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// knowledgebase for logic problems

// system includes
#include <string>
#include <list>

// local includes
#include "system/Debug.h"
#include "errors/Errors.h"
#include "hdr/KnowledgeBase.h"

namespace ombt {

// allocate 
KnowledgeBase *
KnowledgeBase::allocate(const Program &program)
{
    return new KnowledgeBase(program);
}

// ctors and dtor
KnowledgeBase::KnowledgeBase():
    error_(),
    program_(),
    BaseObject()
{
}

KnowledgeBase::KnowledgeBase(const Program &program): 
    error_(),
    program_(program),
    BaseObject()
{
}

KnowledgeBase::KnowledgeBase(const KnowledgeBase &src): 
    error_(src.error_),
    program_(src.program_),
    BaseObject(src)
{
}

KnowledgeBase::~KnowledgeBase()
{
}

// assignment
KnowledgeBase &
KnowledgeBase::operator=(const Program &program)
{
    program_ = program;
    return(*this);
}

KnowledgeBase &
KnowledgeBase::operator=(const KnowledgeBase &rhs)
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
operator<<(std::ostream &os, const KnowledgeBase &p)
{
    return os;
}

}

