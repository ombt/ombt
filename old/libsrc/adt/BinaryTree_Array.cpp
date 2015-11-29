// member functions for binary tree class

// binary tree node constructors and destructor
template <class DataType>
BinaryTreeNode_Array<DataType>::BinaryTreeNode_Array():
	data(), left(-1), right(-1)
{
	// do nothing
}

template <class DataType>
BinaryTreeNode_Array<DataType>::BinaryTreeNode_Array(const DataType &d):
	data(d), left(-1), right(-1)
{
	// do nothing
}

template <class DataType>
BinaryTreeNode_Array<DataType>::BinaryTreeNode_Array(const BinaryTreeNode_Array<DataType> &btn):
	data(btn.data), left(-1), right(-1)
{
	// do nothing
}

template <class DataType>
BinaryTreeNode_Array<DataType>::~BinaryTreeNode_Array()
{
	left = -1;
	right = -1;
}

// assignment
template <class DataType>
BinaryTreeNode_Array<DataType> &
BinaryTreeNode_Array<DataType>::operator=(const BinaryTreeNode_Array<DataType> &btn)
{
	if (this != &btn)
	{
		left = -1;
		right = -1;
		data = btn.data;
	}
	return(*this);
}

// binary tree constructors and destructor
template <class DataType>
BinaryTree_Array<DataType>::BinaryTree_Array(int asz):
	arraySize(asz), root(-1), available(0),
	array(new BinaryTreeNode_Array<DataType> [asz])
{
	// sanity check
	MustBeTrue(asz > 0 && array != NULL);

	// initialize available list
	for (int ia = 0; ia < asz; ia++)
	{
		array[ia].left = -1;
		array[ia].right = ia + 1;
	}
	array[asz-1].right = -1;
}

template <class DataType>
BinaryTree_Array<DataType>::BinaryTree_Array(
		const BinaryTree_Array<DataType> &bt):
	arraySize(bt.arraySize), root(bt.root), available(bt.available),
	array(new BinaryTreeNode_Array<DataType> [bt.arraySize])
{
	// sanity check
	MustBeTrue(arraySize> 0 && array != NULL);

	// copy data
	for (int ia = 0; ia < bt.arraySize; ia++)
	{
		array[ia].left = bt.array[ia].left;
		array[ia].right = bt.array[ia].right;
		array[ia].data = bt.array[ia].data;
	}
}

template <class DataType>
BinaryTree_Array<DataType>::~BinaryTree_Array()
{
	delete [] array;
	array = NULL;
	arraySize = 0;
	root = -1;
	available = -1;
}

// assignment
template <class DataType>
BinaryTree_Array<DataType> &
BinaryTree_Array<DataType>::operator=(const BinaryTree_Array<DataType> &bt)
{
	if (this != &bt)
	{
		// delete old array
		delete [] array;

		// allocate new array
		arraySize = bt.arraySize;
		MustBeTrue(arraySize > 0);
		array = new BinaryTreeNode_Array<DataType> [arraySize];
		MustBeTrue(array != NULL);

		// copy all data
		root = bt.root;
		available = bt.available;
		for (int ia = 0; ia < bt.arraySize; ia++)
		{
			array[ia].left = bt.array[ia].left;
			array[ia].right = bt.array[ia].right;
			array[ia].data = bt.array[ia].data;
		}
	}
	return(*this);
}

// binary tree operations
template <class DataType>
int
BinaryTree_Array<DataType>::insert(const DataType &data)
{
	// find where new data goes
	int n = root;
	int *pn = &root;
	while (n != -1)
	{
		if (data < array[n].data)
		{
			// less than current data
			pn = &(array[n].left);
			n = array[n].left;
		}
		else if (data > array[n].data)
		{
			// greater than current data
			pn = &(array[n].right);
			n = array[n].right;
		}
		else
		{
			// found it. overwrite existing data
			array[n].data = data;
			return(OK);
		}
	}

	// not found. add a new node.
	*pn = newNode(data);
	if (*pn == -1) 
		return(NOTOK);
	else
		return(OK);
}

template <class DataType>
int
BinaryTree_Array<DataType>::update(const DataType &data)
{
	if (includes(data))
		return(insert(data));
	else
		return(NOMATCH);
}

