/* use msgs for communications */
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msgmsgs.h"

/* return queue id, and create queue */
int openqueue(key)
long key;
{
	static struct {
		long key;
		int qid;
	} queues[MAXOPEN] = { 0 };
	int i, avail, qid;


	avail = -1;
	for (i = 0; i < MAXOPEN; i++)
	{
		if (queues[i].key == key) return(queues[i].qid);
		if (queues[i].key == 0 && avail == -1) avail = i;
	}
	if (avail == -1)
	{
		errno = 0;
		return(-1);
	}
	if ((qid = msgget(key, (0666|IPC_CREAT))) == -1) return(-1);
	queues[avail].key = key;
	queues[avail].qid = qid;
	return(qid);
}

/* send a msg */
int send(dstkey, buf, nbytes)
long dstkey;
struct msgbuf *buf;
int nbytes;
{
	int qid;

	if((qid = openqueue(dstkey)) == -1) return(-1);
	buf->mtype = 1;
	return(msgsnd(qid, buf, nbytes - sizeof(buf->mtype), 0));
}

/* receive a msg */
int receive(srckey, buf, nbytes)
long srckey;
struct msgbuf *buf;
int nbytes;
{
	int qid;

	if((qid = openqueue(srckey)) == -1) return(-1);
	return(msgrcv(qid, buf, nbytes - sizeof(buf->mtype), 0L, 0));
}

/* remove a queue */
void rmqueue(key)
long key;
{
	int qid;

	if ((qid = openqueue(key)) == -1 ||
	    msgctl(qid, IPC_RMID,NULL) == -1) syserr("rmqueue");
	return;
}
