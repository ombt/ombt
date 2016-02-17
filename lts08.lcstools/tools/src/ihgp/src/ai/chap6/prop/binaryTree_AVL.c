// member functions for recursive binary tree class

// required headers
#include "binaryTree_AVL.h"

// binary tree node constructors and destructor
template <class DataType>
BinaryTreeNode_AVL<DataType>::BinaryTreeNode_AVL(const DataType &d):
	balance(0), data(d), left(NULL), right(NULL)
{
	// do nothing
}

template <class DataType>
BinaryTreeNode_AVL<DataType>::BinaryTreeNode_AVL(const BinaryTreeNode_AVL<DataType> &btn):
	balance(0), data(btn.data), left(NULL), right(NULL)
{
	// do nothing
}

template <class DataType>
BinaryTreeNode_AVL<DataType>::~BinaryTreeNode_AVL()
{
	balance = 0;
	left = NULL;
	right = NULL;
}

// assignment
template <class DataType>
BinaryTreeNode_AVL<DataType> &
BinaryTreeNode_AVL<DataType>::operator=(const BinaryTreeNode_AVL<DataType> &btn)
{
	if (this != &btn)
	{
		balance = 0;
		left = NULL;
		right = NULL;
		data = btn.data;
	}
	return(*this);
}

// binary tree constructors and destructor
template <class DataType>
BinaryTree_AVL<DataType>::BinaryTree_AVL():
	root(NULL)
{
	// do nothing
}

template <class DataType>
BinaryTree_AVL<DataType>::BinaryTree_AVL(const BinaryTree_AVL<DataType> &bt):
	root(NULL)
{
	root = bt.copy();
}

template <class DataType>
BinaryTree_AVL<DataType>::~BinaryTree_AVL()
{
	clear();
}

// assignment
template <class DataType>
BinaryTree_AVL<DataType> &
BinaryTree_AVL<DataType>::operator=(const BinaryTree_AVL<DataType> &bt)
{
	if (this != &bt)
	{
		clear();
		root = bt.copy();
	}
	return(*this);
}

// binary tree operations
template <class DataType>
void
BinaryTree_AVL<DataType>::insert(const DataType &data)
{
	int heightChanged;
	insert(root, data, heightChanged);
	return;
}

template <class DataType>
int
BinaryTree_AVL<DataType>::remove(DataType &data)
{
	int heightChanged;
	return(remove(root, data, heightChanged));
}

template <class DataType>
int
BinaryTree_AVL<DataType>::retrieve(DataType &data) const
{
	return(retrieve(root, data));
}

template <class DataType>
int
BinaryTree_AVL<DataType>::isInTree(const DataType &data) const
{
	return(isInTree(root, data));
}

template <class DataType>
int
BinaryTree_AVL<DataType>::isEmpty() const
{
	return(root == NULL);
}

template <class DataType>
void
BinaryTree_AVL<DataType>::clear()
{
	clear(root);
	return;
}

// tree traversal functions
template <class DataType>
int
BinaryTree_AVL<DataType>::preOrder(int (*work)(DataType &))
{
	MustBeTrue(work != NULL);
	return(preOrder(root, work));
}

template <class DataType>
int
BinaryTree_AVL<DataType>::inOrder(int (*work)(DataType &))
{
	MustBeTrue(work != NULL);
	return(inOrder(root, work));
}

template <class DataType>
int
BinaryTree_AVL<DataType>::postOrder(int (*work)(DataType &))
{
	MustBeTrue(work != NULL);
	return(postOrder(root, work));
}

template <class DataType>
int
BinaryTree_AVL<DataType>::levelOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == NULL)
		return(OK);

	// define queue for level-order traversal
	Queue_List<BinaryTreeNode_AVL<DataType> * > *pq;
	pq = new Queue_List<BinaryTreeNode_AVL<DataType> * >;
	MustBeTrue(pq != NULL);

	// insert root into queue to start traversal
	pq->enqueue(root);

	// loop over nodes in queue
	while ( ! pq->isEmpty())
	{
		// get next node from queue
		BinaryTreeNode_AVL<DataType> *btn;
		MustBeTrue((pq->dequeue(btn)) == OK);

		// execute work function for data
		int status = work(btn->data);
		if (status != OK)
		{
			delete pq;
			return(status);
		}

		// queue up left and right nodes, if any.
		if (btn->left != NULL)
			pq->enqueue(btn->left);
		if (btn->right != NULL)
			pq->enqueue(btn->right);
	}

	// release queue
	delete pq;

	// all done
	return(OK);
}

