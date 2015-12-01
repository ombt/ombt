// swab in elf file data

// headers
#include "ocelf.h"

// local definitions
#define SWABIT(_data) { \
	(_data) = swabit((unsigned char *)(&_data), sizeof((_data))); \
}

// swab file contents?
int
isSwabable(const Elf32_Ehdr *pehdr)
{
	// on SUN machine, least significant byte at lowest address
	// is swabbed.
	return(pehdr->e_ident[EI_DATA] == ELFDATA2LSB);
}

// swab bytes
unsigned long
swabit(unsigned char *p, int szofp)
{
	unsigned long nval = 0;

	switch (szofp)
	{
	case 1:
		nval = p[0];
		break;
	case 2:
		nval = p[0] | (p[1] << 8);
		break;
	case 4:
		nval = p[0] | (p[1]<<8) | (p[2]<<16) | (p[3]<<24);
		break;
	default:
		MustBeTrue(szofp == 1 || szofp == 2 || szofp == 4);
	}
	return(nval);
}

// swab file headers
void
swabehdr(Elf32_Ehdr *pehdr)
{
	// sanity check
	MustBeTrue(pehdr != NULL);

	// swab header data
	SWABIT(pehdr->e_type);
	SWABIT(pehdr->e_machine);
	SWABIT(pehdr->e_version);
	SWABIT(pehdr->e_entry);
	SWABIT(pehdr->e_phoff);
	SWABIT(pehdr->e_shoff);
	SWABIT(pehdr->e_flags);
	SWABIT(pehdr->e_ehsize);
	SWABIT(pehdr->e_phentsize);
	SWABIT(pehdr->e_phnum);
	SWABIT(pehdr->e_shentsize);
	SWABIT(pehdr->e_shnum);
	SWABIT(pehdr->e_shstrndx);

	// all done
	return;
}

// swab section headers
void
swabshdr(Elf32_Ehdr *, Elf32_Shdr *pshdr)
{
	// sanity check
	MustBeTrue(pshdr != NULL);

	// swab header data
	SWABIT(pshdr->sh_name);
	SWABIT(pshdr->sh_type);
	SWABIT(pshdr->sh_flags);
	SWABIT(pshdr->sh_addr);
	SWABIT(pshdr->sh_offset);
	SWABIT(pshdr->sh_size);
	SWABIT(pshdr->sh_link);
	SWABIT(pshdr->sh_info);
	SWABIT(pshdr->sh_addralign);
	SWABIT(pshdr->sh_entsize);
	return;
}

// swab segment headers
void
swabphdr(Elf32_Ehdr *, Elf32_Phdr *pphdr)
{
	// sanity check
	MustBeTrue(pphdr != NULL);

	// swab header data
	SWABIT(pphdr->p_type);
	SWABIT(pphdr->p_offset);
	SWABIT(pphdr->p_vaddr);
	SWABIT(pphdr->p_paddr);
	SWABIT(pphdr->p_filesz);
	SWABIT(pphdr->p_memsz);
	SWABIT(pphdr->p_flags);
	SWABIT(pphdr->p_align);
	return;
}

