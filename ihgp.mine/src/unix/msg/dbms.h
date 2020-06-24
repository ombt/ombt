/* header for dbms */

typedef struct {
    char name[20];
    char street[20];
    char city[20];
    char state[20];
    char zip[20];
    char tel[20];
} RCD;

typedef struct {
    long mtype;
    long clientkey;
    char cmd;
    char file[20];
    RCD rcd;
    int status;
    int errno;
} MESSAGE;

extern int Dopen();
extern int Dcreate();
extern int Dclose();
extern int Dtop();
extern int Dget();
extern int Dgetnext();
extern int Dput();
extern int Ddelete();

#define NOTOK 0
#define OK 1
#define ERROR 2
#define NOTFOUND 3
#define TRUE 1
#define FALSE 0
