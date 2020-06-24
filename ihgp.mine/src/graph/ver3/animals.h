#ifndef __ANIMAL_H
#define __ANIMAL_H
/* class for defining where an animal is found */

/* required headers */
#include <stdlib.h>
#include <sysent.h>
#include <string.h>
#include "association.h"

/* define classes */
class Animal;
class Continent;
class LivesIn;

/* animal class */
class Animal: public Object {
public:
	/* constructors and destructor */
	Animal();
	Animal(const char *);
	Animal(const Animal &);
	~Animal();

	/* initialization */
	int init();
	int init(const char *);
	int init(const Animal &);
	int deleteAll();

	/* assignment operator */
	Animal &operator=(const Animal &);

protected:
	/* internal data */
	char *name;
};

/* country class */
class Country: public Object {
public:
	/* constructors and destructor */
	Country();
	Country(const char *);
	Country(const Country &);
	~Country();

	/* initialization */
	int init();
	int init(const char *);
	int init(const Country &);
	int deleteAll();

	/* assignment operator */
	Country &operator=(const Country &);

protected:
	/* internal data */
	char *name;
};

/* association class */
class LivesIn: public Association {
public:
	/* constructors and destructor */
	LivesIn();
	LivesIn(const LivesIn &);
	LivesIn(const char *, const char *);
	~LivesIn();

	/* initialization */
	int init();
	int init(const LivesIn &);
	int init(const char *, const char *);
	int deleteAll();

	/* assignment operator */
	LivesIn &operator=(const LivesIn &);

protected:
	/* internal data */
	char *animal;
	char *country;
};

#endif
