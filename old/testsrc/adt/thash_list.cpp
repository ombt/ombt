// headers
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <random/Random.h>
#include <adt/HashTable_List.h>
#include <adt/List.h>
#include <adt/Tuple.h>

using namespace ombt;
using namespace std;

int
myhf(const Tuple<int, int> &data)
{
	return(data.key);
}

main(int argc, char **argv)
{
	// get options
	assert(argc == 4);
	int seed = atoi(argv[1]);
	int howmany = atoi(argv[2]);
	int tblsz = atoi(argv[3]);

	// set random gernerator
	setKey(seed);

	// generate list of random numbers
	cout << "inserting data ... " << endl;
	List<Tuple<int, int> > l;
	for (int i=1; i <= howmany; i++)
	{
		Tuple<int, int> tuple(random(), random());
		l.insertAtEnd(tuple);
	}
	cout << "FINAL list is ... " << l << endl;

	// create hash table
	HashTable_List<Tuple<int, int> > hta(tblsz, myhf);

	// insert into hash table
	ListIterator<Tuple<int, int> >  li(l);
	for ( ; ! li.done(); li++)
	{
		MustBeTrue(hta.insert(li()) == OK);
	}
	cout << "FINAL hash table is ... " << hta << endl;

	// check if everything is in table
	for (li.reset() ; ! li.done(); li++)
	{
		Tuple<int, int> tuple;
		tuple.key = li().key;
		tuple.data = 0;
		cout << " retrieving for ... " << tuple.key << endl;
		if (hta.retrieve(tuple) != OK)
			cout << "retrieve failed for ... ";
		else
			cout << "retrieved data is ... ";
		cout << tuple << endl;
	}

	// dump array using an iterator
	HashTable_List_Iterator<Tuple<int, int> > htai(hta);
	for ( ; ! htai.done(); htai++)
	{
		cout << "contents are ... " << htai() << endl;
	}

	// delete everything in table
	for (li.reset() ; ! li.done(); li++)
	{
		Tuple<int, int> tuple;
		tuple.key = li().key;
		tuple.data = 0;
		cout << "deleting for ... " << tuple.key << endl;
		if (hta.remove(tuple) != OK)
			cout << "delete failed for ... ";
		else
			cout << "deleted data is ... ";
		cout << tuple << endl;
		cout << "hash table is ... " << hta << endl;
	}

	return(0);
}
