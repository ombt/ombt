#include "term.h"

main(int argc, char **argv)
{
	Term t(argv[1]);
	cout << "term is ... " << t << endl;
	return(0);
}
