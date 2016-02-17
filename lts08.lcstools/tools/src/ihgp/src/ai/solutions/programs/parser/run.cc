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

#include <stdio.h>
#include <new.h>
#include "parser.h"

#ifdef _MSC_VER
int no_mem(size_t size)
{
    fprintf(stderr, "Out of memory\n");
    exit(1);
    return(0);
}
#else
void no_mem()
{
    fprintf(stderr, "Out of memory\n");
    exit(1);
}
#endif


int main(int argc, char *argv[])
{

    /* Install new handler to catch out of memory errors. */
#ifdef _MSC_VER
    _set_new_handler(no_mem);
#else
    set_new_handler(no_mem);
#endif
 
    if (argc == 1)
    {
        printf("Usage: %s <string>\n", argv[0]);
        exit(0);
    }
    Parse
        sentence(++argv, argc -1, new Element(S));

    if (sentence.generate())
	sentence.display();
}
