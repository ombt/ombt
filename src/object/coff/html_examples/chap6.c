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
#include "reloc.h"
#include "linenum.h"

// other local headers
#include "debug.h"
#include "returns.h"

// globals
char *currentfile = NULL;

// read file header 
int
readfhdr(int fd, filehdr &fhdr)
{
	// read in file header
	if (read(fd, (void *)&fhdr, FILHSZ) != FILHSZ)
	{
		fprintf(stderr, 
			"unable to read fhdr, file is %s, errno=%d.\n",
			currentfile, errno);
		return(NOTOK);
	}
	else
		return(OK);
}

// print file header data
void
printfhdr(filehdr &fhdr)
{
	fprintf(stdout,"file     : %s\n", currentfile);
	fprintf(stdout,"fhdr.f_magic  : 0%o\n", fhdr.f_magic);
	fprintf(stdout,"fhdr.f_nscns  : 0%o\n", fhdr.f_nscns);
	fprintf(stdout,"fhdr.f_timdat : 0x%x\n", fhdr.f_timdat);
	fprintf(stdout,"fhdr.f_symptr : %d\n", fhdr.f_symptr);
	fprintf(stdout,"fhdr.f_nsyms  : %d\n", fhdr.f_nsyms);
	fprintf(stdout,"fhdr.f_opthdr : %d\n", fhdr.f_opthdr);
	fprintf(stdout,"fhdr.f_flags  : 0%o\n", fhdr.f_flags);
	return;
}

// read a.out header 
int
readaouthdr(int fd, aouthdr &ahdr)
{
	// read in a.out optional header
	if (read(fd, (void *)&ahdr, sizeof(ahdr)) != sizeof(ahdr))
	{
		fprintf(stderr, 
			"unable to read aouthdr, file is %s, errno=%d.\n",
			currentfile, errno);
		return(NOTOK);
	}
	else
		return(OK);
}

// print a.out header data
void
printaouthdr(aouthdr &ahdr)
{
	// print data
	fprintf(stdout,"aouthdr.magic      : 0x%x\n", ahdr.magic);
	fprintf(stdout,"aouthdr.vstamp     : 0x%x\n", ahdr.vstamp);
	fprintf(stdout,"aouthdr.tsize      : 0x%lx\n", ahdr.tsize);
	fprintf(stdout,"aouthdr.dsize      : 0x%lx\n", ahdr.dsize);
	fprintf(stdout,"aouthdr.bsize      : 0x%lx\n", ahdr.bsize);
	fprintf(stdout,"aouthdr.entry      : 0x%lx\n", ahdr.entry);
	fprintf(stdout,"aouthdr.text_start : 0x%lx\n", ahdr.text_start);
	fprintf(stdout,"aouthdr.data_start : 0x%lx\n", ahdr.data_start);
	return;
}

// seek to a section 
int
lseek2sec(int fd, filehdr &fhdr, int isec)
{
	// offset to section from start of file
	long offset = FILHSZ + fhdr.f_opthdr + (isec-1)*SCNHSZ;

	// seek from start of file
	if (lseek(fd, offset, SEEK_SET) == -1)
	{
		fprintf(stderr, 
		"unable to lseek to section header %d, file is %s, errno=%d.\n",
			isec, currentfile, errno);
		return(NOTOK);
	}
	else
		return(OK);
}

// read in a section header
int
readscnhdr(int fd, int isec, scnhdr &shdr)
{
	if (read(fd, (void *)&shdr, SCNHSZ) != SCNHSZ)
	{
		fprintf(stderr, 
		"unable to read section header[%d], file is %s, errno=%d.\n",
			isec, currentfile, errno);
		return(NOTOK);
	}
	else
		return(OK);
}

// print section header data
void
printscnhdr(int isec, scnhdr &shdr)
{
	// copy section name
	char cbuf[10];
	strncpy(cbuf, shdr.s_name, 8);
	cbuf[8] = 0;

	// print data
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
	return;
}

// seek to a relocation entry
int
lseek2reloc(int fd, int isec, scnhdr &shdr, int ir)
{
	// offset to reloction entry from start of file
	long offset = shdr.s_relptr + (ir-1)*RELSZ;

	// seek from start of file
	int status;
	if ((status = lseek(fd, offset, SEEK_SET)) == -1)
	{
		fprintf(stderr, 
		"unable to lseek to sec[%d], reloc[%d], file[%s], errno=%d.\n",
			isec, ir, currentfile, errno);
		return(NOTOK);
	}
	else
		return(OK);
}

// read in relocation entry
int
readreloc(int fd, int isec, int ir, reloc &rhdr)
{
	if (read(fd, (void *)&rhdr, RELSZ) != RELSZ)
	{
		fprintf(stderr, 
		"unable to read sec[%d]. reloc[%d], file[%s], errno=%d.\n",
			isec, ir, currentfile, errno);
		return(NOTOK);
	}
	else
		return(OK);
}

