#include <random/Random.h>
#include <adt/Set_HashTable.h>

using namespace ombt;
using namespace std;

int
hash(const int &data)
{
	return(data);
}

int
main(int argc, char **argv)
{
	// check arguments
	if (argc != 4)
	{
		cout << "usage: " << argv[0];
		cout << " seed tablesz number_of_members" << endl;
		return(2);
	}

	// get arguments
	unsigned long key = atol(argv[1]);
	int hashsz = atoi(argv[2]);
	int mmax = atoi(argv[3]);

	// set key for random number generator
	setKey(key);

	// create sets
	Set_HashTable<int> set1(hashsz, hash);
	Set_HashTable<int> set2(set1);

	// add members to sets
	int im;
	for (im = 1; im <= mmax; im++)
	{
		unsigned long newm = random();
		set1 += newm;
	}
	cout << "set1 is  ... " << set1 << endl;
	for (im = 1; im <= mmax; im++)
	{
		unsigned long newm = random();
		set2 += newm;
	}
	cout << "set2 is  ... " << set2 << endl;

	// union of sets
	Set_HashTable<int> set3(set1|set2);
	cout << "set3 = set1|set2 = " << set3 << endl;
	
	// intersection of sets
	Set_HashTable<int> set4(set3&set2);
	cout << "set4 = set3&set2 = " << set4 << endl;
	
	// difference of sets
	Set_HashTable<int> set5(set4-set2);
	cout << "set5 = set4-set2 = " << set5 << endl;
	
	return(0);
}
