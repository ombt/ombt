// convert a key to a number

// headers
#include <sysent.h>
#include <stdlib.h>
#include <iostream.h>

// base for arithmetic
const int base = 7;

// convert pass word to a number
unsigned long
MYatoi(char *pw)
{
	// find end of pass word
	char *pwend = pw + strlen(pw) - 1;

	// convert to a number
	int key = 0;
	int multiplier = 1;
	for ( ; pwend >= pw; pwend--, multiplier*=base)
	{
		key += (*pwend)*multiplier;
	}

	// return key
	return(key|1);
}

// main entry point
main(int argc, char **argv)
{
	for (int arg = 1; arg < argc; arg++)
	{
		cout << "KEY: " << argv[arg] << " ===>>> NUM: ";
		cout << MYatoi(argv[arg]) << endl;
	}
	return(0);
}
