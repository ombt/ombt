
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <set>
#include <timers/OldTimer.h>

using namespace ombt;
using namespace std;

int
main(int argc, char *argv[])
{
	OldTimer t1(100000, 1000);
	OldTimer t2(100000, 1001);
	OldTimer t3(100000, 999);
	OldTimer t4(88888, 999);

	std::multiset<OldTimer> tmrq;

	tmrq.insert(t1);
	tmrq.insert(t1);
	tmrq.insert(t4);
	tmrq.insert(t3);
	tmrq.insert(t1);
	tmrq.insert(t3);
	tmrq.insert(t1);
	tmrq.insert(t4);
	tmrq.insert(t1);
	tmrq.insert(t1);
	tmrq.insert(t2);
	tmrq.insert(t2);
	tmrq.insert(t4);
	tmrq.insert(t2);
	tmrq.insert(t1);
	tmrq.insert(t1);
	tmrq.insert(t4);
	tmrq.insert(t1);
	tmrq.insert(t4);
	tmrq.insert(t2);
	tmrq.insert(t4);
	tmrq.insert(t2);
	tmrq.insert(t4);
	tmrq.insert(t3);
	tmrq.insert(t4);
	tmrq.insert(t3);
	tmrq.insert(t1);
	tmrq.insert(t1);
	tmrq.insert(t4);
	tmrq.insert(t1);
	tmrq.insert(t4);
	tmrq.insert(t1);
	tmrq.insert(t4);
	tmrq.insert(t3);
	tmrq.insert(t4);
	tmrq.insert(t3);
	tmrq.insert(t4);
	tmrq.insert(t4);
	tmrq.insert(t4);
	tmrq.insert(t3);

	while (!tmrq.empty())
	{
		std::multiset<OldTimer>::iterator tit = tmrq.begin();
		cout << *tit << endl;
		tmrq.erase(tit);
	}

	return(0);
}

