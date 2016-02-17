// test for link list

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "list.h"

main(int argc, char **argv)
{
        cout << "List l1;" << endl;
        List l1;
        cout << l1 << endl;

        cout << "insert(l1, DataItem);" << endl;
        for (int il = 0; il < atoi(argv[1]); il++)
        {
                DataItem *d = new DataItem(2*il);
                assert(d != 0);
                assert(insert(l1, d, STARTOFLIST) == OK);
        }
        cout << l1 << endl;

        cout << "List l2;" << endl;
        List l2;
        cout << l2 << endl;

        cout << "insert(l1, DataItem);" << endl;
        for (il = 0; il < atoi(argv[1]); il++)
        {
                DataItem *d = new DataItem(2*il);
                assert(d != 0);
                assert(insert(l2, d, ENDOFLIST) == OK);
        }
        cout << l2 << endl;

        cout << "locate(l1, DataItem(10), lp);" << endl;
        ListPosition lp;
        if (locate(l1, &DataItem(10), lp) == OK)
                cout << *lp << endl;
        else
                cout << "DataItem(10) not found." << endl;

        cout << "retrieve(l1, d, lp);" << endl;
        DataItem *d;
        if (retrieve(l1, d, lp) == OK)
        {
                cout << *d << endl;
        }
        else
                cout << "DataItem(10) not found." << endl;

        cout << "ListIterator li(l1);" << endl;
        ListIterator li(l1);
        while ((d = li()) != (DataItem *)0)
        {
                cout << *d << endl;
        }
        cout << "li = l2);" << endl;
        li = l2;
        while ((d = li()) != (DataItem *)0)
        {
                cout << *d << endl;
        }

        cout << "retrieveAndRemove(l1, d, STARTOFLIST);" << endl;
        while (retrieveAndRemove(l1, d, STARTOFLIST) == OK)
        {
                cout << "removed ..." << *d << endl;
                cout << "list remaining = " << l1 << endl;
        }

        cout << "retrieveAndRemove(l2, d, ENDOFLIST);" << endl;
        while (retrieveAndRemove(l2, d, ENDOFLIST) == OK)
        {
                cout << "removed ..." << *d << endl;
                cout << "list remaining = " << l2 << endl;
        }

        // all done
        return(0);
}

