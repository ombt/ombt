// member functions for recursive binary tree class

// required headers
#include "binaryTree.h"

// binary tree node constructors and destructor
template <class DataType>
BinaryTreeNode<DataType>::BinaryTreeNode(const DataType &d):
	data(d), left(NULL), right(NULL)
{
	// do nothing
}

template <class DataType>
BinaryTreeNode<DataType>::BinaryTreeNode(const BinaryTreeNode<DataType> &btn):
	data(btn.data), left(NULL), right(NULL)
{
	// do nothing
}

template <class DataType>
BinaryTreeNode<DataType>::~BinaryTreeNode()
{
	left = NULL;
	right = NULL;
}

// assignment
template <class DataType>
BinaryTreeNode<DataType> &
BinaryTreeNode<DataType>::operator=(const BinaryTreeNode<DataType> &btn)
{
	if (this != &btn)
	{
		left = NULL;
		right = NULL;
		data = btn.data;
	}
	return(*this);
}

// binary tree constructors and destructor
#ifdef MUTABLE_ITERATOR
template <class DataType>
BinaryTree<DataType>::BinaryTree():
	root(NULL), iterList()
{
	// do nothing
}

template <class DataType>
BinaryTree<DataType>::BinaryTree(const BinaryTree<DataType> &bt):
	root(NULL), iterList()
{
	root = bt.copy();
}
#else
template <class DataType>
BinaryTree<DataType>::BinaryTree():
	root(NULL)
{
	// do nothing
}

template <class DataType>
BinaryTree<DataType>::BinaryTree(const BinaryTree<DataType> &bt):
	root(NULL)
{
	root = bt.copy();
}
#endif

template <class DataType>
BinaryTree<DataType>::~BinaryTree()
{
	clear();
}

// assignment
template <class DataType>
BinaryTree<DataType> &
BinaryTree<DataType>::operator=(const BinaryTree<DataType> &bt)
{
	if (this != &bt)
	{
		clear();
#ifdef MUTABLE_ITERATOR
		iterList.clear();
#endif
		root = bt.copy();
	}
	return(*this);
}

// binary tree operations
template <class DataType>
int
BinaryTree<DataType>::insert(const DataType &data)
{
	// insert new data
	if (insert(root, data) != OK) return(NOTOK);

#ifdef MUTABLE_ITERATOR
	// check if any iterators are traversing this tree
	if ( ! iterList.isEmpty())
	{
		// now update any iterators
		ListIterator<AbstractIterator<DataType> * > piter(iterList);
		for ( ; ! piter.done(); piter++)
		{
			// save current iterator
			AbstractIterator<DataType> *paiter = piter();

			// get current value
			DataType current = (*paiter)();

			// update iterator
			(*paiter).resetByValue(current);
		}
	}
#endif
	return(OK);
}

template <class DataType>
int
BinaryTree<DataType>::update(const DataType &data)
{
	if (includes(data))
		return(insert(data));
	else
		return(NOMATCH);
}

template <class DataType>
int
BinaryTree<DataType>::remove(DataType &data)
{
#ifdef MUTABLE_ITERATOR
	// check if any iterators are traversing this tree
	int status;
	if (iterList.isEmpty())
		return(remove(root, data));
	else
	{
		ListIterator<AbstractIterator<DataType> * > piter(iterList);
		for ( ; ! piter.done(); piter++)
		{
			// save current iterator
			AbstractIterator<DataType> *paiter = piter();

			// check of iterator is done
			if ((*paiter).done()) continue;

			// check if iterator is pointing to deleted node
			if (data == (*paiter)()) (*paiter)++;
		}

		// delete element
		int status = remove(root, data);
		if (status != OK) return(status);

		// reset all iterators
		for (piter.reset(); ! piter.done(); piter++)
		{
			// save current iterator
			AbstractIterator<DataType> *paiter = piter();

			// get current value
			DataType current = (*paiter)();

			// check if iterator is done
			if ( ! (*paiter).done())
				(*paiter).resetByValue(current);
		}
		return(OK);
	}
#else
	return(remove(root, data));
#endif
}

template <class DataType>
int
BinaryTree<DataType>::retrieve(DataType &data) const
{
	return(retrieve(root, data));
}

template <class DataType>
int
BinaryTree<DataType>::includes(const DataType &data) const
{
	return(includes(root, data));
}

template <class DataType>
int
BinaryTree<DataType>::isEmpty() const
{
	return(root == NULL);
}

