
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

#include <stdio.h>
#include <stdlib.h>

#include "vecdefs.h"
#include VECTOR_H

int TestVec_double(int N, int start, int end);
int TestVec_int(int N, int start, int end);
int TestVec_float(int N, int start, int end);

int TestBlas1_double(int N);
int TestBlas1_float(int N);

int TestMat_double(int M, int N, int Istart, int Jstart, 
                    int Iend, int Jend);
int TestMat_int(int M, int N, int Istart, int Jstart, 
                    int Iend, int Jend);
int TestMat_float(int M, int N, int Istart, int Jstart, 
                    int Iend, int Jend);


int TestBlas1_COMPLEX(int N);
int TestVec_COMPLEX(int N, int start, int end);
int TestMat_COMPLEX(int M, int N, int Istart, int Jstart, 
                    int Iend, int Jend);

int main(int argc, char *argv[])
{
    int M;
    int N;
    int Istart,  Iend;
    int Jstart, Jend;

    if (argc < 7)
    {
        printf("Usage: M, N, Istart, Jstart, Iend, Jend\n");
        exit(1);
    }

    M = atoi(argv[1]);
    N = atoi(argv[2]);
    Istart = atoi(argv[3]);
    Jstart = atoi(argv[4]);
    Iend = atoi(argv[5]);
    Jend = atoi(argv[6]);

    TestVec_double(N, Istart, Iend);
    TestVec_float(N, Istart, Iend);
    TestVec_int(N, Istart, Iend);

    TestMat_double(M, N, Istart, Jstart, Iend, Jend);
    TestMat_float(M, N, Istart, Jstart, Iend, Jend);
    TestMat_int(M, N, Istart, Jstart, Iend, Jend);

    TestBlas1_double(N);
    TestBlas1_float(N);

    TestVec_COMPLEX(N, Istart, Iend);
    TestMat_COMPLEX(M, N, Istart, Jstart, Iend, Jend);
    TestBlas1_COMPLEX(N);

    
    cout << endl << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "+       Successful completion of testing for MV++       +" << endl;
    cout << "+   No errors detected in conversion or blas routines.  +" << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << endl << endl;

    return 0;
}
