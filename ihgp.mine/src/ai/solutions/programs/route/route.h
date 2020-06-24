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
#include <stdlib.h>
#include <string.h>
#include <new.h>
#include "aigraph.h"


/*
 * In this problem we want to find the shortest route from city
 * X to city Y. We design a class Path derived from class 
 * UnicostGraph which performs a uniform cost search
 *
 * Because in this problem there is no fixed number of operators
 * for each node generated we use expand() instead of do_operator().
 */


/*
 * Route is used to store the road or map info: pairs of cities and
 * the distance of the roads between the cities.
 */
typedef struct Route
{
    char
        *from,
        *to;
    int
        distance;
} Route;


/*
 * Class City defines objects that represent the cities that are
 * visited during the search, or put differently: the nodes generated
 * by the search. Because a uniform cost search must be done class
 * City is derived from class UniNode.
 */
class City : public UniNode
{
    public:
        City(const char *, int);
        int getdist() const;

       /*
        * The following three functions define the virtual functions
        * that must be implemented.
        */
        int operator==(const Node &) const;
        void display() const;
        IntrList<Node> *expand(int);
    private:
        const char
            *city;       // city that is currently being visited
        int
            dist;        // distance from the city previously visited
                         // to this city
};


/*
 * Class Path implements the search algorithm. Because uniform cost
 * is selected class Path is derived from UniCostGraph.
 */
class Path : public UnicostGraph
{
    public:
        Path(City *start, City *target);
        int compute_g(const Node *);
};


