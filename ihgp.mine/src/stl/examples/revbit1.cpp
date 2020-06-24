// This example compiles using the new STL<ToolKit> from ObjectSpace, Inc.
// STL<ToolKit> is the EASIEST to use STL that works on most platform/compiler 
// combinations, including cfront, Borland, Visual C++, C Set++, ObjectCenter, 
// and the latest Sun & HP compilers. Read the README.STL file in this 
// directory for more information, or send email to info@objectspace.com.
// For an overview of STL, read the OVERVIEW.STL file in this directory.

#include <iostream.h>
#include <stl.h>

int array [] = { 1, 5, 2, 3 };

int main ()
{
  list<int> v (array, array + 4);
  reverse_bidirectional_iterator<list<int>::iterator, int, 
    list<int>::reference, list<int>::difference_type> r (v.end ());
  while (r != v.begin ())
    cout << *r++ << endl;
  return 0;
}
