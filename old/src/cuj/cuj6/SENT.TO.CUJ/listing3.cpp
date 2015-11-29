------------- Listing 3: Shared Memory Reader Example ------------------------

// test case for shared-memory/semaphore code.

// headers
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include "vsem.h"
#include "vshm5.h"

// dummy class
struct Dummy {
    Dummy() {
        strcpy(buf, "I'm initialized !!!  ");
    }
    ~Dummy() {
    }
    char buf[BUFSIZ];
};

// main entry into test case
int
main(int argc, char **argv)
{
    key_t shmkey = -1;
    key_t rdkey = -1;
    key_t wrkey = -1;
    int shmid = -1;
    int asize = 0;

    // check number of arguments
    if (argc != 5)
    {
        cout << "usage: reader shmkey readkey writekey arraysize" << endl;
        return(2);
    }

    // try/catch block for everything
    try {
        // get keys
        shmkey = atoi(argv[1]);
        rdkey = atoi(argv[2]);
        wrkey = atoi(argv[3]);
        asize = atoi(argv[4]);

        // number of readers and writers
        int rd_no = 0;
        int wr_no = 1;

        // attach to shared memory
        SharedMemory<Dummy> shm(shmkey, asize);
        shmid = shm.getShmId();

        // check if we own the shared memory
        MustBeTrue(!shm.isOwner())

        // get semaphore
        Semaphore writer(wrkey, 0666, wr_no);
        Semaphore reader(rdkey, 0666, rd_no);

        // reader process
        for (int done=0; !done; )
        {
            // get permission to read
            MustBeTrue(reader.P(-1) == OK);

            // read shared memory
            for (int ia = 0; ia < asize; ia++)
            {
                cout << "READER: reading buffer [";
                cout << ia << "] ... " << shm[ia].buf << endl;
            }

            // are we done
            if (strcmp(shm->buf, "all done") == 0)
                done = 1;

            // let writer write
            MustBeTrue(writer.V() == OK);

            // relax, take a break.
            sleep(1);
        }
    }
    catch (const char *pe)
    {
        ERROR(pe, errno);
        return(2);
    }

    // delete resources
    Semaphore::removeLockFile(rdkey);
    Semaphore::removeLockFile(wrkey);

    // all done
    return(0);
}

