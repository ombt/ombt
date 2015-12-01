// headers
#include <stdio.h>
#include <string.h>
#include "stabtokenizer.h"
#include "spaces.h"

// forward declarations
int p_numbits(StabTokenizer &);
int p_numbytes(StabTokenizer &);
int p_typeid(StabTokenizer &);
int p_ordvalue(StabTokenizer &);
int p_bound(StabTokenizer &);
int p_fieldlist(StabTokenizer &);
int p_variantpart(StabTokenizer &);
int p_picsize(StabTokenizer &);

// local definitions for debugging
#define MYENTRY(_f_) \
	char _funcname_[BUFSIZ]; \
	strcpy(_funcname_, _f_); \
	traceentry(_f_, __FILE__, __LINE__);

#define MYRETURN(_status_) \
	return(tracereturn(_status_, _funcname_, __FILE__, __LINE__))

// trace functions
void traceentry(const char *fu, const char *f, int l)
{
	Spaces spaces;
	fprintf(stdout, "%sentry %s at %s'%d ...\n", spaces(), fu, f, l);
	spaces+=3;
	return;
}
int tracereturn(int s, const char *fu, const char *f, int l)
{
	Spaces spaces;
	switch (s)
	{
	case OK:
		spaces-=3;
		fprintf(stdout, "%sreturn OK from %s at %s'%d ...\n", spaces(), fu, f, l);
		return(OK);
	case NOTOK:
		spaces-=3;
		fprintf(stdout, "%sreturn NOTOK from %s at %s'%d ...\n", spaces(), fu, f, l);
		return(NOTOK);
	case NOMATCH:
		spaces-=3;
		fprintf(stdout, "%sreturn NOMATCH from %s at %s'%d ...\n", spaces(), fu, f, l);
		return(NOMATCH);
	default:
		spaces-=3;
		fprintf(stdout, "%sreturn UNKNOWN from %s at %s'%d ...\n", spaces(), fu, f, l);
		return(s);
	}
}

