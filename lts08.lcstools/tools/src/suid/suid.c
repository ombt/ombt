/* general set-uid tool for a restricted set of commands */

/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>

/* local headers */
#include "mydebug.h"

/* definitions */
#define ALLOWED_CMDS "/home/lcstools/.allowed_cmds"
#define DEBUGFILE "/tmp/suid_debug"
#define SUID_PATH "/home/lcstools/tools/bin/suid"
#if 0
#define ALLOWED_CMDS "/opt/home/lcstools/.allowed_cmds"
#define DEBUGFILE "/tmp/suid_debug"
#define SUID_PATH "/opt/home/lcstools/tools/bin/suid"
#endif

static char *owneryes = "FILEOWNER=yes";
static char *ownerno = "FILEOWNER=no";

/* strip newlines from a record */
void
stripnl(char *ps)
{
	for ( ; ps != NULL && *ps != '\0'; ps++)
	{
		if (*ps != '\n')
			continue;
		*ps = '\0';
		break;
	}
	return;
}

/* allows a user to run as us */
int 
main(int argc, char **argv)
{
	int ok, arg, status;
	FILE *infd;
	struct utsname udata;
	struct stat statbuf;
	struct passwd *pw;
	char *pc, inbuf[BUFSIZ];
	char user[BUFSIZ], mach[BUFSIZ], cmd[BUFSIZ];
	char alwduser[BUFSIZ], alwdmach[BUFSIZ], alwdcmd[BUFSIZ];

	/* open debug file */
	dbgopen(DEBUGFILE);

	/* check if any cmds were given */
	DBGDUMP("%s\n", "starting suid ...");
	if (argc < 2)
	{
		dbgprintf("%s: no cmds given. (errno=%d)\n", 
			argv[0], EINVAL);
		fprintf(stderr, "%s: no cmds given. (errno=%d)\n", 
			argv[0], EINVAL);
		dbgclose();
		exit(2);
	}

	/* dump ids */
	DBGDUMP("uid = %d\n", getuid());
	DBGDUMP("euid = %d\n", geteuid());
	DBGDUMP("gid = %d\n", getgid());
	DBGDUMP("egid = %d\n", getegid());

	/* this tool is used specifically to allow set-uid.
	 * if the setuid has not taken, then we should error
	 * out immediately. verify that the effective uid 
	 * matches the owning uid for the suid tool. 
	 */
	DBGDUMP("%s\n", "checking SETUID worked ...");
	if (stat(SUID_PATH, &statbuf) != 0)
	{
		dbgprintf("%s: stat failed for file %s. (errno=%d)\n", 
			argv[0], SUID_PATH, errno);
		fprintf(stderr, 
			"%s: stat failed for file %s. (errno=%d)\n", 
			argv[0], SUID_PATH, errno);
		exit(2);
	}
	if (statbuf.st_uid != geteuid())
	{
		dbgprintf("%s: SUID != EUID !!!. SUID=%d, EUID=%d\n", 
			argv[0], statbuf.st_uid, geteuid());
		fprintf(stderr, 
			"%s: SET UID FAILED !!!. EUID=%d, SUID=%d\n", 
			argv[0], statbuf.st_uid, geteuid());
		dbgclose();
		exit(2);
	}
	if (statbuf.st_gid != getegid())
	{
		dbgprintf("%s: SGID != EGID !!!. SGID=%d, EGID=%d\n", 
			argv[0], statbuf.st_gid, getegid());
		fprintf(stderr, 
			"%s: SET GID FAILED !!!. EGID=%d, SGID=%d\n", 
			argv[0], statbuf.st_gid, getegid());
		dbgclose();
		exit(2);
	}

	/* set environment variable to indicate owner called a tool */
	DBGDUMP("statbuf.st_uid = %d\n", statbuf.st_uid);
	DBGDUMP("statbuf.st_gid = %d\n", statbuf.st_gid);
	DBGDUMP("getuid = %d\n", getuid());
	DBGDUMP("getgid = %d\n", getgid());
	if ((statbuf.st_uid == getuid()) && (statbuf.st_gid == getgid()))
	{
		DBGDUMP("FILEOWNER ...  %s\n", owneryes);
		(void)putenv(owneryes);
	}
	else
	{
		DBGDUMP("FILEOWNER ...  %s\n", ownerno);
		(void)putenv(ownerno);
	}

	/* get the name of the current machine */
	DBGDUMP("%s\n", "calling uname ...");
	if (uname(&udata) == -1)
	{
		dbgprintf("%s: uname failed. (errno=%d)\n", 
			argv[0], errno);
		fprintf(stderr, "%s: uname failed. (errno=%d)\n", 
			argv[0], errno);
		dbgclose();
		exit(2);
	}
	strcpy(mach, udata.nodename);
	DBGDUMP("machine is %s\n", mach);

	/* get name of current user; user REAL UID, not EFFECTIVE UID */
	DBGDUMP("calling getpwuid with uid=%d\n", getuid());
	if ((pw = getpwuid(getuid())) == NULL)
	{
		dbgprintf("%s: unable to determine user name. (errno=%d)\n", 
			argv[0], errno);
		fprintf(stderr, 
			"%s: unable to determine user name. (errno=%d)\n", 
			argv[0], errno);
		dbgclose();
		exit(2);
	}
	strcpy(user, pw->pw_name);
	DBGDUMP("user is %s\n", user);

	/* open file of allowed commands */
	DBGDUMP("opening file %s for read\n", ALLOWED_CMDS);
	if ((infd = fopen(ALLOWED_CMDS, "r")) == NULL)
	{
		dbgprintf("%s: allows cmds file %s not readable. (errno=%d)\n",
			argv[0], ALLOWED_CMDS, errno);
		fprintf(stderr, 
			"%s: allows cmds file %s not readable. (errno=%d)\n",
			argv[0], ALLOWED_CMDS, errno);
		dbgclose();
		exit(2);
	}
	DBGDUMP("allowed cmds file is %s\n", ALLOWED_CMDS);

	/* scan if cmd is allowed on this machine for this user */
	ok = 0;
	strcpy(cmd, argv[1]);
	while (fgets(inbuf, BUFSIZ-1, infd) != NULL)
	{
		stripnl(inbuf);
		if (*inbuf == '#' || *inbuf == '\0') 
			continue;
		sscanf(inbuf, "%s%s%s", alwdmach, alwduser, alwdcmd);
		if (strcmp(cmd, alwdcmd) == 0 && 
		   (strcmp(alwduser, "*") == 0 || 
		    strcmp(user, alwduser) == 0) &&
		   (strcmp(alwdmach, "*") == 0 || 
		    strcmp(mach, alwdmach) == 0))
		{
			ok = 1;
			break;
		}
	}
	if (!ok)
	{
		dbgprintf("%s: cmd %s not allowed on machine %s for user %s.\n", 
			argv[0], cmd, mach, user);
		fprintf(stderr, 
			"%s: cmd %s not allowed on machine %s for user %s.\n", 
			argv[0], cmd, mach, user);
		dbgclose();
		exit(2);
	}
	DBGDUMP("cmd %s is allowed\n", cmd);
	
	/* make sure euid is set */
	setuid(geteuid());
	setgid(getegid());

	/* put together one big command */
	cmd[0] = '\0';
	for (pc = cmd, arg = 1; arg < argc; arg++, pc++)
	{
		strcpy(pc, argv[arg]);
		pc += strlen(argv[arg]);
		*pc = ' ';
	}
	*pc = '\0';
	
	/* run the command */
	DBGDUMP("excuting cmd %s ...\n", cmd);
	status = system(cmd);
	DBGDUMP("return value is %d ...\n", status);
	status >>= 8;
	DBGDUMP("return value after shift is %d ...\n", status);

	/* close debug file */
	dbgclose();

	/* return status */
	exit(status);
}