// print binary tree data
template <class DataType>
ostream &
BinaryTree_AVL<DataType>::dump(ostream &os) const
{
	dump(root, os);
	return(os);
}

// utility functions
template <class DataType>
BinaryTreeNode_AVL<DataType> *
BinaryTree_AVL<DataType>::copy() const
{
	return(copy(root));
}

template <class DataType>
BinaryTreeNode_AVL<DataType> *
BinaryTree_AVL<DataType>::copy(const BinaryTreeNode_AVL<DataType> *node) const
{
	BinaryTreeNode_AVL<DataType> *pnew = NULL;
	if (node != NULL)
	{
		pnew = new BinaryTreeNode_AVL<DataType>(node->data);
		MustBeTrue(pnew != NULL);
		pnew->balance = node->balance;
		pnew->left = copy(node->left);
		pnew->right = copy(node->right);
	}
	return(pnew);
}

template <class DataType>
void
BinaryTree_AVL<DataType>::clear(BinaryTreeNode_AVL<DataType> *&node)
{
	if (node != NULL)
	{
		clear(node->left);
		clear(node->right);
		delete node;
		node = NULL;
	}
	return;
}

template <class DataType>
int
BinaryTree_AVL<DataType>::retrieve(const BinaryTreeNode_AVL<DataType> *btn, 
			     DataType &data) const
{
	// search for data
	if (btn == NULL)
	{
		return(NOMATCH);
	}
	else if (data < btn->data)
	{
		return(retrieve(btn->left, data));
	}
	else if (data > btn->data)
	{
		return(retrieve(btn->right, data));
	}
	else
	{
		data = btn->data;
		return(OK);
	}
}

template <class DataType>
int
BinaryTree_AVL<DataType>::isInTree(const BinaryTreeNode_AVL<DataType> *node, 
				const DataType &data) const
{
	// look for a match
	if (node == NULL)
	{
		// not found
		return(0);
	}
	else if (data < node->data)
	{
		return(isInTree(node->left, data));
	}
	else if (data > node->data)
	{
		return(isInTree(node->right, data));
	}
	else
	{
		// found
		return(1);
	}
}

template <class DataType>
int
BinaryTree_AVL<DataType>::preOrder(BinaryTreeNode_AVL<DataType> *node, 
				int (*work)(DataType &))
{
	if (node != NULL)
	{
		int status;
		if ((status = work(node->data)) != OK)
			return(status);
		if ((status = preOrder(node->left, work)) != OK)
			return(status);
		if ((status = preOrder(node->right, work)) != OK)
			return(status);
	}
	return(OK);
}

template <class DataType>
int
BinaryTree_AVL<DataType>::inOrder(BinaryTreeNode_AVL<DataType> *node,
				int (*work)(DataType &))
{
	if (node != NULL)
	{
		int status;
		if ((status = inOrder(node->left, work)) != OK)
			return(status);
		if ((status = work(node->data)) != OK)
			return(status);
		if ((status = inOrder(node->right, work)) != OK)
			return(status);
	}
	return(OK);
}

template <class DataType>
int
BinaryTree_AVL<DataType>::postOrder(BinaryTreeNode_AVL<DataType> *node,
				int (*work)(DataType &))
{
	if (node != NULL)
	{
		int status;
		if ((status = postOrder(node->left, work)) != OK)
			return(status);
		if ((status = postOrder(node->right, work)) != OK)
			return(status);
		if ((status = work(node->data)) != OK)
			return(status);
	}
	return(OK);
}

template <class DataType>
void
BinaryTree_AVL<DataType>::dump(BinaryTreeNode_AVL<DataType> *node,
				ostream &os) const
{
	if (node != NULL)
	{
		dump(node->left, os);
		os << node->data << " ";
		dump(node->right, os);
	}
	return;
}

