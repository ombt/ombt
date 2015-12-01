// unix headers
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

// COFF headers
#include "filehdr.h"
#include "aouthdr.h"
#include "scnhdr.h"

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
		fprintf(stdout,"fhdr.f_nscns  : 0%o\n", fhdr.f_nscns);
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
		fprintf(stdout,"aouthdr.magic      : 0x%x\n", ahdr.magic);
		fprintf(stdout,"aouthdr.vstamp     : 0x%x\n", ahdr.vstamp);
		fprintf(stdout,"aouthdr.tsize      : 0x%lx\n", ahdr.tsize);
		fprintf(stdout,"aouthdr.dsize      : 0x%lx\n", ahdr.dsize);
		fprintf(stdout,"aouthdr.bsize      : 0x%lx\n", ahdr.bsize);
		fprintf(stdout,"aouthdr.entry      : 0x%lx\n", ahdr.entry);
		fprintf(stdout,"aouthdr.text_start : 0x%lx\n", 
			ahdr.text_start);
		fprintf(stdout,"aouthdr.data_start : 0x%lx\n", 
			ahdr.data_start);

		// print section header information; seek to start
		// of section headers.
		if (lseek(fd, sizeof(fhdr)+fhdr.f_opthdr, SEEK_SET) == -1)
		{
			fprintf(stderr, 
				"unable to lseek to section headers, file is %s, errno=%d.\n",
				argv[arg], errno);
			close(fd);
			continue;
		}

		// read and print section header data
		for (int isec = 1; isec <= fhdr.f_nscns; isec++)
		{
			// read in a section header
			scnhdr shdr;
			if (read(fd, (void *)&shdr, sizeof(shdr)) != sizeof(shdr))
			{
				fprintf(stderr, 
				"unable to read section header[%d], file is %s, errno=%d.\n",
					isec, argv[arg], errno);
				break;
			}

			// copy section name
			char cbuf[10];
			strncpy(cbuf, shdr.s_name, 8);
			cbuf[8] = 0;

			// print section header data
			fprintf(stdout,"shdr[%d].s_name    : %s\n", 
				isec, cbuf);
			fprintf(stdout,"shdr[%d].s_paddr   : 0x%lx\n", 
				isec, shdr.s_paddr);
			fprintf(stdout,"shdr[%d].s_vaddr   : 0x%lx\n", 
				isec, shdr.s_vaddr);
			fprintf(stdout,"shdr[%d].s_size    : 0x%lx\n", 
				isec, shdr.s_size);
			fprintf(stdout,"shdr[%d].s_scnptr  : 0x%lx\n", 
				isec, shdr.s_scnptr);
			fprintf(stdout,"shdr[%d].s_relptr  : 0x%lx\n", 
				isec, shdr.s_relptr);
			fprintf(stdout,"shdr[%d].s_lnnoptr : 0x%lx\n", 
				isec, shdr.s_lnnoptr);
			fprintf(stdout,"shdr[%d].s_nreloc  : %d\n", 
				isec, shdr.s_nreloc);
			fprintf(stdout,"shdr[%d].s_nlnno   : %d\n", 
				isec, shdr.s_nlnno);
			fprintf(stdout,"shdr[%d].s_flags   : 0x%lx\n", 
				isec, shdr.s_flags);
		}

		// close file
		close(fd);
	}

	// all done
	return(0);
}
