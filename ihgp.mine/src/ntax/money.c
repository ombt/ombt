// money class functions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// local headers
#include "money.h"

// local database table.
static Database db;

// static flags
Boolean Money::screeningIsOn = True;

// constructors and destructor
Money::Money():
	amount(0.0),
	name(new char [MoneyDefaultNameSize])
{
	assert(name != 0);
	strcpy(name, MoneyDefaultName);
}

Money::Money(const char *nm, const Number &amt):
	amount(amt),
	name(0)
{
	assert(nm != 0);
	name = new char [strlen(nm)+1];
	assert(name != 0);
	strcpy(name, nm);
#ifdef REGISTER
	cerr << "Money:" << name << endl;
#endif
}

Money::Money(const Number &amt):
	amount(amt),
	name(new char [MoneyDefaultNameSize])
{
	assert(name != 0);
	strcpy(name, MoneyDefaultName);
}

Money::Money(const Money &m):
	amount(m.amount),
	name(0)
{
	assert(m.name != 0);
	name = new char [strlen(m.name)+1];
	assert(name != 0);
	strcpy(name, m.name);
}

Money::~Money()
{
	delete [] name;
	name = 0;
}

// arithmetic operators
Money &
Money::operator=(const Money &m)
{
        amount = m.amount;
        return(*this);
}

Money &
Money::operator+=(const Money &m)
{
        amount += m.amount;
        return(*this);
}

Money &
Money::operator-=(const Money &m)
{
        amount -= m.amount;
        return(*this);
}

Money
Money::operator+(const Money &m) const
{
        return(Money(*this) += m);
}

Money
Money::operator-(const Money &m) const
{
        return(Money(*this) -= m);
}

// logical operators
int
Money::operator==(const Money &m) const
{
        return(amount == m.amount);
}

int
Money::operator!=(const Money &m) const
{
        return(amount != m.amount);
}

int
Money::operator<(const Money &m) const
{
        return(amount < m.amount);
}

int
Money::operator>(const Money &m) const
{
        return(amount > m.amount);
}

int
Money::operator<=(const Money &m) const
{
        return(amount <= m.amount);
}

int
Money::operator>=(const Money &m) const
{
        return(amount >= m.amount);
}

// arithmetic operators
Money &
Money::operator=(const Number &n)
{
        amount = n;
        return(*this);
}

Money &
Money::operator+=(const Number &n)
{
        amount += n;
        return(*this);
}

Money &
Money::operator-=(const Number &n)
{
        amount -= n;
        return(*this);
}

Money &
Money::operator*=(const Number &n)
{
        amount *= n;
        return(*this);
}

Money &
Money::operator/=(const Number &n)
{
        assert(n != 0.0);
        amount /= n;
        return(*this);
}

Money
Money::operator+(const Number &n) const
{
        return(Money(*this) += n);
}

Money
Money::operator-(const Number &n) const
{
        return(Money(*this) -= n);
}

Money
Money::operator*(const Number &n) const
{
        return(Money(*this) *= n);
}

Money
Money::operator/(const Number &n) const
{
        return(Money(*this) /= n);
}

Money
operator+(const Number &n, const Money &m)
{
        return(Money(n) += m);
}

Money
operator-(const Number &n, const Money &m)
{
        return(Money(n) -= m);
}

Money
operator*(const Number &n, const Money &m)
{
        return(Money(m) *= n);
}

// logical operators
int
Money::operator==(const Number &n) const
{
        return(amount == n);
}

int
Money::operator!=(const Number &n) const
{
        return(amount != n);
}

int
Money::operator<(const Number &n) const
{
        return(amount < n);
}

int
Money::operator>(const Number &n) const
{
        return(amount > n);
}

int
Money::operator<=(const Number &n) const
{
        return(amount <= n);
}

int
Money::operator>=(const Number &n) const
{
        return(amount >= n);
}

int
operator==(const Number &n, const Money &m)
{
	return(n.operator==(m.amount));
}

int
operator!=(const Number &n, const Money &m)
{
	return(n.operator!=(m.amount));
}

int
operator<(const Number &n, const Money &m)
{
	return(n.operator<(m.amount));
}

int
operator>(const Number &n, const Money &m)
{
	return(n.operator>(m.amount));
}

int
operator<=(const Number &n, const Money &m)
{
	return(n.operator<=(m.amount));
}

int
operator>=(const Number &n, const Money &m)
{
	return(n.operator>=(m.amount));
}

// read and write a number
ostream &
operator<<(ostream &os, const Money &m)
{
	Boolean yesno = False;

	// check if screening is on and if field should be screened
	if (Money::screeningIsOn == True)
	{
		if ((db.query(m.name, "use this field?", yesno) == OK) &&
		    (yesno == False))
		{
			// skip field
			return(os);
		}
	}

	// print data
	if (strcmp(m.name, MoneyDefaultName) != 0)
		os << m.name << ": ";
	os << m.amount;
		
	// all done
	return(os);
}

istream &
operator>>(istream &is, Money &m)
{
	Boolean yesno = False;

	// check if screening is on and if field should be screened
	if (Money::screeningIsOn == True)
	{
		if ((db.query(m.name, "use this field?", yesno) == OK) &&
		    (yesno == False))
		{
			// skip field
			return(is);
		}
	}

	// get input
	if (strcmp(m.name, MoneyDefaultName) != 0)
		cout << "Enter " << m.name << ": ";
	is >> m.amount;

	// all done
	return(is);
}

Money &
Money::operator()()
{
	return(*this);
}
