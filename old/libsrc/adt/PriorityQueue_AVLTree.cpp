// member functions for priority queue class

// constructors and destructor
template <class DataType>
PriorityQueue_AVLTree<DataType>::PriorityQueue_AVLTree(): 
	tree()
{
        // do nothing
}

template <class DataType>
PriorityQueue_AVLTree<DataType>::PriorityQueue_AVLTree(
	const PriorityQueue_AVLTree<DataType> &pq): 
		tree(pq.tree)
{
        // do nothing
}

template <class DataType>
PriorityQueue_AVLTree<DataType>::~PriorityQueue_AVLTree()
{
        // do nothing
}

// assignment
template <class DataType>
PriorityQueue_AVLTree<DataType> &
PriorityQueue_AVLTree<DataType>::operator=(const PriorityQueue_AVLTree<DataType> &pq)
{
        if (this != &pq)
                tree = pq.tree;
        return(*this);
}

// queue operations
template <class DataType>
void 
PriorityQueue_AVLTree<DataType>::clear()
{
        tree.clear();
        return;
}

template <class DataType>
int 
PriorityQueue_AVLTree<DataType>::enqueue(const DataType &data)
{
        return(tree.insert(data));
}

template <class DataType>
int 
PriorityQueue_AVLTree<DataType>::dequeue(DataType &data)
{
	BinaryTree_AVL_Iterator_InOrder<DataType> treeIter(tree);
	if (treeIter.done())
		return(NOTOK);
	else
	{
		data = treeIter();
        	return(tree.remove(data));
	}
}

template <class DataType>
int 
PriorityQueue_AVLTree<DataType>::front(DataType &data) const
{
	BinaryTree_AVL_Iterator_InOrder<DataType> treeIter(tree);
	if (treeIter.done())
		return(NOTOK);
	else
	{
		data = treeIter();
        	return(OK);
	}
}

template <class DataType>
int 
PriorityQueue_AVLTree<DataType>::isEmpty() const
{
        return(tree.isEmpty());
}

template <class DataType>
int 
PriorityQueue_AVLTree<DataType>::includes(const DataType &data) const
{
	return(tree.includes(data));
}


// print data
template <class DataType>
std::ostream &
PriorityQueue_AVLTree<DataType>::dump(std::ostream &os) const
{
	os << tree;
        return(os);
}

