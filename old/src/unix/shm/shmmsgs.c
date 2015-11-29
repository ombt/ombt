/* semaphore/shared memory db */
#include <errno.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmmsgs.h"

/* find msg info */
int findinfo(key, sndsidp, rcvsidp, addrp, segidp)
int key;
int *sndsidp, *rcvsidp, *segidp;
char **addrp;
{
	static struct {
		int key;
		int sndsid;
		int rcvsid;
		char *addr;
		int segid;
	} sems[MAXOPEN];
	int i, avail;

	/* find info for existing key, if any */
	avail = -1;
	for (i = 0; i < MAXOPEN; i++)
	{
		if (sems[i].key == key)
		{
			/* found info */
			*sndsidp = sems[i].sndsid;
			*rcvsidp = sems[i].rcvsid;
			*segidp = sems[i].segid;
			*addrp = sems[i].addr;
			return(0);
		}
		if (sems[i].key == 0 && avail == -1)
			avail = i;
	}

	/* check if any available */
	if (avail == -1)
	{
		errno = 0;
		return(-1);
	}

	/* save new key */
	sems[avail].key = key;

	/* semaphore for sending info */
	sems[avail].sndsid = semtran(key);
	*sndsidp = sems[avail].sndsid;

	/* initialize to 1 so that other processes can send */
	seminit(*sndsidp, 1);

	/* semaphore for receiving info */
	sems[avail].rcvsid = semtran(key+10000);
	*rcvsidp = sems[avail].rcvsid;

	/* shared memory for msgs */
	sems[avail].segid = shmget((key_t)key, MAXMSG, (0666|IPC_CREAT));
	*segidp = sems[avail].segid;
	if (*segidp == -1)
		return(-1);

	/* attach shared memory for msgs */
	sems[avail].addr = shmat(*segidp, 0, 0);
	*addrp = sems[avail].addr;
	if (*addrp == (char *)-1)
		return(-1);

	/* all done */
	return(0);
}

/* send a message */
int send(dstkey, buf, nbytes)
int dstkey;
char *buf;
int nbytes;
{
	int sndsid, rcvsid, segid;
	char *addr;

	/* find keys to semaphores and shared memory */
	if (findinfo(dstkey, &sndsid, &rcvsid, &addr, &segid) != 0)
		return(-1);

	/* check msg size */
	if (nbytes > MAXMSG)
		nbytes = MAXMSG;

	/* try to gain access to shared memory */
	P(sndsid);

	/* copy msg into shared memory */
	memcpy(addr, buf, nbytes);

	/* release shared memory */
	V(rcvsid);
	return(0);
}


/* receive a message */
int receive(srckey, buf, nbytes)
int srckey;
char *buf;
int nbytes;
{
	int sndsid, rcvsid, segid;
	char *addr;

	/* find keys to semaphores and shared memory */
	if (findinfo(srckey, &sndsid, &rcvsid, &addr, &segid) != 0) 
		return(-1);

	/* check msg size */
	if (nbytes > MAXMSG) 
		nbytes = MAXMSG;

	/* try to gain access to shared memory */
	P(rcvsid);

	/* copy msg from shared memory */
	memcpy(buf, addr, nbytes);

	/* release shared memory */
	V(sndsid);
	return(0);
}

/* remove everything */
void rmqueue(key)
int key;
{
	int sndsid, rcvsid, segid;
	char *addr;

	/* find keys */
	if (findinfo(key, &sndsid, &rcvsid, &addr, &segid) != 0)
		syserr("findinfo");

	/* remove semaphores */
	(void) semctl(sndsid, 0, IPC_RMID, 0);
	(void) semctl(rcvsid, 0, IPC_RMID, 0);

	/* detach shared memory and delete */
	(void) shmdt(addr);
	(void) shmctl(segid, IPC_RMID, 0);
	return;
}
