// code for semantic record classes

// headers
#include "semantic.h"

// semantic record representation constructors and destructor
SemanticRep::SemanticRep(int c):
	conclusion(c)
{
	// nothing to do here
}

SemanticRep::SemanticRep(const SemanticRep &src):
	conclusion(src.conclusion)
{
	// nothing to do here
}

SemanticRep::~SemanticRep()
{
	conclusion = 0;
}

// assignment operator
SemanticRep &
SemanticRep::operator=(const SemanticRep &rhs)
{
	// check for self-assignment
	if (this != &rhs)
		conclusion = rhs.conclusion;
	return(*this);
}

// typing info
int
SemanticRep::isAtomic() const
{
	return(0);
}

int
SemanticRep::isExpression() const
{
	return(0);
}

// print data
void 
SemanticRep::dumpData(ostream &) const
{
	return;
}

ostream &
operator<<(ostream &os, const SemanticRep &sr)
{
	sr.dumpData(os);
	return(os);
}

// atomic class constructors and destructor
Atomic::Atomic():
	SemanticRep(), type(Unknown)
{
	name[0] = 0;
	value[0] = 0;
}

Atomic::Atomic(const Atomic &src):
	SemanticRep(src), type(src.type)
{
	strcpy(name, src.name);
	strcpy(value, src.value);
}

Atomic::Atomic(Type t, const char *n, const char *v, int c):
	SemanticRep(c), type(t)
{
	name[0] = 0;
	if (n != NULL)
		strcpy(name, n);
	value[0] = 0;
	if (v != NULL)
		strcpy(value, v);
}

Atomic::~Atomic()
{
	type = Unknown;
	name[0] = 0;
	value[0] = 0;
}

// duplicate the item
SemanticRep *
Atomic::copy() const
{
	SemanticRep *p = new Atomic(*this);
	MustBeTrue(p != NULL);
	return(p);
}

// assignment operator
Atomic &
Atomic::operator=(const Atomic &rhs)
{
	// check for self-assignment
	if (this != &rhs)
	{
		// copy any semantic data
		SemanticRep::operator=(rhs);

		// copy data specific to atomic record
		type = rhs.type;
		strcpy(name, rhs.name);
		strcpy(value, rhs.value);
	}
	return(*this);
}

// typing info
int
Atomic::isAtomic() const
{
	return(1);
}

int
Atomic::isExpression() const
{
	return(0);
}

// get clause data
int
Atomic::getClauses(Set_List<Clause> &clist, Clause &c, 
		int storeClause, int negated, int concluded) const
{
	Atom atom;
	switch (type)
	{
	case Constant:
		atom = Atom(value);
		if (negated)
		{
			if (atom == Atom("true"))
				atom = Atom("false");
			else
				atom = Atom("true");
		}
		break;
	case Variable:
		if (negated)
			atom = ~Atom(name);
		else
			atom = Atom(name);
		break;
	default:
		MustBeTrue(0);
		return(NOTOK);
	}
	c.setPartOfConclusion(concluded);
	c.insert(atom);
	return(OK);
}

// print data
void 
Atomic::dumpData(ostream &os) const
{
	switch (type)
	{
	case Constant:
		os << value << " ";
		break;
	case Variable:
		os << name << " ";
		break;
	}
	return;
}

ostream &
operator<<(ostream &os, const Atomic &sr)
{
	sr.dumpData(os);
	return(os);
}

// expression class constructors and destructor
Expression::Expression():
	SemanticRep(), type(Unknown), left(NULL), right(NULL)
{
	// nothing to do 
}

Expression::Expression(const Expression &src):
	SemanticRep(src), type(src.type), left(NULL), right(NULL)
{
	if (src.left != NULL)
	{
		left = new Semantic(*src.left);
		MustBeTrue(left != NULL);
	}
	if (src.right != NULL)
	{
		right = new Semantic(*src.right);
		MustBeTrue(right != NULL);
	}
}

Expression::Expression(OperatorType t, Semantic *l, Semantic *r, int c):
	SemanticRep(c), type(t), left(l), right(r)
{
	// nothing to do
}

