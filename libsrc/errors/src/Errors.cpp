//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// report errors 

#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#include "system/Debug.h"
#include "hdr/Errors.h"

namespace ombt {

// local convenience define
#undef STRINGIFYENUM
#define STRINGIFYENUM(ENUM) { ENUM, #ENUM }

// static class members
Errors::ErrorMessages Errors::messages_[] = 
{
    STRINGIFYENUM(EC_Unknown),
    STRINGIFYENUM(EC_Success),
    STRINGIFYENUM(EC_Bad_Arguments),
    STRINGIFYENUM(EC_Program_Not_A_Parsed_Form),
    STRINGIFYENUM(EC_Unsupported_Logic_Type),
    STRINGIFYENUM(EC_Last)
};

// ctors and dtor
Errors::Errors():
    error_code_(EC_Unknown),
    error_msg_(BUFSIZ, ' '),
    BaseObject()
{
    MustBeTrue((sizeof(messages_)/sizeof(ErrorMessages)) == (EC_Last+2));
}

Errors::Errors(const Errors &src):
    error_code_(src.error_code_),
    error_msg_(src.error_msg_),
    BaseObject(src)
{
}

Errors::~Errors()
{
}

// assignment
Errors &
Errors::operator=(const Errors &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        error_code_ = rhs.error_code_;
        error_msg_ = rhs.error_msg_;
    }
    return *this;
}

// translate to message
const std::string
Errors::message(Errors::ErrorCodes ec)
{
    return messages_[ec+1].message_;
}

// output operator
int
Errors::ok()
{
    error_code_ = EC_Success;
    error_msg_ = "";
    return OK;
}

int
Errors::notok(Errors::ErrorCodes ec, const std::string &format, ...)
{
    char buffer[BUFSIZ];
    va_list ap;
    va_start(ap, format);
    vsprintf(buffer, format.c_str(), ap);
    va_end(ap);
    error_code_ = ec;
    error_msg_ = buffer;
    return NOTOK;
}

void
Errors::printf(Errors::ErrorCodes ec, const std::string &format, ...)
{
    char buffer[BUFSIZ];
    va_list ap;
    va_start(ap, format);
    vsprintf(buffer, format.c_str(), ap);
    va_end(ap);
    error_code_ = ec;
    error_msg_ = buffer;
}

std::ostream &
operator<<(std::ostream &os, const Errors &e)
{
    os << "Code: " << Errors::message(e.error_code_)
       << ", "
       << "Msg: " << e.error_msg_;
    return(os);
}

void
Errors::dump(std::ostream &os)
{
    int width = log10((double)Errors::EC_Last)+2;
    for (int i=0; Errors::messages_[i].code_ <= Errors::EC_Last; ++i)
    {
        os << "Code: ";
        os << std::setw(width) << std::right << Errors::messages_[i].code_;
        os << ", Msg: " << Errors::messages_[i].message_;
        os << std::endl;
    }
}

// check
static Errors checks;

}

