//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdlib.h>
#include <assert.h>
#include <iostream>

#include "numerics/UnsignedInteger.h"

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
	cerr << endl << "TESTS FOR UnsignedInteger - addition and subtraction ..." << endl;

	UnsignedInteger<50> bint0(65535);
	cerr << endl << "bint0 = " << bint0 << endl;

	UnsignedInteger<50> bint1;
	cerr << endl << "bint1 = " << bint1 << endl;

	UnsignedInteger<50>::Number n2(3);
	n2[0] = 10000;
	n2[1] = 0;
	n2[2] = 20000;
	UnsignedInteger<50> bint2(n2);
	cerr << endl << "bint2 = " << bint2 << endl;

	UnsignedInteger<50> bint3(bint2);
	cerr << endl << "bint3 = " << bint3 << endl;

	UnsignedInteger<50>::Number n4(3);
	n4[0] = 10000;
	n4[1] = 0;
	n4[2] = 20000;
	UnsignedInteger<50> bint4(n4);
	cerr << endl << "bint4 = " << bint4 << endl;

	UnsignedInteger<50>::Number n5(3);
	n5[0] = 60000;
	n5[1] = 60000;
	n5[2] = 60000;
	UnsignedInteger<50> bint5(n5);
	cerr << endl << "bint5 = " << bint5 << endl;

	bint4 += bint5;
	cerr << endl << "bint4+=bint5 = " << bint4 << endl;

	UnsignedInteger<50>::Number n6(3);
	n6[0] = 60000;
	n6[1] = 60000;
	n6[2] = 60000;
	UnsignedInteger<50> bint6(n6);
	cerr << endl << "bint6 = " << bint6 << endl;

	UnsignedInteger<50>::Number n7(3);
	n7[0] = 60000;
	n7[1] = 60000;
	n7[2] = 60000;
	UnsignedInteger<50> bint7(n7);
	cerr << endl << "bint7 = " << bint7 << endl;

	UnsignedInteger<50> bint8;
	cerr << endl << "bint8 = " << bint8 << endl;

	bint8 = bint6 + bint7;
	cerr << endl << "bint8 = bint6 + bint7 = " << bint8 << endl;

	bint8 = bint6 - bint7;
	cerr << endl << "bint8 = bint6 - bint7 = " << bint8 << endl;

	UnsignedInteger<50>::Number n9(4);
	n9[0] = 60000;
	n9[1] = 0;
	n9[2] = 20000;
	n9[3] = 1;
	UnsignedInteger<50> bint9(n9);
	cerr << endl << "bint9 = " << bint9 << endl;

	UnsignedInteger<50>::Number n10(3);
	n10[0] = 60000;
	n10[1] = 60000;
	n10[2] = 60000;
	UnsignedInteger<50> bint10(n10);
	cerr << endl << "bint10 = " << bint10 << endl;

	bint9 -= bint10;
	cerr << endl << "bint9-=bint10 = " << bint9 << endl;

	bint9 += bint10;
	cerr << endl << "bint9+=bint10 = " << bint9 << endl;

	bint9 -= bint9;
	cerr << endl << "bint9-=bint9 = " << bint9 << endl;

	UnsignedInteger<50>::Number n11(1);
	n11[0] = 2;
	UnsignedInteger<50> bint11(n11);
	cerr << endl << "bint11 = " << bint11 << endl;

	UnsignedInteger<50>::Number n12(1);
	n12[0] = 1;
	UnsignedInteger<50> bint12(n12);
	cerr << endl << "bint12 = " << bint12 << endl;

	try {
		cerr << endl << "bint12 - bint11 = " << endl;
		cerr << (bint12 - bint11) << endl;
		assert(0);
	} catch (const char *) {
		cerr << endl << "CAUGHT expected exception ..." << endl;
	}

	UnsignedInteger<50>::Number n13(10);
	n13[0] = 60000;
	n13[1] = 60000;
	n13[2] = 60000;
	n13[3] = 60000;
	n13[4] = 60000;
	n13[5] = 60000;
	n13[6] = 60000;
	n13[7] = 60000;
	n13[8] = 60000;
	n13[9] = 60000;
	UnsignedInteger<50> bint13(n13);
	cerr << endl << "bint13 = " << bint13 << endl;

	cerr << endl;
	for (int i=0; i<bint13.numberOfDigits(); ++i)
	{
		cerr << "bint13[" << i << "] = " << bint13[i] << endl;
	}

	UnsignedInteger<50> bint14;
	cerr << endl << "bint14 = " << bint14 << endl;

	bint14 = bint13 + bint13 + bint13;
	cerr << endl << "bint14 = bint13 + bint13 + bint13 = " << bint14 << endl;

	bint14 -= bint13;
	cerr << endl << "bint14 -= bint13 = " << bint14 << endl;

	bint14 -= bint13;
	cerr << endl << "bint14 -= bint13 = " << bint14 << endl;

	bint14 -= bint13;
	cerr << endl << "bint14 -= bint13 = " << bint14 << endl;

	cerr << endl << "bint14 == bint13 = " << (bint14 == bint13) << endl;
	cerr << endl << "bint14 != bint13 = " << (bint14 != bint13) << endl;
	cerr << endl << "bint14 < bint13 = " << (bint14 < bint13) << endl;
	cerr << endl << "bint14 > bint13 = " << (bint14 > bint13) << endl;
	cerr << endl << "bint14 <= bint13 = " << (bint14 <= bint13) << endl;
	cerr << endl << "bint14 >= bint13 = " << (bint14 >= bint13) << endl;

	cerr << endl << "bint14 == n13 = " << (bint14 == n13) << endl;
	cerr << endl << "bint14 != n13 = " << (bint14 != n13) << endl;
	cerr << endl << "bint14 < n13 = " << (bint14 < n13) << endl;
	cerr << endl << "bint14 > n13 = " << (bint14 > n13) << endl;
	cerr << endl << "bint14 <= n13 = " << (bint14 <= n13) << endl;
	cerr << endl << "bint14 >= n13 = " << (bint14 >= n13) << endl;

	UnsignedInteger<50>::Number n15(4);
	n15[0] = 60000;
	n15[1] = 60000;
	n15[2] = 60000;
	n15[3] = 60000;
	UnsignedInteger<50> bint15(n15);
	cerr << endl << "bint15 = " << bint15 << endl;

	UnsignedInteger<50>::Number n16(3);
	n16[0] = 10000;
	n16[1] = 20000;
	n16[2] = 30000;
	UnsignedInteger<50> bint16(n16);
	cerr << endl << "bint16 = " << bint16 << endl;

	UnsignedInteger<50> bint17();
	cerr << endl << "bint17 = " << bint17 << endl;

	cerr << endl << "bint15 + bint16 = " << (bint15 + bint16) << endl;
	cerr << endl << "bint15 + n16 = " << (bint15 + n16) << endl;
	cerr << endl << "bint15 - bint16 = " << (bint15 - bint16) << endl;

	cerr << endl << "bint15 - n16 = " << (bint15 - n16) << endl;
	cerr << endl << "bint15 - n16 + n16 - n15 = " << (bint15 - n16 + n16 - n15) << endl;
	return(0);
}