Expression::~Expression()
{
	type = Unknown;
	if (left != NULL) 
	{
		delete left;
		left = NULL;
	}
	if (right != NULL) 
	{
		delete right;
		right = NULL;
	}
}

// duplicate the item
SemanticRep *
Expression::copy() const
{
	SemanticRep *p = new Expression(*this);
	MustBeTrue(p != NULL);
	return(p);
}

// assignment operator
Expression &
Expression::operator=(const Expression &rhs)
{
	// check for self-assignment
	if (this != &rhs)
	{
		// copy any semantic data
		SemanticRep::operator=(rhs);

		// copy data specific to expression record
		type = rhs.type;
		if (left != NULL)
		{
			delete left;
			left = NULL;
		}
		if (rhs.left != NULL)
		{
			left = new Semantic(*rhs.left);
			MustBeTrue(left != NULL);
		}
		if (right != NULL)
		{
			delete right;
			right = NULL;
		}
		if (rhs.right != NULL)
		{
			right = new Semantic(*rhs.right);
			MustBeTrue(right != NULL);
		}
	}
	return(*this);
}

// typing info
int
Expression::isAtomic() const
{
	return(0);
}

int
Expression::isExpression() const
{
	return(1);
}

// get clause data
int
Expression::getClauses(Set_List<Clause> &clist, Clause &c, 
		int storeClause, int negated, int concluded) const
{
	switch (type)
	{
	case And:
		MustBeTrue(left != NULL && right != NULL);
		if (left->getClauses(clist, c, 1, negated, concluded) != OK)
			return(NOTOK);
		if (right->getClauses(clist, c, 1, negated, concluded) != OK)
			return(NOTOK);
		break;
	case Or:
		MustBeTrue(left != NULL && right != NULL);
		if (left->getClauses(clist, c, 0, negated, concluded) != OK)
			return(NOTOK);
		if (right->getClauses(clist, c, 0, negated, concluded) != OK)
			return(NOTOK);
		break;
	case Negation:
		MustBeTrue(right != NULL && left == NULL);
		if (right->getClauses(clist, c, 0, 1, concluded) != OK)
			return(NOTOK);
		break;
	default:
		MustBeTrue(0);
	}
	c.setPartOfConclusion(concluded);
	return(OK);
}

// print data
void 
Expression::dumpData(ostream &os) const
{
	os << "( ";
	if (left != NULL)
		os << *left;
	switch (type)
	{
	case And:
		os << "&& ";
		break;
	case Or:
		os << "|| ";
		break;
	case Conditional:
		os << "--> ";
		break;
	case Biconditional:
		os << "<--> ";
		break;
	case Negation:
		os << "~";
		break;
	}
	if (right != NULL)
		os << *right;
	os << ") ";
}

ostream &
operator<<(ostream &os, const Expression &sr) {
	sr.dumpData(os);
	return(os);
}

// semantic record constructors and destructor
Semantic::Semantic():
	prep(NULL)
{
	// nothing to do
}

Semantic::Semantic(const Semantic &src):
	prep(NULL)
{
	if (src.prep != NULL)
	{
		prep = src.prep->copy();
		MustBeTrue(prep != NULL);
	}
}

Semantic::Semantic(Atomic::Type t, const char *n, const char *v, int c):
	prep(NULL)
{
	prep = new Atomic(t, n, v, c);
	MustBeTrue(prep != NULL);
}

Semantic::Semantic(Expression::OperatorType t, Semantic *l, Semantic *r, int c):
	prep(NULL)
{
	prep = new Expression(t, l, r, c);
	MustBeTrue(prep != NULL);
}

Semantic::~Semantic()
{
	if (prep != NULL)
	{
		delete prep;
		prep = NULL;
	}
}

// assignment operator
Semantic &
Semantic::operator=(const Semantic &rhs)
{
	if (this != &rhs)
	{
		if (prep != NULL)
		{
			delete prep;
			prep = NULL;
		}
		if (rhs.prep != NULL)
		{
			prep = rhs.prep->copy();
			MustBeTrue(prep != NULL);
		}
	}
	return(*this);
}

