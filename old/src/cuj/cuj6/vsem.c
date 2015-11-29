// source for SYSTEM-V semaphores

// headers
#include "vsem.h"

// static variable
const int Semaphore::MaxRepetitions = 10;

// constructors and destructor
Semaphore::Semaphore(key_t k, int m, unsigned int v):
	fname(NULL), state(Unknown), key(k), semid(-1), mode(m), value(v)
{
	// allocate file name, etc.
	if (k != -1)
	{
		// set new file name
		newKey(k);

		// open or create semaphore
		semid = semget(key, 1, IPC_CREAT|IPC_EXCL|mode);
		MustBeTrue(semid >= 0 || errno == EEXIST);
		if (semid >= 0)
		{
			union semun arg;
			arg.val = value;
			MustBeTrue(semctl(semid, 0, SETVAL, arg) == OK);
		}
		else if (errno == EEXIST)
		{
			semid = semget(key, 1, mode);
			MustBeTrue(semid >= 0);
			union semun arg;
			semid_ds seminfo;
			arg.buf = &seminfo;
			int i;
			for (i = 0; i < MaxRepetitions; i++)
			{
				MustBeTrue(semctl(semid, 
					0, IPC_STAT, arg) == OK);
				if(arg.buf->sem_otime != 0)
					break;
				sleep(1);
			}
			MustBeTrue(i < MaxRepetitions);
		}
	}

	// sanity checks
	MustBeTrue((state == Unknown && key == -1 && semid == -1) ||
		   (state != Unknown && key != -1 && semid != -1));
}

Semaphore::Semaphore(const char *fnm, int m, unsigned int v):
	fname(NULL), state(Unknown), key(-1), semid(-1), mode(m), value(v)
{
	// allocate file name, etc.
	if (fnm != NULL && *fnm != '\0')
	{
		// set new file name
		newFileName(fnm);

		// open or create semaphore
		semid = semget(key, 1, IPC_CREAT|IPC_EXCL|mode);
		MustBeTrue(semid >= 0 || errno == EEXIST);
		if (semid >= 0)
		{
			union semun arg;
			arg.val = value;
			MustBeTrue(semctl(semid, 0, SETVAL, arg) == OK);
		}
		else if (errno == EEXIST)
		{
			semid = semget(key, 1, mode);
			MustBeTrue(semid >= 0);
			union semun arg;
			semid_ds seminfo;
			arg.buf = &seminfo;
			int i;
			for (i = 0; i < MaxRepetitions; i++)
			{
				MustBeTrue(semctl(semid, 
					0, IPC_STAT, arg) == OK);
				if(arg.buf->sem_otime != 0)
					break;
				sleep(1);
			}
			MustBeTrue(i < MaxRepetitions);
		}
	}

	// sanity checks
	MustBeTrue((state == Unknown && key == -1 && semid == -1) ||
		   (state != Unknown && key != -1 && semid != -1));
}

Semaphore::Semaphore(const Semaphore &src):
	fname(NULL), state(Unknown), key(src.key),
	semid(src.semid), mode(src.mode), value(src.value)
{
	// allocate filename, etc.
	if (src.fname != NULL && *src.fname != '\0')
	{
		// set new file name
		newFileName(src.fname);

		// open or create semaphore
		semid = semget(key, 1, IPC_CREAT|IPC_EXCL|mode);
		MustBeTrue(semid >= 0 || errno == EEXIST);
		if (semid >= 0)
		{
			union semun arg;
			arg.val = value;
			MustBeTrue(semctl(semid, 0, SETVAL, arg) == OK);
		}
		else if (errno == EEXIST)
		{
			semid = semget(key, 1, mode);
			MustBeTrue(semid >= 0);
			union semun arg;
			semid_ds seminfo;
			arg.buf = &seminfo;
			int i;
			for (i = 0; i < MaxRepetitions; i++)
			{
				MustBeTrue(semctl(semid, 
					0, IPC_STAT, arg) == OK);
				if(arg.buf->sem_otime != 0)
					break;
				sleep(1);
			}
			MustBeTrue(i < MaxRepetitions);
		}
	}
	else if (key != -1)
	{
		// set key
		newKey(src.key);

		// open or create semaphore
		semid = semget(key, 1, IPC_CREAT|IPC_EXCL|mode);
		MustBeTrue(semid >= 0 || errno == EEXIST);
		if (semid >= 0)
		{
			union semun arg;
			arg.val = value;
			MustBeTrue(semctl(semid, 0, SETVAL, arg) == OK);
		}
		else if (errno == EEXIST)
		{
			semid = semget(key, 1, mode);
			MustBeTrue(semid >= 0);
			union semun arg;
			semid_ds seminfo;
			arg.buf = &seminfo;
			int i;
			for (i = 0; i < MaxRepetitions; i++)
			{
				MustBeTrue(semctl(semid, 
					0, IPC_STAT, arg) == OK);
				if(arg.buf->sem_otime != 0)
					break;
				sleep(1);
			}
			MustBeTrue(i < MaxRepetitions);
		}
	}

	// sanity checks
	MustBeTrue((state == Unknown && key == -1 && semid == -1) ||
		   (state != Unknown && key != -1 && semid != -1));
}

