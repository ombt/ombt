#include "term.h"
#include "list.h"

main(int argc, char **argv)
{
	List<Term> list;

	for (int arg = 1; arg < argc; arg++)
	{
		cout << "inserting ... " << argv[arg] << endl;
		list.insertOrdered(Term(argv[arg]));
	}
	cout << "list is ... " << list << endl;

	ListIterator<Term> listIter(list);
	for ( ; !listIter.done(); listIter++)
	{
		cout << "term is ... " << listIter() << endl;
		cout << "predicate is ... " << listIter().predicate() << endl;
	}
	
	return(0);
}
