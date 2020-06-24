#ifndef __SYMBOLTYPES_H
#define __SYMBOLTYPES_H

// symbol types
enum SymbolType {
	Negation,
	Biconditional,
	Conditional,
	Or,
	And,
	Universal,
	Existential,
	Variable,
	UniversalVariable,
	ExistentialVariable,
	LogicalConstant,
	PredicateConstant,
	PredicateFunction,
	PredicateEqual,
	QuotedString,
	Number,
	Constant,
	Function,
	Unknown
};
#endif
