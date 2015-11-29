//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <string>

#include <gstring/gstring.h>
#include <gstring/trie.h>
#include <parsing/Tokenizer.h>

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
	// check number of arguments
	MustBeTrue(argc > 1);

	// create an empty trie
	Trie<string> trie;

	// insert the words
	Tokenizer tokenizer;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		MustBeTrue(trie.insert(word) == OK);
	}

	// print out trie
	cout << "INSERT START" << endl;
	cout << trie << endl;
	cout << "INSERT END" << endl << endl;

	// check if includes works
	cout << "READ START" << endl;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		if (trie.includes(word))
			cout << "trie includes ... " << word << endl;
		else
			cout << "trie lacks ... " << word << endl;
	}
	cout << "READ END" << endl << endl;

	// use iterator
	cout << "ITERATOR 1 START" << endl;
	TrieIterator<string> trieIter(trie);
	for ( ; !trieIter.done(); trieIter++)
	{
		cout << "[1] word is ... " << trieIter() << endl;
	}
	cout << "ITERATOR 1 END" << endl << endl;

	cout << "ITERATOR 2 START" << endl;
	for (trieIter.reset(); !trieIter.done(); trieIter++)
	{
		cout << "[2] word is ... " << trieIter() << endl;
	}
	cout << "ITERATOR 2 END" << endl << endl;

	// copy as trie
	Trie<string> trie2(trie);
	cout << "COPY START" << endl << endl;
	cout << "[1] original trie is ... " << trie << endl;
	cout << "[2] copied trie is ... " << trie2 << endl;
	cout << "COPY END" << endl << endl;

	// remove all words in trie
	cout << "DELETE START" << endl;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		if (trie.remove(word) == OK)
			cout << "word deleted ... " << word << endl;
		else
			cout << "word not found ... " << word << endl;
	}
	cout << "DELETE END" << endl << endl;

	// all done
	return(0);
}
