//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// pearson type VI distribution
//
// 1) y1 ~ gamma(alpha1, beta), y2 ~ gamma(alpha2, beta).
// 2) return x = y1/y2;

// headers
#include <PearsonTypeVI.h>

namespace ombt {

// ctors and dtor
PearsonTypeVI::PearsonTypeVI(): 
    BaseObject(false),
    alpha1_(0), alpha2_(0), pgrng1_(NULL), pgrng2_(NULL)
{
    setOk(false);
}

PearsonTypeVI::PearsonTypeVI(double alpha1, double alpha2, double beta, const Random &rng): 
    BaseObject(false), 
    alpha1_(alpha1), alpha2_(alpha2), beta_(beta), pgrng1_(NULL), pgrng2_(NULL)
{
    pgrng1_ = new Gamma(alpha1_, beta_, rng);
    pgrng2_ = new Gamma(alpha2_, beta_, rng);
    setOk(true);
}

PearsonTypeVI::PearsonTypeVI(const PearsonTypeVI &src): 
    BaseObject(src), 
    alpha1_(src.alpha1_), alpha2_(src.alpha2_), beta_(src.beta_),
    pgrng1_(src.pgrng1_), pgrng2_(src.pgrng2_)
{
    // nothing to do
}

PearsonTypeVI::~PearsonTypeVI()
{
    setOk(false);
}

// assignment
PearsonTypeVI &
PearsonTypeVI::operator=(const PearsonTypeVI &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        alpha1_ = rhs.alpha1_;
        alpha2_ = rhs.alpha2_;
        beta_ = rhs.beta_;
        pgrng1_ = rhs.pgrng1_;
        pgrng2_ = rhs.pgrng2_;
    }
    return(*this);
}

// get value
double
PearsonTypeVI::nextValue()
{
    double y1 = pgrng1_->nextValue();
    double y2 = pgrng2_->nextValue();
    return(y1/y2);
}

}
