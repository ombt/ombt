// member functions for database class

// required headers
#include "db.h"

// declare static symbols
static char *NullLengthString = "";

DatabaseList Database::stbl[DATABASETABLESIZE];

// database entry constrcutors and destructor
DatabaseEntry::DatabaseEntry(const DatabaseEntry &src):
	classAndQuery(new char [strlen(src.classAndQuery)+1]),
	string(new char [strlen(src.string)+1])
{
	// check allocations
	assert(classAndQuery != 0);
	assert(string != 0);

	// copy data
	strcpy(classAndQuery, src.classAndQuery);
	strcpy(string, src.string);
}

DatabaseEntry::DatabaseEntry(const char *c, const char *q, const char *s):
	classAndQuery(new char [strlen(c)+strlen(q)+2]),
	string(new char [strlen(s)+1])
{
	// check allocations
	assert(classAndQuery != 0);
	assert(string != 0);

	// copy data
	strcpy(classAndQuery, c);
	strcat(classAndQuery, ":");
	strcat(classAndQuery, q);
	strcpy(string, s);
}

DatabaseEntry::~DatabaseEntry()
{
	delete [] classAndQuery;
	classAndQuery = 0;
	delete [] string;
	string = 0;
}

// assignment and logical operators
DatabaseEntry &
DatabaseEntry::operator=(const DatabaseEntry &rhs)
{
	// check for self-assignment
	if (this != &rhs)
	{
		// delete old data
		delete [] classAndQuery;
		delete [] string;

		// copy class and query
		classAndQuery = new char [strlen(rhs.classAndQuery)+1];
		assert(classAndQuery != 0);
		strcpy(classAndQuery, rhs.classAndQuery);

		// copy values
		string = new char [strlen(rhs.string)+1];
		assert(string != 0);
		strcpy(string, rhs.string);
	}
	return(*this);
}

int
DatabaseEntry::operator==(const DatabaseEntry &rhs) const
{
	return(strcmp(classAndQuery, rhs.classAndQuery) == 0);
}

int
DatabaseEntry::operator!=(const DatabaseEntry &rhs) const
{
	return(strcmp(classAndQuery, rhs.classAndQuery) != 0);
}

int
DatabaseEntry::operator<(const DatabaseEntry &rhs) const
{
	return(strcmp(classAndQuery, rhs.classAndQuery) < 0);
}

int
DatabaseEntry::operator<=(const DatabaseEntry &rhs) const
{
	return(strcmp(classAndQuery, rhs.classAndQuery) <= 0);
}

int
DatabaseEntry::operator>(const DatabaseEntry &rhs) const
{
	return(strcmp(classAndQuery, rhs.classAndQuery) > 0);
}

int
DatabaseEntry::operator>=(const DatabaseEntry &rhs) const
{
	return(strcmp(classAndQuery, rhs.classAndQuery) >= 0);
}

// queries
int
DatabaseEntry::operator()(char *s) const
{
	strcpy(s, string);
	return(OK);
}

// hash function
unsigned long
DatabaseEntry::hash() const
{
	int c;
	unsigned long sum;

	// hash function from "Dragon" book
	sum = 0;
	for (char *p = classAndQuery; *p != 0; p++)
	{
		c = *p;
		if (sum & 01)
			sum = (sum >> 1) + 0x8000;
		else
			sum >>= 1;
		sum += c;
		sum &= 0xffff;
	}
	return(sum);
}

// print data
ostream &
operator<<(ostream &os, const DatabaseEntry &se)
{
	os << "class and query is ... " << se.classAndQuery << endl;
	os << "string is ... " << se.string << endl;
	return(os);
}

// list item constructors and destructor
DatabaseListItem::DatabaseListItem(const DatabaseEntry &src):
	data(src), next(ENDOFLIST), previous(ENDOFLIST)
{
	// do nothing
}

DatabaseListItem::DatabaseListItem(const DatabaseListItem &item):
	data(item.data), next(ENDOFLIST), previous(ENDOFLIST)
{
	// do nothing
}

DatabaseListItem::~DatabaseListItem()
{
	next = ENDOFLIST;
	previous = ENDOFLIST;
}

DatabaseListItem &
DatabaseListItem::operator=(const DatabaseListItem &item)
{
	if (this != &item)
	{
		data = item.data;
		next = ENDOFLIST;
		previous = ENDOFLIST;
	}
	return(*this);
}

int
DatabaseListItem::operator==(const DatabaseListItem &item) const
{
	return(data == item.data);
}

int
DatabaseListItem::operator!=(const DatabaseListItem &item) const
{
	return( ! (data == item.data));
}

// list contructors and destructor
DatabaseList::DatabaseList(): 
	first(ENDOFLIST), last(ENDOFLIST)
{
	// do nothing
}

DatabaseList::DatabaseList(const DatabaseList &list):
	first(ENDOFLIST), last(ENDOFLIST)
{
	for (DatabaseListItem *pos = list.first; pos != ENDOFLIST; 
	     pos = pos->next)
	{
		assert(insert(pos->data) == OK);
	}
}

