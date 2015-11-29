#include <threads/SimpleThread.h>

#include <stdio.h>
#include <iostream>
#include <Debug.h>
#include <threads/SimpleThread.h>
#include <logging/Logging.h>

using namespace ombt;
using namespace std;

static bool spin = true;

class First: public SimpleThread::Task {
public:
    virtual int init() {
        DBGTRACE();
        return 0;
    }
    virtual int run() {
        DBGTRACE();
        sleep(2); 
        spin = false;
        sleep(1);
        DBGTRACE();
        return 0;
    }
    virtual int finish() {
        DBGTRACE();
        return 0;
    }
};

class Second: public SimpleThread::Task {
public:
    virtual int init() {
        DBGTRACE();
        return 0;
    }
    virtual int run() {
        DBGTRACE();
        while (spin);
        DBGTRACE();
        return 0;
    }
    virtual int finish() {
        DBGTRACE();
        return 0;
    }
};
int
main(int argc, char **argv)
{
    dbgopen(stderr);

    MustBeTrue(argc == 2);

    int imax = ::atoi(argv[1]);
    MustBeTrue(imax > 0);

    for (int i=1; i<=imax; ++i)
    {
        DBGTRACE();
        SimpleThread st(new First);
        MustBeTrue(st.isOk());

        DBGTRACE();
        MustBeTrue(st.run() == 0);
        MustBeTrue(st.isOk());

        DBGTRACE();
        SimpleThread st2(new Second);
        MustBeTrue(st2.isOk());

        DBGTRACE();
        MustBeTrue(st2.run() == 0);
        MustBeTrue(st2.isOk());

        void *retval;

        DBGTRACE();
        st.join(retval);
        MustBeTrue(!st.isOk());

        DBGTRACE();
        st2.join(retval);
        MustBeTrue(!st2.isOk());

        DBGTRACE();
    }

    return(0);
}

