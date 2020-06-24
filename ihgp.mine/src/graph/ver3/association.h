#ifndef __ASSOCIATION_H
#define __ASSOCIATION_H
/* class for defining associations between classes */

/* local definitions */
#define ASSOCIATIONTABLESIZE 1021

/* define classes */
class Object;
class Association;
class AssociationIterator;

/* object in list class */
class Object {
	/* define friend classes for defining associations */
	friend class Association;

public:
	/* constructors and destructor */
	Object() { }
	Object(const Object &) { }
	virtual ~Object() { }

	/* equal operator */
	Object &operator=(const Object &) { return(*this); }

	/* other member functions */
	inline int getStatus() { return(status); }

protected:
	/* internal data */
	int status;
};

/* association class */
class Association {
public:
	/* link list structure */
	struct Link {
		Link() {
			next = (Link *)0;
			object = (Object *)0;
		}
		~Link() {
			next = (Link *)0;
			object = (Object *)0;
		}
		Link *next;
		Object *object;
	};

	/* hash table structure */
	struct Bucket {
		Bucket() {
			next = (Bucket *)0;
			head = (Link *)0;
			object = (Object *)0;
		}
		~Bucket() {
			next = (Bucket *)0;
			head = (Link *)0;
			object = (Object *)0;
		}
		Bucket *next;
		Link *head;
		Object *object;
	};

	/* friend interator class */
	friend class AssociationIterator;

	/* constructors and destructor */
	Association();
	Association(const Association &);
	virtual ~Association();

	/* initialization functions */
	virtual int init();
	virtual int init(const Association &);
	virtual int deleteAll();

	/* equal operator */
	Association &operator=(const Association &);

	/* functions to insert associations */
	virtual int insert(Object &, Object &);
	virtual int remove(Object &, Object &);
	virtual int remove(Object &);

	/* other member functions */
	inline int getStatus() { return(status); }

private:
	/* utility functions */
	int deleteBucket(Bucket *&);
	int deleteLinkList(Link *&);
	int copyBucket(Bucket *&, Bucket *&);
	int copyLinkList(Link *&, Link *&);

protected:
	/* internal data */
	int status;
	int tableSize;
	Bucket **table;
};

/* accociation class interator class */
class AssociationIterator {
public:
	/* constructors and destructor */
	AssociationIterator();
	AssociationIterator(const AssociationIterator &);
	AssociationIterator(const Association &, const Object &);
	virtual ~AssociationIterator();

	/* equal operator */
	AssociationIterator &operator=(const AssociationIterator &);

	/* return next object associated with this object */
	Object *operator()();

	/* other member functions */
	inline int getStatus() { return(status); }

protected:
	/* internal data */
	int status;
	Association::Link *next;
};

#endif