Semaphore::~Semaphore()
{
	// release semaphore 
	if (state == Owner)
		MustBeTrue(V() == OK);

	// release file name or key
	cleanup();

	// sanity checks
	MustBeTrue(state == Unknown && key == -1 && semid == -1);
}

// assignment operation
Semaphore &
Semaphore::operator=(const Semaphore &rhs)
{
	// sanity checks
	MustBeTrue((state == Unknown && key == -1 && semid == -1) ||
		   (state != Unknown && key != -1 && semid != -1));
	MustBeTrue((rhs.state == Unknown && rhs.key == -1 && rhs.semid == -1) ||
		   (rhs.state != Unknown && rhs.key != -1 && rhs.semid != -1));

	// check for self-assignment
	if (this == &rhs)
		return(*this);

	// check if semaphore is owned, if so, release it.
	if (state == Owner)
		MustBeTrue(V() == OK);

	// release old data
	cleanup();

	// copy data
	if (rhs.fname != NULL && *rhs.fname != '\0')
	{
		// set new file name and copy data
		newFileName(rhs.fname);
		mode = rhs.mode;
		value = rhs.value;

		// open or create semaphore
		semid = semget(key, 1, IPC_CREAT|IPC_EXCL|mode);
		MustBeTrue(semid >= 0 || errno == EEXIST);
		if (semid >= 0)
		{
			union semun arg;
			arg.val = value;
			MustBeTrue(semctl(semid, 0, SETVAL, arg) == OK);
		}
		else if (errno == EEXIST)
		{
			semid = semget(key, 1, mode);
			MustBeTrue(semid >= 0);
			union semun arg;
			semid_ds seminfo;
			arg.buf = &seminfo;
			int i;
			for (i = 0; i < MaxRepetitions; i++)
			{
				MustBeTrue(semctl(semid, 
					0, IPC_STAT, arg) == OK);
				if(arg.buf->sem_otime != 0)
					break;
				sleep(1);
			}
			MustBeTrue(i < MaxRepetitions);
		}
	}
	else if (key != -1)
	{
		// set key and copy data
		newKey(rhs.key);
		mode = rhs.mode;
		value = rhs.value;

		// open or create semaphore
		semid = semget(key, 1, IPC_CREAT|IPC_EXCL|mode);
		MustBeTrue(semid >= 0 || errno == EEXIST);
		if (semid >= 0)
		{
			union semun arg;
			arg.val = value;
			MustBeTrue(semctl(semid, 0, SETVAL, arg) == OK);
		}
		else if (errno == EEXIST)
		{
			semid = semget(key, 1, mode);
			MustBeTrue(semid >= 0);
			union semun arg;
			semid_ds seminfo;
			arg.buf = &seminfo;
			int i;
			for (i = 0; i < MaxRepetitions; i++)
			{
				MustBeTrue(semctl(semid, 
					0, IPC_STAT, arg) == OK);
				if(arg.buf->sem_otime != 0)
					break;
				sleep(1);
			}
			MustBeTrue(i < MaxRepetitions);
		}
	}

	// sanity checks
	MustBeTrue((state == Unknown && key == -1 && semid == -1) ||
		   (state != Unknown && key != -1 && semid != -1));

	// all done
	return(*this);
}

// do nothing signal handler
extern "C" {
static void
signalhandler(int sig)
{
	sig;
	return;
}
}

