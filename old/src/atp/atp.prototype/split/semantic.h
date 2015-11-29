#ifndef __SEMANTIC_H
#define __SEMANTIC_H
// semantic record for propositional logic prover

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <unistd.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mylist.h"
#include "mystring.h"
#include "symbol.h"

// forward declarations
class SemanticRep;
class Predicate;
class Expression;
class Term;
class Semantic;

// semantic record representation base class definition
class SemanticRep {
protected:
	// friend classes
	friend class Semantic;

	// constructors and destructor
	SemanticRep(int = 0, int = 0);
	SemanticRep(const SemanticRep &);
	virtual ~SemanticRep();

	// copy object
	virtual SemanticRep *copy() const = 0;

	// assignment operator
	SemanticRep &operator=(const SemanticRep &);

	// typing info
	virtual int isPredicate() const;
	virtual int isExpression() const;
	virtual int isTerm() const;

	// print data
	virtual void dumpData(ostream &) const;
	friend ostream &operator<<(ostream &, const SemanticRep &);

	// internal data
	int conclusion;
	int setofsupport;
};

// function/predicate argument class
class Term: public SemanticRep {
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
	Term();
	Term(const Term &);
	Term(Type, const String &, int = 0);
	Term(Type, const String &, List<Semantic *> *, int, int = 0);
	~Term();

	// copy object
	SemanticRep *copy() const;

	// assignment operator
	Term &operator=(const Term &);

	// typing info
	int isPredicate() const;
	int isExpression() const;
	int isTerm() const;

	// print data
	void dumpData(ostream &) const;
	friend ostream& operator<<(ostream &, const Term &);

	// internal data
	Type type;
	int argnum;
	String name;
	String value;
	List<Semantic *> *pargs;
};

// predicate class definition
class Predicate: public SemanticRep {
public:
	// types of atomic records
	enum Type {
		Equal, Constant, LogicalConstant, Function, Unknown
	};

protected:
	// friend classes
	friend class Semantic;

	// constructor and destructor
	Predicate();
	Predicate(const Predicate &);
	Predicate(Type, const String &, const String &, int = 0);
	Predicate(Type, const String &, List<Semantic *> *, int, int = 0);
	~Predicate();

	// copy object
	SemanticRep *copy() const;

	// assignement
	Predicate &operator=(const Predicate &);

	// typing info
	int isPredicate() const;
	int isExpression() const;
	int isTerm() const;

	// print data
	void dumpData(ostream &) const;
	friend ostream &operator<<(ostream &, const Predicate &);

	// internal data
	int argnum;
	Type type;
	String name;
	String value;
	List<Semantic * > *pargs;
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
	Expression(Type, Semantic *, const String &, int = 0);
	~Expression();

	// copy object
	SemanticRep *copy() const;

	// assignment operator
	Expression &operator=(const Expression &);

	// typing info
	int isPredicate() const;
	int isExpression() const;
	int isTerm() const;

	// accesing variables
	int getVarIsUsed() const;
	void setVarIsUsed(int);

	// print data
	void dumpData(ostream &) const;
	friend ostream &operator<<(ostream &, const Expression &);

	// internal data
	Type type;
	Semantic *left;
	Semantic *right;

	// internal data for quantifiers
	String name;
	int varisused;
};

// semantic record class definitions
class Semantic {
public:
	// constructors and destructor
	Semantic();
	Semantic(const Semantic &);
	Semantic(Expression::Type, Semantic *, Semantic *, int = 0);
	Semantic(Expression::Type, const String &, Semantic *, int = 0);
	Semantic(Predicate::Type, const String &, const String &, int = 0);
	Semantic(Predicate::Type, const String &, List<Semantic * > *, 
		int, int = 0);
	Semantic(Term::Type, const String &, int = 0);
	Semantic(Term::Type, const String &, List<Semantic * > *, 
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
	int isTerm() const;

	// access conclusion data
	int getConclusion() const;
	void setConclusion(int);

	// access set-of-support data
	int getSOS() const;
	void setSOS(int);

	// splitting problem
	int schaum(const char *, const char *, List<Semantic *> &) const;
	int none(const char *, const char *, List<Semantic *> &) const;
	int bledsoe(const char *, const char *, List<Semantic *> &) const;

	// utility functions
	int none(const char *, const char *, List<Semantic *> &, 
		const char *, const char *) const;
	int schaum_and(const char *, const char *, List<Semantic *> &) const;
	int schaum_or(const char *, const char *, List<Semantic *> &) const;
	int schaum_conditional(const char *, 
			const char *, List<Semantic *> &) const;
	int schaum_biconditional(const char *, 
			const char *, List<Semantic *> &) const;
	int bledsoe_and(const char *, const char *, List<Semantic *> &) const;
	int bledsoe_or(const char *, const char *, List<Semantic *> &) const;
	int bledsoe_conditional(const char *, 
			const char *, List<Semantic *> &) const;
	int bledsoe_biconditional(const char *, 
			const char *, List<Semantic *> &) const;
	int writeAndOr(const char *, const char *, 
			const char *, const char * ) const;
	int writeproblem(const char *, const char *, const Semantic *, 
		const char *, const char *, const List<Semantic *> &) const;
	int bledsoe_conditional_or(const char *, 
			const char *, List<Semantic *> &) const;
	int bledsoe_conditional_and(const char *, 
			const char *, List<Semantic *> &) const;
	int bledsoe_conditional_cond(const char *, 
			const char *, List<Semantic *> &) const;

	// print data
	void dumpData(ostream &) const;
	friend ostream &operator<<(ostream &, const Semantic &);

	// static function for creating unique names
	static String uniqueName(const String &);

protected:
	// internal data
	SemanticRep *prep;
};

#endif