// access conclusion data
int
Semantic::getConclusion() const
{
	MustBeTrue(prep != NULL);
	return(prep->conclusion);
}

void
Semantic::setConclusion(int c)
{
	MustBeTrue(prep != NULL);
	prep->conclusion = c;
}

// typing info
int
Semantic::isAtomic() const
{
	return(prep->isAtomic());
}

int
Semantic::isExpression() const
{
	return(prep->isExpression());
}

// convert biconditionals to conditionals, use the following:
//	(A <--> B) is identical to (A --> B) && (B --> A)
int
Semantic::removeBiconditionals()
{
	// check if done
	if (!isExpression())
		return(OK);

	// check left and right sides
	Expression *pe = (Expression *)prep;
	if (pe->left != NULL && pe->left->removeBiconditionals() != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->removeBiconditionals() != OK)
		return(NOTOK);

	// update current node, if required
	if (pe->type == Expression::Biconditional)
	{
		// save left and right records
		Semantic *plsr = pe->left;
		Semantic *pduplsr = new Semantic(*pe->left);
		MustBeTrue(pduplsr != NULL);
		Semantic *prsr = pe->right;
		Semantic *pduprsr = new Semantic(*pe->right);
		MustBeTrue(pduprsr != NULL);

		// create implication records
		pe->left = 
			new Semantic(Expression::Conditional, plsr, prsr);
		MustBeTrue(pe->left != NULL);
		pe->right = 
			new Semantic(Expression::Conditional, pduprsr, pduplsr);
		MustBeTrue(pe->right != NULL);

		// change operator type to AND
		pe->type = Expression::And;
	}

	// all done
	return(OK);
}

// convert implications to ORs, use the following:
//	(A --> B) is identical to (~A || B)
int
Semantic::removeConditionals()
{
	// check if done
	if (!isExpression())
		return(OK);

	// check left and right sides
	Expression *pe = (Expression *)prep;
	if (pe->left != NULL && pe->left->removeConditionals() != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->removeConditionals() != OK)
		return(NOTOK);

	// update current node, if required
	if (pe->type == Expression::Conditional)
	{
		// save left record
		Semantic *plsr = pe->left;

		// create NOT record
		pe->left = new Semantic(Expression::Negation, NULL, plsr);
		MustBeTrue(pe->left != NULL);

		// change operator type to OR
		pe->type = Expression::Or;
	}

	// all done
	return(OK);
}

// remove multiple NOTs, use ~~A <--> A rule.
int
Semantic::removeExtraNots()
{
	// save if a conclusion
	int saveConclusion = prep->conclusion;

	// loop and remove extra NOTs.
	Semantic *ps = this;
	while ( 1 )
	{
		// check we have an expression
		if (!(ps->isExpression()))
			break;

		// get expression record and check for NOT
		Expression *pe = (Expression *)ps->prep;
		if (pe->type != Expression::Negation)
			break;

		// check for extra NOT
		Semantic *ps2 = pe->right;
		if (!(ps2->isExpression()))
			break;
		Expression *pe2 = (Expression *)ps2->prep;
		if (pe2->type != Expression::Negation)
			break;

		// we have two NOTs. remove them.
		Semantic *psaves = pe2->right;
		pe2->right = NULL;
		delete pe;

		// copy saved data
		ps->prep = psaves->prep;
		psaves->prep = NULL;
		delete psaves;
	}

	// restore original conclusion status
	prep->conclusion = saveConclusion;

	// all done
	return(OK);
}