// print relocation data
void
printreloc(int ir, reloc &rhdr)
{
	// print data
	fprintf(stdout,"reloc[%d].r_vaddr  : 0x%lx\n", ir, rhdr.r_vaddr);
	fprintf(stdout,"reloc[%d].r_symndx : %ld\n", ir, rhdr.r_symndx);
	fprintf(stdout,"reloc[%d].r_type   : 0x%x\n", ir, rhdr.r_type);
	return;
}

// read and print relocation data for a section
int
printreloc(int fd, int isec, scnhdr &shdr)
{
	// check if relocation data
	if (shdr.s_nreloc <= 0)
	{
		fprintf(stdout, 
			"NO RELOCATION DATA FOR SECTION %d.\n", isec);
		return(OK);
	}

	// print relocation info
	for (int ir = 1; ir <= shdr.s_nreloc; ir++)
	{
		// seek to relocation entry
		if (lseek2reloc(fd, isec, shdr, ir) != OK)
			return(NOTOK);

		// read in relocation data
		reloc rhdr;
		if (readreloc(fd, isec, ir, rhdr) != OK)
			return(NOTOK);

		// print data
		printreloc(ir, rhdr);
	}
	return(OK);
}

// seek to a line number entry
int
lseek2lnno(int fd, int isec, scnhdr &shdr, int il)
{
	// offset to line number entry from start of file
	long offset = shdr.s_lnnoptr + (il-1)*LINESZ;

	// seek from start of file
	int status;
	if ((status = lseek(fd, offset, SEEK_SET)) == -1)
	{
		fprintf(stderr, 
		"unable to lseek to sec[%d], lnno[%d], file[%s], errno=%d.\n",
			isec, il, currentfile, errno);
		return(NOTOK);
	}
	else
		return(OK);
}

// read in line number entry
int
readlnno(int fd, int isec, int il, lineno &lnno)
{
	if (read(fd, (void *)&lnno, LINESZ) != LINESZ)
	{
		fprintf(stderr, 
		"unable to read sec[%d], lnno[%d], file[%s], errno=%d.\n",
			isec, il, currentfile, errno);
		return(NOTOK);
	}
	else
		return(OK);
}

// print line number data
void
printlnno(int il, lineno &lnno)
{
	// print data
	if (lnno.l_lnno == 0)
	{
		fprintf(stdout,"lnno[%d].l_symndx : %ld\n", 
			il, lnno.l_addr.l_symndx);
	}
	else
	{
		fprintf(stdout,"lnno[%d].l_paddr  : 0x%lx\n", 
			il, lnno.l_addr.l_paddr);
	}
	fprintf(stdout,"lnno[%d].l_lnno   : %d\n", il, lnno.l_lnno);
	return;
}

// read and print line number data for a section
int
printlnno(int fd, int isec, scnhdr &shdr)
{
	// check if relocation data
	if (shdr.s_nlnno <= 0)
	{
		fprintf(stdout, 
			"NO LINE NUMBER DATA FOR SECTION %d.\n", isec);
		return(OK);
	}

	// print relocation info
	for (int il = 1; il <= shdr.s_nlnno; il++)
	{
		// seek to line number data
		if (lseek2lnno(fd, isec, shdr, il) != OK)
			return(NOTOK);

		// read in line number data
		lineno lnno;
		if (readlnno(fd, isec, il, lnno) != OK)
			return(NOTOK);

		// print data
		printlnno(il, lnno);
	}
	return(OK);
}

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
		currentfile = argv[arg];

		// read file header
		filehdr fhdr;
		if (readfhdr(fd, fhdr) != OK)
		{
			close(fd);
			return(2);
		}

		// print data
		printfhdr(fhdr);

		// read in a.out optional header
		aouthdr ahdr;
		if (readaouthdr(fd, ahdr) != OK)
		{
			close(fd);
			return(2);
		}

		// print data
		printaouthdr(ahdr);

		// read and print section header data
		for (int isec = 1; isec <= fhdr.f_nscns; isec++)
		{
			// seek to a section headers
			if (lseek2sec(fd, fhdr, isec) != OK)
			{
				close(fd);
				return(2);
			}

			// read in a section header
			scnhdr shdr;
			if (readscnhdr(fd, isec, shdr) != OK)
			{
				close(fd);
				return(2);
			}

			// print section header data
			printscnhdr(isec, shdr);

			// read and print relocation information for this
			// section
			if (printreloc(fd, isec, shdr) != OK)
			{
				close(fd);
				return(2);
			}

			// read and print line number information for this
			// section
			if (printlnno(fd, isec, shdr) != OK)
			{
				close(fd);
				return(2);
			}
		}

		// close file
		close(fd);
	}

	// all done
	return(0);
}