// utility functions
template <class DataType>
void
BinaryTree_AVL<DataType>::insert(BinaryTreeNode_AVL<DataType> *&btn, 
			     const DataType &data, int &heightChanged)
{
	// check where new node goes
	if (btn == NULL)
	{
		// new node. allocate it.
		btn = new BinaryTreeNode_AVL<DataType>(data);
		MustBeTrue(btn != NULL);
		heightChanged = 1;
	}
	else if (data < btn->data)
	{
		// follow left subtree branch for insert.
		insert(btn->left, data, heightChanged);

		// check if height changed
		if (heightChanged)
		{
			// which case do we have?
			switch (btn->balance)
			{
			case -1:
				btn->balance = 0;
				heightChanged = 0;
				break;
			case 0:
				btn->balance = 1;
				heightChanged = 1;
				break;
			case 1:
				// single or double rotation?
				fixLeftTreeUponInsert(btn);
				heightChanged = 0;
				break;
			default:
				MustBeTrue(btn->balance == -1 || 
				       btn->balance == 0 || 
				       btn->balance == 1);
			}
		}
	}
	else if (data > btn->data)
	{
		// follow right subtree branch for insert.
		insert(btn->right, data, heightChanged);

		// check if height changed
		if (heightChanged)
		{
			// which case do we have?
			switch (btn->balance)
			{
			case 1:
				btn->balance = 0;
				heightChanged = 0;
				break;
			case 0:
				btn->balance = -1;
				heightChanged = 1;
				break;
			case -1:
				// single or double rotation?
				fixRightTreeUponInsert(btn);
				heightChanged = 0;
				break;
			default:
				MustBeTrue(btn->balance == -1 || 
				       btn->balance == 0 || 
				       btn->balance == 1);
			}
		}
	}
	else
	{
		btn->data = data;
	}
	return;
}

template <class DataType>
int
BinaryTree_AVL<DataType>::remove(BinaryTreeNode_AVL<DataType> *&btn, 
			     DataType &data, int &heightChanged)
{
	// search for data, then delete
	if (btn == NULL)
	{
		heightChanged = 0;
		return(NOMATCH);
	}
	else if (data < btn->data)
	{
		int status = remove(btn->left, data, heightChanged);
		if (status != OK) return(status);
		if (heightChanged)
		{
			switch (btn->balance)
			{
			case -1:
				// unbalance on the right
				fixRightTreeUponDelete(btn, heightChanged);
				break;
			case 0:
				btn->balance = -1;
				heightChanged = 0;
				break;
			case 1:
				btn->balance = 0;
				heightChanged = 1;
				break;
			default:
				MustBeTrue(btn->balance == -1 || 
					btn->balance == 0 || 
					btn->balance == 1);
			}
		}
	}
	else if (data > btn->data)
	{
		int status = remove(btn->right, data, heightChanged);
		if (status != OK) return(status);
		if (heightChanged)
		{
			switch (btn->balance)
			{
			case -1:
				btn->balance = 0;
				heightChanged = 1;
				break;
			case 0:
				btn->balance = 1;
				heightChanged = 0;
				break;
			case 1:
				fixLeftTreeUponDelete(btn, heightChanged);
				break;
			default:
				MustBeTrue(btn->balance == -1 || 
					btn->balance == 0 || 
					btn->balance == 1);
			}
		}
	}
	else
	{
		// return data
		data = btn->data;

		// we found it. now delete it.
		if (btn->left == NULL && btn->right == NULL)
		{
			// it's a leaf. just delete it.
			delete btn;
			btn = NULL;
			heightChanged = 1;
		}
		else if (btn->left == NULL)
		{
			// replace with right node
			BinaryTreeNode_AVL<DataType> *p = btn->right;
			delete btn;
			btn = p;
			heightChanged = 1;
		}
		else if (btn->right == NULL)
		{
			// replace with left node
			BinaryTreeNode_AVL<DataType> *p = btn->left;
			delete btn;
			btn = p;
			heightChanged = 1;
		}
		else
		{
			// both children are NOT null. find node
			// immediately before this node and copy it.
			int status = removeRightMost(btn->left, 
					btn->data, heightChanged);
			if (status != OK) return(status);
			if (heightChanged)
			{
				switch (btn->balance)
				{
				case -1:
					fixRightTreeUponDelete(btn, 
						heightChanged);
					break;
				case 0:
					btn->balance = -1;
					heightChanged = 0;
					break;
				case 1:
					btn->balance = 0;
					heightChanged = 1;
					break;
				default:
					MustBeTrue(btn->balance == -1 || 
						btn->balance == 0 || 
						btn->balance == 1);
				}
			}
		}
	}
	return(OK);
}

