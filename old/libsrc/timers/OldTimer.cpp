// timer class

// headers
#include <OldTimer.h>

namespace ombt {

const OldTimer::Tag OldTimer::InvalidTag = -1;

// ctors and dtor
OldTimer::OldTimer(): 
    BaseObject(false), tag_(InvalidTag), type_(OneShotTimer),
    seconds_(0), microseconds_(0)
{
    ok_ = false;
}

OldTimer::OldTimer(Seconds s, MicroSeconds us, Tag t, Type y): 
    BaseObject(true), seconds_(s), 
    microseconds_(us), tag_(t), type_(y)
{
    ok_ = true;
}

OldTimer::OldTimer(const OldTimer &src):
    BaseObject(src), tag_(src.tag_), type_(src.type_),
    seconds_(src.seconds_), microseconds_(src.microseconds_)
{
    // do nothing
}

OldTimer::~OldTimer()
{
    ok_ = false;
}

}
