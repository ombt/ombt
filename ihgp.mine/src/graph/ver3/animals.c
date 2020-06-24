/* define member functions for animal classes */

/* unix headers */
#include <stdlib.h>
#include <sysent.h>
#include <errno.h>
#include <assert.h>

/* other headers */
#include "returns.h"
#include "debug.h"
#include "animals.h"

/* constructors and destructor */
Animal::Animal(): Object()
{
	init();
}

Animal::Animal(const char *src): Object()
{
	init(src);
}

Animal::Animal(const Animal &src): Object(src)
{
	init(src);
}

Animal::~Animal()
{
	deleteAll();
}

/* initialization */
int
Animal::init()
{
	name = (char *)0;
	status = OK;
	return(OK);
}

int
Animal::init(const char *src)
{
	if (src != (char *)0)
	{
		name = new char[strlen(src)+1];
		strcpy(name, src);
	}
	status = OK;
	return(OK);
}

int
Animal::init(const Animal &src)
{
	if (src.name != (char *)0)
	{
		name = new char[strlen(src.name)+1];
		strcpy(name, src.name);
	}
	status = OK;
	return(OK);
}

int
Animal::deleteAll()
{
	if (name != (char *)0) delete [] name;
	name = (char *)0;
	status = OK;
	return(NOTOK);
}

/* assignment operator */
Animal &
Animal::operator=(const Animal &rhs)
{
	if (this == &rhs) return(*this);
	deleteAll();
	Object::operator=(rhs);
	init(rhs);
	return(*this);
}

/* constructors and destructor */
Country::Country(): Object()
{
	init();
}

Country::Country(const char *src): Object()
{
	init(src);
}

Country::Country(const Country &src): Object(src)
{
	init(src);
}

Country::~Country()
{
	deleteAll();
}

/* initialization */
int
Country::init()
{
	name = (char *)0;
	status = OK;
	return(OK);
}

int
Country::init(const char *src)
{
	if (src != (char *)0)
	{
		name = new char[strlen(src)+1];
		strcpy(name, src);
	}
	status = OK;
	return(OK);
}

int
Country::init(const Country &src)
{
	if (src.name != (char *)0)
	{
		name = new char[strlen(src.name)+1];
		strcpy(name, src.name);
	}
	status = OK;
	return(OK);
}

int
Country::deleteAll()
{
	if (name != (char *)0) delete [] name;
	name = (char *)0;
	status = OK;
	return(NOTOK);
}

/* assignment operator */
Country &
Country::operator=(const Country &rhs)
{
	if (this == &rhs) return(*this);
	deleteAll();
	Object::operator=(rhs);
	init(rhs);
	return(*this);
}

/* constructors and destructor */
LivesIn::LivesIn(): Association()
{
	init();
}

LivesIn::LivesIn(const LivesIn &src): Association(src)
{
	init(src);
}

LivesIn::LivesIn(const char *an, const char *co): Association()
{
	init(an, co);
}

LivesIn::~LivesIn()
{
	deleteAll();
}

/* initialization */
int
LivesIn::init()
{
	animal = (char *)0;
	country = (char *)0;
	status = OK;
	return(OK);
}

int
LivesIn::init(const char *an, const char *co)
{
	animal = new char[strlen(an)+1];
	strcpy(animal, an);
	country = new char[strlen(co)+1];
	strcpy(country, co);
	status = OK;
	return(OK);
}

int
LivesIn::init(const LivesIn &src)
{
	animal = new char[strlen(src.animal)+1];
	strcpy(animal, src.animal);
	country = new char[strlen(src.country)+1];
	strcpy(country, src.country);
	status = OK;
	return(OK);
}

int
LivesIn::deleteAll()
{
	if (animal != (char *)0) delete [] animal;
	if (country != (char *)0) delete [] country;
	animal = (char *)0;
	country = (char *)0;
	status = OK;
	return(NOTOK);
}

/* assignment operator */
LivesIn &
LivesIn::operator=(const LivesIn &rhs)
{
	if (this == &rhs) return(*this);
	deleteAll();
	Association::operator=(rhs);
	init(rhs);
	return(*this);
}

