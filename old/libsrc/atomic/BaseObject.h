#ifndef __OMBT_BASE_OBJECT_H
#define __OMBT_BASE_OBJECT_H

// headers
#include <atomic/UseCntBase.h>
namespace ombt {

// object class
class BaseObject: virtual public UseCntBase {
public:
    BaseObject(bool status = false): UseCntBase(), ok_(status) { }
    BaseObject(const BaseObject &src): ok_(src.ok_) { }
    virtual ~BaseObject() { };

    inline BaseObject &operator=(const BaseObject &rhs) { ok_ = rhs.ok_; }

    inline bool isOk() const { return(ok_); }
    inline bool isNotOk() const { return(!ok_); }

    inline bool getOk() const { return(ok_); }
    inline bool setOk(bool newok) {
        bool oldok = ok_;
        ok_ = newok;
        return(oldok);
    }

protected:
    mutable bool ok_;	// is object in a valid state?
};

}

#endif
