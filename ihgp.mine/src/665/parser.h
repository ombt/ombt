#ifndef __PARSER_H
#define __PARSER_H

/* headers for parser */
#include "gettoken.h"
#include "symtab.h"

/* definitions for parser */
#define OK 0
#define NOTOK 1
#define NOTFOUND 2
#define FIRSTOPEN 3
#define REOPEN 4
#define NOREOPEN 5
#define FIRSTPASS 6
#define SECONDPASS 7
#define DUPLICATE 8
#define ON 1
#define OFF 0
#define NO 0
#define YES 1
#define ALLBLOCKS -1
#define GLOBALBLOCK 1
#define IABS(value) (((value) < 0) ? -1*(value) : (value))
#define STRINGTABLESIZE 100

/* structure for returning semantic information */
struct SEMANTICINFOTYPE {
        TYPE type;
        KIND kind;
        int isVariable;
        int integerValue;
        int booleanValue;
        char stringValue[MAXIDENTSIZE];
        int symbolTableIndex;
};
typedef struct SEMANTICINFOTYPE SEMANTICINFO;

/* debugging macros */
#ifdef DEBUG
#define WHERE() fprintf(stderr, "%s'%d\n", __FILE__, __LINE__)
#else
#define WHERE()
#endif

/* extern parser functions */
extern int initTrace();
extern int initParser();
extern int parser();
extern void closeTrace();
extern void closeParser();
extern void program();
extern int vardecl();
extern void progbody();
extern void vardecllist();
extern void morevars();
extern int vardeclitem();
extern TYPE typeid();
extern void params();
extern void paramlist();
extern int paramitem();
extern void progbody();
extern void stmtlist();
extern void block();
extern void stmt();
extern void funcdecl();
extern void procdecl();
extern void assignment();
extern SEMANTICINFO variableitem();
extern void loopstmt();
extern void whenclause();
extern void inputstmt();
extern void outputstmt();
extern SEMANTICINFO incr();
extern void ifstmt();
extern void forstmt();
extern void proccall();
extern void arguments();
extern void arguments();
extern void returnstmt();
extern void elseifpart();
extern void elsepart();
extern TOKEN multop();
extern TOKEN addop();
extern TOKEN optionalsign();
extern SEMANTICINFO expr();
extern SEMANTICINFO term();
extern SEMANTICINFO simpleexpr();
extern void AcceptToken();
extern SEMANTICINFO factor();
extern SEMANTICINFO factorident();
extern SEMANTICINFO unsignedconst();
extern void fp_program();
extern void fp_funcdecl();
extern void fp_procdecl();
extern void fp_params();
extern void fp_paramlist();
extern int fp_paramitem();
extern TYPE fp_typeid();
extern void writeVariable();
extern void readVariable();

#endif
