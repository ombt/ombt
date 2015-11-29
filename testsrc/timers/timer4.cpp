//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <timers/Timer.h>

using namespace ombt;
using namespace std;

int
main(int argc, char *argv[])
{
	Timer t;

	cout << "before now() ... " << t << endl;
	t.setToNow();
	cout << "after now() ... " << t << endl;

	cout << "before add 60 seconds ... " << t << endl;
        Timer t60(60,0);
        t += t60;
	cout << "after add 60 seconds ... " << t << endl;

	cout << "before subtract 60 seconds ... " << t << endl;
        t -= t60;
	cout << "after subtract 60 seconds ... " << t << endl;

	cout << "before add 60 seconds and 600000 usecs ... " << t << endl;
        Timer t60600000(60,600000);
        t += t60600000;
	cout << "after add 60 seconds and 600000 usecs ... " << t << endl;

	cout << "before subtract 60 seconds and 600000 usecs ... " << t << endl;
        t -= t60600000;
	cout << "after subtract 60 seconds and 600000 usecs ... " << t << endl;

	cout << "before reset to now() ... " << t << endl;
	t.setToNow();
	cout << "after reset to now() ... " << t << endl;

	Timer t2;
	t2.setToNow();
	Timer t1hour(60*60, 0);

	cout << "before for adding 60 minutes ... " << t2 << endl;
	t2 += t1hour;
	cout << "after for adding 60 minutes ... " << t2 << endl;
	
	Timer t30min(30*60, 0);

	cout << "before for subtracting 30 minutes ... " << t2 << endl;
	t2 -= t30min;
	cout << "after for subtracting 30 minutes ... " << t2 << endl;

        Timer t200secs(0,200000000);
	cout << "before adding 200,000,000 usecs ... " << t2 << endl;
	t2 += t200secs;
	cout << "after adding 200,000,000 usecs ... " << t2 << endl;

        Timer t65secs4000usecs(0,65004000);
	cout << "before adding 65,004,000 usecs ... " << t2 << endl;
	t2 += t65secs4000usecs;
	cout << "after adding 65,004,000 usecs ... " << t2 << endl;

	return(0);
}