template <class DataType>
void
BinaryTree<DataType>::clear()
{
	// clear binary tree
	clear(root);

#ifdef MUTABLE_ITERATOR
	// reset all iterators
	ListIterator<AbstractIterator<DataType> * > piter(iterList);
	for ( ; ! piter.done(); piter++)
	{
		// current iterator
		AbstractIterator<DataType> *paiter = piter();

		// reset iterator
		(*paiter).reset();
	}
#endif
	return;
}

// tree traversal functions
template <class DataType>
int
BinaryTree<DataType>::preOrder(int (*work)(DataType &))
{
	MustBeTrue(work != NULL);
	return(preOrder(root, work));
}

template <class DataType>
int
BinaryTree<DataType>::inOrder(int (*work)(DataType &))
{
	MustBeTrue(work != NULL);
	return(inOrder(root, work));
}

template <class DataType>
int
BinaryTree<DataType>::postOrder(int (*work)(DataType &))
{
	MustBeTrue(work != NULL);
	return(postOrder(root, work));
}

template <class DataType>
int
BinaryTree<DataType>::levelOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == NULL)
		return(OK);

	// define queue for level-order traversal
	Queue_List<BinaryTreeNode<DataType> * > pq;

	// insert root into queue to start traversal
	pq.enqueue(root);

	// loop over nodes in queue
	while ( ! pq.isEmpty())
	{
		// get next node from queue
		BinaryTreeNode<DataType> *btn;
		MustBeTrue(pq.dequeue(btn) == OK);

		// execute work function for data
		int status = work(btn->data);
		if (status != OK)
			return(status);

		// queue up left and right nodes, if any.
		if (btn->left != NULL)
			pq.enqueue(btn->left);
		if (btn->right != NULL)
			pq.enqueue(btn->right);
	}

	// all done
	return(OK);
}

// print binary tree data
template <class DataType>
ostream &
BinaryTree<DataType>::dump(ostream &os) const
{
	dump(root, os);
	return(os);
}

// utility functions
template <class DataType>
BinaryTreeNode<DataType> *
BinaryTree<DataType>::copy() const
{
	return(copy(root));
}

template <class DataType>
BinaryTreeNode<DataType> *
BinaryTree<DataType>::copy(const BinaryTreeNode<DataType> *node) const
{
	BinaryTreeNode<DataType> *pnew = NULL;
	if (node != NULL)
	{
		pnew = new BinaryTreeNode<DataType>(node->data);
		MustBeTrue(pnew != NULL);
		pnew->left = copy(node->left);
		pnew->right = copy(node->right);
	}
	return(pnew);
}

template <class DataType>
void
BinaryTree<DataType>::clear(BinaryTreeNode<DataType> *&node)
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
BinaryTree<DataType>::insert(BinaryTreeNode<DataType> *&btn, 
			     const DataType &data)
{
	// check where new node goes
	if (btn == NULL)
	{
		btn = new BinaryTreeNode<DataType>(data);
		if (btn == NULL) return(NOTOK);
	}
	else if (data < btn->data)
	{
		return(insert(btn->left, data));
	}
	else if (data > btn->data)
	{
		return(insert(btn->right, data));
	}
	else
	{
		btn->data = data;
	}
	return(OK);
}

template <class DataType>
int
BinaryTree<DataType>::remove(BinaryTreeNode<DataType> *&btn, 
			     DataType &data)
{
	// search for data, then delete
	if (btn == NULL)
	{
		return(NOMATCH);
	}
	else if (data < btn->data)
	{
		return(remove(btn->left, data));
	}
	else if (data > btn->data)
	{
		return(remove(btn->right, data));
	}

	// return data
	data = btn->data;

	// we found it. now delete it.
	if (btn->left == NULL && btn->right == NULL)
	{
		// it's a leaf. just delete it.
		delete btn;
		btn = NULL;
	}
	else if (btn->left == NULL)
	{
		// replace with right node
		BinaryTreeNode<DataType> *p = btn->right;
		delete btn;
		btn = p;
	}
	else if (btn->right == NULL)
	{
		// replace with left node
		BinaryTreeNode<DataType> *p = btn->left;
		delete btn;
		btn = p;
	}
	else
	{
		// both children are NOT null. find node
		// immediately before this node and copy it.
		return(removeRightMost(btn->left, btn->data));
	}
	return(OK);
}

template <class DataType>
int
BinaryTree<DataType>::removeRightMost(BinaryTreeNode<DataType> *&btn, 
			     DataType &data)
{
	// keep looking for right-most node
	if (btn->right != NULL)
	{
		return(removeRightMost(btn->right, data));
	}
	else
	{
		// found right-most node. copy data.
		data = btn->data;

		// remove node.
		BinaryTreeNode<DataType> *p = btn->left;
		delete btn;
		btn = p;
		return(OK);
	}
}

