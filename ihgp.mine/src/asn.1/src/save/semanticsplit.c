// code for semantic record classes

// headers
#include "semantic.h"

// accessing array
#define prog0 0
#define prog1 1
#define prog2 2
#define prog3 3
#define prog4 4
#define prog5 5
#define prog6 6
#define prog7 7
#define prog8 8
#define prog9 9
#define prog10 10
#define arraysz(p) ((p)+1)

// split problems
int
Semantic::split(const String &rtype, String &andor, 
		Array<List<Semantic * > > &asubc, 
		Array<List<Semantic * > > &asubx) const
{
	// check if we reducing the correct types
	if (!isExpression() && !isPredicate())
		return(NOTOK);

	// determine reducing strategy
	if (rtype == String("bledsoe"))
	{
		return(bledsoe(andor, asubc, asubx));
	}
	else if (rtype == String("schaum"))
	{
		return(schaum(andor, asubc, asubx));
	}
	else if (rtype == String("cp"))
	{
		return(cp(andor, asubc, asubx));
	}
	else if (rtype == String("none"))
	{
		return(none(andor, asubc, asubx, "none", "%p0"));
	}
	else
	{
		ERRORD("invalid reduction type", rtype, EINVAL);
		return(NOTOK);
	}
}

// different reduction strategies
int
Semantic::bledsoe(String &andor, Array<List<Semantic * > > &asubc, 
		Array<List<Semantic * > > &asubx) const
{
	// check the type of record
	if (isPredicate())
	{
		return(none(andor, asubc, asubx, "bledsoe", "%p0"));
	}
	else if (isExpression())
	{
		switch (type)
		{
		case And:
			return(bledsoe_and(andor, asubc, asubx, "bledsoe"));
		case Or:
			return(bledsoe_or(andor, asubc, asubx, "bledsoe"));
		case Conditional:
			return(bledsoe_conditional(andor, asubc, asubx, "bledsoe"));
		case Biconditional:
			return(bledsoe_biconditional(andor, asubc, asubx, "bledsoe"));
		default:
			return(none(andor, asubc, asubx, "bledsoe", "%p0"));
		}
	}

	// all done
	ERRORD("bledsoe called with wrong type.", type, EINVAL);
	return(NOTOK);
}

int
Semantic::schaum(String &andor, Array<List<Semantic * > > &asubc, 
		Array<List<Semantic * > > &asubx) const
{
	// check the type of record
	if (isPredicate())
	{
		return(none(andor, asubc, asubx, "schaum", "%p0"));
	}
	else if (isExpression())
	{
		switch (type)
		{
		case And:
			return(schaum_and(andor, asubc, asubx, "schaum"));
		case Or:
			return(schaum_or(andor, asubc, asubx, "schaum"));
		case Conditional:
			return(schaum_conditional(andor, asubc, asubx, "schaum"));
		case Biconditional:
			return(schaum_biconditional(andor, asubc, asubx, "schaum"));
		default:
			return(none(andor, asubc, asubx, "schaum", "%p0"));
		}
	}

	// all done
	ERRORD("schaum called with wrong type.", type, EINVAL);
	return(NOTOK);
}

int
Semantic::cp(String &andor, Array<List<Semantic * > > &asubc, 
		Array<List<Semantic * > > &asubx) const
{
	// check the type of record
	if (isPredicate())
	{
		return(none(andor, asubc, asubx, "cp", "%p0"));
	}
	else if (isExpression())
	{
		switch (type)
		{
		case Conditional:
			return(cp_conditional(andor, asubc, asubx, "cp"));
		case Biconditional:
			return(cp_biconditional(andor, asubc, asubx, "cp"));
		default:
			return(none(andor, asubc, asubx, "cp", "%p0"));
		}
	}

	// all done
	ERRORD("contrapositive called with wrong type.", type, EINVAL);
	return(NOTOK);
}

