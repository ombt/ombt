#ifndef __SEMANTIC_H
#define __SEMANTIC_H
// semantic record for propositional logic prover

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "adt/List.h"
#include "adt/String.h"
#include <symbol.h>
#include <irclause.h>

using namespace std;
using namespace ombt;

// forward declarations
class SemanticRep;
class Predicate;
class Expression;
class Argument;
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
	virtual int isPredicate() const;
	virtual int isExpression() const;
	virtual int isArgument() const;

	// print data
	virtual void dumpData(std::ostream &) const;
	friend std::ostream &operator<<(std::ostream &, const SemanticRep &);

	// internal data
	int conclusion;
};

// function/predicate argument class
class Argument: public SemanticRep {
public:
	// enumeration for argument type
	enum Type {
		Variable, Constant, Function, 
		QuotedString, Number, Unknown
	};

protected:
	// friend classes
	friend class Semantic;

	// constructors and destructor
	Argument();
	Argument(const Argument &);
	Argument(Type, const ombt::String &, int = 0);
	Argument(Type, const ombt::String &, ombt::List<Semantic *> *, int, int = 0);
	~Argument();

	// copy object
	SemanticRep *copy() const;

	// assignment operator
	Argument &operator=(const Argument &);

	// typing info
	int isPredicate() const;
	int isExpression() const;
	int isArgument() const;

	// print data
	void dumpData(std::ostream &) const;
	friend std::ostream& operator<<(std::ostream &, const Argument &);

	// internal data
	Type type;
	int argnum;
	ombt::String name;
	ombt::String value;
	ombt::List<Semantic *> *pargs;
};

// predicate class definition
class Predicate: public SemanticRep {
public:
	// types of atomic records
	enum Type {
		Constant, LogicalConstant, Function, Unknown
	};

protected:
	// friend classes
	friend class Semantic;

	// constructor and destructor
	Predicate();
	Predicate(const Predicate &);
	Predicate(Type, const ombt::String &, const ombt::String &, int = 0);
	Predicate(Type, const ombt::String &, ombt::List<Semantic *> *, int, int = 0);
	~Predicate();

	// copy object
	SemanticRep *copy() const;

	// assignement
	Predicate &operator=(const Predicate &);

	// typing info
	int isPredicate() const;
	int isExpression() const;
	int isArgument() const;

	// print data
	void dumpData(std::ostream &) const;
	friend std::ostream &operator<<(std::ostream &, const Predicate &);

	// internal data
	int argnum;
	Type type;
	ombt::String name;
	ombt::String value;
	ombt::List<Semantic * > *pargs;
};

// expression record class definition
class Expression: public SemanticRep {
public:
	// types of expressions records
	enum Type {
		And, Or, Conditional, Biconditional, Negation, 
		Universal, Existential, Unknown
	};

protected:
	// friend classes
	friend class Semantic;

	// constructor and destructor
	Expression();
	Expression(const Expression &);
	Expression(Type, Semantic *, Semantic *, int = 0);
	Expression(Type, Semantic *, const ombt::String &, int = 0);
	~Expression();

	// copy object
	SemanticRep *copy() const;

	// assignment operator
	Expression &operator=(const Expression &);

	// typing info
	int isPredicate() const;
	int isExpression() const;
	int isArgument() const;

	// accesing variables
	int getVarIsUsed() const;
	void setVarIsUsed(int);

	// print data
	void dumpData(std::ostream &) const;
	friend std::ostream &operator<<(std::ostream &, const Expression &);

	// internal data
	Type type;
	Semantic *left;
	Semantic *right;

	// internal data for quantifiers
	ombt::String name;
	int varisused;
};

// semantic record class definitions
class Semantic {
public:
	// constructors and destructor
	Semantic();
	Semantic(const Semantic &);
	Semantic(Expression::Type, Semantic *, Semantic *, int = 0);
	Semantic(Expression::Type, const ombt::String &, Semantic *, int = 0);
	Semantic(Predicate::Type, const ombt::String &, const ombt::String &, int = 0);
	Semantic(Predicate::Type, const ombt::String &, ombt::List<Semantic * > *, 
		int, int = 0);
	Semantic(Argument::Type, const ombt::String &, int = 0);
	Semantic(Argument::Type, const ombt::String &, ombt::List<Semantic * > *, 
		int, int = 0);
	~Semantic();

	// assignment
	Semantic &operator=(const Semantic &);

	// forwarding operator
	SemanticRep *operator->() const {
		return(prep);
	}

	// typing info
	int isPredicate() const;
	int isExpression() const;
	int isArgument() const;

	// functions to convert to CNF
	int removeBiconditionals();
	int removeConditionals();
	int demorgans();
	int renameVariables();
	int skolemize();
	int removeUniversals();
	int distribution();
	int getClauses(ombt::List<Semantic * > &);
	int renameAgain();
	int getClausesAsLists(ombt::List<IRClause> &);

	// helper functions
	int removeExtraNots();
	int renameVariables(ombt::List<Symbol> &);
	int skolemize(ombt::List<Symbol> &);
	int removeExistentials();
	int distributeLeftAnd(int &);
	int distributeRightAnd(int &);
	int distribution(int &);
	int getClauses(ombt::List<Semantic * > &, int);
	int renameAgain(ombt::List<Symbol> &);
	int getClausesAsLists(IRClause &);
	int getClausesAsLists(IRClause &, ombt::String &, int);

	// access conclusion data
	int getConclusion() const;
	void setConclusion(int);

	// print data
	void dumpData(std::ostream &) const;
	friend std::ostream &operator<<(std::ostream &, const Semantic &);

	// static function for creating unique names
	static ombt::String uniqueName(const ombt::String &);

protected:
	// internal data
	SemanticRep *prep;
	static long counter;
};

#endif

