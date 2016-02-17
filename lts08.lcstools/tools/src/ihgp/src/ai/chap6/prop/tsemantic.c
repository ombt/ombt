// test for semantic record

// headers
#include "semantic.h"

// test cases
main()
{
	Semantic *pa1 = new Semantic(Atomic::Constant, NULL, "true");
	Semantic *pa2 = new Semantic(Atomic::Variable, "A", NULL);
	Semantic pe1(Expression::And, pa1, pa2, 1);
	return(0);
}
