#include <gstring/gstring.h>
#include <gstring/trie.h>

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
	// check number of arguments
	MustBeTrue(argc > 1);

	// create an empty trie
TRACE();
	Trie<char> trie;

	// insert the words
TRACE();
	for (int iarg = 1; iarg < argc; iarg++)
	{
		GString<char> word(strlen(argv[iarg]), argv[iarg]);
DUMP(word);
		MustBeTrue(trie.insert(word) == OK);
	}

	// print out trie
TRACE();
	cout << trie << endl;
TRACE();

	// check if includes works
	for (int iarg = 1; iarg < argc; iarg++)
	{
		GString<char> word(strlen(argv[iarg]), argv[iarg]);
DUMP(word);
		if (trie.includes(word))
			cout << "trie includes ... " << word << endl;
		else
			cout << "trie lacks ... " << word << endl;
	}
TRACE();

	// use iterator
	TrieIterator<char> trieIter(trie);
	for ( ; !trieIter.done(); trieIter++)
	{
		cout << "[1] word is ... " << trieIter() << endl;
	}
	for (trieIter.reset(); !trieIter.done(); trieIter++)
	{
		cout << "[2] word is ... " << trieIter() << endl;
	}

	// copy as trie
TRACE();
	Trie<char> trie2(trie);
TRACE();
	cout << "[1] original trie is ... " << trie << endl;
TRACE();
	cout << "[2] copied trie is ... " << trie2 << endl;
TRACE();

	// remove all words in trie
TRACE();
	for (int iarg = 1; iarg < argc; iarg++)
	{
		GString<char> word(strlen(argv[iarg]), argv[iarg]);
DUMP(word);
		if (trie.remove(word) == OK)
			cout << "word deleted ... " << word << endl;
		else
			cout << "word not found ... " << word << endl;
DUMP(trie);
	}
	// all done
	return(0);
}
