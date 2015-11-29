all:	dbms user

dbms:	dbmsmain.c dbms.c msgmsgs.c syserr.c dbms.h msgmsgs.h
	cc -g -o dbms dbmsmain.c dbms.c msgmsgs.c syserr.c

user:	user.c dbmslib.c msgmsgs.c syserr.c dbms.h msgmsgs.h
	cc -g -o user user.c dbmslib.c msgmsgs.c syserr.c
