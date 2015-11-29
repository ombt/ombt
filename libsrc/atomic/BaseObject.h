//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_BASE_OBJECT_H
#define __OMBT_BASE_OBJECT_H

// headers
#include <Returns.h>

namespace ombt {

// non-use-counted object class
class BaseObject {
public:
    BaseObject(unsigned int status = NOTOK): status_(status) { }
    BaseObject(const BaseObject &src): status_(src.status_) { }
    virtual ~BaseObject() { };

    inline BaseObject &operator=(const BaseObject &rhs) {
        status_ = rhs.status_;
        return(*this);
    }

    inline bool isOk() const { return(status_ == OK); }
    inline bool isNotOk() const { return(status_ != OK); }

    inline bool getOk() const { return(status_ == OK); }
    inline void setOk(bool ok) {
        if (ok)
            status_ = OK;
        else
            status_ = NOTOK;
    }

    inline unsigned int getStatus() const { return(status_); }
    inline unsigned int setStatus(unsigned int newstatus) {
        unsigned int oldstatus = status_;
        status_ = newstatus;
        return(oldstatus);
    }


protected:
    mutable volatile unsigned int status_;
};

}

#endif
