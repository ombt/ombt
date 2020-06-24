/* define member functions for association class */

/* unix headers */
#include <stdlib.h>
#include <sysent.h>
#include <errno.h>
#include <assert.h>

/* other headers */
#include "returns.h"
#include "debug.h"
#include "association.h"

/* constructors and destructor */
Association::Association()
{
	init();
}

Association::Association(const Association &src)
{
	init(src);
}

Association::~Association()
{
	deleteAll();
}

/* initialization functions */
int
Association::init()
{
	/* create hash table which maps objects */
	tableSize = ASSOCIATIONTABLESIZE;
	table = new Bucket *[tableSize];
	if (table == (Bucket **)0)
	{
		status = errno;
		ERROR("unable allocate a hashtable.", status);
		return(NOTOK);
	}
	for (int bucket = 0; bucket < ASSOCIATIONTABLESIZE; bucket++)
	{
		table[bucket] = (Bucket *)0;
	}

	/* all done */
	status = OK;
	return(NOTOK);
}

int
Association::init(const Association &src)
{
	/* create hash table which maps objects */
	tableSize = src.tableSize;
	table = new Bucket *[tableSize];
	if (table == (Bucket **)0)
	{
		status = errno;
		ERROR("unable allocate a hashtable.", status);
		return(NOTOK);
	}
	for (int bucket = 0; bucket < tableSize; bucket++)
	{
		/* check if anything to copy */
		if (src.table[bucket] == (Bucket *)0)
		{
			/* empty bucket */
			table[bucket] = (Bucket *)0;
		}
		else if (copyBucket(table[bucket], src.table[bucket]) != OK)
		{
			ERROR("unable to copy a hashtable bucket.", status);
			return(NOTOK);
		}
	}

	/* all done */
	status = OK;
	return(NOTOK);
}

int
Association::deleteAll()
{
	/* check if table is empty */
	if (table == (Bucket **)0)
	{
		tableSize = 0;
		status = OK;
		return(OK);
	}

	/* delete everything */
	for (int bucket = 0; bucket < tableSize; bucket++)
	{
		/* check if anything to delete */
		if (table[bucket] != (Bucket *)0)
		{
			/* delete contents of bucket */
			deleteBucket(table[bucket]);
		}
	}
	delete [] table;
	table = (Bucket **)0;
	tableSize = 0;

	/* all done */
	status = OK;
	return(OK);
}

/* copy a bucket */
int
Association::copyBucket(Bucket *&dest, Bucket *&src)
{
	/* copy all bucket contents to new table */
	Bucket *psrc, **pdest;
	for (psrc = src, pdest = &dest; psrc != (Bucket *)0; psrc = psrc->next)
	{
		/* create and initialize a new bucket */
		if ((*pdest = new Bucket) == (Bucket *)0)
		{
			status = errno;
			ERROR("unable allocate a bucket.", status);
			return(NOTOK);
		}
		(*pdest)->next = (Bucket *)0;
		(*pdest)->head = (Link *)0;

		/* copy links */
		if (copyLinkList((*pdest)->head, src->head) != OK)
		{
			ERROR("unable to copy a link list.", status);
			return(NOTOK);
		}

		/* set up link in hash table */
		pdest = &(*pdest)->next;
	}

	/* all done */
	status = OK;
	return(OK);
}

/* copy a link list */
int
Association::copyLinkList(Link *&dest, Link *&src)
{
	/* copy entire link list */
	Link *psrc, **pdest;
	for (psrc = src, pdest = &dest; psrc != (Link *)0; psrc = psrc->next)
	{
		/* create and initialize a new link */
		if ((*pdest = new Link) == (Link *)0)
		{
			status = errno;
			ERROR("unable allocate a link.", status);
			return(NOTOK);
		}
		(*pdest)->next = (Link *)0;
		(*pdest)->object = src->object;

		/* set up link */
		pdest = &(*pdest)->next;
	}


	/* all done */
	status = OK;
	return(OK);
}

/* delete a bucket */
int
Association::deleteBucket(Bucket *&bucket)
{
	/* delete all buckets and link lists */
	Bucket *pb, *ptmpb;
	for (pb = bucket; pb != (Bucket *)0; )
	{
		/* delete link list */
		if (pb->head != (Link *)0) deleteLinkList(pb->head);

		/* delete node */
		ptmpb = pb->next;
		delete pb;
		pb = ptmpb;
	}
	/* all done */
	status = OK;
	return(OK);
}

/* delete a link list */
int
Association::deleteLinkList(Link *&list)
{
	/* delete link list */
	Link *pl, *ptmpl;
	for (pl = list; pl != (Link *)0; )
	{
		/* delete node */
		ptmpl = pl->next;
		delete pl;
		pl = ptmpl;
	}

	/* all done */
	status = OK;
	return(OK);
}

/* equality operator */
Association &
Association::operator=(const Association &rhs)
{
	if (this != &rhs)
	{
		deleteAll();
		init(rhs);
	}
	else
	{
		status = OK;
	}
	return(*this);
}

