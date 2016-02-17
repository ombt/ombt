#ifndef __SEMANTIC_H
#define __SEMANTIC_H
// semantic record for propositional logic prover

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <string.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "list.h"
#include "set_List.h"
#include "clause.h"
#include "mystring.h"

// local definitions
#define NameSize 32
#define ValueSize 32

// forward declarations
class SemanticRep;
class Atomic;
class Expression;
class Semantic;

// semantic record representation base class definition
class SemanticRep {
protected:
	// friend classes
	friend class Semantic;

	// constructors and destructor
	SemanticRep(int = 0);
	SemanticRep(const SemanticRep &);
	virtual ~SemanticRep();

	// copy object
	virtual SemanticRep *copy() const = 0;

	// assignment operator
	SemanticRep &operator=(const SemanticRep &);

	// typing info
	virtual int isAtomic() const;
	virtual int isExpression() const;

	// convert to CNF functions
	virtual int getClauses(Set_List<Clause> &, Clause &, int, int, int) const = 0;

	// print data
	virtual void dumpData(ostream &) const;
	friend ostream &operator<<(ostream &, const SemanticRep &);

	// internal data
	int conclusion;
};

// atomic record class definition
class Atomic: public SemanticRep {
public:
	// types of atomic records
	enum Type {
		Constant, Variable, Unknown
	};

protected:
	// friend classes
	friend class Semantic;

	// constructor and destructor
	Atomic();
	Atomic(const Atomic &);
	Atomic(Type, const char *, const char *, int = 0);
	~Atomic();

	// copy object
	virtual SemanticRep *copy() const;

	// assignement
	Atomic &operator=(const Atomic &);

	// typing info
	int isAtomic() const;
	int isExpression() const;

	// convert to CNF functions
	virtual int getClauses(Set_List<Clause> &, Clause &, int, int, int) const;

	// print data
	void dumpData(ostream &) const;
	friend ostream &operator<<(ostream &, const Atomic &);

	// internal data
	Type type;
	char name[NameSize];
	char value[ValueSize];
};

// expression record class definition
class Expression: public SemanticRep {
public:
	// types of expressions records
	enum OperatorType {
		And, Or, Conditional, Biconditional, Negation, Unknown
	};

protected:
	// friend classes
	friend class Semantic;

	// constructor and destructor
	Expression();
	Expression(const Expression &);
	Expression(OperatorType, Semantic *, Semantic *, int = 0);
	~Expression();

	// copy object
	virtual SemanticRep *copy() const;

	// assignment operator
	Expression &operator=(const Expression &);

	// typing info
	int isAtomic() const;
	int isExpression() const;

	// convert to CNF functions
	virtual int getClauses( Set_List<Clause> &, Clause &, int, int, int) const;

	// print data
	void dumpData(ostream &) const;
	friend ostream &operator<<(ostream &, const Expression &);

	// internal data
	OperatorType type;
	Semantic *left;
	Semantic *right;
};

// semantic record class definitions
class Semantic {
public:
	// constructors and destructor
	Semantic();
	Semantic(const Semantic &);
	Semantic(Atomic::Type, const char *, const char *, int = 0);
	Semantic(Expression::OperatorType, Semantic *, Semantic *, int = 0);
	~Semantic();

	// assignment
	Semantic &operator=(const Semantic &);

	// forwarding operator
	SemanticRep *operator->() const {
		return(prep);
	}

	// typing info
	int isAtomic() const;
	int isExpression() const;

	// access conclusion data
	int getConclusion() const;
	void setConclusion(int);

	// functions to convert to CNF
	int removeBiconditionals();
	int removeConditionals();
	int demorgans();
	int distribution();
	int removeExtraNots();
	int distributeLeftAnd(int &);
	int distributeRightAnd(int &);
	int getClauses(List<Semantic * > &);
	int getClauses(Set_List<Clause> &, Clause &) const;
	int getClauses(Set_List<Clause> &, Clause &, int, int, int) const;
	int distribution(int &);

	// print data
	void dumpData(ostream &) const;
	friend ostream &operator<<(ostream &, const Semantic &);

protected:
	// internal data
	SemanticRep *prep;
};

#endif
