all:	dbms user

dbms:	dbmsmain.c dbms.c sem.c shmmsgs.c syserr.c dbms.h shmmsgs.h
	cc -g -o dbms dbmsmain.c dbms.c sem.c shmmsgs.c syserr.c

user:	user.c dbmslib.c sem.c shmmsgs.c syserr.c dbms.h shmmsgs.h
	cc -g -o user user.c dbmslib.c sem.c shmmsgs.c syserr.c
