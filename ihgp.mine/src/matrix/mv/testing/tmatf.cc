
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
#include MATRIX_H

// #include "systime.c"

// example function to zero-out a matrix:  illustrates how to pass
// matrixes by "reference" (modifiable), as opposed to by "value"
//
void foo(MATRIX_float *Aptr)
{
    MATRIX_float &A = *Aptr;

    A = (float) 0;      // equivalent to "Aptr->operator=(0);"
}

// example funciton to return MV_ColMats:  (returns matrix + scalar)
//
MATRIX_float fexample(const MATRIX_float A, float s)
{
    int M = A.size(0);
    int N = A.size(1);
    MATRIX_float B(M,N);

    for (int j=0; j<N; j++)
        for (int i=0; i<M; i++)
            B(i,j) = A(i,j) + s;

    return B;
}


int TestMat_float(int M, int N, int Istart, int Jstart, int Iend, int Jend)
{

    int i,j;

    cout << endl;
    cout << "TestMat_float : " << endl;
    cout << "Using M = " << M << " N = " << N << 
            "   Istart = " << Istart << "   Iend = " <<  Iend <<
            "   Jstart = " << Jstart << "   Jend = " <<  Jend << endl;



    MATRIX_float C;
    cout << " Test null constructor:  MV_ColMat C() " << endl;
    cout << C << endl;


    MATRIX_float A(M,N);
    cout << " Test MV_ColMat(int,int) constructor:  MV_ColMat A(M,N) " << endl;
    cout << " values should be uninitalized.. " << endl;
    cout << A << endl;

    MATRIX_float B(M,N, 3);
    cout << " Test MV_ColMat(int, int, val) constructor:  MV_ColMat B(N, 3.0) " 
                << endl;
    cout << "   all values should be 3:  " << endl;
    cout << B << endl;

    cout << " Test A(i,j) indexing, set A(i,j) = 100*j + i, and B(i) = -A(i,j)"
        << endl;

    for (j=0; j<N; j++)
        for (i=0; i<M; i++)
        {
            A(i,j) = 100*i+j;
            B(i,j) =  - A(i,j);
        }

    cout << " A " << endl;
    cout << A << endl;
    cout << " B " << endl;
    cout <<  B << endl;



    cout << "Testing MV_VecIndex I(Istart, Iend) " << endl;
    //
    MV_VecIndex I(Istart, Iend);
    MV_VecIndex J(Jstart, Jend);
    //
    cout << "I = (" << Istart << ":"<< Iend <<") " << endl;
    cout << "J = (" << Jstart << ":"<< Jend <<") " << endl;

    cout << "Test A(I,J) = B(I,J) " <<  endl ;
    A(I,J) = B(I,J);
    cout << A << endl;

    cout << "Testing A(I,J) = 1.1 " << endl;
    A(I,J) = 11;
    cout << "A" << endl << A << endl;

    cout << "Testing MV_VecIndex + operators: " << endl;
    cout << " A = 0.0; A(I+1,J-1) = 1.0; " << endl;
    //
    A = (float) 0; 
    A(I+1,J-1) = (float) 1;
    //
    cout << " A " << endl << A << endl;


    cout << endl;

    return 0;
}
