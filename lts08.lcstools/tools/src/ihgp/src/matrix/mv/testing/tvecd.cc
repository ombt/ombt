
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
//      tvec.cc        Testing routine for MV++ vector class
//
//      Usage:  <length> <block-index start>  <block-index end>
//
//      (Suggest these to be relatively small positive numbers, as  
//      output is proportional to "length".)
//
//      This test program is self-explanatory (there are diagnostics
//      output message of what each step is performing)and excercise 
//      most of the MV_Vector methods, thus providing examples of how 
//      these are used.
//


#include <iostream.h>
#include <stdlib.h>

#include "vecdefs.h"
#include VECTOR_H


// example function to zero-out a vector:  illustrates how to pass
// vectors by "reference" (modifiable), as opposed to by "value"
//
void foo(VECTOR_double *Aptr)
{
    VECTOR_double &A = *Aptr;

    A = (double) 0;
}

// example funciton to return MV_Vectors:  (returns vector + scalar)
//
VECTOR_double fexample(const VECTOR_double A, double s)
{
    int N = A.size();
    VECTOR_double B(N);

    for (int i=0; i<N; i++)
        B(i) = A(i) + s;

    return B;
}


int TestVec_double(int N, int start, int End)
{

    int i;

    cout << endl;
    cout << "TestVec_double :" << endl;

    cout << "Using N = " << N << "   start = " << start << "   End = "
        << End << endl;



    VECTOR_double C;
    cout << " Test null constructor:  MV_Vector C() " << endl;
    cout << C << endl;


    VECTOR_double A(N);
    cout << " Test MV_Vector(int) constructor:  MV_Vector A(N) " << endl;
    cout << " values should be uninitalized.. " << endl;
    cout << A << endl;

    VECTOR_double B(N, 3);
    cout << " Test MV_Vector(int, val) constructor:  MV_Vector B(N, 3.0) " << endl;
    cout << "   all values should be 3.0:  " << endl;
    cout << B << endl;

    cout << " Test A(i) indexing, set A(i) = i, and B(i) = -i " << endl;
    for (i=0; i<N; i++)
        {
            A(i) = i;
            B(i) =  -i;
        }

    cout << " A " << endl;
    cout << A << endl;
    cout << " B " << endl;
    cout <<  B << endl;


    cout  << "Test opeator= :  C = A " << endl;
    cout <<  "  C is a new copy of A " << endl;

    C = A;                      

    cout  << "  C : " << endl;
    cout << C << endl;


    cout << "Testing MV_VecIndexI(start, End) " << endl;
    //
    MV_VecIndex I(start, End);
    //
    cout << "I = (" << start << ":"<< End <<") " << endl;

    cout << "Test A(I) = B(I) " <<  endl ;
    A(I) = B(I);
    cout << A << endl;

    cout << "Testing A(I) = 11 " << endl;
    A(I) =  (double) 11;
    cout << "A" << endl << A << endl;

    cout << "Testing MV_VecIndex + operators: " << endl;
    cout << " A = 0.0; A(I+1) = 1.0; " << endl;
    //
    A = (double) 0; 
    A(I+1) =  (double) 1;
    //
    cout << " A " << endl << A << endl;

    MV_VecIndex J( N/4,  (3*N)/4);
    cout << "Testing MV_VecIndex +/- operators: " << endl;
    cout << " J = MV_VecIndex( N/4,  (3*N)/4  );" << endl;
    cout << " J = MV_VecIndex(" <<  N/4  << "," << (3*N)/4 << ");" << endl;
    cout << " A = 0.0; A(J+1) = B(J-1); " << endl;
    //
    A =  (double) 0; 
    A(J+1) = B(J-1);
    //
    cout << " A " << endl << A << endl;


    J = MV_VecIndex( N/2 , N/2  );
    cout << "Testing MV_VecIndex +/- operators: " << endl;
    cout << " J = MV_VecIndex( N/2 , N/2   );" << endl;
    cout << " J = MV_VecIndex(" << N/2  << "," << N/2 << ");" << endl;
    cout << " A = 0.0; A(J+1) = B(J-1); " << endl;
    //
    A = (double) 0; 
    A(J+1) = B(J-1);
    //
    cout << " A " << endl << A << endl;

    cout << " A = 0.0; A(J-1) = B(J+1); " << endl;
    //
    A = (double) 0; 
    A(J-1) = B(J+1);
    //
    cout << " A " << endl << A << endl;

    cout << "Test = when matrices are overlapping  " << endl;
    cout << " A(MV_VecIndex(0,N-1)) = B; A(J+1) = A(J-1); " << endl;
    //
    A(MV_VecIndex(0, N-1)) = B; 
    A(J+1) = A(J-1);
    //
    cout << " A " << endl << A << endl;



    cout << "Testing VECTOR_double &c = C(I)" << endl;

#if 0
// The following seciton attemps to take the address of a temporary
// result and hence not compilable on all C++ platforms.

    VECTOR_double &c = C(I);
    cout << " c " << endl;
    cout << c << endl;

    cout << "Testing c = B(I) " << endl;
    c  = B(I);
    cout << "c " << endl;
    cout << c << endl;

    cout << " C " << endl;
    cout << C << endl;

    cout << "Testing passing subvectors to functions: calling foo(&c) " 
        << endl << "  where foo zeros out a matrix " << endl;
    foo(&c);
    cout << "C" << endl << C << endl;
#endif

    VECTOR_double F(N, 9);
    cout << endl << "Test constructors from exisitng memory : " << endl;
    cout <<  " VECTOR_double E(&F(0),N) " << endl;

    VECTOR_double E(&F[0], N);
    cout << "E : " << endl << E << endl;

    cout << " Now modify F(0) = 999 " << endl;
    F(0) = (double) 999;
    cout << "E should not be changed:" << endl;
    cout << E << endl;

    cout << "Test reference constructors from exisitng memory : " << endl;
    cout <<  " VECTOR_double G(&F(0),N, MV_Vector_::ref) " << endl;

    VECTOR_double G(&F[0], N, MV_Vector_::ref);

    cout << " Now modify F(0) = 999 " << endl;
    F(0) = (double) 999;
    cout << "G " << endl;
    cout << G << endl;

    cout << "Now set G = 5, F should change also " << endl;
    //
    G = (double) 5;
    //
    cout << " F: " << endl << F << endl;


    cout << "Testing returning subvectors from functions:  " 
        << endl << "   VECTOR_double K = fexample(A,1.0); " << endl;
    //
    VECTOR_double K = fexample(A,1);
    //
    cout << "A" << endl << A << endl;
    cout << "K" << endl << K << endl;


    VECTOR_double Q( &A(0), A.size(), MV_Vector_::ref);
    A() = B;
    //
    cout << "Q points to A;  A() = B " << endl;
    cout << "Q  "<< endl;
    cout << Q << endl;

    A(MV_VecIndex()) = B;
    //
    cout << "Q points to A;  A(MV_VecIndex()) = B " << endl;
    cout << "Q  "<< endl;
    cout << Q << endl;

    cout << "Test A(B(I)) constructor " << endl;
    VECTOR_double Z = A(I);
    cout << "A(I) " << endl;
    cout << A(I) << endl;
    Z = (double) 0;
    cout << "After Z=A(I); Z=0;, A should remian unchanged " << endl;
    cout << A << endl;

    cout << endl;
    return 0;
}

