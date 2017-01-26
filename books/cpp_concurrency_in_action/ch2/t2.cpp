//
// using a class to join with any running thread in case 
// an exception is fired.
//

#include <iostream>
#include <thread>

using namespace std;

class ThreadGuard
{
public:
    explicit ThreadGuard(thread &t): _t(t)
    {
    }

    ThreadGuard(const ThreadGuard &) = delete;
    ThreadGuard operator=(const ThreadGuard &) = delete;

    ~ThreadGuard()
    {
        if (_t.joinable())
        {
            cout << "calling join in guard ..." << endl;
            _t.join();
        }
    }

private:
    thread &_t;
    
};

class MyTask
{
public:
    MyTask(int i = ++_counter) { _i = i; }
    void do_somthing()
    {
        cout << "Doing something ... " << _i << endl;
    }
    void do_somthing_else()
    {
        cout << "Doing something else ... " << _i << endl;
    }
    void operator()()
    {
        do_somthing();
        do_somthing_else();
    }

public:
    int _i;
    static int _counter;
};

int MyTask::_counter = 0;

void run_task()
{
    MyTask task;
    thread run_task(task);
    ThreadGuard g(run_task);
}

int
main()
{
    run_task();
}
