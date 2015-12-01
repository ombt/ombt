//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// member functions for binary tree class

// binary tree node constructors and destructor
template <class DataType>
BinaryTreeNode_NR<DataType>::BinaryTreeNode_NR(const DataType &d):
	data(d), left(NULL), right(NULL)
{
	// do nothing
}

template <class DataType>
BinaryTreeNode_NR<DataType>::BinaryTreeNode_NR(const BinaryTreeNode_NR<DataType> &btn):
	data(btn.data), left(NULL), right(NULL)
{
	// do nothing
}

template <class DataType>
BinaryTreeNode_NR<DataType>::~BinaryTreeNode_NR()
{
	left = NULL;
	right = NULL;
}

// assignment
template <class DataType>
BinaryTreeNode_NR<DataType> &
BinaryTreeNode_NR<DataType>::operator=(const BinaryTreeNode_NR<DataType> &btn)
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
template <class DataType>
BinaryTree_NR<DataType>::BinaryTree_NR():
	root(NULL)
{
	// do nothing
}

template <class DataType>
BinaryTree_NR<DataType>::BinaryTree_NR(const BinaryTree_NR<DataType> &bt):
	root(NULL)
{
	root = bt.copy();
}

template <class DataType>
BinaryTree_NR<DataType>::~BinaryTree_NR()
{
	clear();
}

// assignment
template <class DataType>
BinaryTree_NR<DataType> &
BinaryTree_NR<DataType>::operator=(const BinaryTree_NR<DataType> &bt)
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
int
BinaryTree_NR<DataType>::insert(const DataType &data)
{
	// find where new data goes
	BinaryTreeNode_NR<DataType> *pn = root;
	BinaryTreeNode_NR<DataType> **ppn = &root;
	while (pn != NULL)
	{
		if (data < pn->data)
		{
			// less than current data
			ppn = &pn->left;
			pn = pn->left;
		}
		else if (data > pn->data)
		{
			// greater than current data
			ppn = &pn->right;
			pn = pn->right;
		}
		else
		{
			// found it. overwrite existing data
			pn->data = data;
			return(OK);
		}
	}

	// not found. add a new node.
	*ppn = new BinaryTreeNode_NR<DataType>(data);
	if (ppn == NULL) return(NOTOK);

	// all done
	return(OK);
}

template <class DataType>
int
BinaryTree_NR<DataType>::update(const DataType &data)
{
	if (includes(data))
		return(insert(data));
	else
		return(NOMATCH);
}

template <class DataType>
int
BinaryTree_NR<DataType>::remove(DataType &data)
{
	// find where data is stored.
	DataType tmpdata = data;
	BinaryTreeNode_NR<DataType> *pn = root;
	BinaryTreeNode_NR<DataType> **ppn = &root;
	while (pn != NULL)
	{
		if (tmpdata < pn->data)
		{
			// less than current data
			ppn = &pn->left;
			pn = pn->left;
		}
		else if (tmpdata > pn->data)
		{
			// greater than current data
			ppn = &pn->right;
			pn = pn->right;
		}
		else
		{
			// found it. save data.
			data = pn->data;

			// now delete it
			if (pn->left == NULL && pn->right == NULL)
			{
				// leaf node
				*ppn = NULL;
				delete pn;
				return(OK);
			}
			else if (pn->left != NULL && pn->right == NULL)
			{
				// left tree is NOT null
				*ppn = pn->left;
				delete pn;
				return(OK);
			}
			else if (pn->left == NULL && pn->right != NULL)
			{
				// right tree is NOT null
				*ppn = pn->right;
				delete pn;
				return(OK);
			}
			else
			{
				// ugh. both branches are NOT null. 
				// find node immediately before this one.
				BinaryTreeNode_NR<DataType> *ppred = pn->left;
				for ( ; ppred->right != NULL; 
				        ppred = ppred->right) ;

				// copy data and repeat delete process.
				pn->data = ppred->data;
				tmpdata = ppred->data;
				ppn = &pn->left;
				pn = pn->left;
			}
		}
	}

	// not found.
	return(NOMATCH);
}