template <class DataType>
int
BinaryTree<DataType>::retrieve(const BinaryTreeNode<DataType> *btn, 
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
BinaryTree<DataType>::includes(const BinaryTreeNode<DataType> *node, 
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
		return(includes(node->left, data));
	}
	else if (data > node->data)
	{
		return(includes(node->right, data));
	}
	else
	{
		// found
		return(1);
	}
}

template <class DataType>
int
BinaryTree<DataType>::preOrder(BinaryTreeNode<DataType> *node, 
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
BinaryTree<DataType>::inOrder(BinaryTreeNode<DataType> *node,
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
BinaryTree<DataType>::postOrder(BinaryTreeNode<DataType> *node,
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
BinaryTree<DataType>::dump(BinaryTreeNode<DataType> *node,
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

// preorder iterator constructors and destructor
template <class DataType>
BinaryTreeIterator_PreOrder<DataType>::BinaryTreeIterator_PreOrder(
	const BinaryTreeIterator_PreOrder<DataType> &iter): 
	tree(iter.tree), stack(iter.stack)
{
#ifdef MUTABLE_ITERATOR
	tree->iterList.insertOrderedUnique(this);
#endif
}

#ifdef MUTABLE_ITERATOR
template <class DataType>
BinaryTreeIterator_PreOrder<DataType>::BinaryTreeIterator_PreOrder(
	BinaryTree<DataType> &bt): 
	tree(&bt), stack()
{
	tree->iterList.insertOrderedUnique(this);
	reset();
}
#else
template <class DataType>
BinaryTreeIterator_PreOrder<DataType>::BinaryTreeIterator_PreOrder(
	const BinaryTree<DataType> &bt): 
	tree(&bt), stack()
{
	reset();
}
#endif

template <class DataType>
BinaryTreeIterator_PreOrder<DataType>::~BinaryTreeIterator_PreOrder()
{
#ifdef MUTABLE_ITERATOR
	AbstractIterator<DataType> *ptr = this;
	(void)tree->iterList.removeOrderedUnique(ptr);
#endif
}

// reset iterator to beginning
template <class DataType>
void
BinaryTreeIterator_PreOrder<DataType>::reset()
{
	stack.clear();
	stack.push(tree->root);
	return;
}

// is list empty
template <class DataType>
int
BinaryTreeIterator_PreOrder<DataType>::done() const
{
	return(stack.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTreeIterator_PreOrder<DataType>::operator()()
{
	BinaryTreeNode<DataType> *btn;
	MustBeTrue(stack.top(btn) == OK);
	return(btn->data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTreeIterator_PreOrder<DataType>::operator++(int)
{
	// is iterator done
	if (stack.isEmpty()) return(NOTOK);

	// get next node from stack
	BinaryTreeNode<DataType> *btn;
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
BinaryTreeIterator_InOrder<DataType>::BinaryTreeIterator_InOrder(
	const BinaryTreeIterator_InOrder<DataType> &iter): 
	tree(iter.tree), stack(iter.stack)
{
#ifdef MUTABLE_ITERATOR
	tree->iterList.insertOrderedUnique(this);
#endif
}

#ifdef MUTABLE_ITERATOR
template <class DataType>
BinaryTreeIterator_InOrder<DataType>::BinaryTreeIterator_InOrder(
	BinaryTree<DataType> &bt): 
	tree(&bt), stack()
{
	tree->iterList.insertOrderedUnique(this);
	reset();
}
#else
template <class DataType>
BinaryTreeIterator_InOrder<DataType>::BinaryTreeIterator_InOrder(
	const BinaryTree<DataType> &bt): 
	tree(&bt), stack()
{
	reset();
}
#endif

template <class DataType>
BinaryTreeIterator_InOrder<DataType>::~BinaryTreeIterator_InOrder()
{
#ifdef MUTABLE_ITERATOR
	AbstractIterator<DataType> *ptr = this;
	(void)tree->iterList.removeOrderedUnique(ptr);
#endif
}

// reset iterator to beginning
template <class DataType>
void
BinaryTreeIterator_InOrder<DataType>::reset()
{
	stack.clear();
	for (BinaryTreeNode<DataType> *pn = tree->root; 
		pn != NULL; pn = pn->left)
	{
		stack.push(pn);
	}
	return;
}

// is list empty
template <class DataType>
int
BinaryTreeIterator_InOrder<DataType>::done() const
{
	return(stack.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTreeIterator_InOrder<DataType>::operator()()
{
	BinaryTreeNode<DataType> *btn;
	MustBeTrue(stack.top(btn) == OK);
	return(btn->data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTreeIterator_InOrder<DataType>::operator++(int)
{
	// is iterator done
	if (stack.isEmpty()) return(NOTOK);

	// get next node from stack
	BinaryTreeNode<DataType> *btn;
	MustBeTrue(stack.pop(btn) == OK);

	// insert right node left links into stack
	if (btn->right != NULL)
	{
		for (BinaryTreeNode<DataType> *pn = btn->right;
			pn != NULL; pn = pn->left)
		{
			stack.push(pn);
		}
	}
	return(OK);
}

// postorder iterator constructors and destructor
template <class DataType>
BinaryTreeIterator_PostOrder<DataType>::BinaryTreeIterator_PostOrder(
	const BinaryTreeIterator_PostOrder<DataType> &iter): 
	tree(iter.tree), stack(iter.stack), vstack(iter.vstack)
{
#ifdef MUTABLE_ITERATOR
	tree->iterList.insertOrderedUnique(this);
#endif
}

#ifdef MUTABLE_ITERATOR
template <class DataType>
BinaryTreeIterator_PostOrder<DataType>::BinaryTreeIterator_PostOrder(
	BinaryTree<DataType> &bt): 
	tree(&bt), stack(), vstack()
{
	tree->iterList.insertOrderedUnique(this);
	reset();
}
#else
template <class DataType>
BinaryTreeIterator_PostOrder<DataType>::BinaryTreeIterator_PostOrder(
	const BinaryTree<DataType> &bt): 
	tree(&bt), stack(), vstack()
{
	reset();
}
#endif

template <class DataType>
BinaryTreeIterator_PostOrder<DataType>::~BinaryTreeIterator_PostOrder()
{
#ifdef MUTABLE_ITERATOR
	AbstractIterator<DataType> *ptr = this;
	(void)tree->iterList.removeOrderedUnique(ptr);
#endif
}

// reset iterator to beginning
template <class DataType>
void
BinaryTreeIterator_PostOrder<DataType>::reset()
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
	BinaryTreeNode<DataType> *btn = NULL;
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

// is list empty
template <class DataType>
int
BinaryTreeIterator_PostOrder<DataType>::done() const
{
	return(stack.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTreeIterator_PostOrder<DataType>::operator()()
{
	BinaryTreeNode<DataType> *btn;
	MustBeTrue(stack.top(btn) == OK);
	return(btn->data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTreeIterator_PostOrder<DataType>::operator++(int)
{
	// is iterator done
	if (stack.isEmpty()) return(NOTOK);

	// get next node from stack
	int visits;
	BinaryTreeNode<DataType> *btn;
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
BinaryTreeIterator_LevelOrder<DataType>::BinaryTreeIterator_LevelOrder(
	const BinaryTreeIterator_LevelOrder<DataType> &iter): 
	tree(iter.tree), queue(iter.queue)
{
#ifdef MUTABLE_ITERATOR
	tree->iterList.insertOrderedUnique(this);
#endif
}

#ifdef MUTABLE_ITERATOR
template <class DataType>
BinaryTreeIterator_LevelOrder<DataType>::BinaryTreeIterator_LevelOrder(
	BinaryTree<DataType> &bt): 
	tree(&bt), queue()
{
	tree->iterList.insertOrderedUnique(this);
	reset();
}
#else
template <class DataType>
BinaryTreeIterator_LevelOrder<DataType>::BinaryTreeIterator_LevelOrder(
	const BinaryTree<DataType> &bt): 
	tree(&bt), queue()
{
	reset();
}
#endif

template <class DataType>
BinaryTreeIterator_LevelOrder<DataType>::~BinaryTreeIterator_LevelOrder()
{
#ifdef MUTABLE_ITERATOR
	AbstractIterator<DataType> *ptr = this;
	(void)tree->iterList.removeOrderedUnique(ptr);
#endif
}

// reset iterator to beginning
template <class DataType>
void
BinaryTreeIterator_LevelOrder<DataType>::reset()
{
	queue.clear();
	queue.enqueue(tree->root);
	return;
}

// is list empty
template <class DataType>
int
BinaryTreeIterator_LevelOrder<DataType>::done() const
{
	return(queue.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTreeIterator_LevelOrder<DataType>::operator()()
{
	BinaryTreeNode<DataType> *btn;
	MustBeTrue(queue.front(btn) == OK);
	return(btn->data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTreeIterator_LevelOrder<DataType>::operator++(int)
{
	// is iterator done
	if (queue.isEmpty()) return(NOTOK);

	// get next node from queue
	BinaryTreeNode<DataType> *btn;
	MustBeTrue(queue.dequeue(btn) == OK);

	// queue up left and right nodes, if any.
	if (btn->left != NULL)
		queue.enqueue(btn->left);
	if (btn->right != NULL)
		queue.enqueue(btn->right);
	return(OK);
}


