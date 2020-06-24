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

/*
 * The unification algorithm follows the design of Fitting "First-
 * Order Logic and Automated Theorem Proving": if a binding is
 * found for two terms the substitution will not actually be caried
 * out. Instead, if the variable X is to be replaced by term T every
 * occurence of X will be treated as if it were T. These bindings, i.e.
 * pairs of X/T, are saved in an environment list. Every time a
 * variable is encountered this list is checked to determine the
 * "real" value of the variable. The environment list also serves
 * as the solution of the unification: when the unification
 * process ends this list will represent the substitution that unifies
 * both terms.
 * In one important respect we don't follow Fitting: every binding that
 * is added to the environment list is composed immediately with the
 * bindings already in the list.
 */

#include "logic.h"

/*
 * Static variables used to determine the type of the each logic class.
 */
TermType Var::type = Var_T;
TermType Constant::type = Constant_T;
TermType Complex::type = Complex_T;

/*
 * Every object of type Var is treated as a sign (a capital letter) with
 * a value. To keep track of the highest value used for each sign a
 * static array is used.
 */
unsigned int Var::vartable[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

Term::~Term()
{
}

Term *Term::clone() const
{
    return(dup());
}

int Term::operator==(const Term &other)
{
    return(match(other));
}

/*
 * occurs_in() makes sense only for objects of type Complex object, in
 * case of Var and Constant object the result is always false.
 */
int Term::occurs_in(const Var *) const
{
    return(0);
}

/*
 * This routine parses and creates a Var object. A variable consists of a
 * capital and a number. The capital is used as a sign whose value (the number)
 * is stored in an array. When a new value for a sign is introduced the array
 * is updated accordingly. This procedure makes it possible to introduce a
 * simple "variable updating" routine, update_vars(), used to make the
 * variables in a term unqique.
 */
Var::Var(const char *str)
{
    name = *str;

    val = atoi(str + 1);
    if (val > vartable[name - 'A'])
	vartable[name - 'A'] = val;
}

TermType Var::gettype() const
{
    return(type);
}

/*
 * get_realvalue() determines the actual value of a variable:
 * whenever a variable is bound to a new value the binding is not applied
 * immediately but stored in the substitution. This routine checks if
 * the variable is part of the substitution. If it is, the new value
 * of the variable is returned; if not, the variable itself is returned.
 */
const Term *Var::get_realvalue(const Substitution &subst) const
{
    const Term *ret = subst.get_bound(*this);
    return(ret ? ret: this);
}

int Var::unify_me(const Term &other, Substitution &subst) const
{
    const Term *one, *two;

    /*
     * Get the real value of the variable, i.e. the value that the
     * variable is bound to in the substitution. If the resulting
     * term is a different type it will be necessary to call unify_me()
     * again.
     */
    one = get_realvalue(subst);
    if (one->gettype() != Var_T)
	return(one->unify_me(other, subst));

    /*
     * Because the second term may be of type Var also get its real
     * value as well.
     */
    two = other.get_realvalue(subst);

    /* If there's a direct match no substitution is needed. */
    if (one->match(*two))
	return(1);

    /*
     * At this point the unification will succeed unless the variable
     * is part of the second term, i.e perform the occurs check.
     */
    if (two->occurs_in((Var *)one))
	    return(0);

    /* Update the substition with the new binding. */
    subst.add_binding(*(Var *)one, *two);
    return(1);
}

int Var::match(const Term &other) const
{
    if (other.gettype() != type)
	return(0);
    return((((Var &)other).name == name) && (((Var &)other).val == val));
}

Term *Var::dup() const
{
    return(new Var(*this));
}

void Var::display(ostream &stream) const
{
    stream << name;
    if (val)
	stream << val;
}

/*
 * update() updates the variable by increasing the array entry of the
 * sign (the caps letter) associated with the variable and assigning
 * this value to the Var object.
 */
void Var::update()
{
    vartable[name - 'A']++;
    val = vartable[name - 'A'];
}

Constant::Constant(const char *str)
{
    val = str;
}

TermType Constant::gettype() const
{
    return(type);
}

const Term *Constant::get_realvalue(const Substitution &subst) const
{
    /* The real value of a Constant object is the object itself. */
    return(this);
}

int Constant::unify_me(const Term &other, Substitution &subst) const
{ 
    const Term *two;

    /*
     * Because the second term may be of type Var get its actual value,
     * i.e. the value that the variable is bound to in the substitution.
     */
    two = other.get_realvalue(subst);

    /* If there's a direct match no substitution is needed. */
    if (match(*two))
	return(1);

    /* Unification succeeds only if the second term is a variable. */
    if (two->gettype() == Var_T)
    {
	subst.add_binding(*(Var *)two, *this);
	return(1);
    }
    return(0);
}

int Constant::match(const Term &other) const
{
    return((other.gettype() == type) && (((Constant &)other).val == val));
}

Term *Constant::dup() const
{
    return(new Constant(*this));
}

void Constant::display(ostream &stream) const
{
    stream << val;
}

Complex::Complex()
{
}

Complex::Complex(const char *str)
{
    parse_string(str);
}

Complex &Complex::operator=(const Complex &other)
{
    if (this != &other)
    {
        functor = other.functor;
        arguments = other.arguments;
    }

    return(*this);
}

Complex &Complex::operator=(const char *str)
{
    arguments.clear(DoDel);
    parse_string(str);
    return(*this);
}

int Complex::operator==(const Complex &other) const
{
    return(match(other));
}

int Complex::operator!=(const Complex &other) const
{
    return(!match(other));
}

TermType Complex::gettype() const
{
    return(type);
}

const Term *Complex::get_realvalue(const Substitution &subst) const
{
    /* The real value of a Complex object is the object itself. */
    return(this);
}

/*
 * unify() serves as the interface function for the unification
 * process.
 */
int Complex::unify(const Complex &other, Substitution &subst) const
{
    if (!unify_me(other, subst))
    {
	subst.clear();
	return(0);
    }
    return(1);
}

int Complex::unify_me(const Term &other, Substitution &subst) const
{
    const Term *two;
    Complex *c_two;
    int arity;

    /*
     * Because the second term may be of type Var get its actual value,
     * i.e. the value that the variable is bound to in the substitution.
     */
    two = other.get_realvalue(subst);

    /* If there's a direct match no substitution is needed. */
    if (match(*two))
	return(1);

    /*
     * If the second term is a variable and if it does not occur in the
     * current term unification succeeds.
     */
    if (two->gettype() == Var_T && !occurs_in((Var *)two))
    {
	subst.add_binding(*(Var *)two, *this);
	return(1);
    }

    /* If the second term is a constant unification fails. */
    if (two->gettype() == Constant_T)
	return(0);

   /*
    * Because the second term is neither a variable nor a constant
    * it must be of type Complex. First determine if the functor
    * and arity match and next try to unify the arguments.
    */
   c_two = (Complex *)two;
   arity = arguments.getsize();
   if ((c_two->functor != functor) || (c_two->arguments.getsize() != arity))
	return(0);

   /*
    * Call unify_me() recursively to check if the arguments of
    * both terms can be unified.
    */
   for (int i = 0; i < arity; i++)
   {
	if (!arguments[i]->unify_me(*c_two->arguments[i], subst))
	    return(0);
   }
   return(1);
}

int Complex::match(const Term &other) const
{
    if (other.gettype() != type)
	return(0);

    Complex &c_other = (Complex &)other;

    /* Check if the functors match. */
    if (c_other.functor != functor)
	return(0);

    /*
     * Check if the arity matches the arity of the other object
     * and compare the arguments of both objects to each other.
     */
    return(arguments == c_other.arguments);
}

Complex *Complex::clone() const
{
    return(new Complex(*this));
}

Term *Complex::dup() const
{
    return(new Complex(*this));
}

void Complex::display(ostream &stream) const
{
    int arity = arguments.getsize();

    stream << functor << "(";
    if (arity)
    {
  	arguments[0]->display(stream);
	for (int i = 1; i < arity; i++)
	{
	    stream << ", ";
	    arguments[i]->display(stream);
	}
    }
    stream << ")";
}

ostream &operator<<(ostream &stream, const Complex &complex)
{
    complex.display(stream);
    return(stream);
}

void Complex::display() const
{
    cout << *this;
}

int Complex::notempty() const
{
    return(arguments.getsize());
}

/*
 * occurs_in() is the implementation of the ocurrs check: it checks if
 * the Complex object contains the specified Variable.
 */
int Complex::occurs_in(const Var *var) const
{
    int arity = arguments.getsize();

    for (int i = 0; i < arity; i++)
    {
	/*
         * If the argument is itself a Complex object call
	 * occurs_in() on it recursively.
	 */
	if (arguments[i]->gettype() == Complex_T)
	{
	    if (arguments[i]->occurs_in(var))
		return(1);
	}
	/* If the argument matches the variable the occurs check succeeds. */
	else if (arguments[i]->match(*var))
	    return(1);
    }
    return(0);
}

/*
 * apply_subst() applies a substitution to Complex: it updates those
 * variables in the Complex that are bound to a new value in the
 * substitution.
 */
void Complex::apply_subst(Substitution &subst)
{
    const Term *val;
    int i, arity = arguments.getsize();

    for (i = 0; i < arity; i++)
    {
	/*
	 * If the argument is a variable, determine if it is bound
         * in the substitution. If so, the variable is deleted and
	 * replaced by a copy of the term that the variable is bound to.
	 */
        if (arguments[i]->gettype() == Var_T)
        {
            val = subst.get_bound(*(Var *)arguments[i]);
	    if (val)
	    {
                delete(arguments[i]);
                arguments[i] = val->dup();
	    }
	}
	/*
	 * If the argument is itself of type Complex also call apply_subst()
	 * on it recursively.
	 */
        else if (arguments[i]->gettype() == Complex_T)
            ((Complex *)arguments[i])->apply_subst(subst);
    }
}

/*
 * update_vars() updates all variables in Complex, i.e. it makes the
 * variables unique. The original and new values of the variables are
 * stored in the subsitution supplied. The substitution may then be used
 * to update other terms that (may or may not) contain the same variables
 * which must be bound to the same new values.
 * Note that normally the substitution supplied to this function will be
 * empty. But this is not mandatory. If there are already bindings
 * present in the substitution they will be used to update the variables
 * in the Complex object, i.e. the same procedure as used in apply_subst()
 * will be applied. This comes in handy in case these functions must be
 * combined when some of the variables must be replaced by the bindings in
 * the substitution and others must be updated (for an example of this use
 * of update_vars() see the STRIPS planner).
 */
void Complex::update_vars(Substitution &subst)
{
    const Term *term;
    int i, arity = arguments.getsize();

    for (i = 0; i < arity; i++)
    {
	/*
	 * For every variable in the arguments list check it if is
	 * already part of the substitution. If it is not, the variable
	 * must still be updated and a new binding consisting of OldVar/NewVar
	 * is added to the substitution. If it is, the variable has
	 * already been updated and must be replaced by its new value.
	 */
        if (arguments[i]->gettype() == Var_T)
        {
            term = subst.get_bound(*(Var *)arguments[i]);
            if (!term)
            {
                Var old = *(Var *)arguments[i];
                ((Var *)arguments[i])->update();

		subst.add_binding(old, *arguments[i]);
	    }
            else
            {
                delete arguments[i];
                arguments[i] = term->dup();
	    }
	}
	/*
         * If the argument is itself of type Complex call update_vars()
	 * on it recursively.
         */
        else if (arguments[i]->gettype() == Complex_T)
            ((Complex *)arguments[i])->update_vars(subst);
    }
}

void Complex::update_vars()
{
    Substitution subst;
    update_vars(subst);
}

Binding::Binding(const Var &t1, const Term &t2)
{
    first = (Var *)t1.dup();
    second = t2.dup();
}

Binding::Binding(const Binding &other)
{
    first = (Var *)other.first->dup();
    second = other.second->dup();
}

Binding &Binding::operator=(const Binding &other)
{
    if (this != &other)
    {
        delete first;
        delete second;

        first = (Var *)other.first->dup();
        second = other.second->dup();
    }
    return(*this);
}

Binding::~Binding()
{
    delete first;
    delete second;
}

int Binding::operator==(const Binding &other) const
{
    if (!first->match(*other.first))
	return(0);
    return(second->match(*other.second));
}

Binding *Binding::clone() const
{
    return(new Binding(*this));
}

/*
 * replace_binding() determines if the current binding must be
 * updated with the information contained in the other binding and if true,
 * performs the update. For example, if the current binding is X/Y and the
 * other binding is Y/a the current binding is updated to become Y/a.
 */
void Binding::replace_binding(Binding *bind)
{
    /*
     * If the variable of the current binding is bound to an object
     * of type Complex this object may contain variables that must
     * be updated with the binding information of the other
     * binding. To do this a temporary substition will be created
     * consisting of the information in the other binding and this
     * substitution is applied to the Complex object by calling
     * apply_subst().
     */
    if (second->gettype() == Complex_T)
    {
        Substitution s;

        s.add_binding(*bind->first, *bind->second);
        ((Complex *)second)->apply_subst(s);
    }
    /*
     * To determine if the other binding contains new binding
     * information the second term of the binding must match
     * the variable of the other binding. If true, the current 
     * binding is updated by replacing the second term with the
     * the new value (the second term) of the other binding.
     */
    else if(second->match(*bind->first))
    {
        delete(second);
        second = bind->second->dup();
    }
}

ostream &operator<<(ostream &stream, const Binding &bind)
{
    bind.first->display(stream);
    stream << "/";
    bind.second->display(stream);
    return(stream);
}

int Substitution::operator==(const Substitution &other) const
{
    if (bindings.getsize() != other.bindings.getsize())
        return(0);

    for (int i = 0; i < bindings.getsize(); i++)
        if (other.bindings.find(bindings[i]) < 0)
            return(0);
    return(1);
}

int Substitution::operator!=(const Substitution &other) const
{
    return(!(*this == other));
}

Substitution *Substitution::clone() const
{
    return(new Substitution(*this));
}

int Substitution::is_empty() const
{
    return(bindings.getsize() == 0);
}

/*
 * add_binding() adds a new Var/Term pair to the substitution. Two
 * things must be done: a new binding consisting of Var/Term must
 * be created and added to the substitution and existsing binding
 * information must be updated with the new information supplied by
 * this new binding.
 */
void Substitution::add_binding(const Var &var, const Term &term)
{
    /* Create a new binding consisting of Var/Term. */
    Binding *bind = new Binding(var, term);

    /* Update existing bindings where necessary. */
    for (int i = 0; i < bindings.getsize(); i++)
	bindings[i]->replace_binding(bind);

    /* Add the new binding to the other bindings. */
    bindings += bind;
}

/*
 * Compose() composes two substitutions. Before the actual
 * composition can take place two things must be checked. If {X/Y}
 * is a binding in the first substitution X must not be bound to
 * a different value, e.g. {X/a}, in the other substitution. If this
 * check fails the subsitutions are incompatible. The second check is
 * a kind of occurs check: if the first substitution contains a
 * binding A {X/g(Y)} and the second substitution contains a binding
 * B {Y/f(X)} composition is not possible because of the reoccurence of
 * X in the second half of binding B.
 */
int Substitution::compose(const Substitution &other)
{
    const Term *term;
    int i;

    for (i = 0; i < bindings.getsize(); i++)
    {
	/*
	 * Determine if the other substitution contains a binding for
	 * this variable.
	 */
        term = other.get_bound(*(Var *)bindings[i]->first);

	if (term)
        {
	    /*
	     * It does. Check if the two values for the variables are
	     * identical (check 1).
	     */
	    if (!bindings[i]->second->match(*term))
	        return(0);
	}
	else
	{
	    /*
	     * It does not. Check if the variable occurs in the second
	     * half of any of the bindings of the other substitution (check 2).
	     */
	    if (other.occurs_in((Var *)bindings[i]->first))
		return(0);
	}
    }

    /*
     * Perform the composition, taking care not to include any
     * bindings from the other substitution that are already part of
     * the current binding.
     */
    for (i = 0; i < other.bindings.getsize(); i++)
	if (bindings.find(other.bindings[i]) < 0)
            add_binding(*other.bindings[i]->first, *other.bindings[i]->second);

    return(1);
}

/*
 * update() performs a limited form of composition. Its main use
 * is to update substitutions like {X/Y} with other substitutions like
 * {Y/a} (so that {X/Y} becomes {X/a}). It cannot be used to update
 * substitutions like {X/f(Y)} and {Y/a}. It differs from compose() in
 * two respects: it does not add the new binding, e.g., {Y/a}, to the
 * current substitution and it does not perform the "occurs check",
 * so be careful when calling this routine.
 */
void Substitution::update(const Substitution &other)
{
    const Term *term;
    int i, numbind = bindings.getsize();

    for (i = 0; i < numbind; i++)
    {
	if (bindings[i]->second->gettype() == Var_T)
	{
	    /*
	     * Determine if the other substitution contains a binding
	     * for this variable. If it does the old value for the
             * variable is deleted and replaced by the new one.
	     */
	    term = other.get_bound(*(Var *)bindings[i]->second);
	    if (term)
	    {
		delete bindings[i]->second;
		bindings[i]->second = term->dup();
	    }
	}
    }
}

/*
 * occurs_in() checks if a variable is part of any of the bindings
 * that the substitution consists of.
 */
int Substitution::occurs_in(const Var *var) const
{
    int numbind = bindings.getsize();

    for (int i = 0; i < numbind; i++)
	if (bindings[i]->second->occurs_in(var))
	    return(1);

    return(0);
}

/*
 * get_bound() checks if a variable is part of the substitution
 * and if true, returns the term that is it bound to.
 */
const Term *Substitution::get_bound(const Var &term) const
{
    int i, numbind = bindings.getsize();

    for (i = 0; i < numbind; i++)
    {
	if (bindings[i]->first->match(term))
	    return(bindings[i]->second);
    }
    return(NULL);
}

ostream &operator<<(ostream &stream, const Substitution &subst)
{
    int numbind = subst.bindings.getsize();

    stream << "(";

    if (numbind)
    {
	stream << *subst.bindings[0];
	for (int i = 1; i < numbind; i++)
	    stream << ", " << *subst.bindings[i];
    }
    stream << ")";
    return(stream);
}

void Substitution::clear()
{
    bindings.clear(DoDel);
}


/*
 * Parsing routines, including a limited and primitive form of
 * error checking. Examine at your own risk.
 */
void Complex::parse_string(const char *str)
{
    char tmp, *foo, *p;

    if (!(foo = strdup(str)))
    {
	puts("Complex::Complex() - Out of memory");
	exit(0);
    }

    for (p = foo; *p && isalpha(*p); p++)
	;
    if (!*p)
	parse_error("Unexpected end of term", foo, p);
    if (*p != '(')
	parse_error("Unexpected character", foo, p);

    tmp = *p;
    *p = '\0';
    /* Initialize functor. */
    functor = foo;

    *p = tmp;
    /* Initialize arguments. */
    read_body(foo, p + 1);
    delete(foo);
}

char *Complex::read_body(char *str, char *pos)
{
    Term *term;
    char c, *p, *start, *space;
    char *end_msg = "unexpected end of term";

    for (p = pos; *p; p++)
    {
	while (*p && isspace(*p))
	    p++;
	if (!*p)
	    parse_error(end_msg, str, p);
	start = p;

	/* If the term starts with a capital it must be a variable. */
        if (isupper(*p))
	{
	    while (*++p && isdigit(*p))
		;
	    if (!*p)
		parse_error(end_msg, str, p);

	    if (isspace(*p))
	    {
		space = p;
		while (*++p && isspace(*p))
		    ;
		if (!*p)
		    parse_error(end_msg, str, p);
		if (*p != ',' && *p != ')')
		    parse_error("unexpected character", str, p);
		c = *space;
		*space = '\0';
		term = new Var(start);
		*space = c;
	    }
	    else
	    {
		if (*p != ',' && *p != ')')
		    parse_error("digit expected", str, p);
		c = *p;
	        *p = '\0';
	        term = new Var(start);
	        *p = c;
	    }
	}

	/* Otherwise it must be a constant or complex. */
	else
	{
	    while (*++p && isalpha(*p))
		;
	    if (!*p)
		parse_error(end_msg, str, p);

	    /* Found a complex? */
	    if (*p == '(')
	    {
	        Complex *cp = new Complex;
		/* term = new Complex(str, start, p); */
		c = *p;
		*p = '\0';
		cp->functor = start;
		*p = c;
		p = cp->read_body(str, p + 1);
	        if (!*p)
		    parse_error(end_msg, str, p);
		if (*p != ')')
		    parse_error(") expected", str, p);
		else
		    p++;
		term = cp;
	    }

	    /* Otherwise it must be a constant */
	    else
	    {
	        if (!*p)
		    parse_error(end_msg, str, p);
	        if (isspace(*p))
	        {
		    space = p;
		    while (*++p && isspace(*p))
		        ;
		    if (!*p)
		        parse_error(end_msg, str, p);
		    if (*p != ',' && *p != ')')
		        parse_error("unexpected character", str, p);
		    c = *space;
		    *space = '\0';
		    term = new Constant(start);
		    *space = c;
	        }
		else
		{
		    if (*p != ',' && *p != ')')
		        parse_error("character or digit expected", str, p);
		    c = *p;
		    *p = '\0';
		    term = new Constant(start);
		    *p = c;
		}
	    }
	}

        arguments += term;
	if (!isspace(*p) && *p != ',')
	    return(p);
    }

    return(p);	/* never reached, keeps compiler happy */
}

void Complex::parse_error(char *msg, char *str, char *pos)
{
    int i = pos - str;

    printf("%s\n", str);

    while (i--)
	putchar(' ');

    if (*pos)
        printf("^ '%c': %s\n", *pos, msg);
    else
	printf("^: %s\n", msg);

    exit(0);
}
