//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// headers
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <adt/BinaryTree2.h>
#include <adt/String.h>

using namespace ombt;
using namespace std;

int
work(int &number)
{
	cout << number << " ";
	return(OK);
}

main(int, char **)
{
#ifdef MUTABLE_ITERATOR
	int menuFlag = 1;

	String cmd;
	int input;

	BinaryTree<int> bt;

	BinaryTreeIterator_PreOrder<int> *preOrderIter = NULL;
	BinaryTreeIterator_PostOrder<int> *postOrderIter = NULL;
	BinaryTreeIterator_InOrder<int> *inOrderIter = NULL;
	BinaryTreeIterator_LevelOrder<int> *levelOrderIter = NULL;

	while (cmd != "q")
	{
		cout << endl;

		if (menuFlag)
		{
			cout << "turn off menu      - toffm" << endl;
			cout << "turn on menu       - tonm" << endl;

			cout << "quit               - q" << endl;
			cout << "insert             - i" << endl;
			cout << "remove             - r" << endl;
			cout << "clear              - c" << endl;
			cout << "show               - s" << endl;

			cout << "create pre-order   - cp" << endl;
			cout << "create post-order  - co" << endl;
			cout << "create in-order    - cn" << endl;
			cout << "create level-order - cl" << endl;

			cout << "delete pre-order   - dp" << endl;
			cout << "delete post-order  - do" << endl;
			cout << "delete in-order    - dn" << endl;
			cout << "delete level-order - dl" << endl;

			cout << "step pre-order     - stp" << endl;
			cout << "step post-order    - sto" << endl;
			cout << "step in-order      - stn" << endl;
			cout << "step level-order   - stl" << endl;

			cout << "run pre-order      - rp" << endl;
			cout << "run post-order     - ro" << endl;
			cout << "run in-order       - rn" << endl;
			cout << "run level-order    - rl" << endl;

			cout << "show pre-order     - shp" << endl;
			cout << "show post-order    - sho" << endl;
			cout << "show in-order      - shn" << endl;
			cout << "show level-order   - shl" << endl;

			cout << "reset pre-order    - rsp" << endl;
			cout << "reset post-order   - rso" << endl;
			cout << "reset in-order     - rsn" << endl;
			cout << "reset level-order  - rsl" << endl;
		}

		cout << "choose one: ";
		cin >> cmd;
		cout << endl << "cmd is ... " << cmd << endl;

		if (cmd == "toffm")
		{
			menuFlag = 0;
		}
		else if (cmd == "tonm")
		{
			menuFlag = 1;
		}
		else if (cmd == "q")
		{
			cout << endl;
			continue;
		}
		else if (cmd == "i")
		{
			cout << endl;
			cout << "enter number to insert: ";
			cin >> input;
			bt.insert(input);
		}
		else if (cmd == "r")
		{
			cout << endl;
			cout << "enter number to remove: ";
			cin >> input;
			cout << "number to remove is: " << input << endl;
			switch (bt.remove(input))
			{
			case OK:
				break;
			case NOMATCH:
				cout << "NO MATCH for " << input << endl;
				break;
			default:
				cout << "remove failed !!!" << endl;
				break;
			}
		}
		else if (cmd == "c")
		{
			cout << endl;
			bt.clear();
		}
		else if (cmd == "s")
		{
			cout << endl;
			cout << "tree = " << bt << endl;
		}
		else if (cmd == "cp")
		{
			cout << endl;
			if (preOrderIter != NULL)
			{
				cout << "preorder iterator exists." << endl;
				cout << "delete it first." << endl;
			}
			else
			{
				preOrderIter = 
					new BinaryTreeIterator_PreOrder<int>(bt);
				MustBeTrue(preOrderIter != NULL);
			}
		}
		else if (cmd == "co")
		{
			cout << endl;
			if (postOrderIter != NULL)
			{
				cout << "postorder iterator exists." << endl;
				cout << "delete it first." << endl;
			}
			else
			{
				postOrderIter = 
					new BinaryTreeIterator_PostOrder<int>(bt);
				MustBeTrue(postOrderIter != NULL);
			}
		}
		else if (cmd == "cn")
		{
			cout << endl;
			if (inOrderIter != NULL)
			{
				cout << "inorder iterator exists." << endl;
				cout << "delete it first." << endl;
			}
			else
			{
				inOrderIter = 
					new BinaryTreeIterator_InOrder<int>(bt);
				MustBeTrue(inOrderIter != NULL);
			}
		}
		else if (cmd == "cl")
		{
			cout << endl;
			if (levelOrderIter != NULL)
			{
				cout << "levelorder iterator exists." << endl;
				cout << "delete it first." << endl;
			}
			else
			{
				levelOrderIter = 
					new BinaryTreeIterator_LevelOrder<int>(bt);
				MustBeTrue(levelOrderIter != NULL);
			}
		}
		else if (cmd == "dp")
		{
			cout << endl;
			if (preOrderIter == NULL)
			{
				cout << "preorder iterator does not exists." << endl;
			}
			else
			{
				delete preOrderIter;
				preOrderIter = NULL;
			}
		}
		else if (cmd == "do")
		{
			cout << endl;
			if (postOrderIter == NULL)
			{
				cout << "postorder iterator does not exists." << endl;
			}
			else
			{
				delete postOrderIter;
				postOrderIter = NULL;
			}
		}
		else if (cmd == "dn")
		{
			cout << endl;
			if (inOrderIter == NULL)
			{
				cout << "inorder iterator does not exists." << endl;
			}
			else
			{
				delete inOrderIter;
				inOrderIter = NULL;
			}
		}
		else if (cmd == "dl")
		{
			cout << endl;
			if (levelOrderIter == NULL)
			{
				cout << "levelorder iterator does not exists." << endl;
			}
			else
			{
				delete levelOrderIter;
				levelOrderIter = NULL;
			}
		}
		else if (cmd == "rp")
		{
			cout << endl;
			cout << "pre order traversal ... ";
			if (bt.preOrder(work) != OK)
				cout << "pre order failed !!!" << endl;
			cout << endl;
			if (preOrderIter != NULL)
			{
				cout << "pre order iterator ... ";
				for ( ; ! (*preOrderIter).done(); (*preOrderIter)++)
				{
					cout << (*preOrderIter)() << " ";
				}
				cout << endl;
			}
			else
				cout << "preorder iter not defined ..." << endl;
		}
		else if (cmd == "ro")
		{
			cout << endl;
			cout << "post order traversal ... ";
			if (bt.postOrder(work) != OK)
				cout << "post order failed !!!" << endl;
			cout << endl;
			if (postOrderIter != NULL)
			{
				cout << "post order iterator ... ";
				for ( ; ! (*postOrderIter).done(); (*postOrderIter)++)
				{
					cout << (*postOrderIter)() << " ";
				}
				cout << endl;
			}
			else
				cout << "postorder iter not defined ..." << endl;
		}
		else if (cmd == "rn")
		{
			cout << endl;
			cout << "in order traversal ... ";
			if (bt.inOrder(work) != OK)
				cout << "in order failed !!!" << endl;
			cout << endl;
			if (inOrderIter != NULL)
			{
				cout << "in order iterator ... ";
				for ( ; ! (*inOrderIter).done(); (*inOrderIter)++)
				{
					cout << (*inOrderIter)() << " ";
				}
				cout << endl;
			}
			else
				cout << "inorder iter not defined ..." << endl;
		}
		else if (cmd == "rl")
		{
			cout << endl;
			cout << "level order traversal ... ";
			if (bt.levelOrder(work) != OK)
				cout << "level order failed !!!" << endl;
			cout << endl;
			if (levelOrderIter != NULL)
			{
				cout << "level order iterator ... ";
				for ( ; ! (*levelOrderIter).done(); (*levelOrderIter)++)
				{
					cout << (*levelOrderIter)() << " ";
				}
				cout << endl;
			}
			else
				cout << "levelorder iter not defined ..." << endl;
		}
		else if (cmd == "rsp")
		{
			cout << endl;
			if (preOrderIter != NULL)
				(*preOrderIter).reset();
			else
				cout << "preorder iter not defined ..." << endl;
		}
		else if (cmd == "rso")
		{
			cout << endl;
			if (postOrderIter != NULL)
				(*postOrderIter).reset();
			else
				cout << "postorder iter not defined ..." << endl;
		}
		else if (cmd == "rsn")
		{
			cout << endl;
			if (inOrderIter != NULL)
				(*inOrderIter).reset();
			else
				cout << "inorder iter not defined ..." << endl;
		}
		else if (cmd == "rsl")
		{
			cout << endl;
			if (levelOrderIter != NULL)
				(*levelOrderIter).reset();
			else
				cout << "levelorder iter not defined ..." << endl;
		}
		else if (cmd == "shp")
		{
			cout << endl;
			if (preOrderIter != NULL)
				cout << "show preorder ..." << (*preOrderIter)() << endl;
			else
				cout << "preorder iter not defined ..." << endl;
		}
		else if (cmd == "sho")
		{
			cout << endl;
			if (postOrderIter != NULL)
				cout << "show postorder ..." << (*postOrderIter)() << endl;
			else
				cout << "postorder iter not defined ..." << endl;
		}
		else if (cmd == "shn")
		{
			cout << endl;
			if (inOrderIter != NULL)
				cout << "show inorder ..." << (*inOrderIter)() << endl;
			else
				cout << "inorder iter not defined ..." << endl;
		}
		else if (cmd == "shl")
		{
			cout << endl;
			if (levelOrderIter != NULL)
				cout << "show levelorder ..." << (*levelOrderIter)() << endl;
			else
				cout << "levelorder iter not defined ..." << endl;
		}
		else if (cmd == "stp")
		{
			cout << endl;
			if (preOrderIter != NULL)
				(*preOrderIter)++;
			else
				cout << "preorder iter not defined ..." << endl;
		}
		else if (cmd == "sto")
		{
			cout << endl;
			if (postOrderIter != NULL)
				(*postOrderIter)++;
			else
				cout << "postorder iter not defined ..." << endl;
		}
		else if (cmd == "stn")
		{
			cout << endl;
			if (inOrderIter != NULL)
				(*inOrderIter)++;
			else
				cout << "inorder iter not defined ..." << endl;
		}
		else if (cmd == "stl")
		{
			cout << endl;
			if (levelOrderIter != NULL)
				(*levelOrderIter)++;
			else
				cout << "levelorder iter not defined ..." << endl;
		}
		else
		{
			cout << endl;
			cout << "unknown command is ..." << cmd << endl;
		}
	}
#else
	cout << "not built with -DMUTABLE_ITERATOR. check makefile (make.in)";
	cout << endl;
#endif
	return(0);
}
