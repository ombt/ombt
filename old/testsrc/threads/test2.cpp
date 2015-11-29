#include <stdio.h>
#include <iostream>
#include <Debug.h>
#include <threads/SimpleThread.h>
#include <logging/Logging.h>

using namespace ombt;
using namespace std;

static bool spin = true;

class Work: public SimpleThread::Task {
public:
    virtual int init() {
        while (spin);
        DBGTRACE();
        return 0;
    }
    virtual int run() {
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

        SimpleThread st(new Work);
        MustBeTrue(st.isOk());

        MustBeTrue(st.run() == 0);
        MustBeTrue(st.isOk());

        MustBeTrue(st.detach() == 0);
        MustBeTrue(!st.isOk());

        void *retval;
        MustBeTrue(st.join(retval) != 0);
        MustBeTrue(!st.isOk());
        DBGTRACE();
    }

    ::sleep(5);
    spin = false;
    ::sleep(30);

    return(0);
}

