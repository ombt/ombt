#ifndef __VSHM5_H
#define __VSHM5_H
// header for reference-counted SYSTEM-V shared memory class. this class
// requires an aggregate data type containing a default constructor and 
// destructor.

// unix headers
#include <stdlib.h>
#include <iostream.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

// local headers
#include "debug.h"
#include "returns.h"

// forward declarations
template <class DataType> class SharedMemory;
template <class DataType> class SharedMemoryData;

// class for shared memory data, reference-counted
template <class DataType> class SharedMemoryData {
	// friend class
	friend class SharedMemory<DataType>;

private:
	// constructors and destructor
	SharedMemoryData(key_t k, int ne, int m, void *pa):
		owner(0), shmid(-1), key(k), size(ne*sizeof(DataType)), 
		mode(m), nelem(ne), pamem(pa), pmem(NULL), pdmem(NULL),
		counter(0)
	{
		if (key > 0)
		{
			// size and number of elements must be greater than 0
			MustBeTrue(nelem > 0 && size > 0);

			// get shared memory and create it, if necessary.
			// must keep track of whether or not the data
			// already existed or not. if the data did not
			// exist, then we need to initialize it. also,
			// if we init'ed the data, we probably want to 
			// delete it by calling the destructor. hence,
			// we use the owner flag to track all this.
			//
			shmid = shmget(key, size, IPC_CREAT|IPC_EXCL|mode);
			if (shmid < 0)
			{
				MustBeTrue(errno == EEXIST);
				shmid = shmget(key, size, IPC_CREAT|mode);
				MustBeTrue(shmid >= 0);
				owner = 0;
			}
			else
			{
				// data was created, must initialize it.
				owner = 1;
			}

			// attach memory to process space
			if (pamem == (void *)-1)
				pmem = shmat(shmid, NULL, 0);
			else
				pmem = shmat(shmid, pamem, 0);
			MustBeTrue(pmem != (void *)NOTOK);

			// now allocate one more time to call constructor. each
			// item is allocated individually since if an array is 
			// allocated, extra bytes are added by the compiler 
			// to save information specific to the array. For 
			// example, if the address 0xf000 is passed to the 
			// new (placement) operator, and an array is requested,
			// then the address returned by new is 0xf008, not 
			// the expected 0xf000. the extra bytes are for 
			// storing array size, etc. (presumably).
			//
			for (int i = 0; i < nelem && owner; i++)
			{
				unsigned long p = (long)pmem+i*sizeof(DataType);
				pdmem = new ((void *)p) DataType;
				MustBeTrue(pdmem != (DataType *)NULL);
				MustBeTrue((void *)p == (void *)pdmem);
			}
			pdmem = (DataType *)pmem;
		}
	}
	~SharedMemoryData()
	{
		// the destructor is called explicitly
		// since we cannot call delete for memory
		// which is not from the heap (shared memory).
		// the data is deleted if it was initialized
		// by this object, i.e., owner is true.
		//
		for (int i = 0; i < nelem && owner; i++)
		{
			pdmem->DataType::~DataType();
		}
		(void) shmdt((char *)pmem);
		pmem = NULL;
		pdmem = NULL;
		shmid = -1;
		counter = 0;
	}

	// counter operations
	inline int increment()
	{
		return(++counter);
	}
	inline int decrement()
	{
		return(--counter);
	}
	inline int getCounter() const
	{
		return(counter);
	}
	inline int setCounter(int c)
	{
		counter = c;
		return(counter);
	}

	// data for shared memory segment
	int owner;
	int shmid;
	key_t key;
	size_t size;
	int mode;
	int nelem;
	void *pamem;
	void *pmem;
	DataType *pdmem;

	// reference counter 
	int counter;

};

// class for shared memory
template <class DataType> class SharedMemory {
public:
	// constructors and destructor
	SharedMemory():
		prep(new SharedMemoryData<DataType>(-1, 0, 0, NULL)) {
		MustBeTrue(prep != NULL);
		prep->increment();
	}
	SharedMemory(key_t k, int ne = 1, int m = 0666, void *pa = NULL):
		prep(new SharedMemoryData<DataType>(k, ne, m, pa)) {
		MustBeTrue(prep != NULL);
		prep->increment();
	}
	SharedMemory(const SharedMemory &p):
		prep(p.prep) {
		MustBeTrue(prep != NULL);
		prep->increment();
	}
	~SharedMemory() {
		if (prep->decrement() <= 0)
			delete prep;
		prep = NULL;
	}

	// assignment
	SharedMemory &operator=(const SharedMemory &rhs) {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		rhs.prep->increment();
		if (prep->decrement() <= 0)
		{
			delete prep;
			prep = NULL;
		}
		prep = rhs.prep;
		MustBeTrue(prep != NULL);
		return(*this);
	}

	// access operators
	inline operator DataType *() const {
		MustBeTrue(prep != NULL);
		return(prep->pdmem);
	}
	inline DataType *operator->() const {
		MustBeTrue(prep != NULL && prep->pdmem != NULL);
		return(prep->pdmem);
	}
	inline DataType &operator*() {
		MustBeTrue(prep != NULL && prep->pdmem != NULL);
		return(*prep->pdmem);
	}
	inline DataType &operator*() const {
		MustBeTrue(prep != NULL && prep->pdmem != NULL);
		return(*prep->pdmem);
	}
	inline DataType &operator[](int i) {
		MustBeTrue(prep != NULL && prep->pdmem != NULL);
		MustBeTrue(0 <= i && i < prep->nelem);
		return(prep->pdmem[i]);
	}
	inline DataType &operator[](int i) const {
		MustBeTrue(prep != NULL && prep->pdmem != NULL);
		MustBeTrue(0 <= i && i < prep->nelem);
		return(prep->pdmem[i]);
	}
	inline int getShmId() const
	{
		MustBeTrue(prep != NULL);
		return(prep->shmid);
	}

	// is the current object the owner?
	inline int isOwner() const {
		MustBeTrue(prep != NULL);
		return(prep->owner);
	}

	// delete shared memory segment
	static void removeSharedMemory(int shmid)
	{
		if (shmid >= 0)
			(void) shmctl(shmid, IPC_RMID, 0);
		return;
	}

private:
	// internal data
	SharedMemoryData<DataType> *prep;
};

#endif

