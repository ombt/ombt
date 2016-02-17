/*
 * Copyright (C) 1995, 1996 Peter Bouthoorn.
 *
 * This software may be freely distributed and modified provided
 * this copyright message is left intact. The copyright message must be
 * included both with this (the original) software and with any modified
 * copies of this software or with any new software based on this software.
 * Furthermore any modified copies of this software must carry prominent
 * notices stating the software was changed and the date of any change.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. In no event
 * will the copyright holder be liable for any damage arising out of
 * the use of this software.
 *
 * As a matter of courtesy, the author requests to be informed about
 * any bugs found in this software and about improvements that may be of
 * general interest.
 *
 * Peter Bouthoorn
 * peter@obelix.icce.rug.nl
 */

#ifndef _LOGIC_H_
#define _LOGIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iostream.h>
#include "xstring.h"
#include "xarray.h"

/*
 * TermType is used for runtime type information to determine
 * if pointers or references to derivatives of class Term
 * are of the same type.
 */
enum TermType {Var_T, Constant_T, Complex_T};

/* Forward declaration. */
class Substitution;
class Var;

/*
 * Class Term is a base class common to class Var, Constant and
 * Complex. This base class is needed so that (pointers to) objects
 * of each of these types can be stored in the same container
 * class.
 * The actual type of an object is determined by gettype(). Matching
 * and unification of two Term objects is done by match() and
 * unify_me() respectively.
 */
class Term
{
    public:
	virtual ~Term();
	virtual TermType gettype() const = 0;
	virtual int unify_me(const Term &, Substitution &) const = 0;
	virtual int match(const Term &) const = 0;
	virtual Term *dup() const = 0;
	virtual void display(ostream &) const = 0;
	virtual const Term *get_realvalue(const Substitution &) const = 0;
	virtual int occurs_in(const Var *) const;
	Term *clone() const;
	int operator==(const Term &other);
};

/*
 * Class Var represents variables. Variables must be expressions
 * of the form "X1" and "Y5", i.e. one capital letter and an
 * integer. If the integer is 1 it may be left out, so that "X" is
 * equivalent to "X1". The capital is treated as a sign and the integer
 * as the sign's value. An array is used to keep track of the highest
 * value of each sign so that variables can easily be updated to a
 * "fresh" value (a value that is not used by any other variable).
 *
 * During unification the substitution of variables is not actually
 * carried out. Instead, if a variable X is to be replaced by term T this
 * information is saved in an environment list and every occurence of X
 * will be treated as if it were T. Information from the environment
 * list, i.e. the actual value of a variable, is retrieved by function
 * get_realvalue().
 */
class Var : public Term
{
    public:
	Var(const char *);
	TermType gettype() const;
	int unify_me(const Term &, Substitution &) const;
	int match(const Term &) const;
	Term *dup() const;
	void display(ostream &) const;
	void update();
	const Term *get_realvalue(const Substitution &) const;
    private:
	int val;
	char name;
	static TermType type;
	static unsigned int vartable[26];
};

/*
 * Class Constant represents constants. Constants are strings and
 * as such each Constant object contains a String object.
 */
class Constant : public Term
{
    public:
	Constant(const char *);
	TermType gettype() const;
	int unify_me(const Term &, Substitution &) const;
	int match(const Term &) const;
	Term *dup() const;
	void display(ostream &) const;
	const Term *get_realvalue(const Substitution &) const;
    private:
	String val;

	static TermType type;
};

/*
 * Class Complex represents predicates, i.e. expressions of the
 * form "man(X)", "f(a, b(Y2))". A predicate is regarded as
 * a combination of a functor and its arguments: variables, constants
 * and other predicates. The functor is represented by a String object
 * and the arguments by a list of pointers to Term objects:
 * PtrList<Term>.
 */
class Complex : public Term
{
    friend ostream &operator<<(ostream &, const Complex &);
    public:
	Complex();
	Complex(const char *);
        Complex &operator=(const Complex &);
	Complex &operator=(const char *);
	int operator==(const Complex &) const;
	int operator!=(const Complex &) const;
	Complex *clone() const;
	int unify(const Complex &, Substitution &) const;
	void apply_subst(Substitution &);
	void update_vars();
	void update_vars(Substitution &);
	void display() const;
	int notempty() const;
    private:
	TermType gettype() const;
	int match(const Term &) const;
	int unify_me(const Term &, Substitution &) const;
	Term *dup() const;
	const Term *get_realvalue(const Substitution &) const;
	int occurs_in(const Var *) const;
	void display(ostream &) const;
	void parse_string(const char *);
	char *read_body(char *, char *);
	void parse_error(char *, char *, char *);

	String functor;
        PtrArray<Term> arguments;
	static TermType type;
};

/*
 * Class Binding represents bindings: pairs of variables and
 * values that the variables are bound to. Bindings are stored
 * in a Substitution object.
 */
class Binding
{
    friend ostream &operator<<(ostream &, const Binding &);
    friend Substitution;
    public:
	Binding(const Binding &);
	Binding(const Var &, const Term &);
	~Binding();
        Binding &operator=(const Binding &);
	int operator==(const Binding &) const;
	Binding *clone() const;
	void replace_binding(Binding *);
    private:
        Var *first;
	Term *second;
};


/*
 * Class Substitution represents substitutions: collections of
 * bindings. As such class Substitution contains a list of Binding
 * objects: PtrList<Binding>.
 */
class Substitution
{
    friend ostream &operator<<(ostream &, const Substitution &);
    public:
	int operator==(const Substitution &) const;
	int operator!=(const Substitution &) const;
	Substitution *clone() const;
	int is_empty() const;
	int compose(const Substitution &);
	void update(const Substitution &);
	void clear();

	void add_binding(const Var &, const Term &);
	const Term *get_bound(const Var &) const;
    private:
	int occurs_in(const Var *) const;

	PtrArray<Binding> bindings;
};

#endif
