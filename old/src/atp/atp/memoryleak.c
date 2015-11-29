// member functions for memory leak classes

// headers
#include "memoryleak.h"

// memory leak item class constuctors and destructor
MemoryLeakItem::MemoryLeakItem(unsigned long addr, int ucnt):
	usecounts(ucnt), address(addr), previous(NULL), next(NULL)
{
	// nothing to do
}

MemoryLeakItem::~MemoryLeakItem()
{
	usecounts = 0;
	address = NULL;
	previous = NULL;
	next = NULL;
}

// output operators
ostream &
operator<<(ostream &os, const MemoryLeakItem &mli)
{
	os << "(addr,ucnt) = (" << (unsigned long)mli.address << ",";
	os << mli.usecounts << ")" << endl;
	return(os);
}

// memory leak class constructors and destructor
MemoryLeak::MemoryLeak(int sz, int dsz):
	size(sz), datasize(dsz), maximumtotalcounts(0), currenttotalcounts(0), 
	first(NULL), last(NULL), counts(NULL)
{
	MustBeTrue(size > 0);
	first = new MemoryLeakItem * [size];
	MustBeTrue(first != NULL);
	last = new MemoryLeakItem * [size];
	MustBeTrue(last != NULL);
	counts = new int [size];
	MustBeTrue(counts != NULL);
	for (int ia=0; ia < size; ia++)
	{
		counts[ia] = 0;
		first[ia] = NULL;
		last[ia] = NULL;
	}
}

MemoryLeak::~MemoryLeak()
{
	clear();
	delete [] first;
	first = NULL;
	delete [] last;
	last = NULL;
	delete [] counts;
	counts = NULL;
	size = 0;
	datasize = 0;
	maximumtotalcounts = 0;
	currenttotalcounts = 0;
}

// hashing function
unsigned long
MemoryLeak::hash(unsigned long address)
{
	return(address);
}

// memory leak operations
void
MemoryLeak::clear()
{
	for (int ib = 0; ib < size; ib++)
	{
		for (MemoryLeakItem *pos = first[ib]; pos != NULL; )
		{
			MemoryLeakItem *psave = pos->next;
			delete pos;
			pos = psave;
		}
		first[ib] = last[ib] = NULL;
		counts[ib] = 0;
	}
	currenttotalcounts = 0;
	return;
}

int
MemoryLeak::insert(unsigned long address, int usecounts)
{
	// get bucket for data item
	int ib = hash(address)%size;

	// search for correct place to insert new item
	MemoryLeakItem *pos;
	for (pos = first[ib]; pos != NULL && pos->address < address; 
	     pos = pos->next) ;

	// check which case
	if (pos == NULL)
	{
		// insert at end of list, allocate a new item
		MemoryLeakItem *pitem = 
			new MemoryLeakItem(address, usecounts);
		MustBeTrue(pitem != NULL);

		// insert into list
		if (counts[ib] <= 0)
		{
			first[ib] = pitem;
			last[ib] = pitem;
		}
		else
		{
			pitem->previous = last[ib];
			pitem->previous->next = pitem;
			pitem->next = NULL;
			last[ib] = pitem;
		}
		counts[ib]++;
		if (++currenttotalcounts > maximumtotalcounts)
			maximumtotalcounts = currenttotalcounts;
	}
	else if (pos->address == address)
	{
		// we found it, overwrite current data
		pos->usecounts = usecounts;
	}
	else
	{
		// allocate a new item
		MemoryLeakItem *pitem = 
			new MemoryLeakItem(address, usecounts);
		MustBeTrue(pitem != NULL);

		// updata links
		pitem->previous = pos->previous;
		pitem->next = pos;
		pos->previous = pitem;
		if (pitem->previous != NULL)
		{
			// previous link must point to new link
			pitem->previous->next = pitem;
		}
		else
		{
			// at beginning of list
			first[ib] = pitem;
		}
		counts[ib]++;
		if (++currenttotalcounts > maximumtotalcounts)
			maximumtotalcounts = currenttotalcounts;
	}

	// all done
	return(OK);
}

int
MemoryLeak::remove(unsigned long &address, int &usecounts)
{
	// get bucket for data item
	int ib = hash(address)%size;

	// check for an empty list
	if (counts[ib] <= 0)
		return(NOMATCH);

	// search for correct place to delete data item
	MemoryLeakItem *pos;
	for (pos = first[ib]; pos != NULL && pos->address < address; 
	     pos = pos->next) ;

	// was anything found
	if (pos == NULL || address != pos->address) 
		return(NOMATCH);
	else
	{
		address = pos->address;
		usecounts = pos->usecounts;
	}

	// delete item
	if (pos->previous != NULL && pos->next != NULL)
	{
		// delete item in middle of list
		pos->previous->next = pos->next;
		pos->next->previous = pos->previous;
	}
	else if (pos->previous == NULL && pos->next != NULL)
	{
		// delete first item in list
		first[ib] = pos->next;
		pos->next->previous = NULL;
	}
	else if (pos->previous != NULL && pos->next == NULL)
	{
		// delete last item in list
		last[ib] = pos->previous;
		pos->previous->next = NULL;
	}
	else
	{
		// delete only item in list
		first[ib] = NULL;
		last[ib] = NULL;
	}
	delete pos;
	counts[ib]--;
	currenttotalcounts--;

	// all done 
	return(OK);
}

int
MemoryLeak::retrieve(unsigned long &address, int &usecounts) const
{
	// get bucket for data item
	int ib = hash(address)%size;

	// check for an empty list
	if (counts[ib] <= 0)
		return(NOMATCH);

	// search for data item
	MemoryLeakItem *p;
	for (p = first[ib]; p != NULL && p->address < address; p = p->next) ;
	if (address == p->address)
	{
		usecounts = p->usecounts;
		return(OK);
	}
	else
		return(NOMATCH);
}

int
MemoryLeak::update(unsigned long address, int usecounts)
{
	// get bucket for data item
	int ib = hash(address)%size;

	// check for an empty list
	if (counts[ib] <= 0)
		return(NOMATCH);

	// search for data item
	MemoryLeakItem *p;
	for (p = first[ib]; p != NULL && p->address < address; p = p->next) ;
	if (address == p->address)
	{
		p->usecounts = usecounts;
		return(OK);
	}
	else
		return(NOMATCH);
}

int
MemoryLeak::isEmpty() const
{
	// check if all lists are empty
	for (int ib = 0; ib < size; ib++)
	{
		if (counts[ib] > 0)
			return(0);
	}
	return(1);
}

// output operators
ostream &
operator<<(ostream &os, const MemoryLeak &ml)
{
	int ic = 0;
	os << "data size            = " << ml.datasize << endl;
	os << "maximum total counts = " << ml.maximumtotalcounts << endl;
	os << "current total counts = " << ml.currenttotalcounts << endl;
	os << "maximum memory used  = " << ml.datasize*ml.maximumtotalcounts << endl;
#if 0
	for (int ib = 0; ib < ml.size; ib++)
	{
		if (ml.counts[ib] <= 0)
			continue;
		for (MemoryLeakItem *p = ml.first[ib]; p != NULL; p = p->next)
		{
			os << ++ic << ": " << *p << endl;
		}
	}
#endif
	return(os);
}


