#ifndef __DATABASE_H
#define __DATABASE_H

// screening class definition

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <string.h>
#include <values.h>
#include <string.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "boolean.h"
#include "number.h"

// definitions
#define ENDOFLIST NULL
#define DATABASETABLESIZE 299

// forward declaration
class DatabaseEntry;
class DatabaseListItem;
class DatabaseList;
class Database;

// screen table entry class
class DatabaseEntry {
public:
	// constructors and destructor
	DatabaseEntry(const DatabaseEntry &);
	DatabaseEntry(const char *, const char *, const char *);
	~DatabaseEntry();
	
        // assignment and logical operations
        DatabaseEntry &operator=(const DatabaseEntry &);
        int operator==(const DatabaseEntry &) const;
        int operator!=(const DatabaseEntry &) const;
        int operator<(const DatabaseEntry &) const;
        int operator<=(const DatabaseEntry &) const;
        int operator>(const DatabaseEntry &) const;
        int operator>=(const DatabaseEntry &) const;

	// query functions
	int operator()(char *) const;

	// hashing function
	unsigned long hash() const;

	// dumpd date
	friend ostream &operator<<(ostream &os, const DatabaseEntry &);

protected:
	// internal data
	char *classAndQuery;
	char *string;
};

// list item class
class DatabaseListItem {
public:
        // friend classes and functions
        friend class DatabaseList;

        // constructors and destructor
        DatabaseListItem(const DatabaseEntry &);
        DatabaseListItem(const DatabaseListItem &);
        ~DatabaseListItem();

        // assignment and equivalence
        DatabaseListItem &operator=(const DatabaseListItem &);
        int operator==(const DatabaseListItem &) const;
        int operator!=(const DatabaseListItem &) const;

protected:
        // internal data
        DatabaseEntry data;
        DatabaseListItem *next;
        DatabaseListItem *previous;
};

// list class
class DatabaseList {
public:
        // friend classes
        friend class DatabaseListIterator;

        // constructors and destructor
        DatabaseList();
        DatabaseList(const DatabaseList &);
        ~DatabaseList();

        // assignment
        DatabaseList &operator=(const DatabaseList &);

        // list operations
        int insert(const DatabaseEntry &);
        int retrieve(DatabaseEntry &) const;
        int isInDatabaseList(DatabaseEntry &) const;
	int isEmpty() const;
        int clear();

protected:
        // internal data
        DatabaseListItem *first;
        DatabaseListItem *last;
};

// sceening class definition
class Database {
public:
	// constructor and destructor
	Database();
	Database(const char *);
	Database(const Database &);
	~Database();

	// insert data into database
	int insert(const char *, const char *, const char *);

	// query database
	int query(const char *, const char *, char *);
	int query(const char *, const char *, Boolean &);
	int query(const char *, const char *, Number &);

protected:
	// other functions
	void readFile(const char *);
	int parse(char *, char *&, char *&, char *&);

protected:
	// other data
	static DatabaseList stbl[DATABASETABLESIZE];
};

#endif

