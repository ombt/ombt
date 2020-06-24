/* symbol table management functions */

/* OS headers */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>

/* other headers */
#include "parser.h"
#include "symtab.h"

/* externs from parser */
extern FILE *traceFd;
extern FILE *listFd;
extern int traceFlag;

/* symbol table globals */
int symbolTableSize;
SYMBOL **symbolTable;
int blockTableSize;
int *blockTable;
int lastSymbolEntry;
int level;

/* tables for dumping info */
char *symbolTypeNames[] = {
        "INTEGER", "BOOLEAN", "STRING", "VOID", "ERROR", "NULL", ""
};
char *symbolKindNames[] = {
        "ARRAY", "FUNCTION", "PROCEDURE", "SIMPLE", "NULL", ""
};
char *symbolYesNo[] = {
        "NO", "YES", ""
};

/* print error msgs */
void
symmsgs(format, l1, l2, l3, l4, l5)
char *format;
unsigned long l1, l2, l3, l4, l5;
{
        fprintf(listFd, format, l1, l2, l3, l4, l5);
        if (traceFlag) fprintf(traceFd, format, l1, l2, l3, l4, l5);
        return;
}

/* create an empty symbol table */
int
createSymbolTable(symTabSz, blkTabSz)
int symTabSz, blkTabSz;
{
        int isym, iblk;

        /* check sizes */
        if (symTabSz <= 0)
        {
                symmsgs("WARNING: symbol table size <= 0.\n");
                symTabSz = 100;
        }
        symbolTableSize = symTabSz;
        if (blkTabSz <= 0)
        {
                symmsgs("WARNING: block table size <= 0.\n");
                blkTabSz = 20;
        }
        blockTableSize = blkTabSz;

        /* allocate tables */
        symbolTable = (SYMBOL **) malloc(symbolTableSize*sizeof(SYMBOL *));
        if (symbolTable == (SYMBOL **)0)
        {
                symmsgs("ERROR: unable to malloc a symbol table.\n");
                suicide();
        }
        blockTable = (int *)malloc(blockTableSize*sizeof(int));
        if (blockTable == (int *)0)
        {
                symmsgs("ERROR: unable to malloc a block table.\n");
                suicide();
        }

        /* initialize tables and other globals */
        for (isym = 0; isym < symbolTableSize; isym++)
        {
                symbolTable[isym] = (SYMBOL *)0;
        }
        for (iblk = 0; iblk < blockTableSize; iblk++)
        {
                blockTable[iblk] = -1;
        }
        level = 0;
        blockTable[level] = -1;
        lastSymbolEntry = -1;

        /* all done */
        return(OK);
}

/* create a new block */
int
createBlock()
{
        /* increment to next block and check range */
        if (++level >= blockTableSize)
        {
                symmsgs("ERROR: block table overflow.\n");
                suicide();
        }

        /* initialize block entry */
        blockTable[level] = lastSymbolEntry;

        /* all done */
        return(OK);
}

/* destroy a block */
int
destroyBlock()
{
        int isym;

        /* delete all symbol table entries in block */
        for (isym = blockTable[level-1]+1; isym <= blockTable[level]; isym++)
        {
                (void) deleteEntry(isym);
        }

        /* decrement block level and reset last symbol counter */
        level--;
        lastSymbolEntry = blockTable[level];

        /* all done */
        return(OK);
}

/* insert a new entry in symbol table */
int
insertEntry(newEntry, typeOfCheck)
SYMBOL *newEntry;
TYPEOFCHECK typeOfCheck;
{
        SYMBOL tmpsym;
        int tmpno;

        /* check if entry already exists in this block */
        if (findEntry(newEntry->name, &tmpsym, level, typeOfCheck) == OK)
        {
                newEntry->entry = -1;
                newEntry->level = level;
                return(DUPLICATE);
        }

        /* increment symbol table index */
        if (++lastSymbolEntry >= symbolTableSize)
        {
                symmsgs("ERROR: symbol table overflow.\n");
                suicide();
        }

        /* allocate a new entry */
        symbolTable[lastSymbolEntry] = (SYMBOL *)malloc(sizeof(SYMBOL));
        if (symbolTable[lastSymbolEntry] == (SYMBOL *)0)
        {
                symmsgs("ERROR: unable to malloc a symbol table entry.\n");
                suicide();
        }

        /* return symbol table index and block level to caller */
        newEntry->entry = lastSymbolEntry;
        newEntry->level = level;

        /* store information */
        *symbolTable[lastSymbolEntry] = *newEntry;
        blockTable[level] = lastSymbolEntry;

        /* all done */
        return(OK);
}

