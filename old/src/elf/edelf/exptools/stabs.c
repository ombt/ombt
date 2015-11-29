// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"
#include "data.h"
#include "stabtokenizer.h"

// local definitions for debugging
#ifdef DEBUG
#define MYENTRY(_f_) \
	char _funcname_[BUFSIZ]; \
	strcpy(_funcname_, _f_); \
	traceentry(_f_, __FILE__, __LINE__);

#define MYRETURN(_status_) \
	return(tracereturn(_status_, _funcname_, __FILE__, __LINE__))
#else
#define MYENTRY(_f_)

#define MYRETURN(_status_) \
	return(_status_)
#endif

// global and static variables
int dotrace = 0;
static int doparse = 0;
static int useaixsyntax = 0;
static long totalparsesuccess = 0;
static long totalparsefail = 0;

// parser functions to use
struct StabsParsers {
	long key;
	int (*aixpfunc)(StabTokenizer &);
	int (*sunpfunc)(StabTokenizer &);
};

struct TypeSpecsParsers {
	char key;
	int (*pfunc)(StabTokenizer &);
};

// stabs parsers forward declarations 
int aix_stabstring(StabTokenizer &);
int sun_abs(StabTokenizer &);
int sun_alias(StabTokenizer &);
int sun_bcomm(StabTokenizer &);
int sun_bincl(StabTokenizer &);
int sun_brows(StabTokenizer &);
int sun_bss(StabTokenizer &);
int sun_cmdline(StabTokenizer &);
int sun_comm(StabTokenizer &);
int sun_construct(StabTokenizer &);
int sun_data(StabTokenizer &);
int sun_destruct(StabTokenizer &);
int sun_ecoml(StabTokenizer &);
int sun_ecomm(StabTokenizer &);
int sun_eincl(StabTokenizer &);
int sun_endm(StabTokenizer &);
int sun_entry(StabTokenizer &);
int sun_esym(StabTokenizer &);
int sun_excl(StabTokenizer &);
int sun_fn(StabTokenizer &);
int sun_fname(StabTokenizer &);
int sun_fun(StabTokenizer &);
int sun_gsym(StabTokenizer &);
int sun_ildpad(StabTokenizer &);
int sun_isym(StabTokenizer &);
int sun_lbrac(StabTokenizer &);
int sun_lcsym(StabTokenizer &);
int sun_leng(StabTokenizer &);
int sun_lsym(StabTokenizer &);
int sun_main(StabTokenizer &);
int sun_obj(StabTokenizer &);
int sun_opt(StabTokenizer &);
int sun_patch(StabTokenizer &);
int sun_pc(StabTokenizer &);
int sun_psym(StabTokenizer &);
int sun_rbrac(StabTokenizer &);
int sun_rosym(StabTokenizer &);
int sun_rsym(StabTokenizer &);
int sun_sincl(StabTokenizer &);
int sun_sline(StabTokenizer &);
int sun_so(StabTokenizer &);
int sun_sol(StabTokenizer &);
int sun_ssym(StabTokenizer &);
int sun_stsym(StabTokenizer &);
int sun_tcomm(StabTokenizer &);
int sun_text(StabTokenizer &);
int sun_type(StabTokenizer &);
int sun_undf(StabTokenizer &);
int sun_using(StabTokenizer &);
int sun_with(StabTokenizer &);
int sun_xcomm(StabTokenizer &);
int sun_xline(StabTokenizer &);

// types specs parsers forward declarations 
int sun_array(StabTokenizer & );
int sun_volatile(StabTokenizer & );
int sun_basic_integer(StabTokenizer & );
int sun_conformant_array_bounds(StabTokenizer & );
int sun_conformant_array(StabTokenizer & );
int sun_dope_vector_shaped(StabTokenizer & );
int sun_dope_vector_allocated(StabTokenizer & );
int sun_enumeration(StabTokenizer & );
int sun_function_parameter(StabTokenizer & );
int sun_function(StabTokenizer & );
int sun_function_with_prototype_info(StabTokenizer & );
int sun_restricted(StabTokenizer & );
int sun_const(StabTokenizer & );
int sun_pascal_file(StabTokenizer & );
int sun_procedure_parameter(StabTokenizer & );
int sun_floating_point(StabTokenizer & );
int sun_range(StabTokenizer & );
int sun_set(StabTokenizer & );
int sun_structure_or_record(StabTokenizer & );
int sun_union(StabTokenizer & );
int sun_forward_reference(StabTokenizer & );
int sun_cplusplus_specification(StabTokenizer & );
int sun_varying_string(StabTokenizer & );
int sun_c99_variable_length_array(StabTokenizer & );
int sun_pointer(StabTokenizer & );
int sun_reference(StabTokenizer & );

// arrays for determining what parser to use
StabsParsers stabparsers[] = {
	N_ABS, aix_stabstring, sun_abs,
	N_ALIAS, aix_stabstring, sun_alias,
	N_BCOMM, aix_stabstring, sun_bcomm,
	N_BINCL, aix_stabstring, sun_bincl,
	N_BROWS, aix_stabstring, sun_brows,
	N_BSS, aix_stabstring, sun_bss,
	N_CMDLINE, aix_stabstring, sun_cmdline,
	N_COMM, aix_stabstring, sun_comm,
	N_CONSTRUCT, aix_stabstring, sun_construct,
	N_DATA, aix_stabstring, sun_data,
	N_DESTRUCT, aix_stabstring, sun_destruct,
	N_ECOML, aix_stabstring, sun_ecoml,
	N_ECOMM, aix_stabstring, sun_ecomm,
	N_EINCL, aix_stabstring, sun_eincl,
	N_ENDM, aix_stabstring, sun_endm,
	N_ENTRY, aix_stabstring, sun_entry,
	N_ESYM, aix_stabstring, sun_esym,
	N_EXCL, aix_stabstring, sun_excl,
	N_FN, aix_stabstring, sun_fn,
	N_FNAME, aix_stabstring, sun_fname,
	N_FUN, aix_stabstring, sun_fun,
	N_GSYM, aix_stabstring, sun_gsym,
	N_ILDPAD, aix_stabstring, sun_ildpad,
	N_ISYM, aix_stabstring, sun_isym,
	N_LBRAC, aix_stabstring, sun_lbrac,
	N_LCSYM, aix_stabstring, sun_lcsym,
	N_LENG, aix_stabstring, sun_leng,
	N_LSYM, aix_stabstring, sun_lsym,
	N_MAIN, aix_stabstring, sun_main,
	N_OBJ, aix_stabstring, sun_obj,
	N_OPT, aix_stabstring, sun_opt,
	N_PATCH, aix_stabstring, sun_patch,
	N_PC, aix_stabstring, sun_pc,
	N_PSYM, aix_stabstring, sun_psym,
	N_RBRAC, aix_stabstring, sun_rbrac,
	N_ROSYM, aix_stabstring, sun_rosym,
	N_RSYM, aix_stabstring, sun_rsym,
	N_SINCL, aix_stabstring, sun_sincl,
	N_SLINE, aix_stabstring, sun_sline,
	N_SO, aix_stabstring, sun_so,
	N_SOL, aix_stabstring, sun_sol,
	N_SSYM, aix_stabstring, sun_ssym,
	N_STSYM, aix_stabstring, sun_stsym,
	N_TCOMM, aix_stabstring, sun_tcomm,
	N_TEXT, aix_stabstring, sun_text,
	N_UNDF, aix_stabstring, sun_undf,
	N_USING, aix_stabstring, sun_using,
	N_WITH, aix_stabstring, sun_with,
	N_XCOMM, aix_stabstring, sun_xcomm,
	N_XLINE, aix_stabstring, sun_xline,
	-1, NULL, NULL
};

TypeSpecsParsers typespecsparsers[] = {
	'a', sun_array,
	'B', sun_volatile,
	'b', sun_basic_integer,
	'C', sun_conformant_array_bounds,
	'c', sun_conformant_array,
	'D', sun_dope_vector_shaped,
	'd', sun_dope_vector_allocated,
	'e', sun_enumeration,
	'F', sun_function_parameter,
	'f', sun_function,
	'g', sun_function_with_prototype_info,
	'K', sun_restricted,
	'k', sun_const,
	'l', sun_pascal_file,
	'P', sun_procedure_parameter,
	'R', sun_floating_point,
	'r', sun_range,
	'S', sun_set,
	's', sun_structure_or_record,
	'u', sun_union,
	'x', sun_forward_reference,
	'Y', sun_cplusplus_specification,
	'y', sun_varying_string,
	'z', sun_c99_variable_length_array,
	'*', sun_pointer,
	'&', sun_reference,
	'\0', NULL
};

// check if a section is a stab section
int
isStabSection(ElfObject &eo, int csec)
{
	char *psecname = eo.shdrnm(csec);
	return(eo.pshdr(csec).sh_type == SHT_PROGBITS &&
	       ((strncmp(psecname, ".stab.", 6) == 0) ||
	        (strncmp(psecname, ".stab", 5) == 0)));
}

// check if the line continues on the next line
static int isMultilined(const char *ps)
{
	if (ps == NULL || *ps == '\0')
		return(0);
	int pslen = strlen(ps);
	if (*(ps+pslen-1) == '\\')
		return(1);
	else
		return(0);
}

// editing stabs
void
readstabs(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	eo.readstrings();
	eo.readstabs();
	return;
}

static void
listtypes()
{
	printf("\nsupported stabs types:\n");
	for (int i=0; n_type[i].data != NULL; i++)
	{
		printf("%-12s = %ld\n", n_type[i].data, n_type[i].key);
	}
	return;
}

static void
printmenu()
{
	printf("stabs menu:\n");
	printf("? or h - show menu\n");
	printf("? t or h t - list available types\n");
	printf("n - show all stabs tables section names\n");

	printf("r - review current stab\n");
	printf("r * - review all stabs in stab tables\n");
	printf("r * <pattern> - review <pattern> in all stab tables\n");
	printf("rt * <type> - review <type> in all stab tables\n");
	printf("l * <function> - list line numbers for <function> in all stab tables\n");

	printf("r <section> - review 1st stab in <section> stab table\n");
	printf("r <section> * - review all stabs in <section> stab table\n");
	printf("r <section> <pattern> - review <pattern> in <section> stab table\n");
	printf("ri <section> <index#> - review entry <index#> in <section> stab table\n");
	printf("rt <section> <type> - review <type> in <section> stab table\n");
	printf("l <section> <function> - list line numbers for <function> in <section> stab table\n");

	printf("r <section#> - review 1st stab in <section#> stab table\n");
	printf("r <section#> * - review all stabs in <section#> stab table\n");
	printf("r <section#> <pattern> - review <pattern> in <section#> stab table\n");
	printf("ri <section#> <index#> - review entry <index#> in <section#> stab table\n");
	printf("rt <section#> <type> - review <type> in <section#> stab table\n");
	printf("l <section#> <function> - list line numbers for <function> in <section#> stab table\n");

	printf("+ - next stab\n");
	printf("- - previous stab\n");
	printf("u - update current stab in current stab table\n");
	printf("D - toggle demangle mode\n");
	printf("P - toggle parse mode on or off\n");
	printf("P 0 - turn all parse modes off\n");
	printf("P 1 - turn unconditional parse mode on\n");
	printf("P 2 - turn query parse mode on\n");
	printf("A - toggle stabs syntax between SUN and AIX\n");
	printf("S - show parse pass/fail statistics\n");
	printf("T - toggle trace mode\n");
	printf("q - quit\n");
	return;
}

