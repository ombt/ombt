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

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <stdarg.h>
#include "aitree.h"
#include "xarray.h"
#include "xlist.h"
#include "xstring.h"

/*
 * Item defines symbolic names for the syntactic categories used for
 * the CF-PSG rules and the words in the lexikon. Note that if the parser
 * is extended with rules that contain new syntactic categories Item must
 * be updated accordingly.
 */
enum Item
{
    S,
    VP,
    IV,
    TV,
    TV2,
    NP,
    NP1,
    N,
    DET,
    CN,
};

#define END -1

/*
 * Class Rules represents CF-PSG rules, i.e. rules like 'S -> NP VP' and
 * 'NP -> DET N'. The arguments passed to the constructor of Rule are of
 * type Item. The first Item represents the LHS (left hand side) of the
 * rule and the rest the RHS (right hand side) of the rule. A variable
 * argument list is used so that the number of syntactic categories in the
 * RHS is not limited. The last argument passed must be END. Class Rule
 * contains two data fields: the head (LHS) of the rule and the body
 * (the RHS) of the rule.
 */
class Rule
{
    public:
	Rule(Item, ...);
	void display() const;
	const List<Item> *match(Item) const;
    private:
	Item head;
	List<Item> body;
};

/*
 * Class Word represents words that will be stored in the lexicon used
 * by the parser. Class Word contains two data fields: the word
 * itself (a string) and the syntactic category of this word.
 */
class Word
{
    public:
	Word(Item, const char *);
	void display() const;
	int match(Item, const char *) const;
    private:
	Item head;
	String word;
};

extern Rule rules[];
extern int MaxRule;
extern Word lexicon[];
extern int MaxWord;

/*
 * Class Element is a representation of the nodes generated during the
 * search and serves as an abstraction of the syntactic categories.
 * Note that expand() is implemented instead of do_operator() because
 * there is not a fixed number of operators for this problem (every LHS
 * of a CF-PSG may be rewritten to a RHS containing a variable number
 * of syntactic categories). The Item element represents the syntactic
 * category of the word that was parsed by this Element object. The solution
 * path is made up of these elements.
 */
class Element : public OrNode
{
    public:
        Element(Item);
        Item get_item() const;

        int operator==(const Node &) const;
        void display() const;
        IntrList<Node> *expand(int);
    private:
        Item
            item;
};

/*
 * Parse represents the search algorithm. Because a depth-first search of
 * and AND/OR tree must be performed Parse is derived from AODepthTree.
 * Class Parse contains the sentence to be parsed (an array of char *),
 * an index to this sentence that indicates which word will be parsed next
 * and a value representing the number of words in the sentence.
 */
class Parse : public AODepthTree
{
    public:
        Parse(char **, int, Element *);
        int is_terminal(const AONode *);
    private:
        char
            **sentence;
        int
            index,
	    size;
};

#endif
