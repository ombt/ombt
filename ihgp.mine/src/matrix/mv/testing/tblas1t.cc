
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                                                           */
/*                                                                           */
/*                   MV++ Numerical Matrix/Vector C++ Library                */
/*                             MV++ Version 1.5                              */
/*                                                                           */
/*                                  R. Pozo                                  */
/*               National Institute of Standards and Technology              */
/*                                                                           */
/*                                  NOTICE                                   */
/*                                                                           */
/* Permission to use, copy, modify, and distribute this software and         */
/* its documentation for any purpose and without fee is hereby granted       */
/* provided that this permission notice appear in all copies and             */
/* supporting documentation.                                                 */
/*                                                                           */
/* Neither the Institution (National Institute of Standards and Technology)  */
/* nor the author makes any representations about the suitability of this    */
/* software for any purpose.  This software is provided ``as is''without     */
/* expressed or implied warranty.                                            */
/*                                                                           */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//
//      tmat.cc        Testing routine for MV++ matrix class
//
//      Usage:  <M> <N> <block-index start>  <block-index end>
//
//      (Suggest these to be relatively small positive numbers, as  
//      output is proportional to "length".)
//
//      This test program is self-explanatory (there are diagnostics
//      output message of what each step is performing)and excercise 
//      most of the MV_ColMat methods, thus providing examples of how 
//      these are used.
//


#include <iostream.h>
#include <stdlib.h>

#include "vecdefs.h"
#include VECTOR_H

int TestBlas1_$TYPE(int N)
{
    int j;
    cout << " \n TestBlas1_$TYPE: \n" ;
    cout << "Using N = " << N << " N = " << N << endl;

    VECTOR_$TYPE x(N), y(N), z(N);

    for (j=0; j<N; j++)
    {
        x(j) = j;
        y(j) = j / 100;
    }

    cout << " x " << endl;
    cout << x << endl;

    cout << "Testing Vector * scalar:   " << endl;
    z = x * 2;
    cout << " z = x * 2 : " << endl;
    cout << z << endl;
    z = z * 2;
    cout << " z = z * 2 : " << endl;
    cout << z << endl;


    cout << "Testing Vector *= scalar:  " << endl;
     (x *= 2) *= 2;
    cout << " (x *= 2) *= 2: " << endl;
    cout << x << endl;

    cout << "Testing Vector += Vector:  " << endl;
    x = ($TYPE) 2; 
    y = ($TYPE) 1;

    cout << "x=2, y=1; x += y: " << endl;
    x += y;
    cout << x << endl;

    cout << "Testing Vector -= Vector:  " << endl;
    x = ($TYPE) 2; 
    y = ($TYPE) 1;

    cout << "x=2, y=1; x -= y: " << endl;
    x -= y;
    cout << x << endl << endl;


    return 0;
}
