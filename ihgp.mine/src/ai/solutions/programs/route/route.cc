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

#include "route.h"

/*
 * database of cities and roads with associated distances.
 */
Route
    table[] = {
                { "amsterdam", "hamburg" ,440 },
                { "amsterdam", "london", 550 },
                { "amsterdam", "brussels", 210 },
                { "amsterdam", "cologne", 250 },
                { "brussels", "frankfurt", 410},
                { "brussels", "paris", 560 },
                { "brussels", "london", 390 },
                { "brussels", "luxembourg", 220},
                { "brussels", "cologne" , 220 },
                { "bordeaux", "madrid" ,690 },
                { "bordeaux", "marseille", 630 },
                { "cologne", "hamburg", 430 },
                { "cologne", "frankfurt", 200},
                { "cologne", "luxembourg", 200},
                { "cologne", "basel", 480},
                { "paris", "bordeaux", 560},
                { "paris", "marseille", 770},
                { "paris", "luxembourg", 340},
                { "paris", "basel", 490},
                { "madrid", "lisbon", 650},
                { "marseille", "genua", 400},
                { "marseille", "bern", 580},
                { "frankfurt", "basel", 330},
                { "frankfurt", "hamburg", 490},
                { "frankfurt", "berlin", 530},
                { "frankfurt", "vienna", 720},
                { "bern", "basel", 90},
                { "bern", "milan", 350},
                { "bern", "genua", 490},
                { "budapest", "vienna", 250},
                { "trieste", "vienna", 500},
                { "trieste", "milan", 420},
                { "trieste", "frankfurt", 900},
                { "milan", "rome", 590},
                { "hamburg", "copenhagen", 320},
                { "genua", "rome", 540},
                { "genua", "milan", 140},
                { "palermo", "rome", 590},
                { "vienna", "basel", 840},
                { "vienna", "berlin", 660},
                { "vienna", "warsaw", 690},
                { "berlin", "hamburg", 290},
                { "berlin", "amsterdam", 660},
                { "berlin", "warsaw", 560},
                { "luxembourg", "frankfurt", 230},
                { "luxembourg", "basel", 330},
                { 0, 0, 0}       // marks end of table
            };



Path::Path(City *start, City *target)
    :UnicostGraph(0, start, target)
{
}


City::City(const char *p, int d)
{
    city = p;
    dist = d;
}


/*
 * display() prints the current city and the distance of the 
 * solution path generated so far.
 */
void City::display() const
{
    printf("%15s   %5d\n", city, get_g());
}


int City::operator==(const Node &other) const
{
    const City &cother = (City &)other;
    return(!strcmp(city, cother.city));
}


int City::getdist() const
{
    return(dist);
}


/*
 * expand() expands the current node by generating all of its successor
 * nodes. In this problem this means searching the database for all
 * cities that are directly reachable from the current city. While
 * searching the database it must be checked if the current city
 * is in either halves of the road (from A to B == from B to A).
 * The information used to create the successor node consists of the
 * name of the neighbouring city and the distance between that city
 * and the current city.
 */
IntrList<Node> *City::expand(int ) 
{
    int
        i;
    City
        *tmp;
    IntrList<Node>
	*ret = new IntrList<Node>;

    for (i = 0; table[i].from != 0; i++)
    {
        if (!strcmp(table[i].from, city))
            tmp = new City(table[i].to, table[i].distance);
        else if (!strcmp(table[i].to, city))
            tmp = new City(table[i].from, table[i].distance);
        else 
            continue;   // no match found

	ret->addtohead(tmp);
    }
    return(ret);
}


/*
 * compute_g() serves to compute the cost of getting from
 * the parent node to the current node. In this problem the cost is 
 * represented by the distance of the path from the 'parent' city the
 * current city, so return the distance that was associated with the node
 * when it was created.
 */
int Path::compute_g(const Node *node)
{
    const City *cnode = (City *)node;
    return(cnode->getdist());
}


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


int main()
{
    /* Install new handler to catch out of memory errors. */
#ifdef _MSC_VER
    _set_new_handler(no_mem);
#else
    set_new_handler(no_mem);
#endif

    Path
        path(new City("copenhagen", 0), new City("rome", 0));

    if (path.generate())
	path.display();
    return(1);
}

