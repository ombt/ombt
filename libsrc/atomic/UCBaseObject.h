//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_USE_COUNT_BASE_OBJECT_H
#define __OMBT_USE_COUNT_BASE_OBJECT_H

// headers
#include <Returns.h>
#include <atomic/UseCntBase.h>

namespace ombt {

// use-counted object class
class UCBaseObject: virtual public UseCntBase {
public:
    UCBaseObject(unsigned int status = NOTOK): UseCntBase(), status_(status) { }
    virtual ~UCBaseObject() { };

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

private:
    // leper colony
    UCBaseObject(const UCBaseObject &src);
    UCBaseObject &operator=(const UCBaseObject &rhs);

protected:
    mutable volatile unsigned int status_;
};

}

#endif
