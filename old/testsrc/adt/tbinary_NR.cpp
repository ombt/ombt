// headers
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <adt/BinaryTree_NR.h>
#include <adt/String.h>

using namespace ombt;
using namespace std;

int
work(String &ch)
{
	cout << ch << " ";
	return(OK);
}

main(int, char **)
{
	String cmd;

	BinaryTree_NR<String> bt;

	while (cmd != 'q')
	{
		cout << endl;
		cout << "quit   - q" << endl;
		cout << "insert - i" << endl;
		cout << "remove - r" << endl;
		cout << "clear  - c" << endl;
		cout << "show   - s" << endl;
		cout << "copy   - C" << endl;
		cout << "pre-order   - p" << endl;
		cout << "post-order  - o" << endl;
		cout << "in-order    - n" << endl;
		cout << "level-order - l" << endl;
		cout << "choose one: ";
		cin >> cmd;

		switch (cmd[0]) 
		{
		case 'q':
			break;
		case 'p':
		{
			cout << endl;
			cout << "pre order ... ";
			if (bt.preOrder(work) != OK)
				cout << "pre order failed !!!" << endl;
			cout << endl;
			cout << "pre order iterator ... ";
			BinaryTree_NR_Iterator_PreOrder<String> iter(bt);
			for ( ; ! iter.done(); iter++)
			{
				cout << iter() << " ";
			}
			cout << endl;
		}
			break;
		case 'o':
		{
			cout << endl;
			cout << "post order ... ";
			if (bt.postOrder(work) != OK)
				cout << "post order failed !!!" << endl;
			cout << endl;
			cout << "post order iterator ... ";
			BinaryTree_NR_Iterator_PostOrder<String> iter(bt);
			for ( ; ! iter.done(); iter++)
			{
				cout << iter() << " ";
			}
			cout << endl;
		}
			break;
		case 'n':
		{
			cout << endl;
			cout << "in order ... ";
			if (bt.inOrder(work) != OK)
				cout << "in order failed !!!" << endl;
			cout << endl;
			cout << "in order iterator ... ";
			BinaryTree_NR_Iterator_InOrder<String> iter(bt);
			for ( ; ! iter.done(); iter++)
			{
				cout << iter() << " ";
			}
			cout << endl;
		}
			break;
		case 'l':
		{
			cout << endl;
			cout << "level order ... ";
			if (bt.levelOrder(work) != OK)
				cout << "level order failed !!!" << endl;
			cout << endl;
			cout << "level order iterator ... ";
			BinaryTree_NR_Iterator_LevelOrder<String> iter(bt);
			for ( ; ! iter.done(); iter++)
			{
				cout << iter() << " ";
			}
			cout << endl;
		}
			break;
		case 'i':
			cout << endl;
			cout << "character to insert: ";
			cin >> cmd;
			bt.insert(cmd);
			break;
		case 'r':
			cout << endl;
			cout << "character to remove: ";
			cin >> cmd;
			if (bt.remove(cmd) != OK)
				cout << "remove failed !!!" << endl;
			break;
		case 'c':
			cout << endl;
			bt.clear();
			break;
		case 'C':
		{
			cout << endl;
			BinaryTree_NR<String> nbt(bt);
			cout << "old tree = " << bt << endl;
			cout << "new tree = " << nbt << endl;
			break;
		}
		case 's':
			cout << endl;
			cout << "tree = " << bt << endl;
			break;
		default:
			break;
		}
	}

	return(0);
}
