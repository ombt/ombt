//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_FILE_H
#define __OMBT_FILE_H

// system headers
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <atomic/UCBaseObject.h>

namespace ombt {

// file class
class File: public UCBaseObject {
public:
    // operations
    enum Operation {
        None = 1<<0,
        Read = 1<<1,
        Write = 1<<2,
        Append = 1<<3,
        Truncate = 1<<4,
    };

    // ctors and dtor
    File();
    File(const std::string &path);
    virtual ~File();

    // basic file operations
    virtual int read(void *inbuffer, size_t &count);
    virtual int write(const void *outbuffer, size_t count);
    virtual int open(unsigned int oper);
    virtual int close();
    virtual int unlink();

private:
    // not allowed
    File(const File &src);
    File &operator=(const File &rhs);

protected:
    int descriptor_;
    std::string path_;
};

}

#endif
