// unix headers
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// COFF headers
#include "filehdr.h"

// main entry point
main(int argc, char **argv)
{
	// read each file and print file header for each each
	for (int arg = 1; arg < argc; arg++)
	{
		// open file for read
		int fd = open(argv[arg], O_RDONLY);
		if (fd == -1)
		{
			fprintf(stderr, "unable to open file %s, errno=%d.\n",
				argv[arg], errno);
			continue;
		}

		// read file header
		filehdr fhdr;
		if (read(fd, (void *)&fhdr, sizeof(fhdr)) != sizeof(fhdr))
		{
			fprintf(stderr, "unable to read file %s, errno=%d.\n",
				argv[arg], errno);
			close(fd);
			continue;
		}

		// print data
		fprintf(stdout,"file     : %s\n", argv[arg]);
		fprintf(stdout,"f_magic  : 0%o\n", fhdr.f_magic);
		fprintf(stdout,"f_timdat : 0x%x\n", fhdr.f_timdat);
		fprintf(stdout,"f_symptr : %d\n", fhdr.f_symptr);
		fprintf(stdout,"f_nsyms  : %d\n", fhdr.f_nsyms);
		fprintf(stdout,"f_opthdr : %d\n", fhdr.f_opthdr);
		fprintf(stdout,"f_flags  : 0%o\n", fhdr.f_flags);

		// close file
		close(fd);
	}

	// all done
	return(0);
}
