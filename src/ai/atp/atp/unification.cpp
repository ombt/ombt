//
// code for unification. the algorithm is from the book Artificial Intelligence
// by E. Rich and K. Knight (2nd edition) on pages 152, 153.
//

// headers
#include "unification.h"

// unify two term, if possible
int
unify(const Terms &t1, const Terms &t2, Substitutions &s)
{
	// check if a variable or constant
	if (t1.type != Terms::Function || t2.type != Terms::Function)
	{
		// type better be known
		MustBeTrue((t1.type != Terms::Unknown) && 
			   (t2.type != Terms::Unknown));

		// check if they are identical
		if (t1.eq(t2))
		{
			// no substitutions required
			s.clear();
		}
		else if (t1.type == Terms::Variable)
		{
			// t1 is a variable, check if t1 occurs in t2
			if (t2.occurs(t1))
			{
				// not unifiable
				s.clear();
				return(NOMATCH);
			}

			// return { t2 / t1 }
			s.insert(Substitution(t2, t1.value));
		}
		else if (t2.type == Terms::Variable)
		{
			// t2 is a variable, check if t2 occurs in t1
			if (t1.occurs(t2))
			{
				// not unifiable
				s.clear();
				return(NOMATCH);
			}

			// return { t1 / t2 }
			s.insert(Substitution(t1, t2.value));
		}
		else
		{
			// not unifiable
			s.clear();
			return(NOMATCH);
		}
		return(OK);
	}

	// symbol name must the same
	if (t1.ne(t2))
	{
		// not unifiable
		s.clear();
		return(NOMATCH);
	}

	// check number arguments
	MustBeTrue(t1.argnum > 0 && t2.argnum > 0);
	if (t1.argnum != t2.argnum)
	{
		// not unifiable
		s.clear();
		return(NOMATCH);
	}

	// unify function arguments
	ListIterator<Terms * > t1ai(*t1.pargs);
	ListIterator<Terms * > t2ai(*t2.pargs);

	// cycle through the arguments
	Substitutions s2;
	for ( ; !t1ai.done() && !t2ai.done(); t1ai++, t2ai++)
	{
		int status;

		// clear old substitutions
		s2.clear();

		// apply current list of substitutions to 
		// the next term.
		if ((status = s.applyTo(*t1ai())) != OK)
		{
			s.clear();
			return(status);
		}
		if ((status = s.applyTo(*t2ai())) != OK)
		{
			s.clear();
			return(status);
		}


		// attempt to unify the terms
		if ((status = unify(*t1ai(), *t2ai(), s2)) != OK)
		{
			// no match or an error.
			s.clear();
			return(status);
		}

		// compose the new substitutions with the old
		s = s*s2;
	}

	// both iterators should be done
	if (!t1ai.done() || !t2ai.done())
	{
		// not unifiable
		s.clear();
		return(NOMATCH);
	}

	// unified
	return(OK);
}

int
unify(const Literal &l1, const Literal &l2, Substitutions &s)
{
	// check if the literals are equivalent, at least.
	if (l1.ne(l2))
		return(NOMATCH);

	// clear substitutions
	s.clear();

	// check if a not a function 
	if ((l1.type != Literal::Function) && (l1.type != Literal::Equal))
	{
		// a literal constant.
		return(OK);
	}

	// the number of arguments must be the same. it's suppose
	// to be the same predicate function.
	//
	MustBeTrue(l1.argnum > 0 && l2.argnum > 0);
	if (l1.argnum != l2.argnum)
	{
		// not unifiable
		s.clear();
		return(NOMATCH);
	}

	// new substitutions 
	Substitutions s2;

	// we have a function. unify term-by-term, if possible.
	ListIterator<Terms * > l1args(*l1.pargs);
	ListIterator<Terms * > l2args(*l2.pargs);
	for ( ; !l1args.done() && !l2args.done(); l1args++, l2args++)
	{
		int status;

		// clear old substitutions
		s2.clear();

		// apply current list of substitutions to 
		// the next term.
		if ((status = s.applyTo(*l1args())) != OK)
		{
			s.clear();
			return(status);
		}
		if ((status = s.applyTo(*l2args())) != OK)
		{
			s.clear();
			return(status);
		}


		// attempt to unify the terms
		if ((status = unify(*l1args(), *l2args(), s2)) != OK)
		{
			// no match or an error.
			s.clear();
			return(status);
		}

		// compose the new substitutions with the old
		s = s*s2;
	}

	// both iterators should be done
	if (!l1args.done() || !l2args.done())
	{
		// not unifiable
		s.clear();
		return(NOMATCH);
	}

	// all done
	return(OK);
}

// unify lists of literals, if possible
int
unify(List<Literal> &ls, Substitutions &s)
{
	// clear all substitutions
	s.clear();

	// attempt to unify all the literals.
	if (ls.getCount() <= 1)
		return(NOMATCH);

	// attempt to unify all the terms
	Substitutions s2;
	ListIterator<Literal> lsi(ls);
	if (lsi.done())
	{
		ERROR("unexpected end-of-iterator.", EINVAL);
		return(NOTOK);
	}
	Literal l0save = lsi();
	for (lsi++; !lsi.done(); lsi++)
	{
		int status;

		// next literal to try to unify
		Literal l0 = l0save;
		Literal li = lsi();

		// clear old substitutions
		s2.clear();

		// apply current list of substitutions to 
		// the next term.
		if ((status = s.applyTo(li)) != OK)
		{
			s.clear();
			return(status);
		}

		// attempt to unify the terms
		if ((status = unify(l0, li, s2)) != OK)
		{
			// no match or an error.
			s.clear();
			return(status);
		}

		// apply current list of substitutions to 
		// the current unified term.
		if ((status = s2.applyTo(l0save)) != OK)
		{
			s.clear();
			return(status);
		}

		// compose the new substitutions with the old
		s = s*s2;
	}

	// all done
	return(OK);
}
