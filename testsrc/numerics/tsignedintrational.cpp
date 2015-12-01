//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdlib.h>
#include <iostream>

#include "numerics/Rational.h"
#include "numerics/SignedInteger.h"

using namespace ombt;
using namespace std;

const char *
bstring(bool flag)
{
	if (flag)
		return("true");
	else
		return("false");
}

main(int argc, char **argv)
{
	cerr << endl << "TESTS FOR SignedInteger<50> ..." << endl << endl;
	Rational<SignedInteger<50> > a(2,4);
	cerr << "a is ... " << a << endl;

	Rational<SignedInteger<50> > c(a);
	cerr << "copy a to c, c(a) is ... " << c << endl;

	Rational<SignedInteger<50> >::ValueType rawc;
	c.internal(rawc);
	Rational<SignedInteger<50> > cbyrawc;
	cbyrawc = rawc;
	cerr << "raw c is ... " << cbyrawc << endl;


	Rational<SignedInteger<50> > b(3,7);
	cerr << "b is ... " << b << endl;

	cerr << endl;
	cerr << a << "+" << b << "=" << (a+b) << endl;
	cerr << a << "-" << b << "=" << (a-b) << endl;
	cerr << a << "*" << b << "=" << (a*b) << endl;
	cerr << a << "/" << b << "=" << (a/b) << endl;

	a = -a;
	cerr << endl;
	cerr << a << "+" << b << "=" << (a+b) << endl;
	cerr << a << "-" << b << "=" << (a-b) << endl;
	cerr << a << "*" << b << "=" << (a*b) << endl;
	cerr << a << "/" << b << "=" << (a/b) << endl;

	b = -b;
	cerr << endl;
	cerr << a << "+" << b << "=" << (a+b) << endl;
	cerr << a << "-" << b << "=" << (a-b) << endl;
	cerr << a << "*" << b << "=" << (a*b) << endl;
	cerr << a << "/" << b << "=" << (a/b) << endl;

	a = -a;
	cerr << endl;
	cerr << a << "+" << b << "=" << (a+b) << endl;
	cerr << a << "-" << b << "=" << (a-b) << endl;
	cerr << a << "*" << b << "=" << (a*b) << endl;
	cerr << a << "/" << b << "=" << (a/b) << endl;

	cerr << endl;
	cerr << a << "==" << a << "=" << bstring(a==a) << endl;
	cerr << a << "!=" << a << "=" << bstring(a!=a) << endl;
	cerr << a << "<" << a << "=" << bstring(a<a) << endl;
	cerr << a << ">" << a << "=" << bstring(a>a) << endl;
	cerr << a << "<=" << a << "=" << bstring(a<=a) << endl;
	cerr << a << ">=" << a << "=" << bstring(a>=a) << endl;

	Rational<SignedInteger<50> > beq(2);
	cerr << endl;
	SignedInteger<50>  lbeq = 2;
	cerr << lbeq << "==" << beq << "=" << bstring(lbeq==beq) << endl;
	cerr << lbeq << "!=" << beq << "=" << bstring(lbeq!=beq) << endl;
	cerr << lbeq << "<=" << beq << "=" << bstring(lbeq<=beq) << endl;
	cerr << lbeq << ">=" << beq << "=" << bstring(lbeq>=beq) << endl;
	cerr << lbeq << "<" << beq << "=" << bstring(lbeq<beq) << endl;
	cerr << lbeq << ">" << beq << "=" << bstring(lbeq>beq) << endl;

	cerr << endl;
	cerr << a << "==" << b << "=" << bstring(a==b) << endl;
	cerr << a << "!=" << b << "=" << bstring(a!=b) << endl;
	cerr << a << "<" << b << "=" << bstring(a<b) << endl;
	cerr << a << ">" << b << "=" << bstring(a>b) << endl;
	cerr << a << "<=" << b << "=" << bstring(a<=b) << endl;
	cerr << a << ">=" << b << "=" << bstring(a>=b) << endl;

	cerr << endl;
	cerr << "abs(" << a << ")=" << ombt::abs(a) << endl;
	a = -a;
	cerr << "abs(" << a << ")=" << ombt::abs(a) << endl;
	cerr << "-" << a << "=" << -a << endl;

	Rational<SignedInteger<50> > d(-2,4);
	cerr << endl;
	cerr << "d is ... " << d << endl;
	cerr << "+" << a << "=" << +d << endl;
	cerr << "-" << a << "=" << -d << endl;
	cerr << "--" << a << "=" << - -d << endl;
	cerr << "+-" << a << "=" << +-d << endl;

	cerr << endl;
	cerr << "0/4 is ... " << Rational<SignedInteger<50> >(0,4) << endl;
	cerr << "0/-4 is ... " << Rational<SignedInteger<50> >(0,-4) << endl;

#if 0
	cerr << endl;
	cerr << "gcd(69,115) = " << Rational<SignedInteger<50> >::gcd(69, 115) << endl;
	cerr << "gcd(115,69) = " << Rational<SignedInteger<50> >::gcd(115, 69) << endl;
	cerr << "gcd(-69,115) = " << Rational<SignedInteger<50> >::gcd(-69, 115) << endl;
	cerr << "gcd(-115,69) = " << Rational<SignedInteger<50> >::gcd(-115, 69) << endl;
	cerr << "gcd(69,-115) = " << Rational<SignedInteger<50> >::gcd(69, -115) << endl;
	cerr << "gcd(115,-69) = " << Rational<SignedInteger<50> >::gcd(115, -69) << endl;
	cerr << "gcd(-69,-115) = " << Rational<SignedInteger<50> >::gcd(-69, -115) << endl;
	cerr << "gcd(-115,-69) = " << Rational<SignedInteger<50> >::gcd(-115, -69) << endl;
	cerr << "gcd(1,1) = " << Rational<SignedInteger<50> >::gcd(1, 1) << endl;
	cerr << "gcd(0,0) = " << Rational<SignedInteger<50> >::gcd(0, 0) << endl;
	cerr << "gcd(5,0) = " << Rational<SignedInteger<50> >::gcd(5, 0) << endl;
	cerr << "gcd(0,5) = " << Rational<SignedInteger<50> >::gcd(0, 5) << endl;
#endif

	return(0);
}