template <class DataType>
int
BinaryTree_AVL<DataType>::removeRightMost(BinaryTreeNode_AVL<DataType> *&btn, 
			     DataType &data, int &heightChanged)
{
	// keep looking for right-most node
	if (btn->right != NULL)
	{
		int status = removeRightMost(btn->right, data, heightChanged);
		if (status != OK) return(status);
		if (heightChanged)
		{
			switch (btn->balance)
			{
			case -1:
				btn->balance = 0;
				heightChanged = 1;
				break;
			case 0:
				btn->balance = 1;
				heightChanged = 0;
				break;
			case 1:
				fixLeftTreeUponDelete(btn, heightChanged);
				break;
			default:
				MustBeTrue(btn->balance == -1 || 
					btn->balance == 0 || 
					btn->balance == 1);
			}
		}
	}
	else
	{
		// found right-most node. copy data.
		data = btn->data;

		// remove node.
		BinaryTreeNode_AVL<DataType> *p = btn->left;
		delete btn;
		btn = p;
		heightChanged = 1;
	}
	return(OK);
}

template <class DataType>
void
BinaryTree_AVL<DataType>::rotateLeft(BinaryTreeNode_AVL<DataType> *&btn)
{
	BinaryTreeNode_AVL<DataType> *p = btn;
	btn = btn->right;
	p->right = btn->left;
	btn->left = p;
	return;
}

template <class DataType>
void
BinaryTree_AVL<DataType>::rotateRight(BinaryTreeNode_AVL<DataType> *&btn)
{
	BinaryTreeNode_AVL<DataType> *p = btn;
	btn = btn->left;
	p->left = btn->right;
	btn->right = p;
	return;
}

template <class DataType>
void
BinaryTree_AVL<DataType>::fixLeftTreeUponInsert(
		BinaryTreeNode_AVL<DataType> *&btn)
{
	// single or double rotation?
	switch (btn->left->balance)
	{
	case -1:
		rotateLeft(btn->left);
		rotateRight(btn);
		switch (btn->balance)
		{
		case -1:
			btn->balance = 0;
			btn->right->balance = 0;
			btn->left->balance = 1;
			break;
		case 1:
			btn->balance = 0;
			btn->right->balance = -1;
			btn->left->balance = 0;
			break;
		case 0:
			btn->balance = 0;
			btn->right->balance = 0;
			btn->left->balance = 0;
			break;
		default:
			MustBeTrue(btn->balance == -1 || 
			       btn->balance == 0 || 
			       btn->balance == 1);
		}
		break;
	case 1:
		rotateRight(btn);
		btn->balance = 0;
		btn->right->balance = 0;
		break;
	default:
		MustBeTrue(btn->balance == -1 || 
		       btn->balance == 1);
	}
	return;
}

template <class DataType>
void
BinaryTree_AVL<DataType>::fixRightTreeUponInsert(
		BinaryTreeNode_AVL<DataType> *&btn)
{
	// single or double rotation?
	switch (btn->right->balance)
	{
	case -1:
		rotateLeft(btn);
		btn->balance = 0;
		btn->left->balance = 0;
		break;
	case 1:
		rotateRight(btn->right);
		rotateLeft(btn);
		switch (btn->balance)
		{
		case -1:
			btn->balance = 0;
			btn->left->balance = +1;
			btn->right->balance = 0;
			break;
		case 1:
			btn->balance = 0;
			btn->left->balance = 0;
			btn->right->balance = -1;
			break;
		case 0:
			btn->balance = 0;
			btn->right->balance = 0;
			btn->left->balance = 0;
			break;
		default:
			MustBeTrue(btn->balance == -1 || 
			       btn->balance == 0 || 
			       btn->balance == 1);
		}
		break;
	default:
		MustBeTrue(btn->balance == -1 || 
		       btn->balance == 1);
	}
	return;
}