// push negation as far down as possible, use demorgan's laws
//	~(A && B) identical to (~A || ~B), and
//	~(A || B) identical to (~A && ~B)
//
int
Semantic::demorgans()
{
	// check if atomic or an expression
	if (!isExpression())
		return(OK);

	// remove multiple NOTs, 
	//	use ~~A <--> A and
	//	use ~~~A <--> ~A rules.
	//
	if (removeExtraNots() != OK)
		return(NOTOK);

	// check if atomic or an expression. this second check
	// is required since it is possible that when all extra
	// negations are removed, only an Atomic is left.
	//
	if (!isExpression())
		return(OK);

	// check if we have a negated expression
	Expression *pe = (Expression *)prep;
	if (pe->type != Expression::Negation)
	{
		// not a negated expression, check left and right
		if (pe->left != NULL && pe->left->demorgans() != OK)
			return(NOTOK);
		if (pe->right != NULL && pe->right->demorgans() != OK)
			return(NOTOK);
		return(OK);
	}

	// we have a negation, check if rhs is atomic or an expression.
	Semantic *prs = pe->right;
	if (!(prs->isExpression()))
	{
		// what is negated is NOT an expression, just return.
		return(OK);
	}

	// check if expression is an AND or OR
	Expression *pre = (Expression *)prs->prep;
	if (pre->type != Expression::Or && pre->type != Expression::And)
	{
		// not a AND or OR expression, check left and right
		if (pre->left != NULL && pre->left->demorgans() != OK)
			return(NOTOK);
		if (pre->right != NULL && pre->right->demorgans() != OK)
			return(NOTOK);
		return(OK);
	}

	// we have a negated AND or OR expression, apply demorgans laws.
	Semantic *prels = pre->left;
	Semantic *prers = pre->right;

	// create new negation records
	Semantic *pnegls = new Semantic(Expression::Negation, NULL, prels);
	MustBeTrue(pnegls != NULL);
	Semantic *pnegrs = new Semantic(Expression::Negation, NULL, prers);
	MustBeTrue(pnegrs != NULL);

	// write over negation record with the new record info
	pe->left = pnegls;
	pe->right = pnegrs;
	switch (pre->type)
	{
	case Expression::Or:
		pe->type = Expression::And;
		break;
	case Expression::And:
		pe->type = Expression::Or;
		break;
	default:
		MustBeTrue(0);
		break;
	}

	// delete old right semantic record
	pre->left = NULL;
	pre->right = NULL;
	delete prs;

	// check left and right sides
	if (pe->left != NULL && pe->left->demorgans() != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->demorgans() != OK)
		return(NOTOK);

	// all done
	return(OK);
}

// distribute AND over OR using the rule:
//	(A && B) || C <--> (A || C) && (B || C)
//
int
Semantic::distributeLeftAnd(int &changed)
{
	// check if we have an expression
	if (!isExpression())
		return(OK);

	// update change flag
	changed = 1;

	// expression record
	Expression *pe = (Expression *)prep;

	// left and right semantic/expression records
	Semantic *pls = pe->left;
	Expression *ple = (Expression *)pls->prep;
	Semantic *prs = pe->right;
	Expression *pre = (Expression *)prs->prep;

	// left branch data
	Semantic *plels = ple->left;
	Expression *plele = (Expression *)plels->prep;
	Semantic *plers = ple->right;
	Expression *plere = (Expression *)plers->prep;

	// we have and OR with a left AND. we can distribute the
	// OR using the rule (A&&B)||C = (A||C)&&(B||C)
	//
	// create new right semantic record
	pe->right = new Semantic(Expression::Or, plers, prs);
	MustBeTrue(pe->right != NULL);

	// update AND right branch
	ple->right = new Semantic(*prs);
	MustBeTrue(ple->right != NULL);

	// update all operations
	pe->type = Expression::And;
	ple->type = Expression::Or;

	// scan new branches
	if (pe->left != NULL && pe->left->distribution(changed) != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->distribution(changed) != OK)
		return(NOTOK);

	// all done
	return(OK);
}

