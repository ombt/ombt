#include <stdio.h>
#ifdef CC
#include <sys/types.h>
#include <sys/param.h>
#include <sys/signal.h>
#include <sys/pcb.h>
#include <sys/types.h>
#include <sys/user.h>
#else
#include <sys/core.h>
#endif
#include <fcntl.h>
#include <errno.h>

#ifdef CC
#define ISCOREOK(corehdr) ((corehdr.u_exdata.ux_mag & 0x10b) != 0x10b)
#else
#define ISCOREOK(corehdr) ((corehdr.c_magic & CORE_MAGIC) != CORE_MAGIC)
#endif

main(argc, argv)
int argc;
char **argv;
{
	int arg, fd;
#ifdef CC
	struct user prochdr;
#else
	struct core prochdr;
#endif

	if (argc < 2)
	{
		fprintf(stderr, "usage: %s corefile ...\n", argv[0]);
		exit(1);
	}
	for (arg = 1; arg < argc; arg++)
	{
		if ((fd = open(argv[arg], O_RDONLY, 0666)) < 0)
		{
			fprintf(stderr, "open(%s) failed with errno = %d.\n", 
				argv[arg], errno);
			exit(1);
		}
		if (read(fd, &prochdr, sizeof(prochdr)) < 0)
		{
			fprintf(stderr, "read(%s) failed with errno = %d.\n", 
				argv[arg], errno);
			close(fd);
			exit(1);
		}
		close(fd);
		if (ISCOREOK(prochdr))
		{
			fprintf(stderr, "%s is not a core file.\n", argv[arg]);
			exit(1);
		}
		fprintf(stdout, "\ncore file   : %s\n", argv[arg]);
#ifdef CC
		fprintf(stdout, "process name: %s\n", prochdr.u_comm);
		fprintf(stdout, "process args: %s\n", prochdr.u_psargs);
#else
		fprintf(stdout, "process name: %s\n", prochdr.c_cmdname);
#endif
	}
	exit(0);
}
