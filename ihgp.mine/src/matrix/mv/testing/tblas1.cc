
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


#include "mvvtp.h"

int
main(int argc, char *argv[])
{
    if (argc<2)
    {
        cout << "Usage: " << " M " << endl;
        exit(1);
    }

    int N = atoi(argv[1]);
    int j;
    cout << "Using N = " << N << " N = " << N << endl;

    MV_Vector<double> x(N), y(N), z(N);

    for (j=0; j<N; j++)
    {
        x(j) = j;
        y(j) = j / 100.0;
    }

    cout << " x " << endl;
    cout << x << endl;

    cout << "Testing MV_Vector * scalar:   " << endl;
    z = x * 2.0;
    cout << " z = x * 2.0 : " << endl;
    cout << z << endl;
    z = z * 2.0;
    cout << " z = z * 2.0 : " << endl;
    cout << z << endl;


    cout << "Testing MV_Vector *= scalar:  " << endl;
     (x *= 2.0) *= 2.0;
    cout << " (x *= 2.0) *= 2.0: " << endl;
    cout << x << endl;

    cout << "Testing MV_Vector += MV_Vector:  " << endl;
    x = 2.0; 
    y = 1.0;

    cout << "x=2.0, y=1.0; x += y: " << endl;
    x += y;
    cout << x << endl;

    cout << "Testing MV_Vector -= MV_Vector:  " << endl;
    x = 2.0; 
    y = 1.0;

    cout << "x=2.0, y=1.0; x -= y: " << endl;
    x -= y;
    cout << x << endl;


    return 0;
}