/* delete a symbol table entry */
int deleteEntry(entry)
int entry;
{
        /* check if entry exists */
        if (entry < 0 || entry > lastSymbolEntry)
        {
                symmsgs("WARNING: entry (%ld) is out of range (0, %ld).\n",
                        (long) entry, (long) lastSymbolEntry);
                return(NOTOK);
        }
        if (symbolTable[entry] == (SYMBOL *) 0)
        {
                symmsgs("WARNING: cannot delete a NULL symbol table entry (%ld).\n",
                        (long) entry);
                return(NOTOK);
        }

        /* delete entry */
        free(symbolTable[entry]);
        symbolTable[entry] = (SYMBOL *)0;

        /* all done */
        return(OK);
}

/* update a symbol table entry */
int updateEntry(entry)
SYMBOL *entry;
{
        /* update symbol table entry with new info */
        *symbolTable[entry->entry] = *entry;

        /* all done */
        return(OK);
}

/* find an entry in symbol table */
int
findEntry(symName, entry, block, typeOfCheck)
char *symName;
SYMBOL *entry;
int block;
TYPEOFCHECK typeOfCheck;
{
        int isym, symstart, symend;

        /* search entire symbol table or a specific block */
        if (block >= 0 && block <= level)
        {
                /* search given block */
                symstart = blockTable[block-1] + 1;
                symend = blockTable[block];
        }
        else
        {
                /* search entire symbol table */
                symstart = 0;
                symend = lastSymbolEntry;
        }

        /* search backwards for symbol */
        for (isym = symend; isym >= symstart; isym--)
        {
                /* restrict search type */
                switch (typeOfCheck)
                {
                case ONLYPARAMETERS:
                        /*
                         * check only parameters, used only for inserting
                         * function arguments.
                         */
                        if (symbolTable[isym]->isParameter != YES)
                        {
                                /* symbol was not found */
                                return(NOTFOUND);
                        }
                        if (strcmp(symbolTable[isym]->name, symName) == 0)
                        {
                                /* we found it */
                                *entry = *symbolTable[isym];
                                return(OK);
                        }
                        break;

                case NOPARAMETERS:
                        /*
                         * check NO parameters, used for all other
                         * types of searches.
                         */
                        if (symbolTable[isym]->isParameter == YES)
                        {
                                /* skip this entry */
                                continue;
                        }
                        if (strcmp(symbolTable[isym]->name, symName) == 0)
                        {
                                /* we found it */
                                *entry = *symbolTable[isym];
                                return(OK);
                        }
                        break;
                }
        }

        /* symbol was not found */
        return(NOTFOUND);
}

/* update the type of symbols for a range */
void
updateSymbolType(min, max, type)
int min, max;
TYPE type;
{
        int isym;

        /* check if there are any symbols to update */
        if (min == -1 && max == -1)
        {
                /* function had NO parameters */
                return;
        }

        /* check for weird cases */
        if (min == -1 && max != -1)
        {
                symmsgs("ERROR: updateSymbolType, min == -1 but max != -1.\n");
                suicide();
        }
        else if (min != -1 && max == -1)
        {
                symmsgs("ERROR: updateSymbolType, min != -1 but max == -1.\n");
                suicide();
        }

        /* update type */
        for (isym = min; isym <= max; isym++)
        {
                symbolTable[isym]->type = type;
        }

        /* all done */
        return;
}

