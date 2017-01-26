//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_USE_CNT_PTR_H
#define __OMBT_USE_CNT_PTR_H

// use-counted pointers class expects classes to be derived from
// UseCntBase class. to use these classes there are a few rules 
// to remember.
// 
// 1) all use-counted class must be derived from UseCntBase.
// 2) the use-counted derived class lools like this:
//
//     class UseCntDerivedClass: public UseCntBase {
//     ...
//     };
//
// 3) pass use-counted objects by pointer, that is, define 
// the function as:
//
//     void foobar(UseCntDerivedClass *pDerived)
//
// 4) the pointers are then defined as:
//
//     UseCntPtr<UseCntDerivedClass> pDerived;

namespace ombt {

// use-counted pointer
template <class DataType> 
class UseCntPtr
{
public:
    // ctors and dtor
    inline UseCntPtr(): pdata_(NULL) { }
    inline explicit UseCntPtr(DataType *pd):
        pdata_(pd) {
        if (pdata_ != NULL) pdata_->ref();
    }
    inline UseCntPtr(const UseCntPtr &p):
        pdata_(p.pdata_) {
        if (pdata_ != NULL) pdata_->ref();
    }
    inline ~UseCntPtr() {
        if (pdata_ != NULL) pdata_->unref();
    }

    // assignment
    inline UseCntPtr<DataType> &operator=(DataType *rhs) {
        if (rhs != NULL) rhs->ref();
        if (pdata_ != NULL) pdata_->unref();
        pdata_ = rhs;
        return(*this);
    }
    inline UseCntPtr<DataType> &operator=(const UseCntPtr<DataType> &rhs) {
        if (rhs.pdata_ != NULL) rhs.pdata_->ref();
        if (pdata_ != NULL) pdata_->unref();
        pdata_ = rhs.pdata_;
        return(*this);
    }

    // sanity checks
    inline int isNull() const {
        return(pdata_ == NULL);
    }
    inline int isNotNull() const {
        return(pdata_ != NULL);
    }

    // pointer operators
    inline bool operator ! () const {
        return(pdata_ != NULL);
    }
    inline operator DataType *() {
        return(pdata_);
    }
    inline operator const DataType *() const {
        return(pdata_);
    }
    inline DataType *ptr() {
        return(pdata_);
    }
    inline const DataType *ptr() const {
        return(pdata_);
    }
    inline DataType *operator->() {
        return(pdata_);
    }
    inline const DataType *operator->() const {
        return(pdata_);
    }
    inline DataType &operator*() {
        return(*pdata_);
    }
    inline const DataType &operator*() const {
        return(*pdata_);
    }
    inline DataType &operator[](int idx) {
        return(pdata_[idx]);
    }
    inline const DataType &operator[](int idx) const {
        return(pdata_[idx]);
    }

    // comparators for containers
    inline bool operator<(const UseCntPtr<DataType> &rhs) const {
        return(pdata_ < rhs.pdata_);
    }
    inline bool operator<=(const UseCntPtr<DataType> &rhs) const {
        return(pdata_ <= rhs.pdata_);
    }
    inline bool operator>(const UseCntPtr<DataType> &rhs) const {
        return(pdata_ > rhs.pdata_);
    }
    inline bool operator>=(const UseCntPtr<DataType> &rhs) const {
        return(pdata_ >= rhs.pdata_);
    }
    inline bool operator==(const UseCntPtr<DataType> &rhs) const {
        return(pdata_ == rhs.pdata_);
    }
    inline bool operator!=(const UseCntPtr<DataType> &rhs) const {
        return(pdata_ != rhs.pdata_);
    }

protected:
    // internal data
    DataType *pdata_;
};

}

#endif
