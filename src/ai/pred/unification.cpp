// code for unification. the algorithm is from the book Artificial Intelligence
// by E. Rich and K. Knight (2nd edition) on pages 152, 153.

// headers
#include "unification.h"

// unify two term, if possible
int
unify(Term &t1, Term &t2, Substitutions &s)
{
	// check if a variable or constant
	if ((t1.type != Term::PredicateFunction && 
	     t1.type != Term::Function) ||
	    (t2.type != Term::PredicateFunction && 
	     t2.type != Term::Function))
	{
		// type better be known
		MustBeTrue((t1.type != Term::Unknown) && 
			   (t2.type != Term::Unknown));

		// check if they are identical
		if (t1 == t2)
		{
			// no substitutions required
			s.clear();
		}
		else if (t1.type == Term::Variable)
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
		else if (t2.type == Term::Variable)
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
	if (t1 != t2)
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
	ListIterator<Term *> t1ai(*t1.pargs);
	ListIterator<Term *> t2ai(*t2.pargs);

	// cycle through the arguments
	Substitutions s2;
	for ( ; !t1ai.done() && !t2ai.done(); t1ai++, t2ai++)
	{
		// clear old substitutions
		s2.clear();

		// attempt to unify the arguments
		int status = unify(*t1ai(), *t2ai(), s2);
		if (status != OK)
		{
			// not unifiable
			s.clear();
			return(status);
		}

		// check if any substitutions were generated
		if (s2.isEmpty()) continue;

		// apply substitution to remaining arguments
		ListIterator<Term *> restoft1ai(t1ai);
		ListIterator<Term *> restoft2ai(t2ai);
		for (restoft1ai++, restoft2ai++;
		     !restoft1ai.done() && !restoft2ai.done();
		     restoft1ai++, restoft2ai++)
		{
			if (s2.applyTo(*restoft1ai()) != OK)
			{
				// not unifiable
				s.clear();
				return(NOMATCH);
			}
			if (s2.applyTo(*restoft2ai()) != OK)
			{
				// not unifiable
				s.clear();
				return(NOMATCH);
			}
		}

		// save substitution
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

