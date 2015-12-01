//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdlib.h>
#include <iostream>

#include "numerics/Rational.h"

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
	{
	cerr << endl << "TESTS FOR LONG ..." << endl << endl;
	Rational<long> a(2,4);
	cerr << "a is ... " << a << endl;

	Rational<long> c(a);
	cerr << "copy a to c, c(a) is ... " << c << endl;

	Rational<long>::ValueType rawc;
	c.internal(rawc);
	Rational<long> cbyrawc;
	cbyrawc = rawc;
	cerr << "raw c is ... " << cbyrawc << endl;


	Rational<long> b(3,7);
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

	Rational<long> beq(2);
	cerr << endl;
	long lbeq = 2;
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

	Rational<long> d(-2,4);
	cerr << endl;
	cerr << "d is ... " << d << endl;
	cerr << "+" << a << "=" << +d << endl;
	cerr << "-" << a << "=" << -d << endl;
	cerr << "--" << a << "=" << - -d << endl;
	cerr << "+-" << a << "=" << +-d << endl;

	cerr << endl;
	cerr << "0/4 is ... " << Rational<long>(0,4) << endl;
	cerr << "0/-4 is ... " << Rational<long>(0,-4) << endl;

	cerr << endl;
	cerr << "gcd(69,115) = " << Rational<long>::gcd(69, 115) << endl;
	cerr << "gcd(115,69) = " << Rational<long>::gcd(115, 69) << endl;
	cerr << "gcd(-69,115) = " << Rational<long>::gcd(-69, 115) << endl;
	cerr << "gcd(-115,69) = " << Rational<long>::gcd(-115, 69) << endl;
	cerr << "gcd(69,-115) = " << Rational<long>::gcd(69, -115) << endl;
	cerr << "gcd(115,-69) = " << Rational<long>::gcd(115, -69) << endl;
	cerr << "gcd(-69,-115) = " << Rational<long>::gcd(-69, -115) << endl;
	cerr << "gcd(-115,-69) = " << Rational<long>::gcd(-115, -69) << endl;
	cerr << "gcd(1,1) = " << Rational<long>::gcd(1, 1) << endl;
	cerr << "gcd(0,0) = " << Rational<long>::gcd(0, 0) << endl;
	cerr << "gcd(5,0) = " << Rational<long>::gcd(5, 0) << endl;
	cerr << "gcd(0,5) = " << Rational<long>::gcd(0, 5) << endl;
	}

	{
	cerr << endl << "TESTS FOR LONG LONG ..." << endl << endl;
	Rational<long long> a(2,4);
	cerr << "a is ... " << a << endl;

	Rational<long long> c(a);
	cerr << "copy a to c, c(a) is ... " << c << endl;

	Rational<long long> b(3,7);
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

	Rational<long long> beq(2);
	cerr << endl;
	long long lbeq = 2;
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

	Rational<long long> d(-2,4);
	cerr << endl;
	cerr << "d is ... " << d << endl;
	cerr << "+" << a << "=" << +d << endl;
	cerr << "-" << a << "=" << -d << endl;
	cerr << "--" << a << "=" << - -d << endl;
	cerr << "+-" << a << "=" << +-d << endl;

	cerr << endl;
	cerr << "0/4 is ... " << Rational<long long>(0,4) << endl;
	cerr << "0/-4 is ... " << Rational<long long>(0,-4) << endl;

	cerr << endl;
	cerr << "gcd(69,115) = " << Rational<long long>::gcd(69, 115) << endl;
	cerr << "gcd(115,69) = " << Rational<long long>::gcd(115, 69) << endl;
	cerr << "gcd(-69,115) = " << Rational<long long>::gcd(-69, 115) << endl;
	cerr << "gcd(-115,69) = " << Rational<long long>::gcd(-115, 69) << endl;
	cerr << "gcd(69,-115) = " << Rational<long long>::gcd(69, -115) << endl;
	cerr << "gcd(115,-69) = " << Rational<long long>::gcd(115, -69) << endl;
	cerr << "gcd(-69,-115) = " << Rational<long long>::gcd(-69, -115) << endl;
	cerr << "gcd(-115,-69) = " << Rational<long long>::gcd(-115, -69) << endl;
	cerr << "gcd(1,1) = " << Rational<long long>::gcd(1, 1) << endl;
	cerr << "gcd(0,0) = " << Rational<long long>::gcd(0, 0) << endl;
	cerr << "gcd(5,0) = " << Rational<long long>::gcd(5, 0) << endl;
	cerr << "gcd(0,5) = " << Rational<long long>::gcd(0, 5) << endl;
	}

	{
	cerr << endl << "TESTS FOR int ..." << endl << endl;
	Rational<int> a(2,4);
	cerr << "a is ... " << a << endl;

	Rational<int> c(a);
	cerr << "copy a to c, c(a) is ... " << c << endl;

	Rational<int> b(3,7);
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

	Rational<int> beq(2);
	cerr << endl;
	int lbeq = 2;
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

	Rational<int> d(-2,4);
	cerr << endl;
	cerr << "d is ... " << d << endl;
	cerr << "+" << a << "=" << +d << endl;
	cerr << "-" << a << "=" << -d << endl;
	cerr << "--" << a << "=" << - -d << endl;
	cerr << "+-" << a << "=" << +-d << endl;

	cerr << endl;
	cerr << "0/4 is ... " << Rational<int>(0,4) << endl;
	cerr << "0/-4 is ... " << Rational<int>(0,-4) << endl;

	cerr << endl;
	cerr << "gcd(69,115) = " << Rational<int>::gcd(69, 115) << endl;
	cerr << "gcd(115,69) = " << Rational<int>::gcd(115, 69) << endl;
	cerr << "gcd(-69,115) = " << Rational<int>::gcd(-69, 115) << endl;
	cerr << "gcd(-115,69) = " << Rational<int>::gcd(-115, 69) << endl;
	cerr << "gcd(69,-115) = " << Rational<int>::gcd(69, -115) << endl;
	cerr << "gcd(115,-69) = " << Rational<int>::gcd(115, -69) << endl;
	cerr << "gcd(-69,-115) = " << Rational<int>::gcd(-69, -115) << endl;
	cerr << "gcd(-115,-69) = " << Rational<int>::gcd(-115, -69) << endl;
	cerr << "gcd(1,1) = " << Rational<int>::gcd(1, 1) << endl;
	cerr << "gcd(0,0) = " << Rational<int>::gcd(0, 0) << endl;
	cerr << "gcd(5,0) = " << Rational<int>::gcd(5, 0) << endl;
	cerr << "gcd(0,5) = " << Rational<int>::gcd(0, 5) << endl;
	}

	return(0);
}
