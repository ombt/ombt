all:	dbms user

dbms:	dbmsmain.c dbms.c fifomsgs.c syserr.c dbms.h fifomsgs.h
	cc -g -o dbms dbmsmain.c dbms.c fifomsgs.c syserr.c

user:	user.c dbmslib.c fifomsgs.c syserr.c dbms.h fifomsgs.h
	cc -g -o user user.c dbmslib.c fifomsgs.c syserr.c
