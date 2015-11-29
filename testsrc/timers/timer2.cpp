//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <timers/OldTimer.h>
#include <timers/OldTimerQueue.h>

using namespace ombt;
using namespace std;

int
main(int argc, char *argv[])
{
	OldTimer t1(100000, 1000);
	OldTimer t2(100000, 1001);
	OldTimer t3(100000, 999);

	OldTimerQueue tq;

	tq.enqueue(t1);
	tq.enqueue(t2);
	tq.enqueue(t3);

	while (!tq.isEmpty())
	{
		OldTimer t;
		tq.dequeue(t);
		cout << t << endl;
	}

	return(0);
}