template <class DataType>
int
BinaryTree_NR<DataType>::retrieve(DataType &data) const
{
	// find data, if any.
	BinaryTreeNode_NR<DataType> *pn = root;
	while (pn != NULL)
	{
		if (data < pn->data)
		{
			// less than current data
			pn = pn->left;
		}
		else if (data > pn->data)
		{
			// greater than current data
			pn = pn->right;
		}
		else
		{
			// found it.
			data = pn->data;
			return(OK);
		}
	}

	// not found.
	return(NOMATCH);
}

template <class DataType>
int
BinaryTree_NR<DataType>::includes(const DataType &data) const
{
	// find data, if any.
	BinaryTreeNode_NR<DataType> *pn = root;
	while (pn != NULL)
	{
		if (data < pn->data)
		{
			// less than current data
			pn = pn->left;
		}
		else if (data > pn->data)
		{
			// greater than current data
			pn = pn->right;
		}
		else
		{
			// found it.
			return(1);
		}
	}

	// not found.
	return(0);
}

template <class DataType>
int
BinaryTree_NR<DataType>::isEmpty() const
{
	return(root == NULL);
}

template <class DataType>
void
BinaryTree_NR<DataType>::clear()
{
	// check if tree is empty
	if (root == NULL) return;

	// define queue for level-order traversal
	Queue_List<BinaryTreeNode_NR<DataType> * > pq;

	// insert root into queue to start traversal
	pq.enqueue(root);

	// loop over nodes in queue
	while ( ! pq.isEmpty())
	{
		// get next node from queue
		BinaryTreeNode_NR<DataType> *btn;
		MustBeTrue(pq.dequeue(btn) == OK);

		// queue up left and right nodes, if any.
		if (btn->left != NULL)
		{
			pq.enqueue(btn->left);
		}
		if (btn->right != NULL)
		{
			pq.enqueue(btn->right);
		}

		// delete node
		delete btn;
	}

	// clear root and delete queue
	root = NULL;
	return;
}

// non-recursive tree traversal functions
template <class DataType>
int
BinaryTree_NR<DataType>::preOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == NULL)
		return(OK);
	MustBeTrue(work != NULL);

	// define stack for pre-order traversal
	Stack_List<BinaryTreeNode_NR<DataType> * > ps;

	// insert root into stack to start traversal
	ps.push(root);

	// loop over nodes in stack
	while ( ! ps.isEmpty())
	{
		// get next node from stack
		BinaryTreeNode_NR<DataType> *btn;
		MustBeTrue(ps.pop(btn) == OK);

		// execute work function for data
		int status = work(btn->data);
		if (status != OK)
		{
			return(status);
		}

		// stack left and right nodes, if any.
		if (btn->right != NULL)
			ps.push(btn->right);
		if (btn->left != NULL)
			ps.push(btn->left);
	}

	// all done
	return(OK);
}

template <class DataType>
int
BinaryTree_NR<DataType>::inOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == NULL)
		return(OK);
	MustBeTrue(work != NULL);

	// define stack for in-order traversal
	Stack_List<BinaryTreeNode_NR<DataType> * > ps;

	// insert root and left links into stack to start traversal
	for (BinaryTreeNode_NR<DataType> *pn = root; pn != NULL; pn = pn->left)
	{
		ps.push(pn);
	}

	// loop over nodes in stack
	while ( ! ps.isEmpty())
	{
		// get next node from stack
		BinaryTreeNode_NR<DataType> *btn;
		MustBeTrue(ps.pop(btn) == OK);

		// execute work function for data
		int status = work(btn->data);
		if (status != OK)
			return(status);

		// insert right node left links into stack
		if (btn->right != NULL)
		{
			for (BinaryTreeNode_NR<DataType> *pn = btn->right; 
			     pn != NULL; pn = pn->left)
			{
				ps.push(pn);
			}
		}
	}

	// all done
	return(OK);
}