// trace functions
void traceentry(const char *fu, const char *f, int l)
{
	if (!dotrace) return;
	Spaces spaces;
	fprintf(stdout, "%sentry %s at %s'%d ...\n", spaces(), fu, f, l);
	spaces+=3;
	return;
}

int tracereturn(int s, const char *fu, const char *f, int l)
{
	if (!dotrace) return(s);
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

// forward declarations AIX-STABS parse functions
int aix_numbits(StabTokenizer &);
int aix_numbytes(StabTokenizer &);
int aix_typeid(StabTokenizer &);
int aix_ordvalue(StabTokenizer &);
int aix_bound(StabTokenizer &);
int aix_fieldlist(StabTokenizer &);
int aix_variantpart(StabTokenizer &);
int aix_picsize(StabTokenizer &);

// parse functions
int aix_bitoffset(StabTokenizer &st)
{
	MYENTRY("bitoffset");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_field(StabTokenizer &st)
{
	MYENTRY("field");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_bitoffset(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_numbits(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int aix_optvirtualfuncindex(StabTokenizer &st)
{
	MYENTRY("optvirtualfuncindex");
	if (st.isInteger())
		st.accept();
	MYRETURN(OK);
}

int aix_initbody(StabTokenizer &st)
{
	MYENTRY("initbody");
	if (st.isString())
		st.accept();
	MYRETURN(OK);
}

int aix_functype(StabTokenizer &st)
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

int aix_memberfuncattrs(StabTokenizer &st)
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

int aix_memberfunction(StabTokenizer &st)
{
	MYENTRY("memberfunction");
	if (!st.isChar() || st.getChar() != '[')
		MYRETURN(NOMATCH);
	st.accept();
	if (aix_functype(st) != OK)
		MYRETURN(NOTOK);
	if (aix_memberfuncattrs(st) != OK)
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
	if (aix_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int aix_isstatic(StabTokenizer &st)
{
	MYENTRY("isstatic");
	if (st.isChar() && st.getChar() == 's')
		st.accept();
	MYRETURN(OK);
}

int aix_isvtblptr(StabTokenizer &st)
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

int aix_isvbaseptr(StabTokenizer &st)
{
	MYENTRY("isvbaseptr");
	if (st.isChar() && 
	   (st.getChar() == 'b' || st.getChar() == 'r'))
		st.accept();
	MYRETURN(OK);
}

int aix_memberattrs(StabTokenizer &st)
{
	MYENTRY("memberattrs");
	int status = aix_isstatic(st);
	if (status != OK && status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_isvtblptr(st);
	if (status != OK && status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_isvbaseptr(st);
	if (status != OK && status != NOMATCH)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int aix_datamember(StabTokenizer &st)
{
	MYENTRY("datamember");
	int status = aix_memberattrs(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(OK);
	st.accept();
	status = aix_field(st);
	if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int aix_friendclass(StabTokenizer &st)
{
	MYENTRY("friendclass");
	if (!st.isChar() || st.getChar() != '(')
		MYRETURN(NOMATCH);
	st.accept();
	if (aix_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int aix_friendfunction(StabTokenizer &st)
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
	if (aix_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int aix_nestedclass(StabTokenizer &st)
{
	MYENTRY("nestedclass");
	if (!st.isChar() || st.getChar() != 'N')
		MYRETURN(NOMATCH);
	st.accept();
	if (aix_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int aix_accessspec(StabTokenizer &st)
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

int aix_anonspec(StabTokenizer &st)
{
	MYENTRY("anonspec");
	if (!st.isChar() || st.getChar() != 'a')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int aix_genspec(StabTokenizer &st)
{
	MYENTRY("genspec");
	if (!st.isChar() || st.getChar() != 'c')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int aix_virtualspec(StabTokenizer &st)
{
	MYENTRY("virtualspec");
	if (!st.isChar() || st.getChar() != 'v')
		MYRETURN(OK);
	st.accept();
	if (st.isChar() && st.getChar() == 'p')
		st.accept();
	MYRETURN(OK);
}

int aix_virtualaccessspec(StabTokenizer &st)
{
	MYENTRY("virtualaccessspec");
	int status = aix_accessspec(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != 'v')
		MYRETURN(OK);
	st.accept();
	status = aix_accessspec(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else 
		MYRETURN(NOTOK);
}

int aix_classtypeid(StabTokenizer &st)
{
	MYENTRY("calsstypeid");
	int status = aix_typeid(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int aix_baseclassoffset(StabTokenizer &st)
{
	MYENTRY("baseclassoffset");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_basespec(StabTokenizer &st)
{
	MYENTRY("basespec");
	int status;
	status = aix_virtualaccessspec(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_baseclassoffset(st);
	if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	status = aix_classtypeid(st);
	if (status != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int aix_basespeclist(StabTokenizer &st)
{
	MYENTRY("basespeclist");
	int status = aix_basespec(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar())
		MYRETURN(NOTOK);
	if (st.getChar() != ',')
		MYRETURN(OK);
	status = aix_basespec(st);
	if (status != OK)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int aix_optbasespeclist(StabTokenizer &st)
{
	MYENTRY("optbasespeclist");
	int status = aix_basespeclist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int aix_vtag(StabTokenizer &st)
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
	if (aix_field(st) != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}


int aix_vrange(StabTokenizer &st)
{
	MYENTRY("vrange");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'b':
		st.accept();
		if (aix_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'c':
		st.accept();
		if (aix_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'e':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_ordvalue(st) != OK)
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
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_bound(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_bound(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int aix_vrangelist(StabTokenizer &st)
{
	MYENTRY("vrangelist");
	int status = aix_vrange(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar())
		MYRETURN(NOTOK);
	if (st.getChar() != ',')
		MYRETURN(OK);
	status = aix_vrangelist(st);
	if (status != OK)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int aix_vfield(StabTokenizer &st)
{
	MYENTRY("vfield");
	if (!st.isChar() || st.getChar() != '(')
		MYRETURN(NOMATCH);
	st.accept();
	if (aix_vrangelist(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_fieldlist(st) != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}

int aix_vlist(StabTokenizer &st)
{
	MYENTRY("vlist");
	int status = aix_vfield(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_variantpart(st);
	if (status != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}

int aix_vfieldlist(StabTokenizer &st)
{
	MYENTRY("vfieldlist");
	int status = aix_vlist(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_vfieldlist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int aix_variantpart(StabTokenizer &st)
{
	MYENTRY("variantpart");
	if (!st.isChar() || st.getChar() != '[')
		MYRETURN(NOMATCH);
	st.accept();
	if (aix_vtag(st) != OK)
		MYRETURN(NOTOK);
	if (aix_vfieldlist(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ']')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int aix_passby(StabTokenizer &st)
{
	MYENTRY("passby");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_fieldlist(StabTokenizer &st)
{
	MYENTRY("fieldlist");
	int status = aix_field(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_fieldlist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int aix_accessmethod(StabTokenizer &st)
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
int aix_organization(StabTokenizer &st)
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

int aix_cobolfiledesc(StabTokenizer &st)
{
	MYENTRY("cobolfiledesc");
	int status = aix_organization(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (aix_accessmethod(st) != OK)
		MYRETURN(NOTOK);
	if (aix_numbytes(st) != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}

int aix_classkey(StabTokenizer &st)
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

int aix_optpbv(StabTokenizer &st)
{
	MYENTRY("optpbv");
	if (!st.isChar() || st.getChar() != 'V')
		MYRETURN(NOMATCH);
	else
		MYRETURN(OK);
	MYRETURN(OK);
}

int aix_extendedfield(StabTokenizer &st)
{
	MYENTRY("extendedfield");
	int status = aix_genspec(st);
	if (status == OK)
	{
		status = aix_virtualspec(st);
		if (status == NOMATCH)
		{
			if (aix_accessspec(st) != OK)
				MYRETURN(NOTOK);
			if (aix_anonspec(st) != OK)
				MYRETURN(NOTOK);
			if (aix_datamember(st) != OK)
				MYRETURN(NOTOK);
			MYRETURN(OK);
		}
		else if (status == OK)
		{
			if (aix_accessspec(st) != OK)
				MYRETURN(NOTOK);
			if (aix_optvirtualfuncindex(st) != OK)
				MYRETURN(NOTOK);
			if (aix_memberfunction(st) != OK)
				MYRETURN(NOTOK);
			MYRETURN(OK);
		}
		else
			MYRETURN(NOTOK);
	}
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_accessspec(st);
	if (status == OK)
	{
		if (aix_anonspec(st) != OK)
			MYRETURN(NOTOK);
		if (aix_nestedclass(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_anonspec(st);
	if (status == OK)
	{
		status = aix_friendclass(st);
		if (status == OK)
			MYRETURN(OK);
		else if (status != NOMATCH)
			MYRETURN(NOTOK);
		status = aix_friendfunction(st);
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	else if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else
		MYRETURN(NOTOK);
}

int aix_extendedfieldlist(StabTokenizer &st)
{
	MYENTRY("extendedfieldlist");
	int status = aix_extendedfield(st);
	if (status == NOMATCH)
		MYRETURN(OK);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_extendedfieldlist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int aix_membername(StabTokenizer &st)
{
	MYENTRY("membername");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_nameresolution(StabTokenizer &st)
{
	MYENTRY("nameresolution");
	int status = aix_membername(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	status = aix_classtypeid(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else 
		MYRETURN(NOTOK);
}

int aix_nameresolutionlist(StabTokenizer &st)
{
	MYENTRY("nameresolutionlist");
	int status = aix_nameresolution(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(OK);
	status = aix_nameresolutionlist(st);
	if (status != OK)
		MYRETURN(NOTOK);
	else 
		MYRETURN(OK);
}

int aix_optnameresolutionlist(StabTokenizer &st)
{
	MYENTRY("optnameresolutionlist");
	if (!st.isChar() || st.getChar() != ')')
		MYRETURN(OK);
	st.accept();
	if (aix_nameresolutionlist(st) != OK)
		MYRETURN(NOTOK);
	else
		MYRETURN(OK);
}

int aix_redefinition(StabTokenizer &st)
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

int aix_sign(StabTokenizer &st)
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

int aix_decimalsite(StabTokenizer &st)
{
	MYENTRY("decimalsite");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_kanjichar(StabTokenizer &st)
{
	MYENTRY("kanjichar");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_conditionprimitive(StabTokenizer &st)
{
	MYENTRY("conditionprimitive");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'n':
		st.accept();
		if (aix_sign(st) != OK)
			MYRETURN(NOTOK);
		if (aix_decimalsite(st) != OK)
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

int aix_conditiontype(StabTokenizer &st)
{
	MYENTRY("conditiontype");
	int status = aix_conditionprimitive(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_kanjichar(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int aix_arbitrarycharacters(StabTokenizer &st)
{
	MYENTRY("arbitrarycharacters");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_value(StabTokenizer &st)
{
	MYENTRY("value");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_arbitrarycharacters(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int aix_valuelist(StabTokenizer &st)
{
	MYENTRY("valuelist");
	int status = aix_value(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_valuelist(st);
	if (status == NOMATCH || status == OK)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int aix_condition(StabTokenizer &st)
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
	if (aix_conditiontype(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_valuelist(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int aix_conditions(StabTokenizer &st)
{
	MYENTRY("conditions");
	int status = aix_condition(st);
	if (status == NOMATCH)
		MYRETURN(OK);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_conditions(st);
	if (status == NOMATCH || status == OK)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int aix_condfieldlist(StabTokenizer &st)
{
	MYENTRY("condfieldlist");
	int status = aix_fieldlist(st);
	if (status == OK)
	{
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	}
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_conditions(st);
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
	if (aix_fieldlist(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int aix_numparams(StabTokenizer &st)
{
	MYENTRY("numparams");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_namedtparam(StabTokenizer &st)
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
	if (aix_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_passby(st) != OK)
		MYRETURN(NOTOK);
	if (aix_initbody(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int aix_tparam(StabTokenizer &st)
{
	MYENTRY("tparam");
	int status;
	status = aix_typeid(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	status = aix_passby(st);
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

int aix_namedtplist(StabTokenizer &st)
{
	MYENTRY("nametplist");
	int status;
	status = aix_namedtparam(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_namedtplist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int aix_namedtparamlist(StabTokenizer &st)
{
	MYENTRY("nametparamlist");
	int status;
	status = aix_namedtplist(st);
	if (status == NOMATCH || status == OK)
		MYRETURN(OK);
	else 
		MYRETURN(NOTOK);
}
int aix_tparamlist(StabTokenizer &st)
{
	MYENTRY("tparamlist");
	int status;
	status = aix_tparam(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_tparamlist(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int aix_array(StabTokenizer &st)
{
	MYENTRY("array");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'a':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'A':
		st.accept();
		if (aix_typeid(st) != OK)
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
		if (aix_typeid(st) != OK)
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
		if (aix_typeid(st) != OK)
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
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'P':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
	MYRETURN(OK);
}

int aix_enum(StabTokenizer &st)
{
	MYENTRY("enum");
	int status;
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	status = aix_ordvalue(st);
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

int aix_enumlist(StabTokenizer &st)
{
	MYENTRY("enumlist");
	int status;
	status = aix_enum(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	status = aix_enumlist(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(status);
	else
		MYRETURN(NOTOK);
}

int aix_enumspec(StabTokenizer &st)
{
	MYENTRY("enumspec");
	int status;
	status = aix_enumlist(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_typeid(st);
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
	status = aix_enumlist(st);
	if (status != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int aix_optmultibasespec(StabTokenizer &st)
{
	MYENTRY("optmultibasespec");
	if (!st.isChar() || st.getChar() != 'm')
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_editdescription(StabTokenizer &st)
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

int aix_picstoragetype(StabTokenizer &st)
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

int aix_usage(StabTokenizer &st)
{
	MYENTRY("usage");
	int status = aix_redefinition(st);
	if (status != NOMATCH && status != OK)
		MYRETURN(NOTOK);
	status = aix_picstoragetype(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (aix_numbits(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_editdescription(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_picsize(st) != OK)
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
		if (aix_condition(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	}
	else 
		MYRETURN(NOTOK);
}

int aix_picsize(StabTokenizer &st)
{
	MYENTRY("picsize");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_boundtype(StabTokenizer &st)
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

int aix_bound(StabTokenizer &st)
{
	MYENTRY("bound");
	int status;
	if ((st.isInteger()) ||
	    (st.isChar() && st.getChar() == 'J'))
	{
		st.accept();
		MYRETURN(OK);
	}
	status = aix_boundtype(st);
	if (status == NOMATCH)
		MYRETURN(NOMATCH);
	else if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int aix_subrange(StabTokenizer &st)
{
	MYENTRY("subrange");
	if (!st.isChar() || st.getChar() != 'r')
		MYRETURN(NOMATCH);
	st.accept();
	if (aix_typeid(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_bound(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_bound(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int aix_proceduretype(StabTokenizer &st)
{
	MYENTRY("proceduretype");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'f':
		st.accept();
		if (aix_typeid(st) != OK)
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
			if (aix_numparams(st) != OK)
				MYRETURN(NOTOK);
			if (!st.isChar() || st.getChar() != ';')
				MYRETURN(NOTOK);
			st.accept();
			if (aix_tparamlist(st) != OK)
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
		if (aix_numparams(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_tparamlist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'R':
		st.accept();
		if (aix_numparams(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_namedtparamlist(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'F':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_numparams(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_tparamlist(st) != OK)
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

int aix_record(StabTokenizer &st)
{
	MYENTRY("record");
	int status;
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 's':
		st.accept();
		if (aix_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_fieldlist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'u':
		st.accept();
		if (aix_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_fieldlist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'v':
		st.accept();
		if (aix_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_fieldlist(st) != OK)
			MYRETURN(NOTOK);
		if (aix_variantpart(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'Y':
		st.accept();
		if (aix_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (aix_classkey(st) != OK)
			MYRETURN(NOTOK);
		if (aix_optpbv(st) != OK)
			MYRETURN(NOTOK);
		if (aix_optbasespeclist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != '(')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_extendedfieldlist(st) != OK)
			MYRETURN(NOTOK);
		if (aix_optnameresolutionlist(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'G':
		st.accept();
		status = aix_redefinition(st);
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
				if (aix_numbits(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != '#')
					MYRETURN(NOTOK);
				st.accept();
				if (aix_fieldlist(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != ';')
					MYRETURN(NOTOK);
				st.accept();
				MYRETURN(OK);
			case 'c':
				st.accept();
				if (aix_numbits(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != '#')
					MYRETURN(NOTOK);
				st.accept();
				if (aix_condfieldlist(st) != OK)
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
				if (aix_numbits(st) != OK)
					MYRETURN(NOTOK);
				if (aix_fieldlist(st) != OK)
					MYRETURN(NOTOK);
				if (!st.isChar() || st.getChar() != ';')
					MYRETURN(NOTOK);
				st.accept();
				MYRETURN(OK);
			case 'c':
				st.accept();
				if (aix_numbits(st) != OK)
					MYRETURN(NOTOK);
				if (aix_condfieldlist(st) != OK)
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

int aix_optvbasespec(StabTokenizer &st)
{
	MYENTRY("optvbasespec");
	if (!st.isChar() || st.getChar() != 'v')
		MYRETURN(NOMATCH);
	else
		MYRETURN(OK);
}

int aix_typedef(StabTokenizer &st)
{
	MYENTRY("typedef");
	int status;
	if (st.isInteger())
	{
		st.accept();
		MYRETURN(OK);
	}
	status = aix_array(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	status = aix_subrange(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	status = aix_proceduretype(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	status = aix_record(st);
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
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_numbytes(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'c':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_numbits(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'd':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'e':
		st.accept();
		if (aix_enumspec(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'g':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_numbits(st) != OK)
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
			if (aix_typeid(st) != OK)
				MYRETURN(NOTOK);
			MYRETURN(OK);
		}
		else
			MYRETURN(NOTOK);
	case 'k':
		st.accept();
		if (aix_typeid(st) != OK)
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
		status = aix_optvbasespec(st);
		if (status != OK && status != NOMATCH)
			MYRETURN(NOTOK);
		status = aix_optmultibasespec(st);
		if (status != OK && status != NOMATCH)
			MYRETURN(NOTOK);
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'n':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_numbytes(st) != OK)
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
			if (aix_typeid(st) != OK)
				MYRETURN(NOTOK);
			MYRETURN(OK);
		}
		else
			MYRETURN(NOTOK);
	case 'w':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'z':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_numbytes(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'C':
		st.accept();
		if (aix_usage(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'I':
		st.accept();
		if (aix_numbytes(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_picsize(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'K':
		st.accept();
		if (aix_cobolfiledesc(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'M':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_bound(st) != OK)
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
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case '*':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case '&':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'V':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int aix_typeattr(StabTokenizer &st)
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

int aix_typeattrlisttail(StabTokenizer &st)
{
	MYENTRY("typeattrlisttail");
	int status;
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != '@')
		MYRETURN(NOTOK);
	st.accept();
	if (aix_typeattr(st) != OK)
		MYRETURN(NOTOK);
	status = aix_typeattrlisttail(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int aix_typeattrlist(StabTokenizer &st)
{
	MYENTRY("typeattrlist");
	int status;
	status = aix_typeattr(st);
	if (status != OK)
	{
		if (status != NOMATCH)
			MYRETURN(NOTOK);
		MYRETURN(NOMATCH);
	}
	status = aix_typeattrlisttail(st);
	if (status == OK || status == NOMATCH)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int aix_typeattrs(StabTokenizer &st)
{
	MYENTRY("typeattrs");
	int status;
	if (!st.isChar() || st.getChar() != '@')
		MYRETURN(NOMATCH);
	st.accept();
	status = aix_typeattrlist(st);
	if (status != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int aix_typeidtail(StabTokenizer &st)
{
	MYENTRY("typeidtail");
	int status;
	if (st.isChar() && st.getChar() == '=')
	{
		st.accept();
		if (aix_typedef(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	else if ((status = aix_typeattrs(st)) != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		if (aix_typedef(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	else 
	{
		// epsilon transition
		MYRETURN(OK);
	}
}

int aix_typeidsun(StabTokenizer &st)
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

int aix_typeidno(StabTokenizer &st)
{
	MYENTRY("typeidno");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_typeid(StabTokenizer &st)
{
	MYENTRY("typeid");
	int status;
	status = aix_typeidno(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		status = aix_typeidtail(st);
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	status = aix_typeidsun(st);
	if (status != NOMATCH)
	{
		if (status != OK)
			MYRETURN(NOTOK);
		status = aix_typeidtail(st);
		if (status != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	}
	MYRETURN(NOMATCH);
}

int aix_ordvalue(StabTokenizer &st)
{
	MYENTRY("ordvalue");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_numelements(StabTokenizer &st)
{
	MYENTRY("numelements");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_numbits(StabTokenizer &st)
{
	MYENTRY("numbits");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_numbytes(StabTokenizer &st)
{
	MYENTRY("numbytes");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_bitpattern(StabTokenizer &st)
{
	MYENTRY("bitpattern");
	if (!st.isHexInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_namedtype(StabTokenizer &st)
{
	MYENTRY("namedtype");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 't':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'T':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int aix_parameter(StabTokenizer &st)
{
	MYENTRY("parameter");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'a':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'v':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'C':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'D':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'R':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int aix_proc(StabTokenizer &st)
{
	MYENTRY("proc");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'f':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'g':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'm':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'J':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'F':
		st.accept();
		if (aix_typeid(st) != OK)
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

int aix_proctail(StabTokenizer &st)
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

int aix_procedure(StabTokenizer &st)
{
	MYENTRY("procedure");
	int status;
	status = aix_proc(st);
	if (status != OK)
		MYRETURN(status);
	status = aix_proctail(st);
	if (status != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int aix_variable(StabTokenizer &st)
{
	MYENTRY("variable");
	int status;
	status = aix_typeid(st);
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
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'r':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'G':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'S':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'V':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'Y':
		st.accept();
		MYRETURN(OK);
	case 'Z':
		st.accept();
		if (aix_typeid(st) != OK)
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

int aix_label(StabTokenizer &st)
{
	MYENTRY("label");
	if (!st.isChar() || st.getChar() != 'L')
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int aix_constant(StabTokenizer &st)
{
	MYENTRY("constant");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'b':
		st.accept();
		if (aix_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'c':
		st.accept();
		if (aix_ordvalue(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'e':
		st.accept();
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_ordvalue(st) != OK)
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
		if (aix_typeid(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_numelements(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_numbits(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_bitpattern(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int aix_class(StabTokenizer &st)
{
	MYENTRY("class");
	int status;
	if (st.isChar() && st.getChar() == 'c')
	{
		st.accept();
		if (!st.isChar() || st.getChar() != '=')
			MYRETURN(NOTOK);
		st.accept();
		if (aix_constant(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	}
	status = aix_namedtype(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_parameter(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_procedure(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_variable(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	status = aix_label(st);
	if (status == OK)
		MYRETURN(OK);
	else if (status != NOMATCH)
		MYRETURN(NOTOK);
	MYRETURN(NOMATCH);
}

int aix_stabstring(StabTokenizer &st)
{
	MYENTRY("stabstring");
	int status;
	if (st.isName())
	{
		st.accept();
		if (st.isChar() && st.getChar() == ':')
		{
			st.accept();
			if (aix_class(st) != OK)
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
		if (aix_class(st) != OK)
			MYRETURN(NOTOK);
		else
			MYRETURN(OK);
	}
	else
	{
		MYRETURN(NOMATCH);
	}
}

// parse function for NON-AIX STABS parse functions
int sun_typeno(StabTokenizer &st)
{
	MYENTRY("typeno");
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
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_array(StabTokenizer &st)
{
	MYENTRY("array");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_volatile(StabTokenizer &st)
{
	MYENTRY("volatile");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_c99_variable_length_array(StabTokenizer &st)
{
	MYENTRY("c99_variable_length_array");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_basic_integer(StabTokenizer &st)
{
	MYENTRY("basic_integer");
	if (!st.isChar())
		MYRETURN(NOTOK);
	switch (st.getChar())
	{
	case 'u':
	case 's':
		st.accept();
		break;
	default:
		MYRETURN(NOTOK);
	}
	if (st.isChar())
	{
		switch (st.getChar())
		{
		case 'c':
		case 'b':
		case 'v':
			st.accept();
			break;
		}
	}
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_conformant_array_bounds(StabTokenizer &st)
{
	MYENTRY("conformant_array_bounds");
	MYRETURN(OK);
}

int sun_conformant_array(StabTokenizer &st)
{
	MYENTRY("conformant_array");
	MYRETURN(OK);
}

int sun_classname(StabTokenizer &st)
{
	MYENTRY("classname");
	if (st.isName())
		st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_virt_ppp_code(StabTokenizer &st)
{
	MYENTRY("virt_ppp_code");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'I':
	case 'J':
	case 'K':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int sun_non_virt_ppp_code(StabTokenizer &st)
{
	MYENTRY("non_virt_ppp_code");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'A':
	case 'B':
	case 'C':
	case 'E':
	case 'F':
	case 'G':
	case 'M':
	case 'N':
	case 'O':
		st.accept();
		MYRETURN(OK);
	case 'Q':
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int sun_ppp_code(StabTokenizer &st)
{
	MYENTRY("ppp_code");
	switch (sun_virt_ppp_code(st))
	{
	case OK:
		MYRETURN(OK);
	case NOMATCH:
		switch (sun_non_virt_ppp_code(st))
		{
		case OK:
			MYRETURN(OK);
		case NOMATCH:
			MYRETURN(NOMATCH);
		default:
			MYRETURN(NOTOK);
		}
	default:
		MYRETURN(NOTOK);
	}
}

int sun_bases(StabTokenizer &st, int first = 0)
{
	MYENTRY("bases");
	switch (sun_ppp_code(st))
	{
	case OK:
		break;
	case NOMATCH:
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	if (sun_typeno(st) != OK)
		MYRETURN(NOTOK);
	if (sun_bases(st) != OK)
		MYRETURN(NOTOK);
	if (!first)
		MYRETURN(OK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_datamembers(StabTokenizer &st, int first = 0)
{
	MYENTRY("datamembers");
	switch (sun_ppp_code(st))
	{
	case OK:
		break;
	case NOMATCH:
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
	if (!st.isName())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
#if 0
	if (sun_typeno(st) != OK)
		MYRETURN(NOTOK);
#else
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
#endif
	if (!st.isChar() || st.getChar() != ',')
		MYRETURN(NOTOK);
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
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (sun_datamembers(st) != OK)
		MYRETURN(NOTOK);
	if (!first)
		MYRETURN(OK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_memberfunctions(StabTokenizer &st, int first = 0)
{
	MYENTRY("memberfunctions");
	switch (sun_virt_ppp_code(st))
	{
	case OK:
		if (st.isChar() && st.getChar() == '-')
			st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isNoWsName())
			MYRETURN(NOTOK);
		st.accept();
		if (sun_memberfunctions(st) != OK)
			MYRETURN(NOTOK);
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case NOMATCH:
		break;
	default:
		MYRETURN(NOTOK);
	}
	switch (sun_non_virt_ppp_code(st))
	{
	case OK:
		if (!st.isNoWsName())
			MYRETURN(NOTOK);
		st.accept();
		if (sun_memberfunctions(st) != OK)
			MYRETURN(NOTOK);
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case NOMATCH:
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
}

int sun_staticdatamembers(StabTokenizer &st, int first = 0)
{
	MYENTRY("staticdatamembers");
	switch (sun_non_virt_ppp_code(st))
	{
	case OK:
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (sun_staticdatamembers(st) != OK)
			MYRETURN(NOTOK);
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case NOMATCH:
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
}

int sun_friends(StabTokenizer &st)
{
	MYENTRY("friends");
	while ( 1 )
	{
		if (st.isChar() && st.getChar() == '@')
			st.accept();
		if (st.isNoWsName())
			st.accept();
		else
			break;
	}
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_virtualfunctioninfo(StabTokenizer &st)
{
	MYENTRY("virtualfunctioninfo");
	while (st.isInteger())
	{
		st.accept();
	}
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_nestedclasslist(StabTokenizer &st, int first = 0)
{
	MYENTRY("nestedclasslist");
	switch (sun_ppp_code(st))
	{
	case OK:
		if (sun_typeno(st) != OK)
			MYRETURN(NOTOK);
		if (sun_nestedclasslist(st) != OK)
			MYRETURN(NOTOK);
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case NOMATCH:
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
}

int sun_accessadjustments(StabTokenizer &st, int first = 0)
{
	MYENTRY("accessadjustments");
	switch (sun_ppp_code(st))
	{
	case OK:
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (sun_accessadjustments(st) != OK)
			MYRETURN(NOTOK);
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case NOMATCH:
		if (!first)
			MYRETURN(OK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
}

int sun_virtualbaseclassoffsets(StabTokenizer &st, int first = 0)
{
	MYENTRY("virtualbaseclassoffsets");
	if (st.isInteger())
	{
		st.accept();
		if (sun_typeno(st) != OK)
			MYRETURN(NOTOK);
		if (sun_virtualbaseclassoffsets(st) != OK)
			MYRETURN(NOTOK);
	}
	if (!first)
		MYRETURN(OK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_templatemembers(StabTokenizer &st)
{
	MYENTRY("templatemembers");
	MYRETURN(OK);
}

int sun_passmethod(StabTokenizer &st, int first = 0)
{
	MYENTRY("passmethod");
	if (st.isInteger())
	{
		st.accept();
		if (sun_passmethod(st) != OK)
			MYRETURN(NOTOK);
	}
	if (!first)
		MYRETURN(OK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_cplusplus_specification(StabTokenizer &st)
{
	MYENTRY("cplusplus_specification");
	int status;
	if (!st.isChar())
		MYRETURN(NOTOK);
	switch (st.getChar())
	{
	case 'n':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (st.isChar() && st.getChar() == ';')
			st.accept();
		else if (st.isName())
			st.accept();
		MYRETURN(OK);
	case 'D':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'M':
		st.accept();
		if (st.isChar() && st.getChar() == 'K')
			st.accept();
		if (st.isChar() && st.getChar() == 'B')
			st.accept();
#if 0
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		status = sun_type(st);
		if (status != OK && status != NOMATCH)
			MYRETURN(NOTOK);
#else
		while ((status = sun_type(st)) == OK)
		{
			// TRACE();
		}
		if (status != NOMATCH)
			MYRETURN(NOTOK);
#endif
		if (!st.isChar() || st.getChar() != '#')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'c':
	case 's':
	case 'u':
	case 'a':
	case 'o':
	case 'C':
	case 'S':
	case 'U':
	case 'A':
	case 'O':
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (sun_classname(st) != OK)
			MYRETURN(NOTOK);
		if (sun_bases(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_datamembers(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_memberfunctions(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_staticdatamembers(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_friends(st) != OK)
			MYRETURN(NOTOK);
		if (sun_virtualfunctioninfo(st) != OK)
			MYRETURN(NOTOK);
		if (sun_nestedclasslist(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_accessadjustments(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_virtualbaseclassoffsets(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_passmethod(st, 1) != OK)
			MYRETURN(NOTOK);
		// asssume optional ';', but documentation says it is not.
		if (st.isChar() && st.getChar() == ';')
			st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
}

int sun_dope_vector_allocated(StabTokenizer &st)
{
	MYENTRY("dope_vector_allocated");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_dope_vector_shaped(StabTokenizer &st)
{
	MYENTRY("dope_vector_shaped");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_enumeration(StabTokenizer &st)
{
	MYENTRY("enumeration");
	int status = sun_typeno(st);
	if (status != OK && status != NOMATCH)
		MYRETURN(NOTOK);
	while (st.isName())
	{
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
		st.accept();
	}
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_function_parameter(StabTokenizer &st)
{
	MYENTRY("function_parameter");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_function(StabTokenizer &st)
{
	MYENTRY("function");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_function_with_prototype_info(StabTokenizer &st)
{
	MYENTRY("function_with_prototype_info");
	int status;
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	while ((status = sun_type(st)) == OK)
	{
		// do nothing
	}
	if (status != NOMATCH)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != '#')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_restricted(StabTokenizer &st)
{
	MYENTRY("restricted");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_const(StabTokenizer &st)
{
	MYENTRY("const");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_pascal_file(StabTokenizer &st)
{
	MYENTRY("pascal_file");
	MYRETURN(OK);
}

int sun_procedure_parameter(StabTokenizer &st)
{
	MYENTRY("procedure_parameter");
	MYRETURN(OK);
}

int sun_floating_point(StabTokenizer &st)
{
	MYENTRY("floating_point");
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_range(StabTokenizer &st)
{
	MYENTRY("range");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_set(StabTokenizer &st)
{
	MYENTRY("set");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_structure_or_record(StabTokenizer &st)
{
	MYENTRY("structure_or_record");
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	while (st.isName())
	{
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
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
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
	}
	MYRETURN(OK);
}

int sun_union(StabTokenizer &st)
{
	MYENTRY("union");
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	while (st.isName())
	{
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ',')
			MYRETURN(NOTOK);
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
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
	}
	MYRETURN(OK);
}

int sun_forward_reference(StabTokenizer &st)
{
	MYENTRY("forward_reference");
	int found = 0;
	if (st.isChar())
	{
		switch (st.getChar())
		{
		case 'e':
		case 's':
		case 'u':
			found++;
			st.accept();
			break;
		}
	}
	if (!found && sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_varying_string(StabTokenizer &st)
{
	MYENTRY("varying_string");
	MYRETURN(OK);
}

int sun_pointer(StabTokenizer &st)
{
	MYENTRY("pointer");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_reference(StabTokenizer &st)
{
	MYENTRY("reference");
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_typeid(StabTokenizer &st)
{
	MYENTRY("typeid");

	if (!st.isChar())
		MYRETURN(NOMATCH);
	char ctypeid = st.getChar();
	for (int i=0; typespecsparsers[i].pfunc != NULL; i++)
	{
		if (ctypeid == typespecsparsers[i].key)
		{
			st.accept();
			MYRETURN(typespecsparsers[i].pfunc(st));
		}
	}
	if (ctypeid == '#' || ctypeid == '@')
		MYRETURN(NOMATCH);
	else
		MYRETURN(sun_type(st));
}

int sun_type(StabTokenizer &st)
{
	MYENTRY("type");
	int status = sun_typeno(st);
	if (status == OK)
	{
		if (!st.isChar() || st.getChar() != '=')
			MYRETURN(OK);
		st.accept();
		if (sun_typeid(st) != OK)
			MYRETURN(NOTOK);
		else
			MYRETURN(OK);
	}
	else if (status == NOMATCH)
	{
		status = sun_typeid(st);
		if (status != OK && status != NOMATCH)
			MYRETURN(NOTOK);
		else
			MYRETURN(status);
	}
	else
		MYRETURN(NOTOK);
}

int sun_cplusplus_40(StabTokenizer &st)
{
	MYENTRY("sun_cplusplus_40");

	if (!st.isChar() || st.getChar() != 'T')
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar())
		MYRETURN(NOTOK);
	switch (st.getChar())
	{
	case 'c':
	case 'f':
	case 'm':
		st.accept();
		break;
	default:
		MYRETURN(NOTOK);
	}
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_template_parameter(StabTokenizer &st)
{
	MYENTRY("sun_template_parameter");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (st.isAlphabeta() && st.isEqual("tYC"))
		st.accept();
	if (st.isInteger())
		st.accept();
	else if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_templates(StabTokenizer &st)
{
	MYENTRY("sun_templates");
	if (!st.isChar())
		MYRETURN(NOTOK);
	switch (st.getChar())
	{
	case 'c':
	case 'C':
	case 's':
	case 'S':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		while (st.isChar() && st.getChar() != '@')
		{
			switch (sun_template_parameter(st))
			{
			case OK:
				break;
			case NOMATCH:
				MYRETURN(NOTOK);
			default:
				MYRETURN(NOTOK);
			}
		}
		if (!st.isChar() || st.getChar() != '@')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		while (st.isChar() && st.getChar() == ';')
		{
			st.accept();
		}
		MYRETURN(OK);
	case 'f':
		st.accept();
		while (st.isChar() && st.getChar() != '@')
		{
			switch (sun_template_parameter(st))
			{
			case OK:
				break;
			case NOMATCH:
				MYRETURN(NOTOK);
			default:
				MYRETURN(NOTOK);
			}
		}
		if (!st.isChar() || st.getChar() != '@')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != 'T')
			MYRETURN(NOTOK);
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		while (st.isChar() && st.getChar() != '@')
		{
			switch (sun_type(st))
			{
			case OK:
				break;
			case NOMATCH:
				MYRETURN(NOTOK);
			default:
				MYRETURN(NOTOK);
			}
			if (!st.isChar() || st.getChar() != ';')
				MYRETURN(NOTOK);
			st.accept();
		}
		if (!st.isChar() || st.getChar() != '@')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		while (st.isChar() && st.getChar() != '@')
		{
			if (!st.isName())
				MYRETURN(NOTOK);
			st.accept();
			if (!st.isChar() || st.getChar() != ':')
				MYRETURN(NOTOK);
			st.accept();
			if (!st.isChar() || st.getChar() != 'p')
				MYRETURN(NOTOK);
			st.accept();
			if (sun_type(st) != OK)
				MYRETURN(NOTOK);
			if (!st.isChar() || st.getChar() != ';')
				MYRETURN(NOTOK);
			st.accept();
		}
		if (!st.isChar() || st.getChar() != '@')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
}

int sun_instantiation(StabTokenizer &st)
{
	MYENTRY("sun_instantiation");
	if (!st.isChar())
		MYRETURN(NOTOK);
	switch (st.getChar())
	{
	case 'c':
	case 'C':
	case 's':
	case 'S':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		while (st.isChar() && st.getChar() != '@')
		{
			switch (sun_template_parameter(st))
			{
			case OK:
				break;
			case NOMATCH:
				MYRETURN(NOTOK);
			default:
				MYRETURN(NOTOK);
			}
		}
		if (!st.isChar() || st.getChar() != '@')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		while (st.isChar() && st.getChar() != '@')
		{
			if (!st.isName())
				MYRETURN(NOTOK);
			st.accept();
			if (!st.isChar() || st.getChar() != ':')
				MYRETURN(NOTOK);
			st.accept();
			if (st.isInteger())
				st.accept();
			else if (sun_type(st) != OK)
				MYRETURN(NOTOK);
			if (!st.isChar() || st.getChar() != ';')
				MYRETURN(NOTOK);
			st.accept();
		}
		if (!st.isChar() || st.getChar() != '@')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != 'g')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (sun_bases(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_datamembers(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_memberfunctions(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_staticdatamembers(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_friends(st) != OK)
			MYRETURN(NOTOK);
		if (sun_virtualfunctioninfo(st) != OK)
			MYRETURN(NOTOK);
		if (sun_nestedclasslist(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_accessadjustments(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_virtualbaseclassoffsets(st, 1) != OK)
			MYRETURN(NOTOK);
		if (sun_passmethod(st, 1) != OK)
			MYRETURN(NOTOK);
		while (st.isChar() && st.getChar() == ';')
		{
			st.accept();
		}
		MYRETURN(OK);
	case 'f':
		st.accept();
		while (st.isChar() && st.getChar() != '@')
		{
			switch (sun_template_parameter(st))
			{
			case OK:
				break;
			case NOMATCH:
				MYRETURN(NOTOK);
			default:
				MYRETURN(NOTOK);
			}
		}
		if (!st.isChar() || st.getChar() != '@')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != 'g')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ';')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != 'F')
			MYRETURN(NOTOK);
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		while (st.isChar() && st.getChar() != ';')
		{
			switch (sun_type(st))
			{
			case OK:
				break;
			case NOMATCH:
				MYRETURN(NOTOK);
			default:
				MYRETURN(NOTOK);
			}
		}
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
	MYRETURN(OK);
}

int sun_cplusplus_50(StabTokenizer &st)
{
	MYENTRY("sun_cplusplus_50");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'A':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'a':
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (sun_typeno(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'C':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'c':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'D':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'I':
		st.accept();
		if (sun_instantiation(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'M':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'R':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'S':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 's':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'T':
		st.accept();
		if (sun_templates(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'U':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'u':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOMATCH);
	}
}

int sun_symdesc_type(StabTokenizer &st)
{
	MYENTRY("symdesc_type");
	int status;

	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'A':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'b':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'c':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'E':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'F':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'f':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'G':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'I':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'J':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'L':
		st.accept();
		if (sun_cplusplus_40(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'l':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'M':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'P':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'p':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'r':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'S':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'T':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 't':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'U':
		st.accept();
		status = sun_type(st);
		if (status == OK || status == NOMATCH)
			MYRETURN(OK);
		else 
			MYRETURN(NOTOK);
	case 'V':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'v':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'X':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'Y':
		st.accept();
		if (sun_cplusplus_50(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		status = sun_type(st);
		if (status != OK && status != NOMATCH)
			MYRETURN(NOTOK);
		MYRETURN(status);
	}
}

int sun_abs(StabTokenizer &st)
{
	MYENTRY("abs");
	MYRETURN(OK);
}

int sun_bss(StabTokenizer &st)
{
	MYENTRY("bss");
	MYRETURN(OK);
}

int sun_comm(StabTokenizer &st)
{
	MYENTRY("comm");
	MYRETURN(OK);
}

int sun_data(StabTokenizer &st)
{
	MYENTRY("data");
	MYRETURN(OK);
}

int sun_ecoml(StabTokenizer &st)
{
	MYENTRY("ecoml");
	MYRETURN(OK);
}

int sun_ecomm(StabTokenizer &st)
{
	MYENTRY("ecomm");
	MYRETURN(OK);
}

int sun_endm(StabTokenizer &st)
{
	MYENTRY("endm");
	MYRETURN(OK);
}

int sun_esym(StabTokenizer &st)
{
	MYENTRY("esym");
	// strange case here, documentation says there should
	// be a name, but the actual stabs does not have a name.
	// make it optional.
	if (st.isName())
		st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(OK);
	st.accept();
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_excl(StabTokenizer &st)
{
	MYENTRY("excl");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_fn(StabTokenizer &st)
{
	MYENTRY("fn");
	MYRETURN(OK);
}

int sun_fname(StabTokenizer &st)
{
	MYENTRY("fname");
	MYRETURN(OK);
}

int sun_fun(StabTokenizer &st)
{
	MYENTRY("fun");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	// stabs documentation says that if the following check
	// fails, then a syntax error was detected. but the results
	// from the compiler say otherwise. so change NOTOK to OK.
	// and leave it at that.
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(OK);
	st.accept();
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	while (st.isChar() && st.getChar() == ';')
	{
		st.accept();
		if (st.isInteger())
		{
			if (st.getInteger() != 0)
				MYRETURN(NOTOK);
			st.accept();
		}
		else if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		// check for default arguments functions
		if (st.isName()) st.accept();
	}
	MYRETURN(OK);
}

int sun_gsym(StabTokenizer &st)
{
	MYENTRY("gsym");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(OK);
	st.accept();
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_ildpad(StabTokenizer &st)
{
	MYENTRY("ildpad");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_isym(StabTokenizer &st)
{
	MYENTRY("isym");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_lbrac(StabTokenizer &st)
{
	MYENTRY("lbrac");
	MYRETURN(OK);
}

int sun_lcsym(StabTokenizer &st)
{
	MYENTRY("lcsym");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_leng(StabTokenizer &st)
{
	MYENTRY("leng");
	MYRETURN(OK);
}

int sun_lsym(StabTokenizer &st)
{
	MYENTRY("lsym");
	if (st.isName())
	{
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
	}
	else
	{
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOMATCH);
		st.accept();
	}
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_main(StabTokenizer &st)
{
	MYENTRY("main");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_obj(StabTokenizer &st)
{
	MYENTRY("obj");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_opt(StabTokenizer &st)
{
	MYENTRY("opt");
	int found = 0;
	while (st.isName())
	{
		found++;
		st.accept();
		if (st.isChar() && st.getChar() == ';')
			st.accept();
		else
			break;
	}
	if (!found)
		MYRETURN(NOMATCH);
	else
		MYRETURN(OK);
}

int sun_patch(StabTokenizer &st)
{
	MYENTRY("patch");
	MYRETURN(OK);
}

int sun_pc(StabTokenizer &st)
{
	MYENTRY("pc");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_psym(StabTokenizer &st)
{
	MYENTRY("psym");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(OK);
	st.accept();
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	if (st.isChar() && st.getChar() == ';')
	{
		st.accept();
		if (!st.isChar() || st.getChar() != 's')
			MYRETURN(OK);
		st.accept();
		if (!st.isInteger())
			MYRETURN(NOTOK);
		st.accept();
	}
	MYRETURN(OK);
}

int sun_rbrac(StabTokenizer &st)
{
	MYENTRY("rbrac");
	MYRETURN(OK);
}

int sun_rosym(StabTokenizer &st)
{
	MYENTRY("rosym");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(OK);
	st.accept();
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_rsym(StabTokenizer &st)
{
	MYENTRY("rsym");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(OK);
	st.accept();
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_sincl(StabTokenizer &st)
{
	MYENTRY("sincl");
	MYRETURN(OK);
}

int sun_sline(StabTokenizer &st)
{
	MYENTRY("sline");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_so(StabTokenizer &st)
{
	MYENTRY("so");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_sol(StabTokenizer &st)
{
	MYENTRY("sol");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_ssym(StabTokenizer &st)
{
	MYENTRY("ssym");
	MYRETURN(OK);
}

int sun_stsym(StabTokenizer &st)
{
	MYENTRY("stsym");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(OK);
	st.accept();
	if (sun_symdesc_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_tcomm(StabTokenizer &st)
{
	MYENTRY("tcomm");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_text(StabTokenizer &st)
{
	MYENTRY("text");
	MYRETURN(OK);
}

int sun_undf(StabTokenizer &st)
{
	MYENTRY("undf");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_using(StabTokenizer &st)
{
	MYENTRY("using");
	if (!st.isChar())
		MYRETURN(NOMATCH);
	switch (st.getChar())
	{
	case 'P':
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'N':
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		switch (sun_type(st))
		{
		case OK:
			MYRETURN(OK);
		case NOMATCH:
			MYRETURN(OK);
		default:
			MYRETURN(NOTOK);
		}
	case 'Q':
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	case 'O':
		st.accept();
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(NOTOK);
		st.accept();
		switch (sun_type(st))
		{
		case OK:
		case NOMATCH:
			MYRETURN(OK);
		default:
			MYRETURN(NOTOK);
		}
	default:
		MYRETURN(NOMATCH);
	}
}

int sun_with(StabTokenizer &st)
{
	MYENTRY("with");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	MYRETURN(OK);
}

int sun_xcomm(StabTokenizer &st)
{
	MYENTRY("xcomm");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_xline(StabTokenizer &st)
{
	MYENTRY("xline");
	MYRETURN(OK);
}

int sun_entry(StabTokenizer &st)
{
	MYENTRY("entry");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != 'e')
		MYRETURN(NOTOK);
	st.accept();
	if (sun_type(st) != OK)
		MYRETURN(NOTOK);
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isName())
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(OK);
	st.accept();
	MYRETURN(OK);
}

int sun_destruct(StabTokenizer &st)
{
	MYENTRY("destruct");
	if (!st.isInteger())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_construct(StabTokenizer &st)
{
	MYENTRY("construct");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isInteger())
		MYRETURN(NOTOK);
	st.accept();
	MYRETURN(OK);
}

int sun_cmdline(StabTokenizer &st)
{
	MYENTRY("cmdline");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ';')
		MYRETURN(NOTOK);
	st.accept();
	int namecnt = 0;
	for ( ; st.isName(); namecnt++)
	{
		st.accept();
	}
	if (namecnt > 0)
		MYRETURN(OK);
	else
		MYRETURN(NOTOK);
}

int sun_brows(StabTokenizer &st)
{
	MYENTRY("brows");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_bincl(StabTokenizer &st)
{
	MYENTRY("bincl");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_eincl(StabTokenizer &st)
{
	MYENTRY("eincl");
	MYRETURN(OK);
}

int sun_bcomm(StabTokenizer &st)
{
	MYENTRY("bcomm");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	MYRETURN(OK);
}

int sun_alias(StabTokenizer &st)
{
	MYENTRY("alias");
	if (!st.isName())
		MYRETURN(NOMATCH);
	st.accept();
	if (!st.isChar() || st.getChar() != ':')
		MYRETURN(NOTOK);
	st.accept();
	if (!st.isChar())
		MYRETURN(NOTOK);
	switch (st.getChar())
	{
	case 'F':
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'V':
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'N':
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'R':
		st.accept();
		if (!st.isName())
			MYRETURN(NOTOK);
		st.accept();
		MYRETURN(OK);
	case 'S':
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != '=')
			MYRETURN(NOTOK);
		st.accept();
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		if (!st.isChar() || st.getChar() != ':')
			MYRETURN(OK);
		if (sun_type(st) != OK)
			MYRETURN(NOTOK);
		MYRETURN(OK);
	default:
		MYRETURN(NOTOK);
	}
	MYRETURN(OK);
}

static void
parseString(const stab *pd, const char *ps, char &flag)
{
	int (*parsefunc)(StabTokenizer &) = NULL;
	const char *prefix = NULL;

	// check if a string was given
	flag = '\0';
	if (!doparse || ps == NULL || *ps == '\0')
		return;

	// start parsing depending on the type of stab. it is 
	// possible that certain stab types may have one type 
	// of parser, but not the other types. therefore, the
	// for-loop check is for any function pointer NOT equal
	// to NULL, not all pointers NOT equal to NULL. the check 
	// for a NULL pointer after the for-loop checks if a function
	// was actually found.
	//
	for (int i=0; stabparsers[i].aixpfunc != NULL ||
		stabparsers[i].sunpfunc != NULL; i++)
	{
		if (pd->n_type == stabparsers[i].key)
		{
			if (useaixsyntax)
			{
				prefix = "AIX";
				parsefunc = stabparsers[i].aixpfunc;
			}
			else
			{
				prefix = "SUN";
				parsefunc = stabparsers[i].sunpfunc;
			}
			break;
		}
	}
	if (parsefunc == NULL)
	{
		// not supported
		printf("parseString: NOT SUPPORTED - <%s>\n", ps);
		return;
	}

	// parse the stab string.
	if (doparse == 1)
	{
		StabTokenizer st(ps);
		if (parsefunc(st) == OK)
		{
			totalparsesuccess += 1;
			printf("%s parseString: SUCCESS ...\n", prefix);
		}
		else
		{
			totalparsefail += 1;
			printf("%s parseString: FAILED ...\n", prefix);
		}
		printf("\ntotal parse success = %ld\n", 
			totalparsesuccess);
		printf("total parse fail    = %ld\n", 
			totalparsefail);
	}
	else if (doparse == 2)
	{
		char s[BUFSIZ];
		printf("Run %s-syntax parseString() ? [n/y/q[uit]/cr=y] ", prefix);
		rmvnlgets(s);
		flag = *s;
		if (*s == 'y' || *s == '\0')
		{
			StabTokenizer st(ps);
			if (parsefunc(st) == OK)
			{
				totalparsesuccess += 1;
				printf("%s parseString: SUCCESS ...\n", prefix);
			}
			else
			{
				totalparsefail += 1;
				printf("%s parseString: FAILED ...\n", prefix);
			}
			printf("\ntotal parse success = %ld\n", 
				totalparsesuccess);
			printf("total parse fail    = %ld\n", 
				totalparsefail);
		}
	}
	else
	{
		doparse = 0;
		printf("out-of-range value for doparse flag (%d).\n", doparse);
	}
	return;
}

static void
showLineNumbers(ElfObject &eo, const stab *pd, const char *ps, int cstabno, int csec, 
		char *p0, char *pe, char &buf)
{
	Spaces spaces(0);
	for (cstabno++; (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe; cstabno++)
	{
		stab *pstab = (stab *)(p0+cstabno*eo.pshdr(csec).sh_entsize);
		if (pstab->n_type == N_FUN)
			break;
		switch (pstab->n_type)
		{
		case N_SLINE:
			break;
		case N_LBRAC:
			spaces+=3;
			continue;
		case N_RBRAC:
			spaces-=3;
			continue;
		default:
			continue;
		}
		printf("%d: (%s) %s lnno=%ld offset=0x%lx\n", 
			cstabno, i2s(n_type, pstab->n_type),
			spaces(), (unsigned long)pstab->n_desc, 
			(unsigned long)pstab->n_value);
	}
	return;
}

static void
showstab(ElfObject &eo, int &cstabno, int &csec, char *&p0, char *&pe, char &buf, int showlnno, int &multilined)
{
	// init variables
	multilined = 0;
	int length = 0;

	// calculate the offset into the string table. this is necessary
	// when more than one file contents are in one object or executable
	// file.
	int showoffset = eo.calcoffset(csec, cstabno);

	// current stab entry
	int cstabno2 = cstabno;
	stab *pstab = (stab *)(p0+cstabno*eo.pshdr(csec).sh_entsize);
	stab *pstab2 = NULL;
	char *pstabstr = eo.pshdrdata(eo.pshdr(csec).sh_link)+pstab->n_strx+showoffset;
	char *pstabstr2 = NULL;

	// determine if stab string is multilined, and how many stabs
	// are included.
	if (isMultilined(pstabstr))
	{
		// determine how many stabs
		length = strlen(pstabstr);
		do {
			multilined++;
			cstabno2 = cstabno+multilined;
			pstab2 = (stab *)(p0+cstabno2*eo.pshdr(csec).sh_entsize);
			pstabstr2 = eo.pshdrdata(eo.pshdr(csec).sh_link)+
				pstab2->n_strx+eo.calcoffset(csec, cstabno2);
			length += strlen(pstabstr2);
		} while (isMultilined(pstabstr2));

		// allocate one buffer containing all the stab strings.
		char *newbuffer = ::new char [length+1];
		*newbuffer = '\0';
		MustBeTrue(newbuffer != NULL);
		for (cstabno2=cstabno; cstabno2<=(cstabno+multilined); cstabno2++)
		{
			pstab2 = (stab *)(p0+cstabno2*eo.pshdr(csec).sh_entsize);
			pstabstr2 = eo.pshdrdata(eo.pshdr(csec).sh_link)+
				pstab2->n_strx+eo.calcoffset(csec, cstabno2);
			strcat(newbuffer, pstabstr2);
			length = strlen(newbuffer);
			if (newbuffer[length-1] == '\\')
			{
				newbuffer[length-1] = '\0';
			}
		}
		pstabstr = newbuffer;
	}

	// print data for current stab entry
	printf("%d: n_strx : 0x%lx (%s)\n", 
		cstabno, (long)pstab->n_strx, pstabstr);
	printf("%d: n_type : 0x%lx (%s)\n", 
		cstabno, (unsigned long)pstab->n_type, 
		i2s(n_type, pstab->n_type));
	printf("%d: n_other: 0x%lx\n", 
		cstabno, (unsigned long)pstab->n_other);
	printf("%d: n_desc : 0x%lx\n", 
		cstabno, (unsigned long)pstab->n_desc);
	printf("%d: n_value: 0x%lx\n", 
		cstabno, (unsigned long)pstab->n_value);

	// parse or show line numbers?
	if (showlnno)
	{
		showLineNumbers(eo, pstab, pstabstr, cstabno, csec, p0, pe, buf);
	}
	else
	{
		// parse the stab string.
		parseString(pstab, pstabstr, buf);
	}

	// release any resources
	if (multilined)
	{
		delete [] pstabstr;
	}

	// all done
	return;
}

static void
line_numbers(ElfObject &eo, int &cstabno, int &csec, char *&p0, char *&pe)
{
	int multilined;
	char buf;
	char save_pt3[BUFSIZ];
	*save_pt3 = '\0';

	// get tokens from user stream
	char *pt1 = gettoken(1);
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL || *pt2 == '\0' || 
	    pt3 == NULL || *pt3 == '\0')
	{
		printf("unknown 'l' cmd.\n");
		return;
	}
	else if (*pt2 == '*')
	{
		// show a specific stab
		for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
		{
			// skip sections that are not stab tables
			if (!isStabSection(eo, csec))
				continue;

			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);

			// print stabs in section
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (cstabno=0; 
			    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe;
			     pt3=NULL, cstabno++)
			{
				stab *pstab = (stab *)(p0 + 
					cstabno*eo.pshdr(csec).sh_entsize);
				if (pt3 == NULL && pstab->n_type != N_FUN)
					continue;
				char *pstring = pstr + pstab->n_strx +
					eo.calcoffset(csec, cstabno);
				if (!REequal(pt3, pstring))
					continue;
				showstab(eo, cstabno, csec, p0, pe, buf, 1, multilined);
				if (buf == 'q') 
					goto all_done;
				char s[BUFSIZ];
				printf("next stab? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					goto all_done;
				cstabno += multilined;
			}
		}
		all_done: ;
	}
	else if (strlen(pt2) == strspn(pt2, "0123456789"))
	{
		// a section number was given
		int sec = MYatoi(pt2);
		if (sec < 0 || sec >= eo.pehdr()->e_shnum)
		{
			printf("out-of-range section number.\n");
			printf("minimum section number = 0\n");
			printf("maximum section number = %d\n",
				eo.pehdr()->e_shnum);
			return;
		}
		if (!isStabSection(eo, sec))
		{
			printf("section is not a stab table.\n");
			return;
		}
		csec = sec;
		cstabno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// print section name
		printf("section %d: %s (%d)\n", csec,
			eo.shdrnm(csec), eo.pshdr(csec).sh_name);

		// print stabs in section
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
		char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
		for (cstabno=0; 
		    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe;
		     pt3=NULL, cstabno++)
		{
			stab *pstab = (stab *)(p0 + 
				cstabno*eo.pshdr(csec).sh_entsize);
			if (pt3 == NULL && pstab->n_type != N_FUN)
				continue;
			char *pstring = pstr + pstab->n_strx +
				eo.calcoffset(csec, cstabno);
			if (!REequal(pt3, pstring))
				continue;
			showstab(eo, cstabno, csec, p0, pe, buf, 1, multilined);
			if (buf == 'q') 
				goto all_done2;
			char s[BUFSIZ];
			printf("next stab? [n/y/cr=y] ");
			rmvnlgets(s);
			if (*s != 'y' && *s != '\0')
				goto all_done;
			cstabno += multilined;
		}
		all_done2: ;
	}
	else
	{
		// a section name was given
		int sec = nametosec(eo, pt2);
		if (sec == NOTOK)
		{
			printf("section not found.\n");
			return;
		}
		if (!isStabSection(eo, sec))
		{
			printf("section is not a stab table.\n");
			return;
		}
		csec = sec;
		cstabno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// print section name
		printf("section %d: %s (%d)\n", csec,
			eo.shdrnm(csec), eo.pshdr(csec).sh_name);

		// print stabs in section
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
		char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
		for (cstabno=0; 
		    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe;
		     pt3=NULL, cstabno++)
		{
			stab *pstab = (stab *)(p0 + 
				cstabno*eo.pshdr(csec).sh_entsize);
			if (pt3 == NULL && pstab->n_type != N_FUN)
				continue;
			char *pstring = pstr + pstab->n_strx +
				eo.calcoffset(csec, cstabno);
			if (!REequal(pt3, pstring))
				continue;
			showstab(eo, cstabno, csec, p0, pe, buf, 1, multilined);
			if (buf == 'q') 
				goto all_done3;
			char s[BUFSIZ];
			printf("next stab? [n/y/cr=y] ");
			rmvnlgets(s);
			if (*s != 'y' && *s != '\0')
				goto all_done;
			cstabno += multilined;
		}
		all_done3: ;
	}
	return;
}

static void
review_type(ElfObject &eo, int &cstabno, int &csec, char *&p0, char *&pe)
{
	int multilined;
	char buf;
	char save_pt3[BUFSIZ];
	*save_pt3 = '\0';

	// get tokens from user stream
	char *pt1 = gettoken(1);
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL)
	{
		printf("unknown 'rt' cmd.\n");
		return;
	}
	else if (*pt2 == '*')
	{
		// check if a string was given
		if (pt3 == NULL || *pt3 == '\0')
		{
			printf("unknown 'rt' cmd.\n");
			return;
		}
		else
		{
			long search_type = -1;

			// get type, either a number or a mnemonic.
			if ((strncmp(pt3, "0x", 2) == 0) ||
			    (strncmp(pt3, "0X", 2) == 0))
			{
				search_type = strtol(pt3, NULL, 16);
			}
			else if (strlen(pt3) == strspn(pt3, "0123456789"))
			{
				search_type = strtol(pt3, NULL, 10);
			}
			else
			{
				strcpy(save_pt3, pt3);
				search_type = s2i(n_type, pt3);
				if (search_type == NOTOK)
				{
					printf("unknown type: %s.\n", save_pt3);
					return;
				}
			}

			// show a specific stab, search by type
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not stab tables
				if (!isStabSection(eo, csec))
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
				// print stabs in section
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				for (cstabno=0; 
				    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe;
				     cstabno++)
				{
					stab *pstab = (stab *)(p0 + cstabno*
						eo.pshdr(csec).sh_entsize);
					if (pstab->n_type != search_type)
						continue;
					showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
					if (buf == 'q') 
						goto all_done2;
					char s[BUFSIZ];
					printf("next stab? [n/y/cr=y] ");
					rmvnlgets(s);
					if (*s != 'y' && *s != '\0')
						goto all_done2;
					cstabno += multilined;
				}
			}
			all_done2: ;
		}
	}
	else if (strlen(pt2) == strspn(pt2, "0123456789"))
	{
		// a section number was given
		int sec = MYatoi(pt2);
		if (sec < 0 || sec >= eo.pehdr()->e_shnum)
		{
			printf("out-of-range section number.\n");
			printf("minimum section number = 0\n");
			printf("maximum section number = %d\n",
				eo.pehdr()->e_shnum);
			return;
		}
		if (!isStabSection(eo, sec))
		{
			printf("section is not a stab table.\n");
			return;
		}
		csec = sec;
		cstabno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			printf("unknown 'rt' cmd.\n");
			return;
		}
		else 
		{
			long search_type = -1;

			// get type, either a number or a mnemonic.
			if ((strncmp(pt3, "0x", 2) == 0) ||
			    (strncmp(pt3, "0X", 2) == 0))
			{
				search_type = strtol(pt3, NULL, 16);
			}
			else if (strlen(pt3) == strspn(pt3, "0123456789"))
			{
				search_type = strtol(pt3, NULL, 10);
			}
			else
			{
				strcpy(save_pt3, pt3);
				search_type = s2i(n_type, pt3);
				if (search_type == NOTOK)
				{
					printf("unknown type: %s.\n", save_pt3);
					return;
				}
			}
			// search for stab within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print stab in section
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (cstabno=0; 
			    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe; cstabno++)
			{
				stab *pstab = (stab *)(p0 + 
					cstabno*eo.pshdr(csec).sh_entsize);
				if (pstab->n_type != search_type)
					continue;
				showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
				if (buf == 'q') break;
				char s[BUFSIZ];
				printf("next string? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					break;
				cstabno += multilined;
			}
		}
	}
	else
	{
		// a section name was given
		int sec = nametosec(eo, pt2);
		if (sec == NOTOK)
		{
			printf("section not found.\n");
			return;
		}
		if (!isStabSection(eo, sec))
		{
			printf("section is not a stab table.\n");
			return;
		}
		csec = sec;
		cstabno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL || *pt3 == '\0')
		{
			printf("unknown 'rt' cmd.\n");
			return;
		}
		else 
		{
			long search_type = -1;

			// get type, either a number or a mnemonic.
			if ((strncmp(pt3, "0x", 2) == 0) ||
			    (strncmp(pt3, "0X", 2) == 0))
			{
				search_type = strtol(pt3, NULL, 16);
			}
			else if (strlen(pt3) == strspn(pt3, "0123456789"))
			{
				search_type = strtol(pt3, NULL, 10);
			}
			else
			{
				strcpy(save_pt3, pt3);
				search_type = s2i(n_type, pt3);
				if (search_type == NOTOK)
				{
					printf("unknown type: %s.\n", save_pt3);
					return;
				}
			}
			// search for stab within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print stab in section
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (cstabno=0; 
			    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe; cstabno++)
			{
				stab *pstab = (stab *)(p0 + 
					cstabno*eo.pshdr(csec).sh_entsize);
				if (pstab->n_type != search_type)
					continue;
				showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
				if (buf == 'q') break;
				char s[BUFSIZ];
				printf("next string? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					break;
				cstabno += multilined;
			}
		}
	}
	return;
}

static void
review_index(ElfObject &eo, int &cstabno, int &csec, char *&p0, char *&pe)
{
	int multilined;
	char buf;
	char save_pt3[BUFSIZ];
	*save_pt3 = '\0';

	// get tokens from user stream
	char *pt1 = gettoken(1);
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL || *pt2 == '\0')
	{
		printf("unknown 'ri' cmd.\n");
		return;
	}
	else if (strlen(pt2) == strspn(pt2, "0123456789"))
	{
		// a section number was given
		int sec = MYatoi(pt2);
		if (sec < 0 || sec >= eo.pehdr()->e_shnum)
		{
			printf("out-of-range section number.\n");
			printf("minimum section number = 0\n");
			printf("maximum section number = %d\n",
				eo.pehdr()->e_shnum);
			return;
		}
		if (!isStabSection(eo, sec))
		{
			printf("section is not a stab table.\n");
			return;
		}
		csec = sec;
		cstabno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL || *pt3 == '\0')
		{
			printf("unknown 'ri' cmd.\n");
			return;
		}
		else 
		{
			long indexno = -1;

			// get type, either a number or a mnemonic.
			if ((strncmp(pt3, "0x", 2) == 0) ||
			    (strncmp(pt3, "0X", 2) == 0))
			{
				indexno = strtol(pt3, NULL, 16);
			}
			else if (strlen(pt3) == strspn(pt3, "0123456789"))
			{
				indexno = strtol(pt3, NULL, 10);
			}
			else
			{
				printf("unknown ri cmd option.\n");
				return;
			}
			// search for stab within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print stab in section
			if ((indexno<0) || 
			   ((p0+indexno*eo.pshdr(csec).sh_entsize)>=pe))
			{
				printf("out-of-range index number.\n");
			}
			else
			{
				cstabno = indexno;
				showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
			}
		}
	}
	else
	{
		// a section name was given
		int sec = nametosec(eo, pt2);
		if (sec == NOTOK)
		{
			printf("section not found.\n");
			return;
		}
		if (!isStabSection(eo, sec))
		{
			printf("section is not a stab table.\n");
			return;
		}
		csec = sec;
		cstabno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL || *pt3 == '\0')
		{
			// show first stab in table
			showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
		}
		else 
		{
			long indexno = -1;

			// get type, either a number or a mnemonic.
			if ((strncmp(pt3, "0x", 2) == 0) ||
			    (strncmp(pt3, "0X", 2) == 0))
			{
				indexno = strtol(pt3, NULL, 16);
			}
			else if (strlen(pt3) == strspn(pt3, "0123456789"))
			{
				indexno = strtol(pt3, NULL, 10);
			}
			else
			{
				printf("unknown ri cmd option.\n");
				return;
			}

			// search for stab within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print stab in section
			if ((indexno<0) || 
			   ((p0+indexno*eo.pshdr(csec).sh_entsize)>=pe))
			{
				printf("out-of-range index number.\n");
			}
			else
			{
				cstabno = indexno;
				showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
			}
		}
	}
	return;
}

static void
review_other(ElfObject &eo, int &cstabno, int &csec, char *&p0, char *&pe)
{
	int multilined;
	char buf;
	char save_pt3[BUFSIZ];
	*save_pt3 = '\0';

	// get tokens from user stream
	char *pt1 = gettoken(1);
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL)
	{
		// check current section and location
		if (csec < 0 || csec >= eo.pehdr()->e_shnum)
		{
			printf("invalid section number.\n");
			return;
		}
		if (!isStabSection(eo, csec))
		{
			printf("current section is not a stab table.\n");
			return;
		}
		if (cstabno < 0 || 
		    cstabno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
		{
			printf("stab number is out of range.\n");
			return;
		}

		// show current stab
		showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
	}
	else if (*pt2 == '*')
	{
		// check if a string was given
		if (pt3 == NULL)
		{
			// show all stabs
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not symbol tables
				if (!isStabSection(eo, csec))
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec), 
					eo.pshdr(csec).sh_name);
	
				// print stabs in section
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
				for (cstabno=0; 
				    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe;
				     cstabno++)
				{
					showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
					if (buf == 'q') goto all_done0;
					cstabno += multilined;
				}
			}
			all_done0: ;
		}
		else 
		{
			// show a specific stab
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not stab tables
				if (!isStabSection(eo, csec))
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec), 
					eo.pshdr(csec).sh_name);
	
				// print stabs in section
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
				for (cstabno=0; 
				    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe;
				     pt3=NULL, cstabno++)
				{
					stab *pstab = (stab *)(p0 + 
						cstabno*eo.pshdr(csec).sh_entsize);
					char *pstring = pstr + pstab->n_strx +
						eo.calcoffset(csec, cstabno);
					if (!REequal(pt3, pstring))
						continue;
					showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
					if (buf == 'q') 
						goto all_done;
					char s[BUFSIZ];
					printf("next stab? [n/y/cr=y] ");
					rmvnlgets(s);
					if (*s != 'y' && *s != '\0')
						goto all_done;
					cstabno += multilined;
				}
			}
			all_done: ;
		}
	}
	else if (strlen(pt2) == strspn(pt2, "0123456789"))
	{
		// a section number was given
		int sec = MYatoi(pt2);
		if (sec < 0 || sec >= eo.pehdr()->e_shnum)
		{
			printf("out-of-range section number.\n");
			printf("minimum section number = 0\n");
			printf("maximum section number = %d\n",
				eo.pehdr()->e_shnum);
			return;
		}
		if (!isStabSection(eo, sec))
		{
			printf("section is not a stab table.\n");
			return;
		}
		csec = sec;
		cstabno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			// show first stab in table
			showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
		}
		else if (*pt3 == '*')
		{
			// show all stabs in section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print stabs in section
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (cstabno=0; 
			    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe; cstabno++)
			{
				showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
				if (buf == 'q') break;
				cstabno += multilined;
			}
		}
		else
		{
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);

			// print stabs in section
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (cstabno=0; 
			    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe;
			     pt3=NULL, cstabno++)
			{
				stab *pstab = (stab *)(p0 + 
					cstabno*eo.pshdr(csec).sh_entsize);
				char *pstring = pstr + pstab->n_strx +
					eo.calcoffset(csec, cstabno);
				if (!REequal(pt3, pstring))
					continue;
				showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
				if (buf == 'q') 
					goto all_done2;
				char s[BUFSIZ];
				printf("next stab? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					goto all_done;
				cstabno += multilined;
			}
			all_done2: ;
		}
	}
	else
	{
		// a section name was given
		int sec = nametosec(eo, pt2);
		if (sec == NOTOK)
		{
			printf("section not found.\n");
			return;
		}
		if (sec < 0 || sec >= eo.pehdr()->e_shnum)
		{
			printf("invalid section number.\n");
			return;
		}
		if (!isStabSection(eo, sec))
		{
			printf("section is not a stab table.\n");
			return;
		}
		csec = sec;
		cstabno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			// show first stab in table
			showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
		}
		else if (*pt3 == '*')
		{
			// show all stabs in section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print stabs in section
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (cstabno=0; 
			    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe; cstabno++)
			{
				showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
				if (buf == 'q') break;
				cstabno += multilined;
			}
		}
		else
		{
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);

			// print stabs in section
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (cstabno=0; 
			    (p0+cstabno*eo.pshdr(csec).sh_entsize)<pe;
			     pt3=NULL, cstabno++)
			{
				stab *pstab = (stab *)(p0 + 
					cstabno*eo.pshdr(csec).sh_entsize);
				char *pstring = pstr + pstab->n_strx +
					eo.calcoffset(csec, cstabno);
				if (!REequal(pt3, pstring))
					continue;
				showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
				if (buf == 'q') 
					goto all_done3;
				char s[BUFSIZ];
				printf("next stab? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					goto all_done;
				cstabno += multilined;
			}
			all_done3: ;
		}
	}
	return;
}

static void
review(ElfObject &eo, int &cstabno, int &csec, char *&p0, char *&pe)
{
	int multilined;
	// get tokens from user stream
	char *pt1 = gettoken(1);

	// check the type of review
	if (*(pt1+1) == 't')
	{
		review_type(eo, cstabno, csec, p0, pe);
	}
	else if (*(pt1+1) == 'i')
	{
		review_index(eo, cstabno, csec, p0, pe);
	}
	else if (*(pt1+1) == '\0')
	{
		review_other(eo, cstabno, csec, p0, pe);
	}
	else
	{
		printf("unknown review type.\n");
	}
	return;
}

static void
update(ElfObject &eo, int &cstabno, int &csec, char *&p0, char *&pe)
{
	int multilined;

	// check section and stab
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (!isStabSection(eo, csec))
	{
		printf("section is not a stab table.\n");
		return;
	}
	if (cstabno < 0 || cstabno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current stab number is out-of-range.\n");
		return;
	}
	MustBeTrue(eo.pshdrdata(csec) == p0);

	// update stab
	int upd = 0;
	char s[BUFSIZ];
	printf("section %d: %s (%d)\n", csec,
		eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	stab *pstab = (stab *)(p0 + cstabno*eo.pshdr(csec).sh_entsize);

	printf("%d: n_strx [cr=0x%lx]: ", cstabno, (long)pstab->n_strx);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pstab->n_type = MYatoi(s);
		printf("n_type: 0x%lx\n", (long)pstab->n_type);
	}
	printf("%d: n_other [cr=0x%lx]: ", cstabno, (long)pstab->n_other);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pstab->n_other = MYatoi(s);
		printf("n_other: 0x%lx\n", (long)pstab->n_other);
	}
	printf("%d: n_desc [cr=0x%lx]: ", cstabno, (long)pstab->n_desc);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pstab->n_desc = MYatoi(s);
		printf("n_desc: 0x%lx\n", (long)pstab->n_desc);
	}
	printf("%d: n_value [cr=0x%lx]: ", cstabno, (long)pstab->n_value);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pstab->n_value = MYatoi(s);
		printf("n_value: 0x%lx\n", (long)pstab->n_value);
	}

	// write to file
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.writestabs(csec);
		}

		// reread data
		char dummy[1];
		readstabs(eo, dummy);
	}
	return;
}

static void
increment(ElfObject &eo, int &cstabno, int &csec, char *&p0, char *&pe, char &buf)
{
	int multilined;

	// check section and stab
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (!isStabSection(eo, csec))
	{
		printf("section is not a stab table.\n");
		return;
	}
	if (cstabno < 0 || cstabno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current stab number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((cstabno+1)*eo.pshdr(csec).sh_entsize < eo.pshdr(csec).sh_size)
	{
		cstabno++;
		showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the next stab table section, if any.
	//
	int ncsec = csec+1;
	for ( ; ncsec < eo.pehdr()->e_shnum; ncsec++)
	{
		if (isStabSection(eo, ncsec))
			break;
	}
	if (ncsec >= eo.pehdr()->e_shnum)
	{
		printf("no next stab table found.\n");
		return;
	}

	// return the first stab in the section
	cstabno = 0;
	csec = ncsec;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
	return;
}

static void
decrement(ElfObject &eo, int &cstabno, int &csec, char *&p0, char *&pe, char &buf)
{
	int multilined;

	// check section and stab
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (!isStabSection(eo, csec))
	{
		printf("section is not a stab table.\n");
		return;
	}
	if (cstabno < 0 || cstabno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current stab number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((cstabno-1) >= 0)
	{
		cstabno--;
		showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the previous stab table section, if any.
	//
	int ncsec = csec-1;
	for ( ; ncsec >= 0; ncsec--)
	{
		if (isStabSection(eo, ncsec))
			break;
	}
	if (ncsec < 0)
	{
		printf("no previous stab table found.\n");
		return;
	}

	// return the last stab in the section
	csec = ncsec;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	cstabno = eo.pshdr(csec).sh_size/eo.pshdr(csec).sh_entsize;
	showstab(eo, cstabno, csec, p0, pe, buf, 0, multilined);
	return;
}

void
editstabs(ElfObject &eo, char *)
{
	char buf;
	char s[BUFSIZ];

	// start of stabs editing
	printf("editing stabs:\n");

	// init counters
	totalparsesuccess = 0;
	totalparsefail = 0;

	// initialize stab pointers
	int csec = eo.pehdr()->e_shnum;
	int cstabno = 0;

	// find the first stab table
	for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
	{
		if (isStabSection(eo, sec))
		{
			csec = sec;
			break;
		}
	}
	if (csec >= eo.pehdr()->e_shnum)
	{
		printf("no stab tables found.\n");
		return;
	}

	// pointers to stab tables
	char *p0 = eo.pshdrdata(csec);
	char *pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("stabs cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			// printf("unknown cmd.\n");
			increment(eo, cstabno, csec, p0, pe, buf);
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("stab table names:\n");
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				if (!isStabSection(eo, sec))
					continue;
				printf("section %d: %s (%d)\n", 
					sec, eo.shdrnm(sec), 
					eo.pshdr(sec).sh_name);
			}
		}
		else if (*pt == '?' || *pt == 'h')
		{
			char *pt2 = gettoken(2);
			if (pt2 == NULL || *pt2 == '\0')
				printmenu();
			else if (*pt2 == 't')
				listtypes();
			else
				printf("unknown help request.\n");
		}
		else if (*pt == 'D')
		{
			dflag = !dflag;
			if (dflag)
				printf("demangle mode ON.\n");
			else
				printf("demangle mode OFF.\n");
		}
		else if (*pt == 'T')
		{
			dotrace = !dotrace;
			if (dotrace)
				printf("trace mode ON.\n");
			else
				printf("trace mode OFF.\n");
		}
		else if (*pt == 'P')
		{
			char *pt2 = gettoken(2);
			if (pt2 == NULL || *pt2 == '\0')
			{
				doparse = !doparse;
			}
			else
				doparse = MYatoi(pt2);
			switch (doparse)
			{
			case 0:
				printf("stabs parse mode OFF.\n");
				break;
			case 1:
				printf("stabs unconditional parse mode ON.\n");
				break;
			case 2:
				printf("stabs query parse mode ON.\n");
				break;
			default:
				doparse = 0;
				printf("stabs parse mode OFF.\n");
				break;
			}
		}
		else if (*pt == 'S')
		{
			printf("\ntotal parse success = %ld\n", 
				totalparsesuccess);
			printf("total parse fail    = %ld\n", 
				totalparsefail);
		}
		else if (*pt == 'A')
		{
			useaixsyntax = !useaixsyntax;
			if (useaixsyntax)
				printf("stabs AIX syntax is ON.\n");
			else
				printf("stabs SUN syntax is ON.\n");
		}
		else if (*pt == 'l')
		{
			line_numbers(eo, cstabno, csec, p0, pe);
		}
		else if (*pt == 'r')
		{
			review(eo, cstabno, csec, p0, pe);
		}
		else if (*pt == 'u')
		{
			update(eo, cstabno, csec, p0, pe);
		}
		else if (*pt == '+')
		{
			increment(eo, cstabno, csec, p0, pe, buf);
		}
		else if (*pt == '-')
		{
			decrement(eo, cstabno, csec, p0, pe, buf);
		}
		else if (*pt == 'q')
		{
			done = 1;
		}
		else
		{
			printf("unknown cmd.\n");
		}
	}
	return;
}