DatabaseList::~DatabaseList()
{
	for (DatabaseListItem *pos = first; pos != ENDOFLIST; )
	{
		DatabaseListItem *save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = ENDOFLIST;
}

//assignment
DatabaseList &
DatabaseList::operator=(const DatabaseList &list)
{
	// check for self-assignment
	if (this == &list) return(*this);

	// delete old list
	for (DatabaseListItem *pos = first; pos != ENDOFLIST; )
	{
		DatabaseListItem *save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = ENDOFLIST;

	// copy new list
	for (pos = list.first; pos != ENDOFLIST; pos = pos->next)
	{
		assert(insert(pos->data) == OK);
	}
	return(*this);
}

// list operations
int
DatabaseList::insert(const DatabaseEntry &data)
{
	// allocate a new item
	DatabaseListItem *pitem = new DatabaseListItem(data);
	assert(pitem != 0);

	// insert new item in list
	if (first == ENDOFLIST && last == ENDOFLIST)
	{
		// first element in list
		first = pitem;
		last = pitem;
	}
	else
	{
		// insert item at end of list
		pitem->previous = last;
		pitem->previous->next = pitem;
		pitem->next = ENDOFLIST;
		last = pitem;
	}
	return(OK);
}

int
DatabaseList::retrieve(DatabaseEntry &data) const
{
	for (DatabaseListItem *p = first; p != ENDOFLIST; p = p->next)
	{
		if (data == p->data)
		{
			data = p->data;
			return(OK);
		}
	}
	return(NOMATCH);
}

int
DatabaseList::isInDatabaseList(DatabaseEntry &data) const
{
	for (DatabaseListItem *p = first; p != ENDOFLIST; p = p->next)
	{
		if (data == p->data)
		{
			return(1);
		}
	}
	return(0);
}

int
DatabaseList::clear()
{
	for (DatabaseListItem *pos = first; pos != ENDOFLIST; )
	{
		DatabaseListItem *save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = ENDOFLIST;
	return(OK);
}

int
DatabaseList::isEmpty() const
{
	return((first == ENDOFLIST) && (last == ENDOFLIST));
}

// constructors and destructor
Database::Database()
{
	// do nothing
}

Database::Database(const char *fnm)
{
	if (fnm != 0 && *fnm != 0)
	{
		readFile(fnm);
	}
}

Database::Database(const Database &)
{
	// do nothing
}

Database::~Database()
{
	// do nothing
}

// read in a file and store contents in table
int
Database::parse(char *inbuf, char *&myclass, char *&myquery, char *&myrest)
{
	// initialize
	myclass = 0;
	myquery = 0;
	myrest = 0;

	// get class
	myclass = strtok(inbuf, ";");
	if (myclass == 0) return(NOMATCH);

	// get query
	myquery = strtok(0, ";");
	if (myquery == 0) return(NOMATCH);

	// get rest of string. also deal with cases with no data.
	myrest = strtok(0, ";");
	if (myrest == 0) myrest = NullLengthString;

	// all done
	return(OK);
}

void
Database::readFile(const char *fnm)
{
	// open file for read only
	FILE *infd;
	if ((infd = fopen(fnm, "r")) == 0)
		return;

	// read in records
	char inbuf[BUFSIZ+1];
	while (fgets(inbuf, BUFSIZ, infd) != NULL)
	{
		char *myclass;
		char *myquery;
		char *myremainder;

		// remove new line
		inbuf[strlen(inbuf)-1] = 0;

		// skip comments or white space-only lines
		if ((strspn(inbuf, " \t\n") == strlen(inbuf)) ||
		    (strncmp(inbuf, "#", 1) == 0))
			continue;

		// parse line into components
		if (parse(inbuf, myclass, myquery, myremainder) != OK)
			continue;

		// insert into database table
		insert(myclass, myquery, myremainder);
	}

	// close file
	fclose(infd);

	// all done
	return;
}

// insert data 
int
Database::insert(const char *c, const char *q, const char *s)
{
	DatabaseEntry se(c, q, s);
	return(stbl[se.hash()%DATABASETABLESIZE].insert(se));
}

// query data 
int
Database::query(const char *c, const char *q, char *s)
{
	DatabaseEntry se(c, q, s);
	if (stbl[se.hash()%DATABASETABLESIZE].retrieve(se) != OK)
		return(NOMATCH);
	else
		return(se(s));
}

int
Database::query(const char *c, const char *q, Boolean &b)
{
	char s[BUFSIZ];
	*s = 0;
	DatabaseEntry se(c, q, s);
	if (stbl[se.hash()%DATABASETABLESIZE].retrieve(se) != OK)
		return(NOMATCH);
	if (se(s) != OK) 
		return(NOMATCH);
	if ((strcmp(s, "yes") == 0) || (strcmp(s, "true") == 0))
		b = True;
	else if ((strcmp(s, "no") == 0) || (strcmp(s, "false") == 0))
		b = False;
	else
		return(NOMATCH);
	return(OK);
}

int
Database::query(const char *c, const char *q, Number &n)
{
	char s[BUFSIZ];
	*s = 0;
	DatabaseEntry se(c, q, s);
	if (stbl[se.hash()%DATABASETABLESIZE].retrieve(se) != OK)
		return(NOMATCH);
	if (se(s) != OK) 
		return(NOMATCH);
	if (strlen(s) != strspn(s, "0123456789.+-Ee"))
		return(NOMATCH);
	n = atof(s);
	return(OK);
}

