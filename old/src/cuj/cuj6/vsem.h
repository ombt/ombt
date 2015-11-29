#ifndef __VSEM_H
#define __VSEM_H
// a semaphore class based on SYSTEM-V semaphores

// system headers
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <iostream.h>
#include <string.h>
#include <errno.h>

// local headers
#include "returns.h"
#include "debug.h"

// class definitions
class Semaphore {
public:
	// internal enumerations
	enum States {
		Unknown, Initialized, Blocked, Owner, Error
	};

	// union for semaphore control
	union semun {
		int val;
		struct semid_ds *buf;
		ushort *array;
	};

	// constructors and destructor
	Semaphore(key_t, int = 0666, unsigned int = 1);
	Semaphore(const char *, int = 0666, unsigned int = 1);
	Semaphore(const Semaphore &);
	~Semaphore();

	// assignment
	Semaphore &operator=(const Semaphore &);

	// semaphore operations
	int P(int);
	int V();

	// other operations
	void setFileName(const char *);
	const char *getFileName() const;
	States getState() const;
	void cleanup();
	void newFileName(const char *);
	void newKey(key_t);

	// remove lock file after done
	static void removeLockFile(const char *);
	static void removeLockFile(key_t);

protected:
	// internal data
	const char *fname;
	States state;
	key_t key;
	int semid;
	int mode;
	unsigned int value;
	static const int MaxRepetitions;
};

#endif
