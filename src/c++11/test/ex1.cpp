
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <memory>
#include <thread>

/*
 * g++ -std=c++11 -o ex1 ex1.cpp -pthread
 */

using namespace std;

#if 1
void 
foo(unordered_map<string, shared_ptr<string>> &string_ptrs)
{
    for (auto &datum : string_ptrs)
    {
        cout << "THREAD: [" << datum.first << "," << *datum.second << "]" << endl;
    }
}
#else
void 
foo()
{
    cout << "THREAD" << endl;
}
#endif

int
main(int argc, char **argv)
{
    map<string, int> strings;
    for (int iarg=1; iarg<argc; ++iarg)
    {
        strings.insert(map<string, int>::value_type(string(argv[iarg]), iarg));
    }

    auto it = strings.begin();
    auto itend = strings.end();
    for ( ; it!=itend; ++it)
    {
        cout << "[" << it->first << "," << it->second << "]" << endl;
    }

    unordered_map<string, shared_ptr<string>> string_ptrs;

    it = strings.begin();
    for (auto &datum : strings)
    {
        cout << "[" << datum.first << "," << datum.second << "]" << endl;
        auto ptr = shared_ptr<string>(new string(datum.first));
        
        string_ptrs.insert(
            map<string, shared_ptr<string>>::value_type(datum.first, ptr));
    }

#if 1
    // thread run_foo(foo, string_ptrs);
    foo(string_ptrs);
#else
    thread run_foo(foo);
#endif

    // run_foo.join();

    exit(0);
}
