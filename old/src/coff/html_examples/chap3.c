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
#include "aouthdr.h"

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
			fprintf(stderr, 
				"unable to read fhdr, file is %s, errno=%d.\n",
				argv[arg], errno);
			close(fd);
			continue;
		}

		// print data
		fprintf(stdout,"file     : %s\n", argv[arg]);
		fprintf(stdout,"fhdr.f_magic  : 0%o\n", fhdr.f_magic);
		fprintf(stdout,"fhdr.f_timdat : 0x%x\n", fhdr.f_timdat);
		fprintf(stdout,"fhdr.f_symptr : %d\n", fhdr.f_symptr);
		fprintf(stdout,"fhdr.f_nsyms  : %d\n", fhdr.f_nsyms);
		fprintf(stdout,"fhdr.f_opthdr : %d\n", fhdr.f_opthdr);
		fprintf(stdout,"fhdr.f_flags  : 0%o\n", fhdr.f_flags);

		// read in a.out optional header
		aouthdr ahdr;
		if (read(fd, (void *)&ahdr, sizeof(ahdr)) != sizeof(ahdr))
		{
			fprintf(stderr, 
				"unable to read aouthdr, file is %s, errno=%d.\n",
				argv[arg], errno);
			close(fd);
			continue;
		}

		// print data
		fprintf(stdout,"aouthdr.magic       : 0x%x\n", ahdr.magic);
		fprintf(stdout,"aouthdr.vstamp      : 0x%x\n", ahdr.vstamp);
		fprintf(stdout,"aouthdr.tsize       : 0x%lx\n", ahdr.tsize);
		fprintf(stdout,"aouthdr.dsize       : 0x%lx\n", ahdr.dsize);
		fprintf(stdout,"aouthdr.bsize       : 0x%lx\n", ahdr.bsize);
		fprintf(stdout,"aouthdr.entry       : 0x%lx\n", ahdr.entry);
		fprintf(stdout,"aouthdr.text_start  : 0x%lx\n", ahdr.text_start);
		fprintf(stdout,"aouthdr.data_start  : 0x%lx\n", ahdr.data_start);

		// close file
		close(fd);
	}

	// all done
	return(0);
}