// default split rule ... no splitting, just take problem as is.
int
Semantic::none(String &andor, Array<List<Semantic * > > &asubc, 
		Array<List<Semantic * > > &asubx, 
		const String &rtype, const String &pname) const
{
	// set and/or instructions
	andor = pname;

	// copy conclusion
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// set size of arrays
	asubx.setSize(arraysz(prog0));
	asubc.setSize(arraysz(prog0));

	// store conclusion data
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pc) == OK);

	// all done
	return(OK);
}

// set all options in array
void
Semantic::setOptions(Array<List<Semantic * > > &asubx) const
{
	int imax = asubx.getSize();
	for (int i=0; i < imax; i++)
	{
		ListIterator<Semantic * > ai(asubx[i]);
		for ( ; !ai.done(); ai++)
		{
			ai()->setOptions(getOptions());
		}
	}
	return;
}

//
// split the problem with an AND as follows:
//
// 	axioms;
//	|- A && B;
//
//	becomes:
//
//	AND/OR graph: (p0 && p1) || p2
//
//	p0) axioms, |- A;
//	p1) axioms, |- B;
//	p2) axioms, |- A && B;
//
#define BLEDSOE_AND_ANDOR "(%p0 && %p1) || %p2"

int
Semantic::bledsoe_and(String &andor, Array<List<Semantic * > > &asubc, 
		Array<List<Semantic * > > &asubx, const String &rtype) const
{
	// set and/or string
	andor = BLEDSOE_AND_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog2));
	asubc.setSize(arraysz(prog2));

	// copy records
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pA) == OK);
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pB) == OK);
	asubx[prog2].clear();
	MustBeTrue(asubc[prog2].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with an OR as follows:
//
// 	axioms;
//	|- A || B;
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2
//
//	p0) axioms, |- A;
//	p1) axioms, |- B;
//	p2) axioms, |- A || B;
//
#define BLEDSOE_OR_ANDOR "%p0 || %p1 || %p2"

int
Semantic::bledsoe_or(String &andor, Array<List<Semantic * > > &asubc, 
		Array<List<Semantic * > > &asubx, 
		const String &rtype) const
{
	// set and/or string
	andor = BLEDSOE_OR_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog2));
	asubc.setSize(arraysz(prog2));

	// copy records
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pA) == OK);
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pB) == OK);
	asubx[prog2].clear();
	MustBeTrue(asubc[prog2].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// the default case for splitting a conditional is 
// as follows. there are special cases which are 
// handled differently.
//
// 	axioms;
//	|- A --> B;
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2 || p3
//
//	p0) axioms, |- ~A;
//	p1) axioms, |- B;
//	p2) axioms, A |- B;
//	p3) axioms, |- A --> B;
//
// special cases which are handled separately:
//
// 	axioms, |- P --> (A --> B);
//	axioms, |- P --> (A && B);
//	axioms, |- (P || Q) --> A;
//
#define BLEDSOE_CONDITIONAL_ANDOR "%p0 || %p1 || %p2 || %p3"

int
Semantic::bledsoe_conditional(String &andor, Array<List<Semantic * > > &asubc, 
		Array<List<Semantic * > > &asubx, const String &rtype) const
{
	// set and/or string
	andor = BLEDSOE_CONDITIONAL_ANDOR;

	// check for special cases
	if (right->type == Conditional)
		return(bledsoe_conditional_cond(andor, asubc, asubx, rtype));
	else if (right->type == And)
		return(bledsoe_conditional_and(andor, asubc, asubx, rtype));
	else if (right->type == Or)
		return(bledsoe_conditional_or(andor, asubc, asubx, rtype));

	// set size of arrays
	asubx.setSize(arraysz(prog3));
	asubc.setSize(arraysz(prog3));

	// construct p0
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);

	// store p0
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pnA) == OK);

	// construct p1
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p1
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pB) == OK);

	// construct p2
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p2
	MustBeTrue(asubx[prog2].insertAtEnd(pA) == OK);
	MustBeTrue(asubc[prog2].insertAtEnd(pB) == OK);

	// construct p3
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog3].clear();
	MustBeTrue(asubc[prog3].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with a conditional and an OR:
//
// 	axioms;
//	|- (P || Q) --> A
//
//	becomes:
//
//	AND/OR graph: ((p0||p1||p2||p3) && (p4||p5||p6||p7)) || p8
//
//	p0) axioms, |- ~P;
//	p1) axioms, |- A;
//	p2) axioms, P |- A;
//	p3) axioms, |- P --> A;
//	p4) axioms, |- ~Q;
//	p5) axioms, |- A;
//	p6) axioms, Q |- A;
//	p7) axioms, |- Q --> A;
//	p8) axioms, |- (P || Q) --> A;
//
#define BLEDSOE_CONDITIONAL_OR_ANDOR \
	"((%p0 || %p1 || %p2 || %p3) && (%p4 || %p5 || %p6 || %p7)) || %p8"

int
Semantic::bledsoe_conditional_or(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = BLEDSOE_CONDITIONAL_OR_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog8));
	asubc.setSize(arraysz(prog8));

	// construct p0
	Semantic *pP = new Semantic(*(left->left));
	MustBeTrue(pP != NULL);
	Semantic *pnP = new Semantic(Negation, NULL, pP);
	MustBeTrue(pnP != NULL);

	// store p0
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pnP) == OK);

	// construct p1
	Semantic *pA = new Semantic(*right);
	MustBeTrue(pA != NULL);

	// store p1
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pA) == OK);

	// constructing p2
	pP = new Semantic(*(left->left));
	MustBeTrue(pP != NULL);
	pA = new Semantic(*right);
	MustBeTrue(pA != NULL);

	// store p2
	MustBeTrue(asubx[prog2].insertAtEnd(pP) == OK);
	MustBeTrue(asubc[prog2].insertAtEnd(pA) == OK);

	// constructing p3
	pP = new Semantic(*(left->left));
	MustBeTrue(pP != NULL);
	pA = new Semantic(*right);
	MustBeTrue(pA != NULL);
	Semantic *pPtoA = new Semantic(Conditional, pP, pA);
	MustBeTrue(pPtoA != NULL);

	// store p3
	asubx[prog3].clear();
	MustBeTrue(asubc[prog3].insertAtEnd(pPtoA) == OK);

	// construct p4
	Semantic *pQ = new Semantic(*(left->right));
	MustBeTrue(pQ != NULL);
	Semantic *pnQ = new Semantic(Negation, NULL, pQ);
	MustBeTrue(pnQ != NULL);

	// store p4
	asubx[prog4].clear();
	MustBeTrue(asubc[prog4].insertAtEnd(pnQ) == OK);

	// construct p5
	pA = new Semantic(*right);
	MustBeTrue(pA != NULL);

	// store p5
	asubx[prog5].clear();
	MustBeTrue(asubc[prog5].insertAtEnd(pA) == OK);

	// constructing p6
	pQ = new Semantic(*(left->right));
	MustBeTrue(pQ != NULL);
	pA = new Semantic(*right);
	MustBeTrue(pA != NULL);

	// store p6
	MustBeTrue(asubx[prog6].insertAtEnd(pQ) == OK);
	MustBeTrue(asubc[prog6].insertAtEnd(pA) == OK);

	// constructing p7
	pQ = new Semantic(*(left->right));
	MustBeTrue(pQ != NULL);
	pA = new Semantic(*right);
	MustBeTrue(pA != NULL);
	Semantic *pQtoA = new Semantic(Conditional, pQ, pA);
	MustBeTrue(pQtoA != NULL);

	// store p7
	asubx[prog7].clear();
	MustBeTrue(asubc[prog7].insertAtEnd(pQtoA) == OK);

	// construct p8
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog8].clear();
	MustBeTrue(asubc[prog8].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with a conditional and an OR:
//
// 	axioms;
//	|- P --> (A && B);
//
//	becomes:
//
//	AND/OR graph: ((p0||p1||p2||p3) && (p4||p5||p6||p7)) || p8
//
//	p0) axioms, |- ~P;
//	p1) axioms, |- A;
//	p2) axioms, P |- A;
//	p3) axioms, |- P --> A;
//	p4) axioms, |- ~P;
//	p5) axioms, |- B;
//	p6) axioms, P |- B;
//	p7) axioms, |- P --> B;
//	p8) axioms, |- P --> (A && B);
//

#define BLEDSOE_CONDITIONAL_AND_ANDOR \
	"((%p0 || %p1 || %p2 || %p3) && (%p4 || %p5 || %p6 || %p7)) || %p8"

int
Semantic::bledsoe_conditional_and(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = BLEDSOE_CONDITIONAL_AND_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog8));
	asubc.setSize(arraysz(prog8));

	// construct p0
	Semantic *pP = new Semantic(*left);
	MustBeTrue(pP != NULL);
	Semantic *pnP = new Semantic(Negation, NULL, pP);
	MustBeTrue(pnP != NULL);

	// store p0
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pnP) == OK);

	// construct p1
	Semantic *pA = new Semantic(*(right->left));
	MustBeTrue(pA != NULL);

	// store p1
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pA) == OK);

	// constructing p2
	pP = new Semantic(*left);
	MustBeTrue(pP != NULL);
	pA = new Semantic(*(right->left));
	MustBeTrue(pA != NULL);

	// store p2
	MustBeTrue(asubx[prog2].insertAtEnd(pP) == OK);
	MustBeTrue(asubc[prog2].insertAtEnd(pA) == OK);

	// constructing p3
	pP = new Semantic(*left);
	MustBeTrue(pP != NULL);
	pA = new Semantic(*(right->left));
	MustBeTrue(pA != NULL);
	Semantic *pPtoA = new Semantic(Conditional, pP, pA);
	MustBeTrue(pPtoA != NULL);

	// store p3
	asubx[prog3].clear();
	MustBeTrue(asubc[prog3].insertAtEnd(pPtoA) == OK);

	// construct p4
	pP = new Semantic(*left);
	MustBeTrue(pP != NULL);
	pnP = new Semantic(Negation, NULL, pP);
	MustBeTrue(pnP != NULL);

	// store p4
	asubx[prog4].clear();
	MustBeTrue(asubc[prog4].insertAtEnd(pnP) == OK);

	// construct p5
	Semantic *pB = new Semantic(*(right->right));
	MustBeTrue(pB != NULL);

	// store p5
	asubx[prog5].clear();
	MustBeTrue(asubc[prog5].insertAtEnd(pB) == OK);

	// constructing p6
	pP = new Semantic(*left);
	MustBeTrue(pP != NULL);
	pB = new Semantic(*(right->right));
	MustBeTrue(pB != NULL);

	// store p6
	MustBeTrue(asubx[prog6].insertAtEnd(pP) == OK);
	MustBeTrue(asubc[prog6].insertAtEnd(pB) == OK);

	// constructing p7
	pP = new Semantic(*left);
	MustBeTrue(pP != NULL);
	pB = new Semantic(*(right->left));
	MustBeTrue(pB != NULL);
	Semantic *pPtoB = new Semantic(Conditional, pP, pB);
	MustBeTrue(pPtoB != NULL);

	// store p7
	asubx[prog7].clear();
	MustBeTrue(asubc[prog7].insertAtEnd(pPtoB) == OK);

	// construct p8
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog8].clear();
	MustBeTrue(asubc[prog8].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with a conditional and an conditional:
//
// 	axioms;
//	|- P --> (A --> B)
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2 || p3 || p4 || p5
//
//	p0) axioms, |- ~P;
//	p1) axioms, |- ~A;
//	p2) axioms, |- B;
//	p3) axioms, P && A |- B;
//	p4) axioms, |- (P && A) --> B;
//	p5) axioms, |- P --> (A --> B);
//

#define BLEDSOE_CONDITIONAL_COND_ANDOR \
	"%p0 || %p1 || %p2 || %p3 || %p4 || %p5"

int
Semantic::bledsoe_conditional_cond(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = BLEDSOE_CONDITIONAL_COND_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog5));
	asubc.setSize(arraysz(prog5));

	// construct p0
	Semantic *pP = new Semantic(*left);
	MustBeTrue(pP != NULL);
	Semantic *pnP = new Semantic(Negation, NULL, pP);
	MustBeTrue(pnP != NULL);

	// store p0
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pnP) == OK);

	// construct p1
	Semantic *pA = new Semantic(*(right->left));
	MustBeTrue(pA != NULL);
	Semantic *pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);

	// store p1
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pnA) == OK);

	// constructing p2
	Semantic *pB = new Semantic(*(right->right));
	MustBeTrue(pB != NULL);

	// store p2
	asubx[prog2].clear();
	MustBeTrue(asubc[prog2].insertAtEnd(pB) == OK);

	// constructing p3
	pP = new Semantic(*left);
	MustBeTrue(pP != NULL);
	pA = new Semantic(*(right->left));
	MustBeTrue(pA != NULL);
	Semantic *pPtoA = new Semantic(And, pP, pA);
	MustBeTrue(pPtoA != NULL);
	pB = new Semantic(*(right->right));
	MustBeTrue(pB != NULL);

	// store p3
	MustBeTrue(asubx[prog3].insertAtEnd(pPtoA) == OK);
	MustBeTrue(asubc[prog3].insertAtEnd(pB) == OK);

	// constructing p4
	pP = new Semantic(*left);
	MustBeTrue(pP != NULL);
	pA = new Semantic(*(right->left));
	MustBeTrue(pA != NULL);
	pPtoA = new Semantic(And, pP, pA);
	MustBeTrue(pPtoA != NULL);
	pB = new Semantic(*(right->right));
	MustBeTrue(pB != NULL);
	Semantic *pPAtoB = new Semantic(Conditional, pPtoA, pB);
	MustBeTrue(pPAtoB != NULL);

	// store p4
	asubx[prog4].clear();
	MustBeTrue(asubc[prog4].insertAtEnd(pPAtoB) == OK);

	// construct p5
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog5].clear();
	MustBeTrue(asubc[prog5].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with a biconditional as follows:
//
// 	axioms;
//	|- A <--> B;
//
//	becomes:
//
//	AND/OR graph: 
//	(p0 && p1) || (p2 && p3) || ((p4 || p5) && (p6 || p7)) || p8
//
//	p0) axioms, |- A --> B;
//	p1) axioms, |- B --> A;
//	p2) axioms, A |- B;
//	p3) axioms, B |- A;
//	p4) axioms, |- ~A;
//	p5) axioms, |- B;
//	p6) axioms, |- ~B;
//	p7) axioms, |- A;
//	p8) axioms, |- A --> B;
//
#define BLEDSOE_BICONDITIONAL_ANDOR \
	"(%p0 && %p1) || (%p2 && %p3) || ((%p4 || %p5) && (%p6 || %p7)) || %p8"

int
Semantic::bledsoe_biconditional(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = BLEDSOE_BICONDITIONAL_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog8));
	asubc.setSize(arraysz(prog8));

	// construct p0
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pAB = new Semantic(Conditional, pA, pB);
	MustBeTrue(pAB != NULL);

	// store p0
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pAB) == OK);

	// construct p1
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	pAB = new Semantic(Conditional, pB, pA);
	MustBeTrue(pAB != NULL);

	// store p1
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pAB) == OK);

	// constructing p2
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p2
	MustBeTrue(asubx[prog2].insertAtEnd(pA) == OK);
	MustBeTrue(asubc[prog2].insertAtEnd(pB) == OK);

	// constructing p3
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p3
	MustBeTrue(asubx[prog3].insertAtEnd(pB) == OK);
	MustBeTrue(asubc[prog3].insertAtEnd(pA) == OK);

	// construct p4
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);

	// store p4
	asubx[prog4].clear();
	MustBeTrue(asubc[prog4].insertAtEnd(pnA) == OK);

	// construct p5
	pB = new Semantic(*left);
	MustBeTrue(pB != NULL);

	// store p5
	asubx[prog5].clear();
	MustBeTrue(asubc[prog5].insertAtEnd(pB) == OK);

	// construct p6
	pB = new Semantic(*left);
	MustBeTrue(pB != NULL);
	Semantic *pnB = new Semantic(Negation, NULL, pB);
	MustBeTrue(pnB != NULL);

	// store p6
	asubx[prog6].clear();
	MustBeTrue(asubc[prog6].insertAtEnd(pnB) == OK);

	// construct p7
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);

	// store p7
	asubx[prog7].clear();
	MustBeTrue(asubc[prog7].insertAtEnd(pA) == OK);

	// construct p8
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog8].clear();
	MustBeTrue(asubc[prog8].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with an AND as follows:
//
// 	axioms;
//	|- A && B;
//
//	becomes:
//
//	AND/OR graph: (p0 && p1) || p2
//
//	p0) axioms, |- A;
//	p1) axioms, |- B;
//	p2) axioms, |- A && B;
//
#define SCHAUM_AND_ANDOR "(%p0 && %p1) || %p2"

int
Semantic::schaum_and(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = SCHAUM_AND_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog2));
	asubc.setSize(arraysz(prog2));

	// copy records
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pA) == OK);
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pB) == OK);
	asubx[prog2].clear();
	MustBeTrue(asubc[prog2].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with an OR as follows:
//
// 	axioms;
//	|- A || B;
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2
//
//	p0) axioms, |- A;
//	p1) axioms, |- B;
//	p2) axioms, |- A || B;
//
#define SCHAUM_OR_ANDOR "%p0 || %p1 || %p2"

int
Semantic::schaum_or(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = SCHAUM_OR_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog2));
	asubc.setSize(arraysz(prog2));

	// copy records
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pA) == OK);
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pB) == OK);
	asubx[prog2].clear();
	MustBeTrue(asubc[prog2].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with a conditional as follows:
//
// 	axioms;
//	|- A --> B;
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2 || p3 || p4
//
//	p0) axioms, |- ~A;
//	p1) axioms, |- B;
//	p2) axioms, A |- B;
//	p3) axioms, ~B |- ~A (follows from contrapositive)
//	p4) axioms, |- A --> B;
//
#define SCHAUM_CONDITIONAL_ANDOR "%p0 || %p1 || %p2 || %p3 || %p4"

int
Semantic::schaum_conditional(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = SCHAUM_CONDITIONAL_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog4));
	asubc.setSize(arraysz(prog4));

	// construct p0
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);

	// store p0
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pnA) == OK);

	// construct p1
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p1
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pB) == OK);

	// construct p2
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p2
	MustBeTrue(asubx[prog2].insertAtEnd(pA) == OK);
	MustBeTrue(asubc[prog2].insertAtEnd(pB) == OK);

	// construct p3
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pnB = new Semantic(Negation, NULL, pB);
	MustBeTrue(pnB != NULL);

	// store p3
	MustBeTrue(asubx[prog3].insertAtEnd(pnB) == OK);
	MustBeTrue(asubc[prog3].insertAtEnd(pnA) == OK);

	// construct p4
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog4].clear();
	MustBeTrue(asubc[prog4].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with a biconditional as follows:
//
// 	axioms;
//	|- A <--> B;
//
//	becomes:
//
//	AND/OR graph: 
//	(p0 && p1) || (p2 && p3) || ((p4 || p5) && (p6 || p7)) || p8
//
//	p0) axioms, |- A --> B;
//	p1) axioms, |- B --> A;
//	p2) axioms, A |- B;
//	p3) axioms, B |- A;
//	p4) axioms, |- ~A;
//	p5) axioms, |- B;
//	p6) axioms, |- ~B;
//	p7) axioms, |- A;
//	p8) axioms, |- A <--> B;
//
#define SCHAUM_BICONDITIONAL_ANDOR \
	"(%p0 && %p1) || (%p2 && %p3) || ((%p4 || %p5) && (%p6 || %p7)) || %p8"

int
Semantic::schaum_biconditional(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = SCHAUM_BICONDITIONAL_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog8));
	asubc.setSize(arraysz(prog8));

	// construct p0
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pAB = new Semantic(Conditional, pA, pB);
	MustBeTrue(pAB != NULL);

	// store p0
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pAB) == OK);

	// construct p1
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	pAB = new Semantic(Conditional, pB, pA);
	MustBeTrue(pAB != NULL);

	// store p1
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pAB) == OK);

	// constructing p2
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p2
	MustBeTrue(asubx[prog2].insertAtEnd(pA) == OK);
	MustBeTrue(asubc[prog2].insertAtEnd(pB) == OK);

	// constructing p3
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p3
	MustBeTrue(asubx[prog3].insertAtEnd(pB) == OK);
	MustBeTrue(asubc[prog3].insertAtEnd(pA) == OK);

	// construct p4
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);

	// store p4
	asubx[prog4].clear();
	MustBeTrue(asubc[prog4].insertAtEnd(pnA) == OK);

	// construct p5
	pB = new Semantic(*left);
	MustBeTrue(pB != NULL);

	// store p5
	asubx[prog5].clear();
	MustBeTrue(asubc[prog5].insertAtEnd(pB) == OK);

	// construct p6
	pB = new Semantic(*left);
	MustBeTrue(pB != NULL);
	Semantic *pnB = new Semantic(Negation, NULL, pB);
	MustBeTrue(pnB != NULL);

	// store p6
	asubx[prog6].clear();
	MustBeTrue(asubc[prog6].insertAtEnd(pnB) == OK);

	// construct p7
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);

	// store p7
	asubx[prog7].clear();
	MustBeTrue(asubc[prog7].insertAtEnd(pA) == OK);

	// construct p8
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog8].clear();
	MustBeTrue(asubc[prog8].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with a conditional as follows:
//
// 	axioms;
//	|- A --> B;
//
//	becomes:
//
//	AND/OR graph: p0 || p1 || p2 || p3 || p4
//
//	p0) axioms, |- ~A;
//	p1) axioms, |- B;
//	p2) axioms, A |- B;
//	p3) axioms, ~B |- ~A (follows from contrapositive)
//	p4) axioms, |- A --> B;
//
#define CP_CONDITIONAL_ANDOR "%p0 || %p1 || %p2 || %p3 || %p4"

int
Semantic::cp_conditional(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = CP_CONDITIONAL_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog4));
	asubc.setSize(arraysz(prog4));

	// construct p0
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);

	// store p0
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pnA) == OK);

	// construct p1
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p1
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pB) == OK);

	// construct p2
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p2
	MustBeTrue(asubx[prog2].insertAtEnd(pA) == OK);
	MustBeTrue(asubc[prog2].insertAtEnd(pB) == OK);

	// construct p3
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pnB = new Semantic(Negation, NULL, pB);
	MustBeTrue(pnB != NULL);

	// store p3
	MustBeTrue(asubx[prog3].insertAtEnd(pnB) == OK);
	MustBeTrue(asubc[prog3].insertAtEnd(pnA) == OK);

	// construct p4
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog4].clear();
	MustBeTrue(asubc[prog4].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

//
// split the problem with a biconditional as follows:
//
// 	axioms;
//	|- A <--> B;
//
//	becomes:
//
//	AND/OR graph: 
//	(p0 && p1)||(p2 && p3)||(p4 && p5)||((p6 || p7) && (p8 || p9))||p10
//
//	p0) axioms, |- A --> B;
//	p1) axioms, |- B --> A;
//	p2) axioms, A |- B;
//	p3) axioms, B |- A;
//	p4) axioms, ~A |- ~B;
//	p5) axioms, ~B |- ~A;
//	p6) axioms, |- ~A;
//	p7) axioms, |- B;
//	p8) axioms, |- ~B;
//	p9) axioms, |- A;
//	p10) axioms, |- A <--> B;
//
#define CP_BICONDITIONAL_ANDOR \
	"(%p0 && %p1) || (%p2 && %p3) || (%p4 && %p5) || ((%p6 || %p7) && (%p8 || %p9)) || %p10"

int
Semantic::cp_biconditional(String &andor, 
	Array<List<Semantic * > > &asubc, Array<List<Semantic * > > &asubx, 
	const String &rtype) const
{
	// set and/or string
	andor = CP_BICONDITIONAL_ANDOR;

	// set size of arrays
	asubx.setSize(arraysz(prog10));
	asubc.setSize(arraysz(prog10));

	// construct p0
	Semantic *pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pAB = new Semantic(Conditional, pA, pB);
	MustBeTrue(pAB != NULL);

	// store p0
	asubx[prog0].clear();
	MustBeTrue(asubc[prog0].insertAtEnd(pAB) == OK);

	// construct p1
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	pAB = new Semantic(Conditional, pB, pA);
	MustBeTrue(pAB != NULL);

	// store p1
	asubx[prog1].clear();
	MustBeTrue(asubc[prog1].insertAtEnd(pAB) == OK);

	// constructing p2
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p2
	MustBeTrue(asubx[prog2].insertAtEnd(pA) == OK);
	MustBeTrue(asubc[prog2].insertAtEnd(pB) == OK);

	// constructing p3
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);

	// store p3
	MustBeTrue(asubx[prog3].insertAtEnd(pB) == OK);
	MustBeTrue(asubc[prog3].insertAtEnd(pA) == OK);

	// constructing p4
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	Semantic *pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	Semantic *pnB = new Semantic(Negation, NULL, pB);
	MustBeTrue(pnB != NULL);

	// store p4
	MustBeTrue(asubx[prog4].insertAtEnd(pnA) == OK);
	MustBeTrue(asubc[prog4].insertAtEnd(pnB) == OK);

	// constructing p5
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);
	pB = new Semantic(*right);
	MustBeTrue(pB != NULL);
	pnB = new Semantic(Negation, NULL, pB);
	MustBeTrue(pnB != NULL);

	// store p5
	MustBeTrue(asubx[prog5].insertAtEnd(pnB) == OK);
	MustBeTrue(asubc[prog5].insertAtEnd(pnA) == OK);

	// construct p6
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);
	pnA = new Semantic(Negation, NULL, pA);
	MustBeTrue(pnA != NULL);

	// store p6
	asubx[prog6].clear();
	MustBeTrue(asubc[prog6].insertAtEnd(pnA) == OK);

	// construct p7
	pB = new Semantic(*left);
	MustBeTrue(pB != NULL);

	// store p7
	asubx[prog7].clear();
	MustBeTrue(asubc[prog7].insertAtEnd(pB) == OK);

	// construct p8
	pB = new Semantic(*left);
	MustBeTrue(pB != NULL);
	pnB = new Semantic(Negation, NULL, pB);
	MustBeTrue(pnB != NULL);

	// store p8
	asubx[prog8].clear();
	MustBeTrue(asubc[prog8].insertAtEnd(pnB) == OK);

	// construct p9
	pA = new Semantic(*left);
	MustBeTrue(pA != NULL);

	// store p9
	asubx[prog9].clear();
	MustBeTrue(asubc[prog9].insertAtEnd(pA) == OK);

	// construct p10
	Semantic *pc = new Semantic(*this);
	MustBeTrue(pc != NULL);

	// store conclusion data
	asubx[prog10].clear();
	MustBeTrue(asubc[prog10].insertAtEnd(pc) == OK);

	// set options in conclusion list
	setOptions(asubc);

	// all done
	return(OK);
}

// accessing array
#undef prog0
#undef prog1
#undef prog2
#undef prog3
#undef prog4
#undef prog5
#undef prog6
#undef prog7
#undef prog8
#undef prog9
#undef prog10
#undef arraysz
