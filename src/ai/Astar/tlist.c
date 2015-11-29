// demo of list
#include "list.h"

// main entry point
main(int argc, char **argv)
{
	// list for tests
	List<int> list;

	// insert numbers in list
	for (int arg = 1; arg < argc; arg++)
	{
		cout << "list(before insert by value) is ... " << list << endl;
		list.insertByValue(atoi(argv[arg]));
		cout << "list(after insert by value) is ... " << list << endl;
	}

	// remove numbers from front
	for (int number; ! list.isEmpty(); )
	{
		cout << "list(before removeAtFront) is ... " << list << endl;
		list.removeAtFront(number);
		cout << "number is ... " << number << endl;
		cout << "list(after removeAtFront) is ... " << list << endl;
	}

	// all done
	return(0);
}
