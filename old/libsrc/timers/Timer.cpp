// timer class

// headers
#include <Debug.h>
#include <Timer.h>

namespace ombt {

const Timer::Tag Timer::InvalidTag = -1;

// ctors and dtor
Timer::Timer(): 
    BaseObject(false), tag_(InvalidTag), type_(OneShotTimer),
    seconds_(0), microseconds_(0)
{
    ok_ = false;
}

Timer::Timer(Seconds s, MicroSeconds us, Tag t, Type y): 
    BaseObject(true), seconds_(s+us/1000000), 
    microseconds_(us%1000000), tag_(t), type_(y)
{
    ok_ = true;
}

Timer::Timer(const timeval &tv, Tag t, Type y): 
    BaseObject(true), seconds_(tv.tv_sec+tv.tv_usec/1000000), 
    microseconds_(tv.tv_usec%1000000), tag_(t), type_(y)
{
    ok_ = true;
}

Timer::Timer(const Timer &src):
    BaseObject(src), tag_(src.tag_), type_(src.type_),
    seconds_(src.seconds_), microseconds_(src.microseconds_)
{
    // do nothing
}

Timer::~Timer()
{
    ok_ = false;
}

}
