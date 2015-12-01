//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// headers
#include "adt/BinaryTree2.h"
#include "adt/String.h"

using namespace ombt;
using namespace std;

main(int, char **)
{
	BinaryTree<String> dictionary;

	dictionary.insert("C++");
	dictionary.insert(("Is"));
	dictionary.insert(("A"));
	dictionary.insert(("Very"));
	dictionary.insert(("Interesting"));
	dictionary.insert(("Language."));

	BinaryTreeIterator_InOrder<String> inorder(dictionary);

	cout << "Inorder: ";
	for ( ; ! inorder.done(); inorder++)
	{
		cout << inorder() << " ";
	}
	cout << endl;

	inorder.reset();
	BinaryTreeIterator_InOrder<String> inorder2(inorder);

	cout << "Inorder (copy version): ";
	for ( ; ! inorder2.done(); inorder2++)
	{
		cout << inorder2() << " ";
	}
	cout << endl;

	String s("C++");
	dictionary.remove(s);
	s = "Interesting";
	dictionary.remove(s);

	dictionary.insert(("BASIC"));
	dictionary.insert(("Simple"));

	cout << "Inorder: ";
	for (inorder.reset() ; ! inorder.done(); inorder++)
	{
		cout << inorder() << " ";
	}
	cout << endl;

	return(0);
}
