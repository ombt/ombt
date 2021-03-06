
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

#include "mvvtp.h"

// example function to zero-out a vector:  illustrates how to pass
// vectors by "reference" (modifiable), as opposed to by "value"
//
void foo(MV_Vector<double> *Aptr)
{
    MV_Vector<double> &A = *Aptr;

    A = 0.0;
}

// example funciton to return MV_Vectors:  (returns vector + scalar)
//
MV_Vector<double> fexample(const MV_Vector<double> A, double s)
{
    int N = A.size();
    MV_Vector<double> B(N);

    for (int i=0; i<N; i++)
        B(i) = A(i) + s;

    return B;
}


main(int argc, char *argv[])
{
    if (argc<4)
    {
        cout << "Usage " << argv[0] << " N start end" << endl;
        exit(1);
    }

    int N = atoi(argv[1]);
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);
    int i;
    cout << "Using N = " << N << "   start = " << start << "   end = "
        << end << endl;



    MV_Vector<double> C;
    cout << " Test null constructor:  MV_Vector C() " << endl;
    cout << C << endl;


    MV_Vector<double> A(N);
    cout << " Test MV_Vector(int) constructor:  MV_Vector A(N) " << endl;
    cout << " values should be uninitalized.. " << endl;
    cout << A << endl;

    MV_Vector<double> B(N, 3.0);
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


    cout << "Testing MV_VecIndexI(start, end) " << endl;
    //
    MV_VecIndex I(start, end);
    //
    cout << "I = (" << start << ":"<< end <<") " << endl;

    cout << "Test A(I) = B(I) " <<  endl ;
    A(I) = B(I);
    cout << A << endl;

    cout << "Testing A(I) = 1.1 " << endl;
    A(I) = 1.1;
    cout << "A" << endl << A << endl;

    cout << "Testing MV_VecIndex + operators: " << endl;
    cout << " A = 0.0; A(I+1) = 1.0; " << endl;
    //
    A = 0.0; 
    A(I+1) = 1.0;
    //
    cout << " A " << endl << A << endl;

    MV_VecIndex J( N/4,  (3*N)/4);
    cout << "Testing MV_VecIndex +/- operators: " << endl;
    cout << " J = MV_VecIndex( N/4,  (3*N)/4  );" << endl;
    cout << " J = MV_VecIndex(" <<  N/4  << "," << (3*N)/4 << ");" << endl;
    cout << " A = 0.0; A(J+1) = B(J-1); " << endl;
    //
    A = 0.0; 
    A(J+1) = B(J-1);
    //
    cout << " A " << endl << A << endl;


    J = MV_VecIndex( N/2 , N/2  );
    cout << "Testing MV_VecIndex +/- operators: " << endl;
    cout << " J = MV_VecIndex( N/2 , N/2   );" << endl;
    cout << " J = MV_VecIndex(" << N/2  << "," << N/2 << ");" << endl;
    cout << " A = 0.0; A(J+1) = B(J-1); " << endl;
    //
    A = 0.0; 
    A(J+1) = B(J-1);
    //
    cout << " A " << endl << A << endl;

    cout << " A = 0.0; A(J-1) = B(J+1); " << endl;
    //
    A = 0.0; 
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



    cout << "Testing MV_Vector<double> &c = C(I)" << endl;

#if 0
// The following seciton attemps to take the address of a temporary
// result and hence not compilable on all C++ platforms.

    MV_Vector<double> &c = C(I);
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

    MV_Vector<double> F(N, 9.0);
    cout << endl << "Test constructors from exisitng memory : " << endl;
    cout <<  " MV_Vector<double> E(&F(0),N) " << endl;

    MV_Vector<double> E(&F[0], N);
    cout << "E : " << endl << E << endl;

    cout << " Now modify F(0) = 99.9 " << endl;
    F(0) = 99.9;
    cout << "E should not be changed:" << endl;
    cout << E << endl;

    cout << "Test reference constructors from exisitng memory : " << endl;
    cout <<  " MV_Vector<double> G(&F(0),N, MV_Vector_::ref) " << endl;

    MV_Vector<double> G(&F[0], N, MV_Vector_::ref);

    cout << " Now modify F(0) = 99.9 " << endl;
    F(0) = 99.9;
    cout << "G " << endl;
    cout << G << endl;

    cout << "Now set G = 5.0, F should change also " << endl;
    //
    G = 5.0;
    //
    cout << " F: " << endl << F << endl;


    cout << "Testing returning subvectors from functions:  " 
        << endl << "   MV_Vector<double> K = fexample(A,1.0); " << endl;
    //
    MV_Vector<double> K = fexample(A,1.0);
    //
    cout << "A" << endl << A << endl;
    cout << "K" << endl << K << endl;


    MV_Vector<double> Q( &A(0), A.size(), MV_Vector_::ref);
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
    return 0;
}

