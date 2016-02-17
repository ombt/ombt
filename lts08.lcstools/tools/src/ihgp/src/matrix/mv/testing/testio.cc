
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

#include <iostream.h>
#include "mvvd.h"
#include "mvvi.h"
#include "mvblasd.h"
#include "iotext.h"

const char *tmpname = "testio0001.tmp";

main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage <vector.text> " << endl;
        exit(1);
    }

    char *filename = argv[1];
    MV_Vector_double A, B;

    readtxtfile_vec(filename, &A);

    writetxtfile_vec(tmpname, A);
    readtxtfile_vec(filename, &B);

    if (norm(A-B) / norm(A+B) < 0.0000001 )
        cout << "testio passed. " << endl;
    else
        cout << "testio failed.\n" << endl;

}

