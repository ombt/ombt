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
 * Table[] defines text values for the syntactic categories defined
 * in Item.
 */
char
    *table[] = {"s", "vp", "iv", "tv", "tv2", "np", "np1",
                "n", "det", "cn"};

Rule::Rule(Item item1, ...)
{
    Item p;
    va_list args;

    va_start(args, item1);
    head = item1;

    while ((p = va_arg(args, Item)) != END)
	body.addtotail(p);

    va_end(args);
}

void Rule::display() const
{
    ROListIterator<Item> iter(body);
    int i;
    Item n;

    printf("%s --> ", table[head]);

    n = iter.getfirst();
    printf("%s ", table[n]);
    for (i = 1; i < body.getcount(); i++)
    {
        n = iter.getnext();
	printf("%s ", table[n]);
    }
    putchar('\n');
}

/*
 * match() checks whether the specified item matches the head
 * (the LHS) of the rule and if true returns the body (RHS) of the
 * rule.
 */
const List<Item> *Rule::match(Item item) const
{
    return(item == head ? &body : 0);
}

Word::Word(Item item, const char *wrd)
{
    head = item;
    word = wrd;
}

void Word::display() const
{
    cout << table[head] << ":" << word << "\n";
}

/*
 * match() checks if the specified syntactic category and word match
 * this word.
 */
int Word::match(Item item, const char *wrd) const
{
    return(head == item && word == wrd);
}

Parse::Parse(char **sntnc, int length, Element *start)
    : AODepthTree(0, start)
{
    sentence = sntnc;
    index = 0;
    size = length;
}

Element::Element(Item it)
{
    item = it;
}

void Element::display() const
{
   printf("%s\n", table[item]);
}

/*
 * The equality operator is not used anywhere in the program, but it
 * must be implemented because if is defined pure virtual in OrNode.
 */
int Element::operator==(const Node &other) const
{
    return(item == ((Element &)other).item);
}

Item Element::get_item() const
{
    return(item);
}

/*
 * To expand a node every rule in the rules DB base is checked to see
 * if its LHS is of the same syntactic category as the node. If a
 * match is found then an AND node is created, and for each of the
 * syntactic categories of the RHS of the rule a new node is created which
 * is added as a successor to the AND node. Keep in mind that the
 * terminology of AND an OR nodes that is used here differs from standard
 * terminology.
 */
IntrList<Node> *Element::expand(int )
{
    IntrList<Node> *ret = new IntrList<Node>;
    int i;

    for (i = 0; i < MaxRule; i++)
    {
	const List<Item> *rhs;
	AndNode *andnode;

        if ((rhs = rules[i].match(item)))
        {
            Item lhs_element;
	    ROListIterator<Item> iter(*rhs);
	    int d;

	    andnode = new AndNode();
	    lhs_element = iter.getfirst();
            andnode->addsucc(new Element(lhs_element));
            for (d = 1; d < rhs->getcount(); d++)
	    {
	        lhs_element = iter.getnext();
                andnode->addsucc(new Element(lhs_element));
	    }

	    ret->addtotail(andnode);
        }
    }
    return(ret);
}


/*
 * A node is terminal if the syntactic category of the item to be parsed
 * is "terminal" itself (in a linguistic sense). To check this condition
 * the lexicon must be searched for a word that matches the word in the
 * sentence (pointed to by the index) represented by the node. Two things
 * are checked: whether both syntactic categories are equal and whether
 * the words represented by these syntactice categories are equal.
 */
int Parse::is_terminal(const AONode *node)
{
    int
        i;

    if (index >= size)
	return(0);

    for (i = 0; i < MaxWord; i++)
	if (lexicon[i].match(((Element *)node)->get_item(), sentence[index]))
        {
            index++;
            return(1);
        }

    return(0);
}
