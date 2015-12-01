//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// headers
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "adt/BinaryTree_AVL.h"
#include "adt/Queue_List.h"
#include "random/Random.h"

using namespace ombt;
using namespace std;

int
work(unsigned long &data)
{
	cout << data << " ";
	return(OK);
}

main(int argc, char **argv)
{
	BinaryTree_AVL<unsigned long> bt;
	Queue_List<unsigned long> q;

	assert(argc == 3);

	setKey(atol(argv[1]));

	long start = 1;
	long end = atol(argv[2]);

	cout << "starting !!!" << endl;

	cout << "start = " << start << endl;
	cout << "end   = " << end << endl;

	cout << "bt before inserts ... " << bt << endl;
	for ( ; start <= end; start++)
	{
		unsigned long data = random();
		cout << "INSERT for ... " << data << endl;
		bt.insert(data);

		cout << "bt is ... ";
		bt.levelOrder(work);
		cout << endl;

		cout << "auditing ... " << endl;
		bt.audit();
		q.enqueue(data);
	}

	cout << "bt after inserts and before deletes ... " << bt << endl;
	while ( ! q.isEmpty())
	{
		unsigned long data;
		assert(q.dequeue(data) == OK);
		cout << "REMOVE for ... " << data << endl;
		assert(bt.remove(data) == OK);

		cout << "bt is ... ";
		bt.levelOrder(work);
		cout << endl;

		cout << "auditing ... " << endl;
		bt.audit();
	}

	cout << "bt after deletes ... " << bt << endl;
	cout << "done !!!" << endl;

	return(0);
}
