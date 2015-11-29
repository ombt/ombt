//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdlib.h>
#include <assert.h>
#include <iostream>

#include <numerics/SignedInteger.h>

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
	cerr << endl << "TESTS FOR SignedInteger - multiplication and division ..." << endl;

////////////////////////////////////////////////////////////////////////////
	SignedInteger<50>::Number n1(2);
	n1[0] = 1;
	n1[1] = 1;
	SignedInteger<50> bint1(n1);
	cerr << endl << "bint1 = " << bint1 << endl;

	SignedInteger<50>::Number n2(2);
	n2[0] = 1;
	n2[1] = 1;
	SignedInteger<50> bint2(n2);
	cerr << endl << "bint2 = " << bint2 << endl;

	bint1 *= bint2;
	cerr << endl << "bint1*=bint2 = " << bint1 << endl;

////////////////////////////////////////////////////////////////////////////
	SignedInteger<50>::Number n3(4);
	n3[0] = 1;
	n3[1] = 1;
	n3[2] = 1;
	n3[3] = 1;
	SignedInteger<50> bint3(n3);
	cerr << endl << "bint3 = " << bint3 << endl;

	SignedInteger<50>::Number n4(4);
	n4[0] = 1;
	n4[1] = 1;
	n4[2] = 1;
	n4[3] = 1;
	SignedInteger<50> bint4(n4);
	cerr << endl << "bint4 = " << bint4 << endl;

	bint3 *= bint4;
	cerr << endl << "bint3*=bint4 = " << bint3 << endl;

////////////////////////////////////////////////////////////////////////////
	SignedInteger<50>::Number n5(1);
	n5[0] = 65535;
	SignedInteger<50> bint5(n5);
	cerr << endl << "bint5 = " << bint5 << endl;

	SignedInteger<50>::Number n6(1);
	n6[0] = 65535;
	SignedInteger<50> bint6(n6);
	cerr << endl << "bint6 = " << bint6 << endl;

	bint5 *= bint6;
	cerr << endl << "bint5*=bint6 = " << bint5 << endl;

////////////////////////////////////////////////////////////////////////////
	SignedInteger<50>::Number n7(2);
	n7[0] = 65535;
	n7[1] = 65535;
	SignedInteger<50> bint7(n7);
	cerr << endl << "bint7 = " << bint7 << endl;

	SignedInteger<50>::Number n8(1);
	n8[0] = 65535;
	SignedInteger<50> bint8(n8);
	cerr << endl << "bint8 = " << bint8 << endl;

	bint7 *= bint8;
	cerr << endl << "bint7*=bint8 = " << bint7 << endl;

	SignedInteger<50>::Number n9(1);
	n9[0] = 65535;
	SignedInteger<50> bint9(n9);
	cerr << endl << "bint9 = " << bint9 << endl;

	SignedInteger<50>::Number n10(2);
	n10[0] = 65535;
	n10[1] = 65535;
	SignedInteger<50> bint10(n10);
	cerr << endl << "bint10 = " << bint10 << endl;

	bint9 *= bint10;
	cerr << endl << "bint9*=bint10 = " << bint9 << endl;

////////////////////////////////////////////////////////////////////////////
	SignedInteger<50>::Number n11(3);
	n11[0] = 1;
	n11[1] = 65535;
	n11[2] = 1;
	SignedInteger<50> bint11(n11);
	cerr << endl << "bint11 = " << bint11 << endl;

	SignedInteger<50>::Number n12(3);
	n12[0] = 65535;
	n12[1] = 0;
	n12[2] = 65535;
	SignedInteger<50> bint12(n12);
	cerr << endl << "bint12 = " << bint12 << endl;

	bint11 *= bint12;
	cerr << endl << "bint11*=bint12 = " << bint11 << endl;

////////////////////////////////////////////////////////////////////////////
	SignedInteger<50>::Number n13(1);
	n13[0] = 10;
	SignedInteger<50> bint13(n13);
	cerr << endl << "bint13 = " << bint13 << endl;

	SignedInteger<50>::Number n14(1);
	n14[0] = 0;
	SignedInteger<50> bint14(n14);
	cerr << endl << "bint14 = " << bint14 << endl;

	cerr << endl << "bint13/=bint14 = " << endl;
	try {
		bint13 /= bint14;
		cerr << bint13 << endl;
		assert(0);
	} catch (const char *) {
		cerr << endl << "CAUGHT expected exception ..." << endl;
	}

////////////////////////////////////////////////////////////////////////////
	SignedInteger<50>::Number n15(2);
	n15[0] = 100;
	n15[1] = 100;
	SignedInteger<50> bint15(n15);
	cerr << endl << "bint15 = " << bint15 << endl;

	SignedInteger<50>::Number n16(1);
	n16[0] = 10;
	SignedInteger<50> bint16(n16);
	cerr << endl << "bint16 = " << bint16 << endl;

	bint15 /= bint16;
	cerr << endl << "bint15/=bint16 = " << bint15 << endl;

////////////////////////////////////////////////////////////////////////////
	SignedInteger<50>::Number n17(3);
	n17[0] = 65535;
	n17[1] = 100;
	n17[2] = 65535;
	SignedInteger<50> bint17(n17);
	cerr << endl << "bint17 = " << bint17 << endl;

	SignedInteger<50>::Number n18(2);
	n18[0] = 10;
	n18[1] = 535;
	SignedInteger<50> bint18(n18);
	cerr << endl << "bint18 = " << bint18 << endl;

	SignedInteger<50> bint19;
	cerr << endl << "bint19 = " << bint19 << endl;

	SignedInteger<50> bint20;
	cerr << endl << "bint20 = " << bint20 << endl;

	bint19 = bint17 / bint18;
	cerr << endl << "bint19 = bint17/bint18 = " << bint19 << endl;

	bint20 = bint17 - bint19*bint18;
	cerr << endl << "bint20 = bint17 - bint19*bint18 = " << bint20 << endl;

	cerr << endl << "bint17 = " << bint17 << endl;
	cerr << endl << "bint19*bint18+bint20= " << (bint19*bint18+bint20) << endl;

////////////////////////////////////////////////////////////////////////////
	SignedInteger<50>::Number n21(20);
	for (int i=0; i<20; ++i)
	{
		n21[i] = i*i;
	}
	SignedInteger<50> bint21(n21);
	cerr << endl << "bint21 = " << bint21 << endl;

	SignedInteger<50>::Number n22(10);
	for (int i=0; i<10; ++i)
	{
		n22[i] = 2*i;
	}
	SignedInteger<50> bint22(n22, -1);
	cerr << endl << "bint22 = " << bint22 << endl;

	SignedInteger<50> bint23;
	cerr << endl << "bint23 = " << bint23 << endl;

	SignedInteger<50> bint24;
	cerr << endl << "bint24 = " << bint24 << endl;

	bint23 = bint21 / bint22;
	cerr << endl << "bint23 = bint21/bint22 = " << bint23 << endl;

	bint24 = bint21 - bint23*bint22;
	cerr << endl << "bint24 = bint21 - bint23*bint22 = " << bint24 << endl;

	cerr << endl << "bint21 = " << bint21 << endl;
	cerr << endl << "bint23*bint22+bint24 = " << (bint23*bint22+bint24) << endl;

	return(0);
}
