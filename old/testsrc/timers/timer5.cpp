
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <timers/Timer.h>
#include <timers/TimerQueue.h>

using namespace ombt;
using namespace std;

int
main(int argc, char *argv[])
{
	Timer t1(100000, 1000, 10);
	Timer t2(100000, 1001, 20);
	Timer t3(100000, 999, 30);
	Timer t4(1000, 4501001, 40);
	Timer t5(45454, 4333, 50);
	Timer t6(33333, 33333, 60);
	Timer t7(0, 1, 70);

	TimerQueue tq;

	tq.enqueue(t1);
	tq.enqueue(t2);
	tq.enqueue(t1);
	tq.enqueue(t2);
	tq.enqueue(t7);
	tq.enqueue(t7);
	tq.enqueue(t7);
	tq.enqueue(t4);
	tq.enqueue(t3);
	tq.enqueue(t4);
	tq.enqueue(t7);
	tq.enqueue(t2);
	tq.enqueue(t4);
	tq.enqueue(t7);
	tq.enqueue(t1);
	tq.enqueue(t4);
	tq.enqueue(t4);
	tq.enqueue(t4);
	tq.enqueue(t4);
	tq.enqueue(t7);
	tq.enqueue(t2);
	tq.enqueue(t4);
	tq.enqueue(t2);
	tq.enqueue(t4);
	tq.enqueue(t1);
	tq.enqueue(t4);
	tq.enqueue(t7);
	tq.enqueue(t5);
	tq.enqueue(t7);
	tq.enqueue(t2);
	tq.enqueue(t4);
	tq.enqueue(t7);
	tq.enqueue(t4);
	tq.enqueue(t2);
	tq.enqueue(t4);
	tq.enqueue(t2);
	tq.enqueue(t4);
	tq.enqueue(t1);
	tq.enqueue(t7);
	tq.enqueue(t6);
	tq.enqueue(t2);
	tq.enqueue(t7);
	tq.enqueue(t7);
	tq.enqueue(t7);
	tq.enqueue(t7);
	tq.enqueue(t2);
	tq.enqueue(t1);
	tq.enqueue(t1);
	tq.enqueue(t2);
	tq.enqueue(t2);
	tq.enqueue(t1);
	tq.enqueue(t7);
	tq.enqueue(t1);
	tq.enqueue(t1);
	tq.enqueue(t1);

	while (!tq.isEmpty())
	{
		Timer t;
		tq.dequeue(t);
		cout << t << endl;
	}

	return(0);
}