template <class DataType>
int
BinaryTree_Array<DataType>::remove(DataType &data)
{
	// find where data is stored.
	DataType tmpdata = data;
	int n = root;
	int *pn = &root;
	while (n != -1)
	{
		if (tmpdata < array[n].data)
		{
			// less than current data
			pn = &(array[n].left);
			n = array[n].left;
		}
		else if (tmpdata > array[n].data)
		{
			// greater than current data
			pn = &(array[n].right);
			n = array[n].right;
		}
		else
		{
			// found it. save data.
			data = array[n].data;

			// now delete it
			if (array[n].left == -1 && array[n].right == -1)
			{
				// leaf node
				*pn = -1;
				deleteNode(n);
				return(OK);
			}
			else if (array[n].left != -1 && array[n].right == -1)
			{
				// left tree is NOT null
				*pn = array[n].left;
				deleteNode(n);
				return(OK);
			}
			else if (array[n].left == -1 && array[n].right != -1)
			{
				// right tree is NOT null
				*pn = array[n].right;
				deleteNode(n);
				return(OK);
			}
			else
			{
				// ugh. both branches are NOT null. 
				// find node immediately before this one.
				int pred = array[n].left;
				for ( ; array[pred].right != -1; 
				        pred = array[pred].right) ;

				// copy data and repeat delete process.
				array[n].data = array[pred].data;
				tmpdata = array[pred].data;
				pn = &(array[n].left);
				n = array[n].left;
			}
		}
	}

	// not found.
	return(NOMATCH);
}

template <class DataType>
int
BinaryTree_Array<DataType>::retrieve(DataType &data) const
{
	// find data, if any.
	int n = root;
	while (n != -1)
	{
		if (data < array[n].data)
		{
			// less than current data
			n = array[n].left;
		}
		else if (data > array[n].data)
		{
			// greater than current data
			n = array[n].right;
		}
		else
		{
			// found it.
			data = array[n].data;
			return(OK);
		}
	}

	// not found.
	return(NOMATCH);
}