template <class DataType>
int
BinaryTree_NR<DataType>::postOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == NULL)
		return(OK);
	MustBeTrue(work != NULL);

	// define stack for post-order traversal
	int visits;
	Stack_List<int> pvs;
	Stack_List<BinaryTreeNode_NR<DataType> * > ps;

	// insert root 
	visits = 1;
	ps.push(root);
	pvs.push(visits);

	// process until the first visit
	BinaryTreeNode_NR<DataType> *btn = NULL;
	while ( ! ps.isEmpty())
	{
		int status;

		// pop current node
		MustBeTrue(ps.pop(btn) == OK);
		MustBeTrue(pvs.pop(visits) == OK);

		// process
		switch (visits)
		{
		case 1:
			visits = 2;
			ps.push(btn);
			pvs.push(visits);
			if (btn->left != NULL)
			{
				visits = 1;
				ps.push(btn->left);
				pvs.push(visits);
			}
			break;

		case 2:
			visits = 3;
			ps.push(btn);
			pvs.push(visits);
			if (btn->right != NULL)
			{
				visits = 1;
				ps.push(btn->right);
				pvs.push(visits);
			}
			break;

		case 3:
			// execute work function for data
			status = work(btn->data);
			if (status != OK)
			{
				return(status);
			}
			break;

		default:
			MustBeTrue(visits <= 3);
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

template <class DataType>
int
BinaryTree_NR<DataType>::levelOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == NULL)
		return(OK);
	MustBeTrue(work != NULL);

	// define queue for level-order traversal
	Queue_List<BinaryTreeNode_NR<DataType> * > pq;

	// insert root into queue to start traversal
	pq.enqueue(root);

	// loop over nodes in queue
	while ( ! pq.isEmpty())
	{
		// get next node from queue
		BinaryTreeNode_NR<DataType> *btn;
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
std::ostream &
BinaryTree_NR<DataType>::dump(std::ostream &os) const
{
	// check if tree is empty
	if (root == NULL)
		return(os);

	// define stack for in-order traversal
	Stack_List<BinaryTreeNode_NR<DataType> * > ps;

	// insert root and left links into stack to start traversal
	for (BinaryTreeNode_NR<DataType> *pn = root; pn != NULL; pn = pn->left)
	{
		ps.push(pn);
	}

	// loop over nodes in stack
	while ( ! ps.isEmpty())
	{
		// get next node from stack
		BinaryTreeNode_NR<DataType> *btn;
		MustBeTrue(ps.pop(btn) == OK);

		// print data
		os << btn->data << " ";

		// insert right node left links into stack
		if (btn->right != NULL)
		{
			for (BinaryTreeNode_NR<DataType> *pn = btn->right; 
			     pn != NULL; pn = pn->left)
			{
				ps.push(pn);
			}
		}
	}

	// all done
	return(os);
}

// utility functions
template <class DataType>
BinaryTreeNode_NR<DataType> *
BinaryTree_NR<DataType>::copy() const
{
	BinaryTreeNode_NR<DataType> *pnewroot = NULL;

	// check if tree is empty
	if (root == NULL)
		return(pnewroot);

	// define stack for copying nodes
	Stack_List<BinaryTreeNode_NR<DataType> * > ps1;
	Stack_List<BinaryTreeNode_NR<DataType> ** > ps2;

	// insert root and new root into stack to start traversal
	ps1.push(root);
	ps2.push(&pnewroot);

	// loop over nodes in stack
	while ( ! ps1.isEmpty())
	{
		// get next node from stack
		BinaryTreeNode_NR<DataType> *pbtn1;
		BinaryTreeNode_NR<DataType> **pbtn2;
		MustBeTrue(ps1.pop(pbtn1) == OK);
		MustBeTrue(ps2.pop(pbtn2) == OK);
	
		// copy node
		*pbtn2 = new BinaryTreeNode_NR<DataType>(pbtn1->data);
		MustBeTrue(*pbtn2 != NULL);

		// stack left and right nodes, if any.
		if (pbtn1->left != NULL)
		{
			ps1.push(pbtn1->left);
			ps2.push(&(*pbtn2)->left);
		}
		if (pbtn1->right != NULL)
		{
			ps1.push(pbtn1->right);
			ps2.push(&(*pbtn2)->right);
		}
	}

	// all done, return new root
	return(pnewroot);
}

// preorder iterator constructors and destructor
template <class DataType>
BinaryTree_NR_Iterator_PreOrder<DataType>::BinaryTree_NR_Iterator_PreOrder(
	const BinaryTree_NR_Iterator_PreOrder<DataType> &iter): 
	tree(iter.tree), stack(iter.stack)
{
	// do nothing
}

template <class DataType>
BinaryTree_NR_Iterator_PreOrder<DataType>::BinaryTree_NR_Iterator_PreOrder(
	const BinaryTree_NR<DataType> &bt): 
	tree(&bt), stack()
{
	reset();
}

template <class DataType>
BinaryTree_NR_Iterator_PreOrder<DataType>::~BinaryTree_NR_Iterator_PreOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_NR_Iterator_PreOrder<DataType>::reset()
{
	stack.clear();
	stack.push(tree->root);
	return;
}

// is list empty
template <class DataType>
int
BinaryTree_NR_Iterator_PreOrder<DataType>::done() const
{
	return(stack.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTree_NR_Iterator_PreOrder<DataType>::operator()()
{
	BinaryTreeNode_NR<DataType> *btn;
	MustBeTrue(stack.top(btn) == OK);
	return(btn->data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_NR_Iterator_PreOrder<DataType>::operator++(int)
{
	// is iterator done
	if (stack.isEmpty()) return(NOTOK);

	// get next node from stack
	BinaryTreeNode_NR<DataType> *btn;
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
BinaryTree_NR_Iterator_InOrder<DataType>::BinaryTree_NR_Iterator_InOrder(
	const BinaryTree_NR_Iterator_InOrder<DataType> &iter): 
	BinaryTree_NR_Iterator_PreOrder<DataType>(iter)
{
	// do nothing
}

template <class DataType>
BinaryTree_NR_Iterator_InOrder<DataType>::BinaryTree_NR_Iterator_InOrder(
	const BinaryTree_NR<DataType> &bt): 
	BinaryTree_NR_Iterator_PreOrder<DataType>(bt)
{
	reset();
}

template <class DataType>
BinaryTree_NR_Iterator_InOrder<DataType>::~BinaryTree_NR_Iterator_InOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_NR_Iterator_InOrder<DataType>::reset()
{
	this->stack.clear();
	for (BinaryTreeNode_NR<DataType> *pn = this->tree->root; 
		pn != NULL; pn = pn->left)
	{
		this->stack.push(pn);
	}
	return;
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_NR_Iterator_InOrder<DataType>::operator++(int)
{
	// is iterator done
	if (this->stack.isEmpty()) return(NOTOK);

	// get next node from stack
	BinaryTreeNode_NR<DataType> *btn;
	MustBeTrue(this->stack.pop(btn) == OK);

	// insert right node left links into stack
	if (btn->right != NULL)
	{
		for (BinaryTreeNode_NR<DataType> *pn = btn->right;
			pn != NULL; pn = pn->left)
		{
			this->stack.push(pn);
		}
	}
	return(OK);
}

// postorder iterator constructors and destructor
template <class DataType>
BinaryTree_NR_Iterator_PostOrder<DataType>::BinaryTree_NR_Iterator_PostOrder(
	const BinaryTree_NR_Iterator_PostOrder<DataType> &iter): 
	BinaryTree_NR_Iterator_PreOrder<DataType>(iter), vstack(iter.vstack)
{
	// do nothing
}

template <class DataType>
BinaryTree_NR_Iterator_PostOrder<DataType>::BinaryTree_NR_Iterator_PostOrder(
	const BinaryTree_NR<DataType> &bt): 
	BinaryTree_NR_Iterator_PreOrder<DataType>(bt), vstack()
{
	reset();
}

template <class DataType>
BinaryTree_NR_Iterator_PostOrder<DataType>::~BinaryTree_NR_Iterator_PostOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_NR_Iterator_PostOrder<DataType>::reset()
{
	int visits;

	// clear stack
	this->stack.clear();
	this->vstack.clear();

	// check if tree is empty
	if (this->tree->isEmpty()) return;

	// push root onto stack
	visits = 1;
	this->stack.push(this->tree->root);
	this->vstack.push(visits);

	// process until the first visit
	BinaryTreeNode_NR<DataType> *btn = NULL;
	while ( ! this->stack.isEmpty())
	{
		// pop current node
		MustBeTrue(this->stack.pop(btn) == OK);
		MustBeTrue(this->vstack.pop(visits) == OK);

		// process
		switch (visits)
		{
		case 1:
			visits = 2;
			this->stack.push(btn);
			this->vstack.push(visits);
			if (btn->left != NULL)
			{
				visits = 1;
				this->stack.push(btn->left);
				this->vstack.push(visits);
			}
			break;

		case 2:
			visits = 3;
			this->stack.push(btn);
			this->vstack.push(visits);
			if (btn->right != NULL)
			{
				visits = 1;
				this->stack.push(btn->right);
				this->vstack.push(visits);
			}
			break;

		case 3:
			visits = 4;
			this->stack.push(btn);
			this->vstack.push(visits);
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
BinaryTree_NR_Iterator_PostOrder<DataType>::operator++(int)
{
	// is iterator done
	if (this->stack.isEmpty()) return(NOTOK);

	// get next node from stack
	int visits;
	BinaryTreeNode_NR<DataType> *btn;
	MustBeTrue(this->stack.pop(btn) == OK);
	MustBeTrue(this->vstack.pop(visits) == OK);
	MustBeTrue(visits > 3);

	// find next node to process
	while ( ! this->stack.isEmpty())
	{
		// pop current node
		MustBeTrue(this->stack.pop(btn) == OK);
		MustBeTrue(this->vstack.pop(visits) == OK);

		// process
		switch (visits)
		{
		case 1:
			visits = 2;
			this->stack.push(btn);
			this->vstack.push(visits);
			if (btn->left != NULL)
			{
				visits = 1;
				this->stack.push(btn->left);
				this->vstack.push(visits);
			}
			break;

		case 2:
			visits = 3;
			this->stack.push(btn);
			this->vstack.push(visits);
			if (btn->right != NULL)
			{
				visits = 1;
				this->stack.push(btn->right);
				this->vstack.push(visits);
			}
			break;

		case 3:
			visits = 4;
			this->stack.push(btn);
			this->vstack.push(visits);
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
BinaryTree_NR_Iterator_LevelOrder<DataType>::BinaryTree_NR_Iterator_LevelOrder(
	const BinaryTree_NR_Iterator_LevelOrder<DataType> &iter): 
	tree(iter.tree), queue(iter.queue)
{
	// do nothing
}

template <class DataType>
BinaryTree_NR_Iterator_LevelOrder<DataType>::BinaryTree_NR_Iterator_LevelOrder(
	const BinaryTree_NR<DataType> &bt): 
	tree(&bt), queue()
{
	reset();
}

template <class DataType>
BinaryTree_NR_Iterator_LevelOrder<DataType>::~BinaryTree_NR_Iterator_LevelOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_NR_Iterator_LevelOrder<DataType>::reset()
{
	queue.clear();
	queue.enqueue(tree->root);
	return;
}

// is list empty
template <class DataType>
int
BinaryTree_NR_Iterator_LevelOrder<DataType>::done() const
{
	return(queue.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTree_NR_Iterator_LevelOrder<DataType>::operator()()
{
	BinaryTreeNode_NR<DataType> *btn;
	MustBeTrue(queue.front(btn) == OK);
	return(btn->data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_NR_Iterator_LevelOrder<DataType>::operator++(int)
{
	// is iterator done
	if (queue.isEmpty()) return(NOTOK);

	// get next node from queue
	BinaryTreeNode_NR<DataType> *btn;
	MustBeTrue(queue.dequeue(btn) == OK);

	// queue up left and right nodes, if any.
	if (btn->left != NULL)
		queue.enqueue(btn->left);
	if (btn->right != NULL)
		queue.enqueue(btn->right);
	return(OK);
}