/* insert a new association between two objects */
int
Association::insert(Object &obja, Object &objb)
{
	/* get bucket for object A */
	int bucket = int(long(&obja)%tableSize);

	/* check if entry is in bucket */
	Bucket *pbucket = table[bucket];
	for ( ; pbucket != (Bucket *)0; pbucket = pbucket->next)
	{
		/* find object, if there */
		if (pbucket->object == &obja)
		{
			/* found the object */
			break;
		}
	}

	/* was object found */
	if (pbucket == (Bucket *)0)
	{
		/* object was not found, allocate a new bucket */
		pbucket = new Bucket;
		if (pbucket == (Bucket *)0)
		{
			status = errno;
			ERROR("unable allocate a bucket.", status);
			return(NOTOK);
		}
		pbucket->object = &obja;
		pbucket->next = table[bucket];
		table[bucket] = pbucket;
	}

	/* determine if association already exists */
	Link *plink = pbucket->head;
	for ( ; plink != (Link *)0; plink = plink->next)
	{
		/* find object, if there */
		if (plink->object == &objb)
		{
			/* found the object */
			break;
		}
	}

	/* was association found */
	if (plink == (Link *)0)
	{
		/* association was not found, insert it */
		plink = new Link;
		if (plink == (Link *)0)
		{
			status = errno;
			ERROR("unable allocate a link.", status);
			return(NOTOK);
		}
		plink->object = &objb;
		plink->next = pbucket->head;
		pbucket->head = plink;
	}

	/* all done */
	status = OK;
	return(OK);
}

/* delete an association between objects */
int
Association::remove(Object &obja, Object &objb)
{
	/* get bucket for object A */
	int bucket = int(long(&obja)%tableSize);

	/* check if entry is in bucket */
	Bucket *pbucket = table[bucket];
	for ( ; pbucket != (Bucket *)0; pbucket = pbucket->next)
	{
		/* find object, if there */
		if (pbucket->object == &obja)
		{
			/* found the object */
			break;
		}
	}

	/* was object found */
	if (pbucket == (Bucket *)0)
	{
		/* object was not found */
		status = NOMATCH;
		return(NOMATCH);
	}

	/* determine if association exists */
	Link *plink = pbucket->head;
	Link **pplink = &pbucket->head;
	for ( ; plink != (Link *)0; pplink = &plink->next, plink = plink->next)
	{
		/* find object, if there */
		if (plink->object == &objb)
		{
			/* found the object, remove the link */
			*pplink = plink->next;
			delete plink;
			status = OK;
			return(OK);
		}
	}

	/* association was not found */
	status = NOMATCH;
	return(NOMATCH);
}

/* delete all associations with an object */
int
Association::remove(Object &obja)
{
	/* get bucket for object A */
	int bucket = int(long(&obja)%tableSize);

	/* check if entry is in bucket */
	Bucket *pbucket = table[bucket];
	Bucket **ppbucket = &table[bucket];
	for ( ; pbucket != (Bucket *)0; 
	     ppbucket = &pbucket->next, pbucket = pbucket->next)
	{
		/* find object, if there */
		if (pbucket->object == &obja)
		{
			/* found the object, delete link list */
			deleteLinkList(pbucket->head);

			/* delete bucket */
			*ppbucket = pbucket->next;
			delete pbucket;
			status = OK;
			return(OK);
		}
	}

	/* bucket was not found */
	status = NOMATCH;
	return(NOMATCH);
}

/* constructors and destructor */
AssociationIterator::AssociationIterator()
{
	next = (Association::Link *)0;
	status = OK;
}

AssociationIterator::AssociationIterator(const AssociationIterator &src)
{
	next = src.next;
	status = OK;
}

AssociationIterator::AssociationIterator(const Association &assoc,
					 const Object &obj)
{
	/* get bucket for object */
	int bucket = int(long(&obj)%assoc.tableSize);

	/* check if entry is in bucket */
	Association::Bucket *pbucket = assoc.table[bucket];
	for ( ; pbucket != (Association::Bucket *)0; pbucket = pbucket->next)
	{
		/* find object, if there */
		if (pbucket->object == &obj)
		{
			/* found the object */
			break;
		}
	}

	/* initialize iterator with list */
	if (pbucket != (Association::Bucket *)0)
		next = pbucket->head;
	else
		next = (Association::Link *)0;

	/* all done */
	status = OK;
}

AssociationIterator::~AssociationIterator()
{
	next = (Association::Link *)0;
	status = OK;
}

/* iterator function */
Object *
AssociationIterator::operator()()
{
	status = OK;
	if (next) 
	{
		next = next->next;
		return(next->object);
	}
	else
	{
		return((Object *)0);
	}
}

/* assignment */
AssociationIterator &
AssociationIterator::operator=(const AssociationIterator &src)
{
	next = src.next;
	status = OK;
	return(*this);
}