// semaphore operations
int
Semaphore::P(int maxtime)
{
	// sanity checks
	MustBeTrue((state == Unknown && key == -1 && semid == -1) ||
		   (state != Unknown && key != -1 && semid != -1));

	// check state
	if (state == Unknown)
		return(NOTOK);

	// assume we have an error
	state = Error;

	// depending on maxtime, block or don't block.
	if (maxtime < 0)
	{
		// block waiting for file lock
		sembuf sb;
		sb.sem_num = 0;
		sb.sem_op = -1;
		sb.sem_flg = SEM_UNDO;
		if (semop(semid, &sb, 1) != OK)
			return(NOTOK);
	}
	else if (maxtime > 0)
	{
		// set an alarm for timing out
		SIG_PF oldfn;
		oldfn = signal(SIGALRM, signalhandler);
		alarm(maxtime);

		// block, but with a time out.
		sembuf sb;
		sb.sem_num = 0;
		sb.sem_op = -1;
		sb.sem_flg = SEM_UNDO;
		if (semop(semid, &sb, 1) != OK)
		{
			if (errno == EINTR)
			{
				// time out
				state = Blocked;
				MustBeTrue((state == Unknown && 
					    key == -1 && semid == -1) ||
					   (state != Unknown && 
					    key != -1 && semid != -1));
				return(EAGAIN);
			}
			else 
				return(NOTOK);
		}

		// clear alarm and reset alarm handler
		alarm(0);
		signal(SIGALRM, oldfn);
	}
	else
	{
		// do not block
		sembuf sb;
		sb.sem_num = 0;
		sb.sem_op = -1;
		sb.sem_flg = IPC_NOWAIT|SEM_UNDO;
		if (semop(semid, &sb, 1) != OK)
		{
			if (errno == EAGAIN)
				return(EAGAIN);
			else
				return(NOTOK);
		}
	}

	// we own the semaphore file, just close it for now.
	state = Owner;

	// sanity checks
	MustBeTrue((state == Unknown && key == -1 && semid == -1) ||
		   (state != Unknown && key != -1 && semid != -1));

	// just return
	return(OK);
}

int
Semaphore::V()
{
	// sanity checks
	MustBeTrue((state == Unknown && key == -1 && semid == -1) ||
		   (state != Unknown && key != -1 && semid != -1));

	// check state
	if (state == Unknown)
		return(NOTOK);

	// assume we have an error
	state = Error;

	// release the semaphore
	sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;
	if (semop(semid, &sb, 1) != OK)
		return(NOTOK);

	// reset state to ready-to-be-used
	state = Initialized;

	// sanity checks
	MustBeTrue((state == Unknown && key == -1 && semid == -1) ||
		   (state != Unknown && key != -1 && semid != -1));

	// just return
	return(OK);
}

// other operations
void
Semaphore::setFileName(const char *newfn)
{
	// release any locks
	(void)V();

	// release old data
	cleanup();

	// copy data
	if (newfn != NULL && *newfn != '\0')
		newFileName(newfn);

	// all done
	return;
}

void
Semaphore::newKey(key_t k)
{
	if (fname != NULL)
		delete [] fname;
	fname = NULL;
	key = k;
	MustBeTrue(key != NOTOK);
	state = Initialized;
	return;
}

void
Semaphore::newFileName(const char *nfn)
{
	if (fname != NULL)
		delete [] fname;
	fname = NULL;
	char *tmpfn = new char [strlen(nfn)+1];
	MustBeTrue(tmpfn != NULL);
	strcpy(tmpfn, nfn);
	fname = tmpfn;
	key = ftok(fname, 1);
	if (key == NOTOK)
		ERRORD("ftok() failed.", fname, errno);
	MustBeTrue(key != NOTOK);
	state = Initialized;
	return;
}

const char *
Semaphore::getFileName() const
{
	return(fname);
}

Semaphore::States
Semaphore::getState() const
{
	return(state);
}

void
Semaphore::cleanup()
{
	if (fname != NULL)
		delete [] fname;
	fname = NULL;
	state = Unknown;
	semid = -1;
	key = -1;
	return;
}

// remove lock file
void
Semaphore::removeLockFile(const char *lfnm)
{
	if (lfnm == NULL || *lfnm == '\0')
		return;
	key_t key = ftok(lfnm, 1);
	if (key == NOTOK) 
		return;
	int semid = semget(key, 1, 0666);
	if (semid == NOTOK) 
		return;
	semctl(semid, 0, IPC_RMID);
	return;
}

void
Semaphore::removeLockFile(key_t key)
{
	if (key == -1)
		return;
	int semid = semget(key, 1, 0666);
	if (semid == NOTOK) 
		return;
	semctl(semid, 0, IPC_RMID);
	return;
}
