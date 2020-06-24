#ifndef __SEMANTIC_H
#define __SEMANTIC_H
// semantic record for predicate logic prover

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
#include "array.h"
#include "mylist.h"
#include "mystring.h"
#include "symboltypes.h"
#include "clausetypes.h"
#include "symbol.h"
#include "uniquename.h"

// forward declarations
class Semantic;

// semantic record class definitions
class Semantic {
public:
	// constructors and destructor
	Semantic();
	Semantic(const Semantic &);
	Semantic(SymbolType, Semantic *, Semantic *, int = 0);
	Semantic(SymbolType, const String &, Semantic *, int = 0);
	Semantic(SymbolType, const String &, const String &, int = 0);
	Semantic(SymbolType, const String &, List<Semantic * > *, int, int = 0);
	Semantic(SymbolType, const String &, int = 0);
	Semantic(const String &, const String &, SymbolType, int = 0);
	~Semantic();

	// assignment
	Semantic &operator=(const Semantic &);

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

	// access query data
	int getQuery() const;
	void setQuery(int);

	// access clause type data
	int getCType() const;
	void setCType(ClauseType);

	// set or get options
	List<String> getOptions() const;
	void setOptions(const List<String> &);
	void clearOptions();
	void setOptions(Array<List<Semantic * > > &) const;

	// write out intermediate forms of statement.
	String options() const;
	void preOrder(ostream &) const;
	void inOrder(ostream &) const;
	void postOrder(ostream &) const;
	int clauseToString(String &) const;
	int cltostr(String &) const;

	// functions to convert to CNF
	int removeBiconditionals();
	int removeConditionals();
	int demorgans();
	int renameVariables();
	int skolemize();
	int removeUniversals();
	int distribution();
	int getClauses(List<Semantic * > &);
	int renameAgain();

	// helper functions
	int removeExtraNots();
	int renameVariables(List<Symbol> &);
	int skolemize(List<Symbol> &);
	int removeExistentials();
	int distributeLeftAnd(int &);
	int distributeRightAnd(int &);
	int distribution(int &);
	int getClauses(List<Semantic * > &, int, int, int);
	int renameAgain(List<Symbol> &);

	// splitting problem
	int split(const String &, String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &) const;
	int bledsoe(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &) const;
	int schaum(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &) const;
	int cp(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &) const;
	int none(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, 
		const String &, const String &) const;

	// schaum splitting functions
	int schaum_and(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;
	int schaum_or(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;
	int schaum_conditional(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;
	int schaum_biconditional(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;

	// contrapositive splitting functions
	int cp_conditional(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;
	int cp_biconditional(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;

	// bledsoe splitting functions
	int bledsoe_and(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;
	int bledsoe_or(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;
	int bledsoe_conditional(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;
	int bledsoe_biconditional(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;

	// bledsoe splitting utility functions
	int bledsoe_conditional_or(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;
	int bledsoe_conditional_and(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;
	int bledsoe_conditional_cond(String &, Array<List<Semantic * > > &, 
		Array<List<Semantic * > > &, const String &) const;

	// negate record
	int negate();

	// print data
	void dumpData(ostream &) const;
	friend ostream &operator<<(ostream &, const Semantic &);

protected:
	// type of symbol or clause
	SymbolType type;
	ClauseType ctype;

	// flags
	int varisused;
	int conclusion;
	int setofsupport;
	int query;

	// identifiers and values
	String name;
	String originalname;
	String value;

	// children pointers and counter
	int argnum;
	Semantic *left;
	Semantic *right;
	List<Semantic *> *pargs;

public:
	static int Ccount;
	static int Dcount;
};

#endif

