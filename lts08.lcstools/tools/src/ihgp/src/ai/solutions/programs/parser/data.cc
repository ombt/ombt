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

#include "parser.h"

/*
 * rules[] is an array that contains the CF-PSG rules that are used by the
 * parser to rewrite a LHS syntactic category to new syntactic categories.
 * The size of the array is indicated by MaxRule.
 */
Rule
    rules[] = {
		Rule(S, NP, VP, END),
                Rule(NP, NP1, END),
                Rule(NP, DET, N, END),
                Rule(VP, IV, END),
                Rule(VP, TV, NP, END),
                Rule(VP, TV2, CN, S, END),
              };

int MaxRule = 6;

/*
 * lexicon[] is an array that contains the words that are recognized by the
 * parser. The size of the array is indicated by MaxWord.
 */
Word
    lexicon[] = {
		    Word(TV, "kills"),
                    Word(TV2, "thinks"),
                    Word(IV, "sleeps"),
                    Word(NP1, "john"),
                    Word(N, "man"),
                    Word(DET, "the"),
                    Word(CN, "that"),
                };

int MaxWord = 7;