template <class DataType>
void
BinaryTree_AVL<DataType>::fixRightTreeUponDelete(
	BinaryTreeNode_AVL<DataType> *&btn, int &heightChanged)
{
	// unbalance on the right
	switch (btn->right->balance)
	{
	case -1:
		rotateLeft(btn);
		btn->balance = 0;
		btn->left->balance = 0;
		heightChanged = 1;
		break;
	case 0:
		rotateLeft(btn);
		btn->balance = 1;
		btn->left->balance = -1;
		heightChanged = 0;
		break;
	case 1:
		rotateRight(btn->right);
		rotateLeft(btn);
		switch (btn->balance)
		{
		case -1:
			btn->balance = 0;
			btn->left->balance = +1;
			btn->right->balance = 0;
			break;
		case 0:
			btn->balance = 0;
			btn->right->balance = 0;
			btn->left->balance = 0;
			break;
		case 1:
			btn->balance = 0;
			btn->left->balance = 0;
			btn->right->balance = -1;
			break;
		default:
			MustBeTrue(btn->balance == -1 || 
			       btn->balance == 0 || 
			       btn->balance == 1);
		}
		heightChanged = 1;
		break;
	default:
		MustBeTrue(btn->balance == -1 || 
		       btn->balance == 0 || 
		       btn->balance == 1);
	}
	return;
}

template <class DataType>
void
BinaryTree_AVL<DataType>::fixLeftTreeUponDelete(
	BinaryTreeNode_AVL<DataType> *&btn, int &heightChanged)
{
	switch (btn->left->balance)
	{
	case -1:
		rotateLeft(btn->left);
		rotateRight(btn);
		switch (btn->balance)
		{
		case -1:
			btn->balance = 0;
			btn->left->balance = 1;
			btn->right->balance = 0;
			break;
		case 0:
			btn->balance = 0;
			btn->left->balance = 0;
			btn->right->balance = 0;
			break;
		case 1:
			btn->balance = 0;
			btn->left->balance = 0;
			btn->right->balance = -1;
			break;
		default:
			MustBeTrue(btn->balance == -1 || 
			       btn->balance == 0 || 
			       btn->balance == 1);
		}
		heightChanged = 1;
		break;
	case 0:
		rotateRight(btn);
		btn->balance = -1;
		btn->right->balance = 1;
		heightChanged = 0;
		break;
	case 1:
		rotateRight(btn);
		btn->balance = 0;
		btn->right->balance = 0;
		heightChanged = 1;
		break;
	default:
		MustBeTrue(btn->balance == -1 || 
		       btn->balance == 0 || 
		       btn->balance == 1);
	}
	return;
}

template <class DataType>
int
BinaryTree_AVL<DataType>::audit(const BinaryTreeNode_AVL<DataType> *btn) const
{
	if (btn == NULL)
		return(0);
	else
	{
		int height = 0;
		int left = audit(btn->left);
		int right = audit(btn->right);
		if (left > right)
			height = left + 1;
		else
			height = right + 1;
		if (btn->balance != (left-right))
		{
			cout << "<" << btn->data << "," << btn->balance << ",";
			cout << left << "," << right << ">" << endl;
			cout << "MISMATCH FOUND !!!" << endl;
			cout << "bt is ... " << *this << endl;
		}
		return(height);
	}
}

template <class DataType>
void
BinaryTree_AVL<DataType>::audit() const
{
	audit(root);
	return;
}

// preorder iterator constructors and destructor
template <class DataType>
BinaryTree_AVL_Iterator_PreOrder<DataType>::BinaryTree_AVL_Iterator_PreOrder(
	const BinaryTree_AVL_Iterator_PreOrder<DataType> &iter): 
	tree(iter.tree), stack(iter.stack)
{
	// do nothing
}

