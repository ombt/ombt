#ifndef __MONEY_H
#define __MONEY_H

// money class definition

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <errno.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "number.h"
#include "boolean.h"
#include "db.h"

// default values
#define MoneyDefaultName "NONAME"
#define MoneyDefaultNameSize 7

// forward declaration
class Money;

// money class definition
class Money {
public:
	// constructors and destructor
	Money();
	Money(const Money &);
	Money(const Number &);
	Money(const char *, const Number &);
	virtual ~Money();

	// arithmetic operations
	virtual Money &operator=(const Money &);
	virtual Money &operator+=(const Money &);
	virtual Money &operator-=(const Money &);
	virtual Money operator+(const Money &) const;
	virtual Money operator-(const Money &) const;

	// logical operators
	virtual int operator==(const Money&) const;
	virtual int operator!=(const Money&) const;
	virtual int operator<(const Money&) const;
	virtual int operator>(const Money&) const;
	virtual int operator<=(const Money&) const;
	virtual int operator>=(const Money&) const;

	// arithmetic operations
	virtual Money &operator=(const Number &);
	virtual Money &operator+=(const Number &);
	virtual Money &operator-=(const Number &);
	virtual Money &operator*=(const Number &);
	virtual Money &operator/=(const Number &);
	virtual Money operator+(const Number &) const;
	virtual Money operator-(const Number &) const;
	virtual Money operator*(const Number &) const;
	virtual Money operator/(const Number &) const;
	friend Money operator+(const Number &, const Money &);
	friend Money operator-(const Number &, const Money &);
	friend Money operator*(const Number &, const Money &);

	// logical operators
	virtual int operator==(const Number &) const;
	virtual int operator!=(const Number &) const;
	virtual int operator<(const Number &) const;
	virtual int operator>(const Number &) const;
	virtual int operator<=(const Number &) const;
	virtual int operator>=(const Number &) const;
	friend int operator==(const Number &, const Money &);
	friend int operator!=(const Number &, const Money &);
	friend int operator<(const Number &, const Money &);
	friend int operator>(const Number &, const Money &);
	friend int operator<=(const Number &, const Money &);
	friend int operator>=(const Number &, const Money &);

	// input and output for money
	virtual Money &operator()();
	friend ostream &operator<<(ostream &, const Money &);
	friend istream &operator>>(istream &, Money &);

	// turn screening on or off
	static void doScreening() {
		screeningIsOn = True;
	}
	static void doNoScreening() {
		screeningIsOn = False;
	}

protected:
	// internal data
	static Boolean screeningIsOn;
	char *name;
	Number amount;
};

#endif
