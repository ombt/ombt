#ifndef __OMBT_NI_USECNT_PTR_H
#define __OMBT_NI_USECNT_PTR_H
//
// NON-intrusive reference-counted pointer. one very important point, since
// this class has the the class member access operator defined 
// (for example, pa->b), it can not be used with primitive
// data types. The following statements are nonsensical for primitive
// type.
//
//    int *pi = new int;    // allocate an int
//    pi->something = 1;    // error: 
//
// you must remove the declaration for operator->() to use this
// pointer class with primitive types, i.e., int, short, long, char, etc.
//
// WARNING: reference-counted pointers do not work when cyclic data
// structures. cycles can cause some major memory leaks since the
// use counts never got to zero and the objects are never deleted.
//

// headers
#include <atomic/Atomic.h>

// reference-counted pointer
template <class DataType> 
class NIUseCntPtr {
public:
    // reference-counted pointer representation
    class NIUseCntPtrRep {
    public:
        NIUseCntPtrRep(DataType *pd = NULL):
            pdata_(pd), counter_(0) {
        }
        ~NIUseCntPtrRep() {
            counter_ = 0;
            if (pdata_ != NULL) delete pdata_;
            pdata_ = NULL;
        }
        int increment() {
            return(counter_.increment());
        }
        int decrement() {
            return(counter_.decrement());
        }

        // internal data
        DataType *pdata_;
        Atomic<int> counter_;
    };

    explicit NIUseCntPtr():
        prep_(new NIUseCntPtrRep(NULL)) {
        prep_->increment();
    }
    explicit NIUseCntPtr(DataType *pd):
        prep_(new NIUseCntPtrRep(pd)) {
        prep_->increment();
    }
    NIUseCntPtr(const NIUseCntPtr &p):
        prep_(p.prep_) {
        prep_->increment();
    }
    ~NIUseCntPtr() {
        if (prep_->decrement() <= 0)
        {
            delete prep_;
        }
        prep_ = NULL;
    }

    // assignment
    NIUseCntPtr<DataType> &operator=(DataType *rhs) {
        if (prep_->pdata_ != rhs)
        {
            if (prep_->decrement() <= 0)
            {
                delete prep_;
                prep_ = NULL;
            }
            prep_ = new NIUseCntPtrRep(rhs);
            prep_->increment();
        }
        return(*this);
    }
    NIUseCntPtr<DataType> &operator=(const NIUseCntPtr<DataType> &rhs) {
        if (prep_ != rhs.prep_)
        {
	    rhs.prep_->increment();
            if (prep_->decrement() <= 0)
            {
                delete prep_;
                prep_ = NULL;
            }
            prep_ = rhs.prep_;
	}
        return(*this);
    }

    // sanity checks
    int isNull() const {
        return(prep_->pdata_ == NULL);
    }
    int isNotNull() const {
        return(prep_->pdata_ != NULL);
    }

    // pointer operators
    operator DataType *() const {
        return(prep_->pdata_);
    }
    DataType *operator->() const {
        return(prep_->pdata_);
    }
    DataType &operator*() {
        return(*(prep_->pdata_));
    }
    DataType &operator*() const {
        return(*(prep_->pdata_));
    }
    DataType &operator[](int idx) {
        return(prep_->pdata_[idx]);
    }
    DataType &operator[](int idx) const {
        return(prep_->pdata_[idx]);
    }

    // comparators for containers
    bool operator<(const NIUseCntPtr<DataType> &rhs) const {
        return(prep_->pdata_ < rhs.prep_->pdata_);
    }
    bool operator<=(const NIUseCntPtr<DataType> &rhs) const {
        return(prep_->pdata_ <= rhs.prep_->pdata_);
    }
    bool operator>(const NIUseCntPtr<DataType> &rhs) const {
        return(prep_->pdata_ > rhs.prep_->pdata_);
    }
    bool operator>=(const NIUseCntPtr<DataType> &rhs) const {
        return(prep_->pdata_ >= rhs.prep_->pdata_);
    }
    bool operator==(const NIUseCntPtr<DataType> &rhs) const {
        return(prep_->pdata_ == rhs.prep_->pdata_);
    }
    bool operator!=(const NIUseCntPtr<DataType> &rhs) const {
        return(prep_->pdata_ != rhs.prep_->pdata_);
    }

protected:
    // internal data
    NIUseCntPtrRep *prep_;
};


#endif