template <class DataType>
int
BinaryTree_Array<DataType>::includes(const DataType &data) const
{
	// find data, if any.
	int n = root;
	while (n != -1)
	{
		if (data < array[n].data)
		{
			// less than current data
			n = array[n].left;
		}
		else if (data > array[n].data)
		{
			// greater than current data
			n = array[n].right;
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
void
BinaryTree_Array<DataType>::clear()
{
	// move all nodes to available list
	root = -1;
	available = 0;

	// initialize available list
	for (int ia = 0; ia < arraySize; ia++)
	{
		array[ia].left = -1;
		array[ia].right = ia + 1;
	}
	array[arraySize-1].right = -1;
	return;
}

template <class DataType>
int
BinaryTree_Array<DataType>::isEmpty() const
{
	return(root == -1);
}

template <class DataType>
int
BinaryTree_Array<DataType>::isFull() const
{
	return(available == -1);
}

// non-recursive tree traversal functions
template <class DataType>
int
BinaryTree_Array<DataType>::preOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == -1)
		return(OK);
	MustBeTrue(work != NULL);

	// define stack for pre-order traversal
	Stack_List<int> ps;

	// insert root into stack to start traversal
	ps.push(root);

	// loop over nodes in stack
	while ( ! ps.isEmpty())
	{
		// get next node from stack
		int btn;
		MustBeTrue(ps.pop(btn) == OK);

		// execute work function for data
		int status = work(array[btn].data);
		if (status != OK)
			return(status);

		// stack left and right nodes, if any.
		if (array[btn].right != -1)
			ps.push(array[btn].right);
		if (array[btn].left != -1)
			ps.push(array[btn].left);
	}

	// all done
	return(OK);
}

template <class DataType>
int
BinaryTree_Array<DataType>::inOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == -1)
		return(OK);
	MustBeTrue(work != NULL);

	// define stack for in-order traversal
	Stack_List<int> ps;

	// insert root and left links into stack to start traversal
	for (int pn = root; pn != -1; pn = array[pn].left)
	{
		ps.push(pn);
	}

	// loop over nodes in stack
	while ( ! ps.isEmpty())
	{
		// get next node from stack
		int btn;
		MustBeTrue(ps.pop(btn) == OK);

		// execute work function for data
		int status = work(array[btn].data);
		if (status != OK)
			return(status);

		// insert right node left links into stack
		if (array[btn].right != -1)
		{
			for (int pn = array[btn].right; 
			     pn != -1; pn = array[pn].left)
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
BinaryTree_Array<DataType>::postOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == -1)
		return(OK);
	MustBeTrue(work != NULL);

	// define stack for post-order traversal
	int visits;
	Stack_List<int> pvs;
	Stack_List<int> ps;

	// insert root 
	visits = 1;
	ps.push(root);
	pvs.push(visits);

	// process until the first visit
	int btn = -1;
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
			if (array[btn].left != -1)
			{
				visits = 1;
				ps.push(array[btn].left);
				pvs.push(visits);
			}
			break;

		case 2:
			visits = 3;
			ps.push(btn);
			pvs.push(visits);
			if (array[btn].right != -1)
			{
				visits = 1;
				ps.push(array[btn].right);
				pvs.push(visits);
			}
			break;

		case 3:
			// execute work function for data
			status = work(array[btn].data);
			if (status != OK)
				return(status);
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
BinaryTree_Array<DataType>::levelOrder(int (*work)(DataType &))
{
	// check if tree is empty
	if (root == -1)
		return(OK);
	MustBeTrue(work != NULL);

	// define queue for level-order traversal
	Queue_List<int> pq;

	// insert root into queue to start traversal
	pq.enqueue(root);

	// loop over nodes in queue
	while ( ! pq.isEmpty())
	{
		// get next node from queue
		int btn;
		MustBeTrue(pq.dequeue(btn) == OK);

		// execute work function for data
		int status = work(array[btn].data);
		if (status != OK)
			return(status);

		// queue up left and right nodes, if any.
		if (array[btn].left != -1)
			pq.enqueue(array[btn].left);
		if (array[btn].right != -1)
			pq.enqueue(array[btn].right);
	}

	// all done
	return(OK);
}

// print binary tree data
template <class DataType>
std::ostream &
BinaryTree_Array<DataType>::dump(std::ostream &os) const
{
	// check if tree is empty
	if (root == -1)
		return(os);

	// define stack for in-order traversal
	Stack_List<int> ps;

	// insert root and left links into stack to start traversal
	for (int pn = root; pn != -1; pn = array[pn].left)
	{
		ps.push(pn);
	}

	// loop over nodes in stack
	while ( ! ps.isEmpty())
	{
		// get next node from stack
		int btn;
		MustBeTrue(ps.pop(btn) == OK);

		// print data
		os << array[btn].data << " ";

		// insert right node left links into stack
		if (array[btn].right != -1)
		{
			for (int pn = array[btn].right; 
			     pn != -1; pn = array[pn].left)
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
int
BinaryTree_Array<DataType>::newNode(const DataType &d)
{
	// check if available list is empty
	if (available == -1)
		return(-1);

	// store data in next available node
	int next = available;
	available = array[next].right;
	array[next].left = -1;
	array[next].right = -1;
	array[next].data = d;

	// all done
	return(next);
}

template <class DataType>
void
BinaryTree_Array<DataType>::deleteNode(int dn)
{
	// add deleted node to available list
	array[dn].right = available;
	available = dn;
	return;
}

// preorder iterator constructors and destructor
template <class DataType>
BinaryTree_Array_Iterator_PreOrder<DataType>::BinaryTree_Array_Iterator_PreOrder(
	const BinaryTree_Array_Iterator_PreOrder<DataType> &iter): 
	tree(iter.tree), stack(iter.stack)
{
	// do nothing
}

template <class DataType>
BinaryTree_Array_Iterator_PreOrder<DataType>::BinaryTree_Array_Iterator_PreOrder(
	const BinaryTree_Array<DataType> &bt): 
	tree(&bt), stack()
{
	reset();
}

template <class DataType>
BinaryTree_Array_Iterator_PreOrder<DataType>::~BinaryTree_Array_Iterator_PreOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_Array_Iterator_PreOrder<DataType>::reset()
{
	stack.clear();
	stack.push(tree->root);
	return;
}

// is list empty
template <class DataType>
int
BinaryTree_Array_Iterator_PreOrder<DataType>::done() const
{
	return(stack.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTree_Array_Iterator_PreOrder<DataType>::operator()()
{
	int btn;
	MustBeTrue(stack.top(btn) == OK);
	return(tree->array[btn].data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_Array_Iterator_PreOrder<DataType>::operator++(int)
{
	// is iterator done
	if (stack.isEmpty()) return(NOTOK);

	// get next node from stack
	int btn;
	MustBeTrue(stack.pop(btn) == OK);

	// stack left and right nodes, if any.
	if (tree->array[btn].right != -1)
		stack.push(tree->array[btn].right);
	if (tree->array[btn].left != -1)
		stack.push(tree->array[btn].left);
	return(OK);
}

// inorder iterator constructors and destructor
template <class DataType>
BinaryTree_Array_Iterator_InOrder<DataType>::BinaryTree_Array_Iterator_InOrder(
	const BinaryTree_Array_Iterator_InOrder<DataType> &iter): 
	BinaryTree_Array_Iterator_PreOrder<DataType>(iter)
{
	// do nothing
}

template <class DataType>
BinaryTree_Array_Iterator_InOrder<DataType>::BinaryTree_Array_Iterator_InOrder(
	const BinaryTree_Array<DataType> &bt): 
	BinaryTree_Array_Iterator_PreOrder<DataType>(bt)
{
	reset();
}

template <class DataType>
BinaryTree_Array_Iterator_InOrder<DataType>::~BinaryTree_Array_Iterator_InOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_Array_Iterator_InOrder<DataType>::reset()
{
	this->stack.clear();
	for (int pn = this->tree->root; pn != -1; pn = this->tree->array[pn].left)
	{
		this->stack.push(pn);
	}
	return;
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_Array_Iterator_InOrder<DataType>::operator++(int)
{
	// is iterator done
	if (this->stack.isEmpty()) return(NOTOK);

	// get next node from stack
	int btn;
	MustBeTrue(this->stack.pop(btn) == OK);

	// insert right node left links into stack
	if (this->tree->array[btn].right != -1)
	{
		for (int pn = this->tree->array[btn].right;
			pn != -1; pn = this->tree->array[pn].left)
		{
			this->stack.push(pn);
		}
	}
	return(OK);
}

// postorder iterator constructors and destructor
template <class DataType>
BinaryTree_Array_Iterator_PostOrder<DataType>::BinaryTree_Array_Iterator_PostOrder(
	const BinaryTree_Array_Iterator_PostOrder<DataType> &iter): 
	BinaryTree_Array_Iterator_PreOrder<DataType>(iter), vstack(iter.vstack)
{
	// do nothing
}

template <class DataType>
BinaryTree_Array_Iterator_PostOrder<DataType>::BinaryTree_Array_Iterator_PostOrder(
	const BinaryTree_Array<DataType> &bt): 
	BinaryTree_Array_Iterator_PreOrder<DataType>(bt), vstack()
{
	reset();
}

template <class DataType>
BinaryTree_Array_Iterator_PostOrder<DataType>::~BinaryTree_Array_Iterator_PostOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_Array_Iterator_PostOrder<DataType>::reset()
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
	int btn = -1;
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
			if (this->tree->array[btn].left != -1)
			{
				visits = 1;
				this->stack.push(this->tree->array[btn].left);
				this->vstack.push(visits);
			}
			break;

		case 2:
			visits = 3;
			this->stack.push(btn);
			this->vstack.push(visits);
			if (this->tree->array[btn].right != -1)
			{
				visits = 1;
				this->stack.push(this->tree->array[btn].right);
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
BinaryTree_Array_Iterator_PostOrder<DataType>::operator++(int)
{
	// is iterator done
	if (this->stack.isEmpty()) return(NOTOK);

	// get next node from stack
	int visits;
	int btn;
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
			if (this->tree->array[btn].left != -1)
			{
				visits = 1;
				this->stack.push(this->tree->array[btn].left);
				this->vstack.push(visits);
			}
			break;

		case 2:
			visits = 3;
			this->stack.push(btn);
			this->vstack.push(visits);
			if (this->tree->array[btn].right != -1)
			{
				visits = 1;
				this->stack.push(this->tree->array[btn].right);
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
BinaryTree_Array_Iterator_LevelOrder<DataType>::BinaryTree_Array_Iterator_LevelOrder(
	const BinaryTree_Array_Iterator_LevelOrder<DataType> &iter): 
	tree(iter.tree), queue(iter.queue)
{
	// do nothing
}

template <class DataType>
BinaryTree_Array_Iterator_LevelOrder<DataType>::BinaryTree_Array_Iterator_LevelOrder(
	const BinaryTree_Array<DataType> &bt): 
	tree(&bt), queue()
{
	reset();
}

template <class DataType>
BinaryTree_Array_Iterator_LevelOrder<DataType>::~BinaryTree_Array_Iterator_LevelOrder()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
BinaryTree_Array_Iterator_LevelOrder<DataType>::reset()
{
	queue.clear();
	queue.enqueue(tree->root);
	return;
}

// is list empty
template <class DataType>
int
BinaryTree_Array_Iterator_LevelOrder<DataType>::done() const
{
	return(queue.isEmpty());
}

// return current data
template <class DataType>
DataType
BinaryTree_Array_Iterator_LevelOrder<DataType>::operator()()
{
	int btn;
	MustBeTrue(queue.front(btn) == OK);
	return(tree->array[btn].data);
}

// increment to next data item in list
template <class DataType>
int
BinaryTree_Array_Iterator_LevelOrder<DataType>::operator++(int)
{
	// is iterator done
	if (queue.isEmpty()) return(NOTOK);

	// get next node from queue
	int btn;
	MustBeTrue(queue.dequeue(btn) == OK);

	// queue up left and right nodes, if any.
	if (tree->array[btn].left != -1)
		queue.enqueue(tree->array[btn].left);
	if (tree->array[btn].right != -1)
		queue.enqueue(tree->array[btn].right);
	return(OK);
}

