#ifndef __SYMTAB_H
#define __SYMTAB_H
/*
 * header for symbol table.
 */

/* required headers */
#include "gettoken.h"

/* definitions */
#define SYMBOLTABLESIZE 200
#define BLOCKTABLESIZE 20

/* offsets for stack */
#define PARAMOFFSET 2
#define AUTOOFFSET -6
#define INTEGERSIZE 2
#define INTEGERPOINTERSIZE 2
#define BOOLEANSIZE 2
#define BOOLEANPOINTERSIZE 2

/* type of symbol */
enum SYMBOLTYPE {
        S_INTEGER, S_BOOLEAN, S_STRING, S_VOID, S_ERROR, S_NULL
};
typedef enum SYMBOLTYPE TYPE;

/* kind of symbol */
enum KINDTYPE {
        K_ARRAY, K_FUNCTION, K_PROCEDURE, K_SIMPLE, K_NULL
};
typedef enum KINDTYPE KIND;

/* type of check to conduct when inserting a nwe entry */
enum TYPEOFCHECKTYPE {
        NOPARAMETERS, ONLYPARAMETERS
};
typedef enum TYPEOFCHECKTYPE TYPEOFCHECK;

/* structure for symbol table */
struct SYMBOLRECORD {
        int entry;
        char name[MAXIDENTSIZE];
        int offset;
        int argOffset;
        int level;
        TYPE type;
        KIND kind;
        int isReference;
        int isParameter;
        int dimension;
        int nextParameter;
};
typedef struct SYMBOLRECORD SYMBOL;

/* structure for tracking blocks */
struct BLOCKRECORD {
        char name[MAXIDENTSIZE];
        int isFunction;
        int size;
};
typedef struct BLOCKRECORD BLOCK;

/* globals for symbol table */
extern int symbolTableSize;
extern SYMBOL **symbolTable;
extern int blockTableSize;
extern int *blockTable;
extern int lastSymbolEntry;
extern int level;

/* externs */
extern int insertEntry();
extern int deleteEntry();
extern int findEntry();
extern int updateEntry();
extern int createBlock();
extern int destroyBlock();
extern void dumpBlock();
extern void dumpSymbolTable();
extern int createSymbolTable();
extern int destroySymbolTable();
extern void updateSymbolType();
extern void updateNextParameter();
extern int updateParamOffsets();
extern int updateVarOffsets();
extern int setParamOffsets();

#endif
