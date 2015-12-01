//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_ERRORS_H
#define __OMBT_ERRORS_H

// basic mechanism for reporting errors.

// os headers
#include <string>
#include <iostream>
#include <iomanip>

// headers
#include "atomic/BaseObject.h"

// convenience macros
#define ERRPRINTF(OBJ, EC, FORMAT, ...) \
    OBJ.printf(EC, \
               std::string("%s'%d: ") + FORMAT, \
               __FILE__, \
               __LINE__, \
               ##__VA_ARGS__)
	
namespace ombt {

// error reporting class
class Errors: public BaseObject
{
public:
    // data type for error messages
    enum ErrorCodes
    {
        EC_Unknown = -1,
        EC_Success = 0,
        EC_Bad_Arguments,
        EC_Program_Not_A_Parsed_Form,
        EC_Unsupported_Logic_Type,
        EC_Last
    };

    struct ErrorMessages
    {
        int         code_;
        const char *message_;
    };

public:
    // ctors and dtor
    Errors();
    Errors(const Errors &src);
    ~Errors();

    // assignment
    Errors &operator=(const Errors &rhs);

    // access error data
    inline bool operator!() const
        { return error_code_ != EC_Success; }
    inline int error_code() const
        { return error_code_; }
    inline void error_code(ErrorCodes ec)
        { error_code_ = ec; }
    inline const std::string error_msg() const
        { return error_msg_; }
    inline void error_msg(const std::string &em)
        { error_msg_ = em; }
    int ok();
    int notok(ErrorCodes ec, const std::string &format, ...);

    // print data
    void printf(ErrorCodes ec, const std::string &format, ...);
    friend std::ostream &operator<<(std::ostream &os, const Errors &e);
    static void dump(std::ostream &os);
    static const std::string message(ErrorCodes ec);

protected:
    // internal data
    ErrorCodes error_code_;
    std::string error_msg_;
    static ErrorMessages messages_[];
};

}

#endif

