//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_USE_CNT_BASE_H
#define __OMBT_USE_CNT_BASE_H

// use-counted pointers base class -- contains use counter. 
// to use these classes there are a few rules to remember.
// 
// 1) all use-counted class must be derived from UseCntBase.
// 2) the use-counted derived class looks like this:
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

#include <atomic/Atomic.h>

namespace ombt {

// use-counted base class
class UseCntBase
{
public:
    // ctor and dtor
    inline UseCntBase(): counter_(0) { }
    inline virtual ~UseCntBase() { }

    // incr and decr use counts
    inline virtual void ref() {
	counter_.increment();
    }
    inline virtual void unref() {
	if (counter_.decrement_and_test(0)) delete this;
    }

private:
    // not allowed
    UseCntBase(const UseCntBase &);
    UseCntBase &operator=(const UseCntBase &);
    
public:
    // data
    Atomic<unsigned long> counter_;
};

}

#endif
