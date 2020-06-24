/* p and v for semaphores */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* map key to id */
int semtran(key)
int key;
{
	int sid;

	if ((sid = semget((key_t)key, 1, (0666|IPC_CREAT))) == -1)
	{
		syserr("semget");
	}
	return(sid);
}

/* execute operation */
static void semcall(sid, op)
int sid;
int op;
{
	struct sembuf sb;

	sb.sem_num = 0;
	sb.sem_op = op;
	sb.sem_flg = 0;
	if (semop(sid, &sb, 1) == -1)
	{
		syserr("semop");
	}
	return;
}

/* acquire semaphore */
void P(sid)
int sid;
{
	semcall(sid, -1);
	return;
}

/* release semaphore */
void V(sid)
int sid;
{
	semcall(sid, 1);
	return;
}

/* initialize semaphores */
void seminit(sid, Howmany)
int sid, Howmany;
{
	union {
		int val;
		struct semid_ds *buf;
		ushort *array;
	} arg;

	arg.val = Howmany; 
	if (semctl(sid, 0, SETVAL, arg) == -1)
	{
		syserr("semctl");
	}
	return;
}