// parse functions
int p_bitoffset(StabTokenizer &st)
{
	MYENTRY("bitoffset");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_field(StabTokenizer &st)
{
	MYENTRY("field");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (p_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (p_bitoffset(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (p_numbits(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_optvirtualfuncindex(StabTokenizer &st)
{
	MYENTRY("optvirtualfuncindex");
	if (st.isInteger())
		st.accept();
	MYRETURN(OK);
}

int p_initbody(StabTokenizer &st)
{
	MYENTRY("initbody");
	if (st.isString())
		st.accept();
	MYRETURN(OK);
}

int p_functype(StabTokenizer &st)
{
	MYENTRY("functype");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'f':
	case 'c':
	case 'd':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_memberfuncattrs(StabTokenizer &st)
{
	MYENTRY("memberfuncattrs");
	if (!st.isChar())
		MYRETURN(OK);
	switch (st.getChar())
	{
	case 's':
	case 'i':
	case 'k':
	case 'V':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(OK);
	}
}

int p_memberfunction(StabTokenizer &st)
{
	MYENTRY("memberfunction");
	if (!st.isChar() || st.getChar() != '[')
		MYRETURN(NOMATCH);
	st.accept();
	if (p_functype(st) != OK)
		MYRETURN(NOTOK);
	if (p_memberfuncattrs(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isName())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (p_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_isstatic(StabTokenizer &st)
{
	MYENTRY("isstatic");
	if (st.isChar() && st.getChar() == 's')
		st.accept();
	MYRETURN(OK);
}

int p_isvtblptr(StabTokenizer &st)
{
	MYENTRY("isvtblptr");
	if (st.isChar() && st.getChar() == 'p')
	{
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
	}
	MYRETURN(OK);
}

int p_isvbaseptr(StabTokenizer &st)
{
	MYENTRY("isvbaseptr");
	if (st.isChar() && 
	   (st.getChar() == 'b' || st.getChar() == 'r'))
		st.accept();
	MYRETURN(OK);
}

int p_memberattrs(StabTokenizer &st)
{
	MYENTRY("memberattrs");
	int status = p_isstatic(st);
	if (status != OK && status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_isvtblptr(st);
	if (status != OK && status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_isvbaseptr(st);
	if (status != OK && status != NOMATCH)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int p_datamember(StabTokenizer &st)
{
	MYENTRY("datamember");
	int status = p_memberattrs(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(OK);
	st.accept();
	status = p_field(st);
	if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int p_friendclass(StabTokenizer &st)
{
	MYENTRY("friendclass");
	if (!st.isChar() || st.getChar() != '(')
		MYRETURN(NOMATCH);
	st.accept();
	if (p_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int p_friendfunction(StabTokenizer &st)
{
	MYENTRY("friendfunction");
	if (!st.isChar() || st.getChar() != ']')
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isName())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (p_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int p_nestedclass(StabTokenizer &st)
{
	MYENTRY("nestedclass");
	if (!st.isChar() || st.getChar() != 'N')
		MYRETURN(NOMATCH);
	st.accept();
	if (p_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int p_accessspec(StabTokenizer &st)
{
	MYENTRY("accessspec");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'i':
	case 'o':
	case 'u':
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(OK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_anonspec(StabTokenizer &st)
{
	MYENTRY("anonspec");
	if (!st.isChar() || st.getChar() != 'a')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int p_genspec(StabTokenizer &st)
{
	MYENTRY("genspec");
	if (!st.isChar() || st.getChar() != 'c')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int p_virtualspec(StabTokenizer &st)
{
	MYENTRY("virtualspec");
	if (!st.isChar() || st.getChar() != 'v')
		MYRETURN(OK);
	st.accept();
	if (st.isChar() && st.getChar() == 'p')
		st.accept();
	MYRETURN(OK);
}

int p_virtualaccessspec(StabTokenizer &st)
{
	MYENTRY("virtualaccessspec");
	int status = p_accessspec(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != 'v')
		MYRETURN(OK);
	st.accept();
	status = p_accessspec(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else 
		MYRETURN(NOTOK);
}

int p_classtypeid(StabTokenizer &st)
{
	MYENTRY("calsstypeid");
	int status = p_typeid(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int p_baseclassoffset(StabTokenizer &st)
{
	MYENTRY("baseclassoffset");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_basespec(StabTokenizer &st)
{
	MYENTRY("basespec");
	int status;
	status = p_virtualaccessspec(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_baseclassoffset(st);
	if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	status = p_classtypeid(st);
	if (status != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int p_basespeclist(StabTokenizer &st)
{
	MYENTRY("basespeclist");
	int status = p_basespec(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar())
		MYRETURN(NOTOK);
	if (st.getChar() != ',')
		MYRETURN(OK);
	status = p_basespec(st);
	if (status != OK)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int p_optbasespeclist(StabTokenizer &st)
{
	MYENTRY("optbasespeclist");
	int status = p_basespeclist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int p_vtag(StabTokenizer &st)
{
	MYENTRY("vtag");
	if (!st.isChar() || st.getChar() != '(')
		MYRETURN(NOMATCH);
	st.accept();
	if (st.isName())
	{
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	}
	if (p_field(st) != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}


int p_vrange(StabTokenizer &st)
{
	MYENTRY("vrange");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'b':
		st.accept();
		if (p_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'c':
		st.accept();
		if (p_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'e':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (p_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'i':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'r':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (p_bound(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (p_bound(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_vrangelist(StabTokenizer &st)
{
	MYENTRY("vrangelist");
	int status = p_vrange(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar())
		MYRETURN(NOTOK);
	if (st.getChar() != ',')
		MYRETURN(OK);
	status = p_vrangelist(st);
	if (status != OK)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int p_vfield(StabTokenizer &st)
{
	MYENTRY("vfield");
	if (!st.isChar() || st.getChar() != '(')
		MYRETURN(NOMATCH);
	st.accept();
	if (p_vrangelist(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (p_fieldlist(st) != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}

int p_vlist(StabTokenizer &st)
{
	MYENTRY("vlist");
	int status = p_vfield(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_variantpart(st);
	if (status != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}

int p_vfieldlist(StabTokenizer &st)
{
	MYENTRY("vfieldlist");
	int status = p_vlist(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_vfieldlist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int p_variantpart(StabTokenizer &st)
{
	MYENTRY("variantpart");
	if (!st.isChar() || st.getChar() != '[')
		MYRETURN(NOMATCH);
	st.accept();
	if (p_vtag(st) != OK)
		MYRETURN(NOTOK);
	if (p_vfieldlist(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ']')
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_passby(StabTokenizer &st)
{
	MYENTRY("passby");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_fieldlist(StabTokenizer &st)
{
	MYENTRY("fieldlist");
	int status = p_field(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_fieldlist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int p_accessmethod(StabTokenizer &st)
{
	MYENTRY("accessmethod");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'd':
	case 'o':
	case 'r':
	case 's':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}
int p_organization(StabTokenizer &st)
{
	MYENTRY("organization");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'i':
	case 'l':
	case 'r':
	case 's':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_cobolfiledesc(StabTokenizer &st)
{
	MYENTRY("cobolfiledesc");
	int status = p_organization(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (p_accessmethod(st) != OK)
		MYRETURN(NOTOK);
	if (p_numbytes(st) != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}

int p_classkey(StabTokenizer &st)
{
	MYENTRY("classkey");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 's':
	case 'u':
	case 'c':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_optpbv(StabTokenizer &st)
{
	MYENTRY("optpbv");
	if (!st.isChar() || st.getChar() != 'V')
		MYRETURN(NOMATCH);
	else
		MYRETURN(OK);
	MYRETURN(OK);
}

int p_extendedfield(StabTokenizer &st)
{
	MYENTRY("extendedfield");
	int status = p_genspec(st);
	if (status == OK)
	{
		status = p_virtualspec(st);
		if (status == NOMATCH)
		{
			if (p_accessspec(st) != OK)
				MYRETURN(NOTOK);
			if (p_anonspec(st) != OK)
				MYRETURN(NOTOK);
			if (p_datamember(st) != OK)
				MYRETURN(NOTOK);
			MYRETURN(OK);
		}
		else if (status == OK)
		{
			if (p_accessspec(st) != OK)
				MYRETURN(NOTOK);
			if (p_optvirtualfuncindex(st) != OK)
				MYRETURN(NOTOK);
			if (p_memberfunction(st) != OK)
				MYRETURN(NOTOK);
			MYRETURN(OK);
		}
		else
			MYRETURN(NOTOK);
	}
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_accessspec(st);
	if (status == OK)
	{
		if (p_anonspec(st) != OK)
			MYRETURN(NOTOK);
		if (p_nestedclass(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_anonspec(st);
	if (status == OK)
	{
		status = p_friendclass(st);
		if (status == OK)
			MYRETURN(OK);
		else if (status != NOMATCH)
			MYRETURN(NOTOK);
		status = p_friendfunction(st);
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	else if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else
		MYRETURN(NOTOK);
}

int p_extendedfieldlist(StabTokenizer &st)
{
	MYENTRY("extendedfieldlist");
	int status = p_extendedfield(st);
	if (status == NOMATCH)
		MYRETURN(OK);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_extendedfieldlist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int p_membername(StabTokenizer &st)
{
	MYENTRY("membername");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_nameresolution(StabTokenizer &st)
{
	MYENTRY("nameresolution");
	int status = p_membername(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	status = p_classtypeid(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else 
		MYRETURN(NOTOK);
}

int p_nameresolutionlist(StabTokenizer &st)
{
	MYENTRY("nameresolutionlist");
	int status = p_nameresolution(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(OK);
	status = p_nameresolutionlist(st);
	if (status != OK)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int p_optnameresolutionlist(StabTokenizer &st)
{
	MYENTRY("optnameresolutionlist");
	if (!st.isChar() || st.getChar() != ')')
		MYRETURN(OK);
	st.accept();
	if (p_nameresolutionlist(st) != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}

int p_redefinition(StabTokenizer &st)
{
	MYENTRY("redefinition");
	if (!st.isChar() || st.getChar() != 'r')
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isName())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_sign(StabTokenizer &st)
{
	MYENTRY("sign");
	if (!st.isChar())
		MYRETURN(OK);
	if (st.getChar() == '+')
	{
		st.accept();
		MYRETURN(OK);
	}
	else if (st.getChar() == '-')
	{
		st.accept();
		MYRETURN(OK);
	}
	else
		MYRETURN(OK);
}

int p_decimalsite(StabTokenizer &st)
{
	MYENTRY("decimalsite");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_kanjichar(StabTokenizer &st)
{
	MYENTRY("kanjichar");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_conditionprimitive(StabTokenizer &st)
{
	MYENTRY("conditionprimitive");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'n':
		st.accept();
		if (p_sign(st) != OK)
			MYRETURN(NOTOK);
		if (p_decimalsite(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'a':
		st.accept();
		MYRETURN(OK);
	case 'f':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_conditiontype(StabTokenizer &st)
{
	MYENTRY("conditiontype");
	int status = p_conditionprimitive(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (p_kanjichar(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int p_arbitrarycharacters(StabTokenizer &st)
{
	MYENTRY("arbitrarycharacters");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_value(StabTokenizer &st)
{
	MYENTRY("value");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (p_arbitrarycharacters(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_valuelist(StabTokenizer &st)
{
	MYENTRY("valuelist");
	int status = p_value(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_valuelist(st);
	if (status == NOMATCH || status == OK)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int p_condition(StabTokenizer &st)
{
	MYENTRY("condition");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != '=')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != 'q')
		MYRETURN(NOTOK);
	st.accept();
	if (p_conditiontype(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (p_valuelist(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_conditions(StabTokenizer &st)
{
	MYENTRY("conditions");
	int status = p_condition(st);
	if (status == NOMATCH)
		MYRETURN(OK);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_conditions(st);
	if (status == NOMATCH || status == OK)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int p_condfieldlist(StabTokenizer &st)
{
	MYENTRY("condfieldlist");
	int status = p_fieldlist(st);
	if (status == OK)
	{
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	}
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_conditions(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	if (p_fieldlist(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int p_numparams(StabTokenizer &st)
{
	MYENTRY("numparams");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_namedtparam(StabTokenizer &st)
{
	MYENTRY("namedtparam");
	int status = NOMATCH;
	if (st.isName())
	{
		st.accept();
		status = NOTOK;
	}
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(status);
	st.accept();
	if (p_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (p_passby(st) != OK)
		MYRETURN(NOTOK);
	if (p_initbody(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_tparam(StabTokenizer &st)
{
	MYENTRY("tparam");
	int status;
	status = p_typeid(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	status = p_passby(st);
	if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_namedtplist(StabTokenizer &st)
{
	MYENTRY("nametplist");
	int status;
	status = p_namedtparam(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_namedtplist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int p_namedtparamlist(StabTokenizer &st)
{
	MYENTRY("nametparamlist");
	int status;
	status = p_namedtplist(st);
	if (status == NOMATCH || status == OK)
		MYRETURN(OK);
	else 
		MYRETURN(NOTOK);
}
int p_tparamlist(StabTokenizer &st)
{
	MYENTRY("tparamlist");
	int status;
	status = p_tparam(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_tparamlist(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int p_array(StabTokenizer &st)
{
	MYENTRY("array");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'a':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'A':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'D':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'E':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'O':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'P':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
	MYRETURN(OK);
}

int p_enum(StabTokenizer &st)
{
	MYENTRY("enum");
	int status;
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	status = p_ordvalue(st);
	if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_enumlist(StabTokenizer &st)
{
	MYENTRY("enumlist");
	int status;
	status = p_enum(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = p_enumlist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(status);
	else
		MYRETURN(NOTOK);
}

int p_enumspec(StabTokenizer &st)
{
	MYENTRY("enumspec");
	int status;
	status = p_enumlist(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_typeid(st);
	if (status != OK)
	{
		if (status == NOMATCH)
			MYRETURN(NOMATCH);
		else
			MYRETURN(NOTOK);
	}
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	status = p_enumlist(st);
	if (status != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int p_optmultibasespec(StabTokenizer &st)
{
	MYENTRY("optmultibasespec");
	if (!st.isChar() || st.getChar() != 'm')
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_editdescription(StabTokenizer &st)
{
	MYENTRY("editdescription");
	if (st.isString())
	{
		st.accept();
		MYRETURN(OK);
	}
	else if (st.isInteger())
	{
		st.accept();
		MYRETURN(OK);
	}
	else 
		MYRETURN(NOMATCH);
}

int p_picstoragetype(StabTokenizer &st)
{
	MYENTRY("picstoragetype");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_usage(StabTokenizer &st)
{
	MYENTRY("usage");
	int status = p_redefinition(st);
	if (status != NOMATCH && status != OK)
		MYRETURN(NOTOK);
	status = p_picstoragetype(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (p_numbits(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (p_editdescription(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (p_picsize(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar())
		MYRETURN(NOTOK);
	if (st.getChar() == ';')
	{
		st.accept();
		MYRETURN(OK);
	}
	else if (st.getChar() == ',')
	{
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_condition(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	}
	else 
		MYRETURN(NOTOK);
}

int p_picsize(StabTokenizer &st)
{
	MYENTRY("picsize");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_boundtype(StabTokenizer &st)
{
	MYENTRY("boundtype");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'A':
	case 'S':
	case 'T':
	case 'a':
	case 't':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_bound(StabTokenizer &st)
{
	MYENTRY("bound");
	int status;
	if ((st.isInteger()) ||
	    (st.isChar() && st.getChar() == 'J'))
	{
		st.accept();
		MYRETURN(OK);
	}
	status = p_boundtype(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_subrange(StabTokenizer &st)
{
	MYENTRY("subrange");
	if (!st.isChar() || st.getChar() != 'r')
		MYRETURN(NOMATCH);
	st.accept();
	if (p_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	if (p_bound(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	if (p_bound(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int p_proceduretype(StabTokenizer &st)
{
	MYENTRY("proceduretype");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'f':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar())
			MYRETURN(NOTOK);
		switch (st.getChar())
		{
		case ';':
			st.accept();
			MYRETURN(OK);
		case ',':
			st.accept();
			if (p_numparams(st) != OK)
				MYRETURN(NOTOK);
			if (!st.isChar() || st.getChar() != ';')
				MYRETURN(NOTOK);
			st.accept();
			if (p_tparamlist(st) != OK)
				MYRETURN(NOTOK);
			if (!st.isChar() || st.getChar() != ';')
				MYRETURN(NOTOK);
			st.accept();
			MYRETURN(OK);
		default:
			MYRETURN(NOTOK);
		}
	case 'p':
		st.accept();
		if (p_numparams(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (p_tparamlist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'R':
		st.accept();
		if (p_numparams(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (p_namedtparamlist(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'F':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (p_numparams(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (p_tparamlist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
	MYRETURN(OK);
}

int p_record(StabTokenizer &st)
{
	MYENTRY("record");
	int status;
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 's':
		st.accept();
		if (p_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_fieldlist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'u':
		st.accept();
		if (p_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_fieldlist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'v':
		st.accept();
		if (p_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_fieldlist(st) != OK)
			MYRETURN(NOTOK);
		if (p_variantpart(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'Y':
		st.accept();
		if (p_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (p_classkey(st) != OK)
			MYRETURN(NOTOK);
		if (p_optpbv(st) != OK)
			MYRETURN(NOTOK);
		if (p_optbasespeclist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != '(')
			MYRETURN(NOTOK);
		st.accept();
		if (p_extendedfieldlist(st) != OK)
			MYRETURN(NOTOK);
		if (p_optnameresolutionlist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'G':
		st.accept();
		status = p_redefinition(st);
		switch (status)
		{
		case OK:
			if (!st.isChar() || st.getChar() != ',')
				MYRETURN(NOTOK);
			st.accept();
			if (!st.isChar())
				MYRETURN(NOTOK);
			switch (st.getChar())
			{
			case 'n':
				st.accept();
				if (p_numbits(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != '#')
					MYRETURN(NOTOK);
				st.accept();
				if (p_fieldlist(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != ';')
					MYRETURN(NOTOK);
				st.accept();
				MYRETURN(OK);
			case 'c':
				st.accept();
				if (p_numbits(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != '#')
					MYRETURN(NOTOK);
				st.accept();
				if (p_condfieldlist(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != ';')
					MYRETURN(NOTOK);
				st.accept();
				MYRETURN(OK);
			default:
				MYRETURN(NOTOK);
			}
		case NOMATCH:
			if (!st.isChar())
				MYRETURN(NOTOK);
			switch (st.getChar())
			{
			case 'n':
				st.accept();
				if (p_numbits(st) != OK)
					MYRETURN(NOTOK);
				if (p_fieldlist(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != ';')
					MYRETURN(NOTOK);
				st.accept();
				MYRETURN(OK);
			case 'c':
				st.accept();
				if (p_numbits(st) != OK)
					MYRETURN(NOTOK);
				if (p_condfieldlist(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != ';')
					MYRETURN(NOTOK);
				st.accept();
				MYRETURN(OK);
			default:
				MYRETURN(NOTOK);
			}
		default:
			MYRETURN(NOTOK);
		}
	default:
		MYRETURN(NOMATCH);
	}
}

int p_optvbasespec(StabTokenizer &st)
{
	MYENTRY("optvbasespec");
	if (!st.isChar() || st.getChar() != 'v')
		MYRETURN(NOMATCH);
	else
		MYRETURN(OK);
}

int p_typedef(StabTokenizer &st)
{
	MYENTRY("typedef");
	int status;
	if (st.isInteger())
	{
		st.accept();
		MYRETURN(OK);
	}
	status = p_array(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	status = p_subrange(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	status = p_proceduretype(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	status = p_record(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'b':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_numbytes(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'c':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_numbits(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'd':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'e':
		st.accept();
		if (p_enumspec(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'g':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_numbits(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'i':
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar())
			MYRETURN(NOTOK);
		if (st.getChar() == ';')
		{
			st.accept();
			MYRETURN(OK);
		}
		else if (st.getChar() == ',')
		{
			st.accept();
			if (p_typeid(st) != OK)
				MYRETURN(NOTOK);
			MYRETURN(OK);
		}
		else
			MYRETURN(NOTOK);
	case 'k':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'l':
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'm':
		st.accept();
		status = p_optvbasespec(st);
		if (status != OK && status != NOMATCH)
			MYRETURN(NOTOK);
		status = p_optmultibasespec(st);
		if (status != OK && status != NOMATCH)
			MYRETURN(NOTOK);
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'n':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_numbytes(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'o':
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar())
			MYRETURN(NOTOK);
		if (st.getChar() == ';')
		{
			st.accept();
			MYRETURN(OK);
		}
		else if (st.getChar() == ',')
		{
			st.accept();
			if (p_typeid(st) != OK)
				MYRETURN(NOTOK);
			MYRETURN(OK);
		}
		else
			MYRETURN(NOTOK);
	case 'w':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'z':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_numbytes(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'C':
		st.accept();
		if (p_usage(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'I':
		st.accept();
		if (p_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_picsize(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'K':
		st.accept();
		if (p_cobolfiledesc(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'M':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (p_bound(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'N':
		st.accept();
		MYRETURN(OK);
	case 'Z':
		st.accept();
		MYRETURN(OK);
	case 'S':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case '*':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case '&':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'V':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_typeattr(StabTokenizer &st)
{
	MYENTRY("typeattr");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'a':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 's':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'p':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'P':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_typeattrlisttail(StabTokenizer &st)
{
	MYENTRY("typeattrlisttail");
	int status;
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != '@')
		MYRETURN(NOTOK);
	st.accept();
	if (p_typeattr(st) != OK)
		MYRETURN(NOTOK);
	status = p_typeattrlisttail(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int p_typeattrlist(StabTokenizer &st)
{
	MYENTRY("typeattrlist");
	int status;
	status = p_typeattr(st);
	if (status != OK)
	{
		if (status != NOMATCH)
			MYRETURN(NOTOK);
		MYRETURN(NOMATCH);
	}
	status = p_typeattrlisttail(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int p_typeattrs(StabTokenizer &st)
{
	MYENTRY("typeattrs");
	int status;
	if (!st.isChar() || st.getChar() != '@')
		MYRETURN(NOMATCH);
	st.accept();
	status = p_typeattrlist(st);
	if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_typeidtail(StabTokenizer &st)
{
	MYENTRY("typeidtail");
	int status;
	if (st.isChar() && st.getChar() == '=')
	{
		st.accept();
		if (p_typedef(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	else if ((status = p_typeattrs(st)) != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		if (p_typedef(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	else 
	{
		// epsilon transition
		MYRETURN(OK);
	}
}

int p_typeidsun(StabTokenizer &st)
{
	MYENTRY("typeidsun");
	if (!st.isChar() || st.getChar() != '(')
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ')')
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_typeidno(StabTokenizer &st)
{
	MYENTRY("typeidno");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_typeid(StabTokenizer &st)
{
	MYENTRY("typeid");
	int status;
	status = p_typeidno(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		status = p_typeidtail(st);
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	status = p_typeidsun(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		status = p_typeidtail(st);
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	MYRETURN(NOMATCH);
}

int p_ordvalue(StabTokenizer &st)
{
	MYENTRY("ordvalue");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_numelements(StabTokenizer &st)
{
	MYENTRY("numelements");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_numbits(StabTokenizer &st)
{
	MYENTRY("numbits");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_numbytes(StabTokenizer &st)
{
	MYENTRY("numbytes");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_bitpattern(StabTokenizer &st)
{
	MYENTRY("bitpattern");
	if (!st.isHexInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_namedtype(StabTokenizer &st)
{
	MYENTRY("namedtype");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 't':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'T':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_parameter(StabTokenizer &st)
{
	MYENTRY("parameter");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'a':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'v':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'C':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'D':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'R':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_proc(StabTokenizer &st)
{
	MYENTRY("proc");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'f':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'g':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'm':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'J':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'F':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'I':
		st.accept();
		MYRETURN(OK);
	case 'P':
		st.accept();
		MYRETURN(OK);
	case 'Q':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_proctail(StabTokenizer &st)
{
	MYENTRY("proctail");
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isName())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isName())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int p_procedure(StabTokenizer &st)
{
	MYENTRY("procedure");
	int status;
	status = p_proc(st);
	if (status != OK)
		MYRETURN(status);
	status = p_proctail(st);
	if (status != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int p_variable(StabTokenizer &st)
{
	MYENTRY("variable");
	int status;
	status = p_typeid(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'd':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'r':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'G':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'S':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'V':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'Y':
		st.accept();
		MYRETURN(OK);
	case 'Z':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
	MYRETURN(OK);
}

int p_label(StabTokenizer &st)
{
	MYENTRY("label");
	if (!st.isChar() || st.getChar() != 'L')
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int p_constant(StabTokenizer &st)
{
	MYENTRY("constant");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'b':
		st.accept();
		if (p_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'c':
		st.accept();
		if (p_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'e':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (p_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'i':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'r':
		st.accept();
		if (!st.isReal())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 's':
		st.accept();
		if (!st.isString())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'C':
		st.accept();
		if (!st.isReal())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isReal())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'S':
		st.accept();
		if (p_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (p_numelements(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (p_numbits(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (p_bitpattern(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int p_class(StabTokenizer &st)
{
	MYENTRY("class");
	int status;
	if (st.isChar() && st.getChar() == 'c')
	{
		st.accept();
		if (!st.isChar() || st.getChar() != '=')
			MYRETURN(NOTOK);
		st.accept();
		if (p_constant(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	}
	status = p_namedtype(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_parameter(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_procedure(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_variable(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = p_label(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	MYRETURN(NOMATCH);
}

int p_stabstring(StabTokenizer &st)
{
	MYENTRY("stabstring");
	int status;
	if (st.isName())
	{
		st.accept();
		if (st.isChar() && st.getChar() == ':')
		{
			st.accept();
			if (p_class(st) != OK)
				MYRETURN(NOTOK);
			else
				MYRETURN(OK);
		}
		else
		{
			MYRETURN(NOTOK);
		}
	}
	else if (st.isChar() && st.getChar() == ':')
	{
		st.accept();
		if (p_class(st) != OK)
			MYRETURN(NOTOK);
		else
			MYRETURN(OK);
	}
	else
	{
		MYRETURN(NOMATCH);
	}
}

int
main(int argc, char **argv)
{
	printf("string to parse is ... <<<%s>>>\n\n", argv[1]);
	StabTokenizer st(argv[1]);
	if (p_stabstring(st) == OK)
	{
		printf("string parsed ...\n");
		return(0);
	}
	else
	{
		printf("string NOT parsed ...\n");
		return(2);
	}
}
