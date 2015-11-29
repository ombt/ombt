#include <string>

#include "gstring.h"
#include "trie.h"
#include "tokenizer.h"

using namespace std;

int
main(int argc, char **argv)
{
	// check number of arguments
	assert(argc > 1);

	// create an empty trie
	Trie<string, int> trie;

	// insert the words
	cout << "INSERT START" << endl;
	Tokenizer tokenizer;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		if (!(trie.insert(word, iarg) == 0)) abort();
	}
	cout << "INSERT END" << endl << endl;
	cout << "VERIFY INSERT START" << endl;
	cout << trie << endl;
	cout << "VERIFY INSERT END" << endl << endl;

	// check if read/includes works
	cout << "READ START" << endl;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		int payload;
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		if (trie.read(word, payload) == 0)
			cout << "trie includes ... " << word << "," << payload << endl;
		else
			cout << "trie lacks ... " << word << endl;
	}
	cout << "READ END" << endl << endl;
	cout << "VERIFY READ START" << endl;
	cout << trie << endl;
	cout << "VERIFY READ END" << endl << endl;

	// check if update works
	cout << "UPDATE START" << endl;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		int payload;
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		assert(trie.update(word, 2*iarg) == 0);
	}
	cout << "UPDATE END" << endl << endl;
	cout << "VERIFY UPDATE START" << endl;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		int payload;
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		if (trie.read(word, payload) == 0)
			cout << "trie includes ... " << word << "," << payload << endl;
		else
			cout << "trie lacks ... " << word << endl;
	}
	cout << "VERIFY UPDATE END" << endl << endl;

	// verify if we can get children
	cout << "GETCHILDREN VERIFY START" << endl;
	for (int iarg = 1; iarg < argc; iarg++)
	{
		std::list<string> children;
		string path(argv[iarg]);
		vector<string> pathparts;
		tokenizer(path, pathparts, "/");
		GString<string> word(pathparts.size(), &pathparts[0]);
		if (trie.getChildren(word, children) == 0)
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
	Trie<string, int> trie2(trie);
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
		if (trie.remove(word) == 0)
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
