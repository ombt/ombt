//
// using a class to join with any running thread in case 
// an exception is fired. this example uses the move-paradygm
// when passing threads around by value instead of reference.
//

#include <iostream>
#include <thread>

using namespace std;

class ScopedThread
{
public:
    explicit ScopedThread(thread t): _t(std::move(t))
    {
    }

    ScopedThread(const ScopedThread &) = delete;
    ScopedThread operator=(const ScopedThread &) = delete;

    ~ScopedThread()
    {
        _t.join();
    }

private:
    thread _t;
    
};

class MyTask
{
public:
    MyTask(int i = ++_counter) { _i = i; }
    void do_something()
    {
        cout << "Doing something ... " << _i << endl;
    }
    void do_something_else()
    {
        cout << "Doing something else ... " << _i << endl;
    }
    void operator()()
    {
        cout << "In operator() ..." << endl;
        do_something();
        do_something_else();
    }

public:
    int _i;
    static int _counter;
};

int MyTask::_counter = 0;

void run_task()
{
    cout << "In run_task ..." << endl;
    ScopedThread st(thread(MyTask(45)));
}

int
main()
{
    run_task();
}
