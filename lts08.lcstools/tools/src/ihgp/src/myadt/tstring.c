// test for string

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "string.h"

main(int argc, char **argv)
{
        cout << "String s1(argv[1]);" << endl;
        String s1(argv[1]);
	cout << "s1 = " << s1 << endl;

        cout << "String s2(argv[2]);" << endl;
        String s2(argv[2]);
	cout << "s2 = " << s2 << endl;

	cout << "s1 += s1;" << endl;
	s1 += s1;
	cout << s1 << endl;
	cout << "s2 += s2;" << endl;
	s2 += s2;
	cout << s2 << endl;

	cout << "(s1 + s2)" << endl;
	cout << (s1 + s2) << endl;

	cout << "(s1 + s2)(10, 20)" << endl;
	cout << (s1 + s2)(10, 20) << endl;

	cout << "s1 + \"abc\";" << endl;
	cout << "123" + s1 + "abc" << endl;

	return(0);
}
