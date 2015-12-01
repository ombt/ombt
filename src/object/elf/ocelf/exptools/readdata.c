// read in elf file data

// headers
#include "ocelf.h"

// read in file header
void
readehdr(int fd, Elf32_Ehdr *&pehdr)
{
	// check if a buffer is allocated
	if (pehdr != NULL)
	{
		delete pehdr;
		pehdr = NULL;
	}
	pehdr = new Elf32_Ehdr;
	MustBeTrue(pehdr != NULL);

	// read in file header 
	MustBeTrue(lseek(fd, 0, SEEK_SET) != NOTOK);
	MustBeTrue(read(fd, pehdr, sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));

	// should data be swabbed?
	if (isSwabable(pehdr)) swabehdr(pehdr);
	return;
}

// read in section headers
void
readshdr(int fd, Elf32_Ehdr *pehdr, Elf32_Shdr *&pshdr, char *&pshstrtbl)
{
	// allocate a buffer for reading in section header table
	if (pshdr != NULL)
	{
		delete [] pshdr;
		pshdr = NULL;
	}
	int numbytes = pehdr->e_shentsize*pehdr->e_shnum;
	MustBeTrue(numbytes > 0);
	pshdr = (Elf32_Shdr *) new char [numbytes];
	MustBeTrue(pshdr != NULL);

	// read in section header tables
	MustBeTrue(lseek(fd, pehdr->e_shoff, SEEK_SET) != NOTOK)
	MustBeTrue(read(fd, pshdr, numbytes) == numbytes);

	// should data be swabbed?
	if (isSwabable(pehdr))
	{
		for (int s=0; s<pehdr->e_shnum; s++)
		{
			swabshdr(pehdr, pshdr+s);
		}
	}

	// read in section header string table
	Elf32_Shdr *pstrtblhdr = pshdr+pehdr->e_shstrndx;
	if (pshstrtbl != NULL)
	{
		delete [] pshstrtbl;
		pshstrtbl = NULL;
	}
	pshstrtbl = new char [pstrtblhdr->sh_size];
	MustBeTrue(pshstrtbl != NULL);
	MustBeTrue(lseek(fd, pstrtblhdr->sh_offset, SEEK_SET) != NOTOK)
	int bytestoread = pstrtblhdr->sh_size;
	MustBeTrue(read(fd, pshstrtbl, bytestoread) == bytestoread);
	return;
}

// read in program headers
void
readphdr(int fd, Elf32_Ehdr *pehdr, Elf32_Phdr *&pphdr)
{
	// allocate a buffer for reading in section header table
	if (pphdr != NULL)
	{
		delete [] pphdr;
		pphdr = NULL;
	}
	int numbytes = pehdr->e_phentsize*pehdr->e_phnum;
	MustBeTrue(numbytes > 0);
	pphdr = (Elf32_Phdr *) new char [numbytes];
	MustBeTrue(pphdr != NULL);

	// read in program header tables
	MustBeTrue(lseek(fd, pehdr->e_phoff, SEEK_SET) != NOTOK)
	MustBeTrue(read(fd, pphdr, numbytes) == numbytes);

	// should data be swabbed?
	if (isSwabable(pehdr))
	{
		for (int p=0; p<pehdr->e_phnum; p++)
		{
			swabphdr(pehdr, pphdr+p);
		}
	}
	return;
}

// read in section data
void
readsdata(int fd, Elf32_Ehdr *pehdr, Elf32_Shdr *pshdr, int sec, char *&buf)
{
	// sanity check
	MustBeTrue(pehdr != NULL && pshdr != NULL);
	MustBeTrue(0 <= sec && sec < pehdr->e_shnum);

	// allocate a buffer
	if (buf != NULL)
	{
		delete [] buf;
		buf = NULL;
	}
	buf = new char [pshdr[sec].sh_size];
	MustBeTrue(buf != NULL);

	// read in data
	MustBeTrue(lseek(fd, pshdr[sec].sh_offset, SEEK_SET) != NOTOK)
	int numbytes = pshdr[sec].sh_size;
	MustBeTrue(read(fd, buf, numbytes) == numbytes);
	return;
}

// read in program data
void
readpdata(int fd, Elf32_Ehdr *pehdr, Elf32_Phdr *pphdr, int sec, char *&buf)
{
	// sanity check
	MustBeTrue(pehdr != NULL && pphdr != NULL);
	MustBeTrue(0 <= sec && sec < pehdr->e_phnum);

	// allocate a buffer
	if (buf != NULL)
	{
		delete [] buf;
		buf = NULL;
	}
	buf = new char [pphdr[sec].p_filesz];
	MustBeTrue(buf != NULL);

	// read in data
	MustBeTrue(lseek(fd, pphdr[sec].p_offset, SEEK_SET) != NOTOK)
	int numbytes = pphdr[sec].p_filesz;
	MustBeTrue(read(fd, buf, numbytes) == numbytes);
	return;
}

