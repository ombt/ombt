// headers
#include <Task.h>

namespace ombt {

// ctor and dtor
Task::Task() { }
Task::Task(const Task &src) { }
Task::~Task() { }

// assignment
Task &Task::operator=(const Task &rhs) { return(*this); }

// make copies
Task *Task::clone() const { return NULL; }

// actual work functions
int Task::init() { return 0; }
int Task::run() { return 0; }
int Task::finish() { return 0; }

}