template <class DataType>
BinaryTree_AVL_Iterator_PreOrder<DataType>::BinaryTree_AVL_Iterator_PreOrder(
	const BinaryTree_AVL<DataType> &bt): 
	tree(&bt), stack()
{
	reset();
}

template <class DataType>
BinaryTree_AVL_Iterator_PreOrder<DataType>::~BinaryTree_AVL_Iterator_PreOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_AVL_Iterator_PreOrder<DataType>::reset()
{
	stack.clear();
	stack.push(tree->root);
	return;
}

// is list empty
template <class DataType>
int
BinaryTree_AVL_Iterator_PreOrder<DataType>::done() const
{
	return(stack.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTree_AVL_Iterator_PreOrder<DataType>::operator()()
{
	BinaryTreeNode_AVL<DataType> *btn;
	MustBeTrue(stack.top(btn) == OK);
	return(btn->data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_AVL_Iterator_PreOrder<DataType>::operator++(int)
{
	// is iterator done
	if (stack.isEmpty()) return(NOTOK);

	// get next node from stack
	BinaryTreeNode_AVL<DataType> *btn;
	MustBeTrue(stack.pop(btn) == OK);

	// stack left and right nodes, if any.
	if (btn->right != NULL)
		stack.push(btn->right);
	if (btn->left != NULL)
		stack.push(btn->left);
	return(OK);
}

// inorder iterator constructors and destructor
template <class DataType>
BinaryTree_AVL_Iterator_InOrder<DataType>::BinaryTree_AVL_Iterator_InOrder(
	const BinaryTree_AVL_Iterator_InOrder<DataType> &iter): 
	BinaryTree_AVL_Iterator_PreOrder<DataType>(iter)
{
	// do nothing
}

template <class DataType>
BinaryTree_AVL_Iterator_InOrder<DataType>::BinaryTree_AVL_Iterator_InOrder(
	const BinaryTree_AVL<DataType> &bt): 
	BinaryTree_AVL_Iterator_PreOrder<DataType>(bt)
{
	reset();
}

template <class DataType>
BinaryTree_AVL_Iterator_InOrder<DataType>::~BinaryTree_AVL_Iterator_InOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_AVL_Iterator_InOrder<DataType>::reset()
{
	stack.clear();
	for (BinaryTreeNode_AVL<DataType> *pn = tree->root; 
		pn != NULL; pn = pn->left)
	{
		stack.push(pn);
	}
	return;
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_AVL_Iterator_InOrder<DataType>::operator++(int)
{
	// is iterator done
	if (stack.isEmpty()) return(NOTOK);

	// get next node from stack
	BinaryTreeNode_AVL<DataType> *btn;
	MustBeTrue(stack.pop(btn) == OK);

	// insert right node left links into stack
	if (btn->right != NULL)
	{
		for (BinaryTreeNode_AVL<DataType> *pn = btn->right;
			pn != NULL; pn = pn->left)
		{
			stack.push(pn);
		}
	}
	return(OK);
}

// postorder iterator constructors and destructor
template <class DataType>
BinaryTree_AVL_Iterator_PostOrder<DataType>::BinaryTree_AVL_Iterator_PostOrder(
	const BinaryTree_AVL_Iterator_PostOrder<DataType> &iter): 
	BinaryTree_AVL_Iterator_PreOrder<DataType>(iter), vstack(iter.vstack)
{
	// do nothing
}

template <class DataType>
BinaryTree_AVL_Iterator_PostOrder<DataType>::BinaryTree_AVL_Iterator_PostOrder(
	const BinaryTree_AVL<DataType> &bt): 
	BinaryTree_AVL_Iterator_PreOrder<DataType>(bt), vstack()
{
	reset();
}

template <class DataType>
BinaryTree_AVL_Iterator_PostOrder<DataType>::~BinaryTree_AVL_Iterator_PostOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_AVL_Iterator_PostOrder<DataType>::reset()
{
	int visits;

	// clear stack
	stack.clear();
	vstack.clear();

	// check if tree is empty
	if (tree->isEmpty()) return;

	// push root onto stack
	visits = 1;
	stack.push(tree->root);
	vstack.push(visits);

	// process until the first visit
	BinaryTreeNode_AVL<DataType> *btn = NULL;
	while ( ! stack.isEmpty())
	{
		// pop current node
		stack.pop(btn);
		vstack.pop(visits);

		// process
		switch (visits)
		{
		case 1:
			visits = 2;
			stack.push(btn);
			vstack.push(visits);
			if (btn->left != NULL)
			{
				visits = 1;
				stack.push(btn->left);
				vstack.push(visits);
			}
			break;

		case 2:
			visits = 3;
			stack.push(btn);
			vstack.push(visits);
			if (btn->right != NULL)
			{
				visits = 1;
				stack.push(btn->right);
				vstack.push(visits);
			}
			break;

		case 3:
			visits = 4;
			stack.push(btn);
			vstack.push(visits);
			return;

		default:
			MustBeTrue(visits <= 3);
			return;
		}
	}
	return;
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_AVL_Iterator_PostOrder<DataType>::operator++(int)
{
	// is iterator done
	if (stack.isEmpty()) return(NOTOK);

	// get next node from stack
	int visits;
	BinaryTreeNode_AVL<DataType> *btn;
	MustBeTrue(stack.pop(btn) == OK);
	MustBeTrue(vstack.pop(visits) == OK);
	MustBeTrue(visits > 3);

	// find next node to process
	while ( ! stack.isEmpty())
	{
		// pop current node
		stack.pop(btn);
		vstack.pop(visits);

		// process
		switch (visits)
		{
		case 1:
			visits = 2;
			stack.push(btn);
			vstack.push(visits);
			if (btn->left != NULL)
			{
				visits = 1;
				stack.push(btn->left);
				vstack.push(visits);
			}
			break;

		case 2:
			visits = 3;
			stack.push(btn);
			vstack.push(visits);
			if (btn->right != NULL)
			{
				visits = 1;
				stack.push(btn->right);
				vstack.push(visits);
			}
			break;

		case 3:
			visits = 4;
			stack.push(btn);
			vstack.push(visits);
			return(OK);

		default:
			MustBeTrue(visits <= 3);
			return(NOTOK);
		}
	}
	return(NOTOK);
}

// levelorder iterator constructors and destructor
template <class DataType>
BinaryTree_AVL_Iterator_LevelOrder<DataType>::BinaryTree_AVL_Iterator_LevelOrder(
	const BinaryTree_AVL_Iterator_LevelOrder<DataType> &iter): 
	tree(iter.tree), queue(iter.queue)
{
	// do nothing
}

template <class DataType>
BinaryTree_AVL_Iterator_LevelOrder<DataType>::BinaryTree_AVL_Iterator_LevelOrder(
	const BinaryTree_AVL<DataType> &bt): 
	tree(&bt), queue()
{
	reset();
}

template <class DataType>
BinaryTree_AVL_Iterator_LevelOrder<DataType>::~BinaryTree_AVL_Iterator_LevelOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_AVL_Iterator_LevelOrder<DataType>::reset()
{
	queue.clear();
	queue.enqueue(tree->root);
	return;
}

// is list empty
template <class DataType>
int
BinaryTree_AVL_Iterator_LevelOrder<DataType>::done() const
{
	return(queue.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTree_AVL_Iterator_LevelOrder<DataType>::operator()()
{
	BinaryTreeNode_AVL<DataType> *btn;
	MustBeTrue(queue.front(btn) == OK);
	return(btn->data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_AVL_Iterator_LevelOrder<DataType>::operator++(int)
{
	// is iterator done
	if (queue.isEmpty()) return(NOTOK);

	// get next node from queue
	BinaryTreeNode_AVL<DataType> *btn;
	MustBeTrue(queue.dequeue(btn) == OK);

	// queue up left and right nodes, if any.
	if (btn->left != NULL)
		queue.enqueue(btn->left);
	if (btn->right != NULL)
		queue.enqueue(btn->right);
	return(OK);
}

