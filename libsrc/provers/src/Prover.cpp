//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// prover for logic problems

// system includes
#include <string>
#include <list>

// local includes
#include "system/Debug.h"
#include "hdr/Prover.h"
#include "hdr/Strategy.h"

namespace ombt {

// ctors and dtor
Prover::Prover():
    error_(),
    program_(),
    BaseObject()
{
}

Prover::Prover(const Prover &src): 
    error_(src.error_),
    program_(src.program_),
    BaseObject(src)
{
}

Prover::~Prover()
{
}

// assignment
Prover &
Prover::operator=(const Prover &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        error_ = rhs.error_;
        program_ = rhs.program_;
    }
    return(*this);
}

// inference functions
void 
Prover::setProgram(const Program &p)
{
    program_ = p;
    error_.ok();
}

// run prover on given program
int 
Prover::prove()
{
    // what type of program do we have?
    if (program_.form_type() != Program::FT_ParsedForm)
    {
        return error_.notok(Errors::EC_Program_Not_A_Parsed_Form,
                           "%s'%d: Unexpected form type: %d\n",
                            __FILE__, __LINE__, program_.form_type());
    }

    // what type of logic?
    switch (program_.logic_type())
    {
    case Program::LT_Propositional:
    case Program::LT_PredicateWithEquality:
    case Program::LT_Predicate:
    {
        ExtUseCntPtr<Strategy> pstrategy(Strategy::allocate(program_));
        return pstrategy->run();
    }
    default:
    {
        return error_.notok(Errors::EC_Unsupported_Logic_Type,
                           "%s'%d: Unexpected logic type: %d\n",
                            __FILE__, __LINE__, program_.logic_type());
    }
    }
    return error_.ok();
}

// output operators
std::ostream &
operator<<(std::ostream &os, const Prover &p)
{
    os << p.program_;
    return os;
}

}

