//
// examples of using a class with a function () operator
// and using lambda functions.
//

#include <iostream>
#include <thread>

using namespace std;

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

int
main()
{
    MyTask task;
    thread run_task(task);
    run_task.join();

    MyTask task2;
    thread run_task2(task2);
    run_task2.join();

    thread run_task3((MyTask()));
    run_task3.join();

    thread run_task4([] {
        cout << "Doing something ... lambda " << endl;
        cout << "Doing something else ... lambda " << endl;
    });
    run_task4.join();
}