// distribute OR over ANDs using the rule:
//	A || (B && C) <--> (A || B) && (A || C)
//
int
Semantic::distributeRightAnd(int &changed)
{
	// check if we have an expression
	if (!isExpression())
		return(OK);

	// update change flag
	changed = 1;

	// expression record
	Expression *pe = (Expression *)prep;

	// left and right semantic/expression records
	Semantic *pls = pe->left;
	Expression *ple = (Expression *)pls->prep;
	Semantic *prs = pe->right;
	Expression *pre = (Expression *)prs->prep;

	// right branch data
	Semantic *prels = pre->left;
	Expression *prele = (Expression *)prels->prep;
	Semantic *prers = pre->right;
	Expression *prere = (Expression *)prers->prep;

	// we have an OR with a right AND. we can distribute the
	// OR using the rule A||(B&&C) = (A||B)&&(A||C).
	//
	// create new left semantic record
	pe->left = new Semantic(Expression::Or, pls, prels);
	MustBeTrue(pe->left != NULL);

	// update AND left branch
	pre->left = new Semantic(*pls);
	MustBeTrue(pre->left != NULL);

	// update all operations
	pe->type = Expression::And;
	pre->type = Expression::Or;

	// scan new branches
	if (pe->left != NULL && pe->left->distribution(changed) != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->distribution(changed) != OK)
		return(NOTOK);

	// all done
	return(OK);
}

// convert program to conjunctive normal form
// distribute OR over ANDs using the rules:
//	A || (B && C) <--> (A || B) && (A || C)
//	(A && B) || C <--> (A || C) && (B || C)
//
int
Semantic::distribution(int &changed)
{
	// check if we have an expression
	if (!isExpression())
		return(OK);

	// check type of expression
	Expression *pe = (Expression *)prep;
	if (pe->type != Expression::Or)
	{
		// we don't have an OR, check left and right
		if (pe->left != NULL && 
		    pe->left->distribution(changed) != OK)
			return(NOTOK);
		if (pe->right != NULL && 
		    pe->right->distribution(changed) != OK)
			return(NOTOK);
		return(OK);
	}

	// we have an OR, check if left is an AND.
	Semantic *pls = pe->left;
	if (pls->isExpression())
	{
		Expression *ple = (Expression *)pls->prep;
		if (ple->type == Expression::And)
			return(distributeLeftAnd(changed));
	}

	// we have an OR, check if right is an AND.
	Semantic *prs = pe->right;
	if (prs->isExpression())
	{
		Expression *pre = (Expression *)prs->prep;
		if (pre->type == Expression::And)
			return(distributeRightAnd(changed));
	}

	// check both sides
	if (pe->left != NULL && pe->left->distribution(changed) != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->distribution(changed) != OK)
		return(NOTOK);

	// all done
	return(OK);
}

int
Semantic::distribution()
{
	for (int changed = 1; changed ; )
	{
		changed = 0;
		if (distribution(changed) != OK)
			return(NOTOK);
	}
	return(OK);
}

// get clauses from CNF statement
int
Semantic::getClauses(List<Semantic *> &clist)
{
	// check if an expression
	if (!isExpression())
	{
		Semantic *pnew = new Semantic(*this);
		MustBeTrue(pnew != NULL);
		clist.insertAtEnd(pnew);
		return(OK);
	}

	// check if expression is an AND
	Expression *pe = (Expression *)prep;
	if (pe->type != Expression::And)
	{
		Semantic *pnew = new Semantic(*this);
		MustBeTrue(pnew != NULL);
		clist.insertAtEnd(pnew);
		return(OK);
	}

	// keep tracing down the ANDs
	if (pe->left != NULL && pe->left->getClauses(clist) != OK)
		return(NOTOK);
	if (pe->right != NULL && pe->right->getClauses(clist) != OK)
		return(NOTOK);

	// all done
	return(OK);
}

int
Semantic::getClauses(Set_List<Clause> &clist, Clause &c) const
{
	return(getClauses(clist, c, 1, 0, prep->conclusion));
}

int
Semantic::getClauses(Set_List<Clause> &clist, Clause &c, 
	int storeClause, int negated, int concluded) const
{
	if (prep->getClauses(clist, c, 0, negated, concluded) != OK)
		return(NOTOK);
	if (storeClause && !c.isEmpty())
	{
		c.setPartOfConclusion(concluded);
		clist.insert(c);
		c.clear();
	}
	return(OK);
}

// print data
void
Semantic::dumpData(ostream &os) const
{
	os << *prep;
}

ostream &
operator<<(ostream &os, const Semantic &sr) {
	sr.dumpData(os);
	return(os);
}

