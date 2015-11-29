// headers
#include "binaryTree2.h"
#include "string.h"

main(int, char **)
{
	BinaryTree<String> dictionary;

	dictionary.insert("C++");
	dictionary.insert("Is");
	dictionary.insert("A");
	dictionary.insert("Very");
	dictionary.insert("Interesting");
	dictionary.insert("Language.");

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

	dictionary.remove(String("C++"));
	dictionary.remove(String("Interesting"));

	dictionary.insert("BASIC");
	dictionary.insert("Simple");

	cout << "Inorder: ";
	for (inorder.reset() ; ! inorder.done(); inorder++)
	{
		cout << inorder() << " ";
	}
	cout << endl;

	return(0);
}
