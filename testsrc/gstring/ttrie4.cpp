//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <string>

#include <Debug.h>
#include <Returns.h>
#include <gstring/gstring.h>
#include <gstring/trieindex.h>
#include <parsing/Tokenizer.h>

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
	// check number of arguments
	MustBeTrue(argc > 1);

	// create an empty trie
	TrieIndex<string> trie;

	// insert the words
	cout << "INSERT START" << endl;
	Tokenizer tokenizer;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		MustBeTrue(trie.insert(word) == OK);
	}
	cout << "INSERT END" << endl << endl;
	cout << "VERIFY INSERT START" << endl;
	cout << trie << endl;
	cout << "VERIFY INSERT END" << endl << endl;

	// check if read/includes works
	cout << "INCLUDES START" << endl;
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
	cout << "INCLUDES END" << endl << endl;
	cout << "VERIFY INCLUDES START" << endl;
	cout << trie << endl;
	cout << "VERIFY INCLUDES END" << endl << endl;

	// verify if we can get children
	cout << "GETCHILDREN VERIFY START" << endl;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		std::list<string> children;
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		if (trie.getChildren(word, children) == OK)
		{
			cout << "children of ... " << word << " are ..." << endl;
			std::list<string>::const_iterator it = children.begin();
			std::list<string>::const_iterator itend = children.end();
			for ( ; it != itend; ++it)
			{
				cout << "\t" << *it << endl;
			}
		}
		else
			cout << "no children for ... " << word << endl;
	}
	cout << "GETCHILDREN VERIFY END" << endl << endl;

	// copy as trie
	cout << "COPY START" << endl << endl;
	TrieIndex<string> trie2(trie);
	cout << "COPY END" << endl << endl;
	cout << "VERIFY COPY START" << endl << endl;
	cout << "[1] original trie is ... " << trie << endl;
	cout << "[2] copied trie is ... " << trie2 << endl;
	cout << "VERIFY COPY END" << endl << endl;

	// trie recursive delete 
	cout << "RECURSIVE DELETE START" << endl << endl;
	string path(argv[1]);
	cout << "RECURSIVE DELETE PATH" << path << endl;
	vector<string> pathparts;
	tokenizer(path, pathparts, "/");
	GString<string> word(pathparts.size(), &pathparts[0]);
	trie2.remove(word, true);
	cout << "RECURSIVE DELETE END" << endl << endl;
	cout << "VERIFY RECURSIVE DELETE START" << endl << endl;
	cout << trie2 << endl;
	cout << "VERIFY RECURSIVE DELETE END" << endl << endl;

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
	cout << "VERIFY DELETE START" << endl;
	cout << "trie after all deletes ... " << trie << endl;
	cout << "VERIFY DELETE END" << endl << endl;

	// all done
	return(0);
}