/* setup link list for a parameter list */
void
updateNextParameter(min, max)
int min, max;
{
        int isym;

        /* check if there are any symbols to update */
        if (min == -1 && max == -1)
        {
                /* function had NO parameters */
                return;
        }

        /* check for weird cases */
        if (min == -1 && max != -1)
        {
                symmsgs("ERROR: updateSymbolType, min == -1 but max != -1.\n");
                suicide();
        }
        else if (min != -1 && max == -1)
        {
                symmsgs("ERROR: updateSymbolType, min != -1 but max == -1.\n");
                suicide();
        }

        /* update type */
        for (isym = min; isym < max; isym++)
        {
                symbolTable[isym]->nextParameter = isym + 1;
        }                          
        symbolTable[isym]->nextParameter = -1;

        /* all done */
        return;
}

/* dump symbol table block */
void
dumpBlock(block)
int block;
{
        int isym, symstart, symend;

        symstart = blockTable[block-1] + 1;
        symend = blockTable[block];

        symmsgs("\nSYMBOL TABLE FOR BLOCK %ld (%ld, %ld)\n", 
                (long) block, (long) symstart, (long) symend);
        for (isym = symstart; isym <= symend; isym++)
        {
                if (symbolTable[isym] == (SYMBOL *)0)
                {
                        symmsgs("WARNING: symbol table %ld is NULL.\n", (long)isym);
                        continue;
                }
                symmsgs("name: %-32s\n", symbolTable[isym]->name);
                symmsgs("ent: %03ld ", (long)symbolTable[isym]->entry);
                symmsgs("off: %04ld ", (long)symbolTable[isym]->offset);
                symmsgs("aoff: %04ld ", (long)symbolTable[isym]->argOffset);
                symmsgs("lvl: %02ld ", (long)symbolTable[isym]->level);
                symmsgs("typ: %-8s ", symbolTypeNames[(int)symbolTable[isym]->type]);
                symmsgs("knd: %-10s ", symbolKindNames[(int)symbolTable[isym]->kind]);
                symmsgs("ref: %-3s ", symbolYesNo[(int)symbolTable[isym]->isReference]);
                symmsgs("par: %-3s ", symbolYesNo[(int)symbolTable[isym]->isParameter]);
                symmsgs("dim: %03ld ", (long)symbolTable[isym]->dimension);
                symmsgs("nxt: %03ld\n", (long)symbolTable[isym]->nextParameter);
        }
        symmsgs("\n");
        return;
}

/* update parameter offsets for a function */
int
updateParamOffsets(funcSym)
SYMBOL *funcSym;
{
        int last, isym, offset;

        /* check if any function has any parameters */
        if (funcSym->nextParameter < 0)
        {
                /* function has no parameters */
                return(OK);
        }

        /* find last parameter */
        for (last = funcSym->nextParameter; 
             symbolTable[last]->nextParameter >= 0;
             last = symbolTable[last]->nextParameter)
        {
                /* do nothing */
        }

        /* update parameter offsets in reverse order */
        offset = PARAMOFFSET;
        for (isym = last; isym >= funcSym->nextParameter; isym--)
        {
                /* set offset for this parameter */
                symbolTable[isym]->offset = offset;

                /* calculate next offset */
                switch (symbolTable[isym]->type)
                {
                case S_INTEGER:
                        /* check for a pointer */
                        if (symbolTable[isym]->isReference == NO)
                        {
                                offset += INTEGERSIZE;
                        }
                        else
                        {
                                offset += INTEGERPOINTERSIZE;
                        }
                        break;

                case S_BOOLEAN:
                        /* check for a pointer */
                        if (symbolTable[isym]->isReference == NO)
                        {
                                offset += BOOLEANSIZE;
                        }
                        else
                        {
                                offset += BOOLEANPOINTERSIZE;
                        }
                        break;

                default:
                        symmsgs("ERROR: updateParamOffsets, bad parameter type %s.\n",
                                symbolTable[isym]->name);
                        suicide();
                }
        }

        /* store total argument offset in function symbol entry */
        funcSym->argOffset = offset;

        /* all done */
        return(OK);
}

