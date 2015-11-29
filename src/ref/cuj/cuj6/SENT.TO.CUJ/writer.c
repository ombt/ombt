// test case for shared-memory/semaphore code.

// headers
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
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
	pid_t pid = -1;
	key_t shmkey = -1;
	key_t rdkey = -1;
	key_t wrkey = -1;
	int shmid = -1;
	int asize = 0;

	// check number of arguments
	if (argc != 5)
	{
		cout << "usage: writer shmkey readkey writekey arraysize" << endl;
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
		MustBeTrue(shm.isOwner())

		// get semaphore
		Semaphore writer(wrkey, 0666, wr_no);
		Semaphore reader(rdkey, 0666, rd_no);

		// fork/exec the reader process
		MustBeTrue((pid = fork()) != (pid_t)NOTOK);
		if (pid == 0)
		{
			// execute the child process
			try {
			MustBeTrue(execl("reader", "reader", argv[1], 
				argv[2], argv[3], argv[4], NULL) != NOTOK);
			}
			catch (const char *pe)
			{
				ERROR(pe, errno);
			}
			return(2);
		}

		// gain ownership
		MustBeTrue(writer.P(-1) == OK);

		// get length of initial msg
		int ilen = strlen(shm[0].buf);
		ilen--;

		// let reader proceed
		MustBeTrue(reader.V() == OK);

		for (char c = 'a'; c <= 'z'; c++)
		{
			// gain ownership
			MustBeTrue(writer.P(-1) == OK);

			// write to shared memory
			for (int ia = 0; ia < asize; ia++)
			{
				shm[ia].buf[ilen] = c;
				shm[ia].buf[ilen+1] = '\0';
				cout << "WRITER: writing into buffer [";
				cout << ia << "] ... " << shm[ia].buf << endl;
			}
			ilen++;

			// release ownership
			MustBeTrue(reader.V() == OK);
		}

		// get permission to write 
		MustBeTrue(writer.P(-1) == OK);

		// write to shared memory
		strcpy(shm->buf, "all done");
		cout << "WRITER: writing ... " << shm->buf << endl;

		// let reader proceed
		MustBeTrue(reader.V() == OK);

		// gain ownership one last time
		MustBeTrue(writer.P(-1) == OK);
	}
	catch (const char *pe)
	{
		ERROR(pe, errno);
		return(2);
	}

	// wait for child to finish
	int status;
	MustBeTrue(wait(&status) == pid);

	// delete resources
	SharedMemory<Dummy>::removeSharedMemory(shmid);

	// all done
	return(0);
}