/* update variable offsets for a function */
int
updateVarOffsets(minSym, maxSym)
int minSym, maxSym;
{
        int isym, offset;

        /* update variable offsets */
        offset = AUTOOFFSET;
        for (isym = minSym; isym <= maxSym; isym++)
        {
                /* calculate offset to variable */
                switch (symbolTable[isym]->type)
                {
                case S_INTEGER:
                        /* check for an array */
                        if (symbolTable[isym]->kind == K_ARRAY)
                        {
                                offset -= INTEGERSIZE*symbolTable[isym]->dimension;
                        }
                        else
                        {
                                offset -= INTEGERSIZE;
                        }
                        break;

                case S_BOOLEAN:
                        /* check for an array */
                        if (symbolTable[isym]->kind == K_ARRAY)
                        {
                                offset -= BOOLEANSIZE*symbolTable[isym]->dimension;
                        }
                        else
                        {
                                offset -= BOOLEANSIZE;
                        }
                        break;

                default:
                        symmsgs("ERROR: updateParamOffsets, bad parameter type %s.\n",
                                symbolTable[isym]->name);
                        suicide();
                }

                /* set offset for this parameter */
                symbolTable[isym]->offset = offset;
        }

        /* all done */
        return(IABS(offset));
}



/* dump symbol table */
void
dumpSymbolTable()
{
        int isym, symstart, symend;

        symstart = 0;
        symend = lastSymbolEntry;

        symmsgs("\nALL SYMBOL TABLE (%ld, %ld)\n", (long) symstart, (long) symend);

        for (isym = symstart; isym <= symend; isym++)
        {
                if (symbolTable[isym] == (SYMBOL *)0)
                {
                        symmsgs("WARNING: symbol table %ld is NULL.\n", (long)isym);
                        continue;
                }
                symmsgs("name: %-32s\n", symbolTable[isym]->name);
                symmsgs("ent: %03ld ", (long)symbolTable[isym]->entry);
                symmsgs("off: %04ld ", (long)symbolTable[isym]->offset);
                symmsgs("lvl: %02ld ", (long)symbolTable[isym]->level);
                symmsgs("typ: %-8s ", symbolTypeNames[(int)symbolTable[isym]->type]);
                symmsgs("knd: %-10s ", symbolKindNames[(int)symbolTable[isym]->kind]);
                symmsgs("ref: %-3s ", symbolYesNo[(int)symbolTable[isym]->isReference]);
                symmsgs("par: %-3s ", symbolYesNo[(int)symbolTable[isym]->isParameter]);
                symmsgs("dim: %03ld ", (long)symbolTable[isym]->dimension);
                symmsgs("nxt: %03ld\n", (long)symbolTable[isym]->nextParameter);
        }
        symmsgs("\n");
        return;
}

/* set parameter offsets for a function */
int
setParamOffsets(minPar, maxPar)
int minPar, maxPar;
{
        int isym, offset;

        /* update parameter offsets in reverse order */
        offset = PARAMOFFSET;
        for (isym = maxPar; isym >= minPar; isym--)
        {
                /* set offset for this parameter */
                symbolTable[isym]->offset = offset;

                /* calculate next offset */
                switch (symbolTable[isym]->type)
                {
                case S_INTEGER:
                        /* check for a pointer */
                        if (symbolTable[isym]->isReference == NO)
                        {
                                offset += INTEGERSIZE;
                        }
                        else
                        {
                                offset += INTEGERPOINTERSIZE;
                        }
                        break;

                case S_BOOLEAN:
                        /* check for a pointer */
                        if (symbolTable[isym]->isReference == NO)
                        {
                                offset += BOOLEANSIZE;
                        }
                        else
                        {
                                offset += BOOLEANPOINTERSIZE;
                        }
                        break;

                default:
                        symmsgs("ERROR: setParamOffsets, bad parameter type %s.\n",
                                symbolTable[isym]->name);
                        suicide();
                }
        }

        /* all done */
        return(OK);
}
