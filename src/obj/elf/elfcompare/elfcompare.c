/* elfcompare.c -- compare selected sections of an ELF format files.
   Portions of this code were based on GNU readelf.c and uses md5.c.
   (from libiberty.a)
   File header comments for readelf.c is included for reference.
   Created by Roy Harkness 9/2001
*/
/* readelf.c -- display contents of an ELF format file
   Copyright 1998, 1999, 2000, 2001 Free Software Foundation, Inc.

   Originally developed by Eric Youngdale <eric@andante.jic.com>
   Modifications by Nick Clifton <nickc@cygnus.com>

   This file is part of GNU Binutils.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  
*/

#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>

#include "bfd.h"

#include "elf/common.h"
#include "elf/external.h"
#include "elf/internal.h"
#include "elf/dwarf2.h"

#include "bucomm.h"

#include "md5.h"

/* Forward declarations for dumb compilers.  */
static bfd_vma (*         byte_get)                   PARAMS ((unsigned char *, int));
static bfd_vma            byte_get_little_endian      PARAMS ((unsigned char *, int));
static bfd_vma            byte_get_big_endian         PARAMS ((unsigned char *, int));
static int                get_32bit_section_headers   PARAMS ((FILE *, int));
static int                get_64bit_section_headers   PARAMS ((FILE *, int));

/*
 * These are indices into the section name to number array.
 * Adding a new section is always done before LAST_dx to avoid
 * gaps 
 */
enum {
    TEXT_dx=0,
    DATA_dx,
    DATA1_dx,
    RODATA_dx,
    RODATA1_dx,
    INIT_dx,
    FINI_dx,
    LAST_dx
} name_number_dx;

/* 
    Common section information
*/
Elf_Internal_Ehdr       elf_header;
Elf_Internal_Ehdr       elf_header2;
Elf_Internal_Shdr *     section_headers;
Elf_Internal_Shdr       s_hdrs[LAST_dx];	/* Hdrs of sections we want */
Elf_Internal_Shdr       s_hdrs2[LAST_dx];
char *			string_table;
char *			section_buf;
char *	program_name = "elfcompare";
unsigned long		string_table_length;
int			is_32bit_elf;
int	do_text;
int	do_data;
int	do_data1;
int	do_rodata;
int	do_rodata1;
int	do_init;
int	do_fini;
int	name_num_array[LAST_dx];	/* Mapping of section name to number */
int	name_num_array2[LAST_dx];

unsigned int		num_dump_sects = 0;

#define VERSION_NO	0
#define	VERSION_PT	1
typedef int Elf32_Word;

#ifndef TRUE
#define TRUE     1
#define FALSE    0
#endif
#define UNKNOWN -1

#define SECTION_NAME(X)	((X) == NULL ? "<none>" : \
				 ((X)->sh_name >= string_table_length \
				  ? "<corrupt>" : string_table + (X)->sh_name))

#define BYTE_GET(field)	byte_get (field, sizeof (field))

/* If we can support a 64 bit data type then BFD64 should be defined
   and sizeof (bfd_vma) == 8.  In this case when translating from an
   external 8 byte field to an internal field, we can assume that the
   internal field is also 8 bytes wide and so we can extract all the data.
   If, however, BFD64 is not defined, then we must assume that the
   internal data structure only has 4 byte wide fields that are the
   equivalent of the 8 byte wide external counterparts, and so we must
   truncate the data.  */
#ifdef  BFD64
#define BYTE_GET8(field)	byte_get (field, -8)
#else
#define BYTE_GET8(field)	byte_get (field, 8)
#endif

#define NUM_ELEM(array)	(sizeof (array) / sizeof ((array)[0]))

#define GET_DATA_ALLOC(offset, size, var, type, reason)			\
  if (fseek (file, offset, SEEK_SET))					\
    {									\
      error (_("Unable to seek to start of %s at %x\n"), reason, offset); \
      return 0;								\
    }									\
									\
  var = (type) malloc (size);						\
									\
  if (var == NULL)							\
    {									\
      error (_("Out of memory allocating %d bytes for %s\n"), size, reason); \
      return 0;								\
    }									\
									\
  if (fread (var, size, 1, file) != 1)					\
    {									\
      error (_("Unable to read in %d bytes of %s\n"), size, reason);	\
      free (var);							\
      var = NULL;							\
      return 0;							\
    }


#define GET_DATA(offset, var, reason)					\
  if (fseek (file, offset, SEEK_SET))					\
    {									\
      error (_("Unable to seek to %x for %s\n"), offset, reason);	\
      return 0;								\
    }									\
  else if (fread (& var, sizeof (var), 1, file) != 1)			\
    {									\
      error (_("Unable to read data at %x for %s\n"), offset, reason);	\
      return 0;								\
    }

#define GET_ELF_SYMBOLS(file, offset, size)			\
  (is_32bit_elf ? get_32bit_elf_symbols (file, offset, size)	\
   : get_64bit_elf_symbols (file, offset, size))

#ifdef ANSI_PROTOTYPES
static void
error (const char * message, ...)
{
    va_list args;

    fprintf (stderr, _("%s: Error: "), program_name);
    va_start (args, message);
    vfprintf (stderr, message, args);
    va_end (args);
    return;
}

static void
warn (const char * message, ...)
{
    va_list args;

    fprintf (stderr, _("%s: Warning: "), program_name);
    va_start (args, message);
    vfprintf (stderr, message, args);
    va_end (args);
    return;
}

static void
elog (const char * message, ...)
{
    va_list args;

#ifndef _ELOG
    return;
#endif
    fprintf (stderr, _("%s: ELog: "), program_name);
    va_start (args, message);
    vfprintf (stderr, message, args);
    va_end (args);
    return;
}
#else
static void
error (va_alist)
     va_dcl
{
    char * message;
    va_list args;
    
    fprintf (stderr, _("%s: Error: "), program_name);
    va_start (args);
    message = va_arg (args, char *);
    vfprintf (stderr, message, args);
    va_end (args);
    return;
}

static void
warn (va_alist)
     va_dcl
{
    char * message;
    va_list args;
    
    fprintf (stderr, _("%s: Warning: "), program_name);
    va_start (args);
    message = va_arg (args, char *);
    vfprintf (stderr, message, args);
    va_end (args);
    return;
}

static void
elog (va_alist)
     va_dcl
{
    char * message;
    va_list args;

#ifndef _ELOG
    return;
#endif
    fprintf (stderr, _("%s: ELog: "), program_name);
    va_start (args);
    message = va_arg (args, char *);
    vfprintf (stderr, message, args);
    va_end (args);
    return;
}
#endif

static int
get_32bit_section_headers (file, file_no)
     FILE * file;
     int file_no;
{
    Elf32_External_Shdr * shdrs;
    Elf32_Internal_Shdr * internal;
    unsigned int          i;
    
    Elf_Internal_Ehdr*	elfhdr_ptr;
    switch(file_no) {
    case 1:	elfhdr_ptr = &elf_header;
        break;
    case 2:	elfhdr_ptr = &elf_header2;
        break;
    default:	return 0;
    }


    GET_DATA_ALLOC (elfhdr_ptr->e_shoff,
		    elfhdr_ptr->e_shentsize * elfhdr_ptr->e_shnum,
		    shdrs, Elf32_External_Shdr *, "section headers");

    section_headers = (Elf_Internal_Shdr *) malloc
	(elfhdr_ptr->e_shnum * sizeof (Elf_Internal_Shdr));

    if (section_headers == NULL) {
	    error (_("Out of memory\n"));
	    return 0;
	}

    for (i = 0, internal = section_headers;
	 i < elfhdr_ptr->e_shnum;
	 i ++, internal ++) {
	    internal->sh_name      = BYTE_GET (shdrs[i].sh_name);
	    internal->sh_type      = BYTE_GET (shdrs[i].sh_type);
	    internal->sh_flags     = BYTE_GET (shdrs[i].sh_flags);
	    internal->sh_addr      = BYTE_GET (shdrs[i].sh_addr);
	    internal->sh_offset    = BYTE_GET (shdrs[i].sh_offset);
	    internal->sh_size      = BYTE_GET (shdrs[i].sh_size);
	    internal->sh_link      = BYTE_GET (shdrs[i].sh_link);
	    internal->sh_info      = BYTE_GET (shdrs[i].sh_info);
	    internal->sh_addralign = BYTE_GET (shdrs[i].sh_addralign);
	    internal->sh_entsize   = BYTE_GET (shdrs[i].sh_entsize);
	}
    
    free (shdrs);

    return 1;
}

static int
get_64bit_section_headers (file, file_no)
     FILE * file;
     int file_no;
{
    Elf64_External_Shdr * shdrs;
    Elf64_Internal_Shdr * internal;
    unsigned int          i;
    
    Elf_Internal_Ehdr*	elfhdr_ptr;
    switch(file_no) {
    case 1:	elfhdr_ptr = &elf_header;
        break;
    case 2:	elfhdr_ptr = &elf_header2;
        break;
    default:	return 0;
    }


    GET_DATA_ALLOC (elfhdr_ptr->e_shoff,
		    elfhdr_ptr->e_shentsize * elfhdr_ptr->e_shnum,
		    shdrs, Elf64_External_Shdr *, "section headers");
    
    section_headers = (Elf_Internal_Shdr *) malloc
	(elfhdr_ptr->e_shnum * sizeof (Elf_Internal_Shdr));
    
    if (section_headers == NULL)
	{
	    error (_("Out of memory\n"));
	    return 0;
	}
    
    for (i = 0, internal = section_headers;
	 i < elfhdr_ptr->e_shnum;
	 i ++, internal ++)
	{
	    internal->sh_name      = BYTE_GET (shdrs[i].sh_name);
	    internal->sh_type      = BYTE_GET (shdrs[i].sh_type);
	    internal->sh_flags     = BYTE_GET8 (shdrs[i].sh_flags);
	    internal->sh_addr      = BYTE_GET8 (shdrs[i].sh_addr);
	    internal->sh_size      = BYTE_GET8 (shdrs[i].sh_size);
	    internal->sh_entsize   = BYTE_GET8 (shdrs[i].sh_entsize);
	    internal->sh_link      = BYTE_GET (shdrs[i].sh_link);
	    internal->sh_info      = BYTE_GET (shdrs[i].sh_info);
	    internal->sh_offset    = BYTE_GET (shdrs[i].sh_offset);
	    internal->sh_addralign = BYTE_GET (shdrs[i].sh_addralign);
	}
    
    free (shdrs);
    
    return 1;
}

static bfd_vma
byte_get_little_endian (field, size)
     unsigned char * field;
     int             size;
{
    switch (size) {
    case 1:
	return * field;
	
    case 2:
	return  ((unsigned int) (field [0]))
	    |    (((unsigned int) (field [1])) << 8);
	
#ifndef BFD64
    case 8:
	/* We want to extract data from an 8 byte wide field and
	   place it into a 4 byte wide field.  Since this is a little
	   endian source we can juts use the 4 byte extraction code.  */
	/* Fall through.  */
#endif
    case 4:
	return  ((unsigned long) (field [0]))
	    |    (((unsigned long) (field [1])) << 8)
	    |    (((unsigned long) (field [2])) << 16)
	    |    (((unsigned long) (field [3])) << 24);
	
#ifdef BFD64
    case 8:
    case -8:
	/* This is a special case, generated by the BYTE_GET8 macro.
	   It means that we are loading an 8 byte value from a field
	   in an external structure into an 8 byte value in a field
	   in an internal strcuture.  */
	return  ((bfd_vma) (field [0]))
	    |    (((bfd_vma) (field [1])) << 8)
	    |    (((bfd_vma) (field [2])) << 16)
	    |    (((bfd_vma) (field [3])) << 24)
	    |    (((bfd_vma) (field [4])) << 32)
	    |    (((bfd_vma) (field [5])) << 40)
	    |    (((bfd_vma) (field [6])) << 48)
	    |    (((bfd_vma) (field [7])) << 56);
#endif
    default:
	error (_("Unhandled data length: %d\n"), size);
	abort ();
    }
}

static bfd_vma
byte_get_big_endian (field, size)
     unsigned char * field;
     int             size;
{
    switch (size) {
    case 1:
	return * field;
	
  case 2:
      return ((unsigned int) (field [1])) | (((int) (field [0])) << 8);
      
    case 4:
	return ((unsigned long) (field [3]))
	    |   (((unsigned long) (field [2])) << 8)
	    |   (((unsigned long) (field [1])) << 16)
	    |   (((unsigned long) (field [0])) << 24);
	
#ifndef BFD64
    case 8:
	/* Although we are extracing data from an 8 byte wide field, we
	   are returning only 4 bytes of data.  */
	return ((unsigned long) (field [7]))
	    |   (((unsigned long) (field [6])) << 8)
	    |   (((unsigned long) (field [5])) << 16)
	    |   (((unsigned long) (field [4])) << 24);
#else
    case 8:
    case -8:
	/* This is a special case, generated by the BYTE_GET8 macro.
	   It means that we are loading an 8 byte value from a field
	   in an external structure into an 8 byte value in a field
	   in an internal strcuture.  */
	return ((bfd_vma) (field [7]))
	    |   (((bfd_vma) (field [6])) << 8)
	    |   (((bfd_vma) (field [5])) << 16)
	    |   (((bfd_vma) (field [4])) << 24)
	    |   (((bfd_vma) (field [3])) << 32)
	    |   (((bfd_vma) (field [2])) << 40)
	    |   (((bfd_vma) (field [1])) << 48)
	    |   (((bfd_vma) (field [0])) << 56);
#endif
	
    default:
	error (_("Unhandled data length: %d\n"), size);
	abort ();
    }
}

char*
name_number_text(index)
    int index;
{
    char*		retstring;
    switch (index) {
    case TEXT_dx:
	retstring=".text";
	break;
    case DATA_dx:
	retstring=".data";
	break;
    case DATA1_dx:
	retstring=".data1";
	break;
    case RODATA_dx:
	retstring=".rodata";
	break;
    case RODATA1_dx:
	retstring=".rodata1";
	break;
    case INIT_dx:
	retstring=".init";
	break;
    case FINI_dx:
	retstring=".fini";
	break;
    default:
	retstring="Unknown name-number index";
	break;
    }

    return retstring;
}

/* Decode the data held in 'elf_header'.  */
static int
process_file_header (file_no)
     int file_no;
{
    Elf_Internal_Ehdr*	elfhdr_ptr;
    switch(file_no) {
    case 1:	elfhdr_ptr = &elf_header;
        break;
    case 2:	elfhdr_ptr = &elf_header2;
        break;
    default:	return 0;
    }
    
    if (   elfhdr_ptr->e_ident [EI_MAG0] != ELFMAG0
	   || elfhdr_ptr->e_ident [EI_MAG1] != ELFMAG1
	   || elfhdr_ptr->e_ident [EI_MAG2] != ELFMAG2
	   || elfhdr_ptr->e_ident [EI_MAG3] != ELFMAG3) {
	error
	    (_("Not an ELF file - it has the wrong magic bytes at the start\n"));
	return 0;
    }
    
    return 1;
}


static int
process_section_headers (file, file_no)
     FILE * file;
     int    file_no;
{
    Elf_Internal_Ehdr*	elfhdr_ptr;
    Elf_Internal_Shdr*    section_aptr;
    Elf_Internal_Shdr * section;
    int                 i;
    int *			sec_array_ptr;
    
    switch(file_no) {
    case 1:	elfhdr_ptr = &elf_header;
	sec_array_ptr = name_num_array;
	section_aptr = s_hdrs;
        break;
    case 2:	elfhdr_ptr = &elf_header2;
	sec_array_ptr = name_num_array2;
	section_aptr = s_hdrs2;
        break;
    default:	return 0;
    }

    section_headers = NULL;

    if (elfhdr_ptr->e_shnum == 0) {
	return 1;
    }

    if (is_32bit_elf) {
	if (! get_32bit_section_headers (file, file_no))
	    return 0;
    }
    else if (! get_64bit_section_headers (file, file_no))
	return 0;


    /* Read in the string table, so that we have names to display.  */
    section = section_headers + elfhdr_ptr->e_shstrndx;
    
    if (section->sh_size != 0) {
	GET_DATA_ALLOC (section->sh_offset, section->sh_size,
			string_table, char *, "string table");
	
	string_table_length = section->sh_size;
    }

    /* Get the names & info for this files section numbers. */
    for (i = 0, section = section_headers;
	 i < elfhdr_ptr->e_shnum;
	 i ++, section ++) {
	char * name = SECTION_NAME (section);

	/* Get needed section info */
	if( strcmp( name, ".text" ) == 0 && do_text == 1) {
	    *(sec_array_ptr + TEXT_dx) = i;
	    (*(section_aptr + TEXT_dx)).sh_addr = section->sh_addr;
	    (*(section_aptr + TEXT_dx)).sh_offset = section->sh_offset;
	    (*(section_aptr + TEXT_dx)).sh_size = section->sh_size;
	    (*(section_aptr + TEXT_dx)).sh_entsize = section->sh_entsize;
	    (*(section_aptr + TEXT_dx)).sh_flags = section->sh_flags;
	    (*(section_aptr + TEXT_dx)).sh_link = section->sh_link;
	    (*(section_aptr + TEXT_dx)).sh_info = section->sh_info;
	    (*(section_aptr + TEXT_dx)).sh_addralign = section->sh_addralign;
	}

	if( strcmp( name, ".data" ) == 0 && do_data == 1) {
	    *(sec_array_ptr + DATA_dx) = i;
	    (*(section_aptr + DATA_dx)).sh_addr = section->sh_addr;
	    (*(section_aptr + DATA_dx)).sh_offset = section->sh_offset;
	    (*(section_aptr + DATA_dx)).sh_size = section->sh_size;
	    (*(section_aptr + DATA_dx)).sh_entsize = section->sh_entsize;
	    (*(section_aptr + DATA_dx)).sh_flags = section->sh_flags;
	    (*(section_aptr + DATA_dx)).sh_link = section->sh_link;
	    (*(section_aptr + DATA_dx)).sh_info = section->sh_info;
	    (*(section_aptr + DATA_dx)).sh_addralign = section->sh_addralign;
	}

	if( strcmp( name, ".data1" ) == 0 && do_data1 == 1) {
	    *(sec_array_ptr + DATA1_dx) = i;
	    (*(section_aptr + DATA1_dx)).sh_addr = section->sh_addr;
	    (*(section_aptr + DATA1_dx)).sh_offset = section->sh_offset;
	    (*(section_aptr + DATA1_dx)).sh_size = section->sh_size;
	    (*(section_aptr + DATA1_dx)).sh_entsize = section->sh_entsize;
	    (*(section_aptr + DATA1_dx)).sh_flags = section->sh_flags;
	    (*(section_aptr + DATA1_dx)).sh_link = section->sh_link;
	    (*(section_aptr + DATA1_dx)).sh_info = section->sh_info;
	    (*(section_aptr + DATA1_dx)).sh_addralign = section->sh_addralign;
	}

	if( strcmp( name, ".rodata" ) == 0 && do_rodata == 1) {
	    *(sec_array_ptr + RODATA_dx) = i;
	    (*(section_aptr + RODATA_dx)).sh_addr = section->sh_addr;
	    (*(section_aptr + RODATA_dx)).sh_offset = section->sh_offset;
	    (*(section_aptr + RODATA_dx)).sh_size = section->sh_size;
	    (*(section_aptr + RODATA_dx)).sh_entsize = section->sh_entsize;
	    (*(section_aptr + RODATA_dx)).sh_flags = section->sh_flags;
	    (*(section_aptr + RODATA_dx)).sh_link = section->sh_link;
	    (*(section_aptr + RODATA_dx)).sh_info = section->sh_info;
	    (*(section_aptr + RODATA_dx)).sh_addralign = section->sh_addralign;
	}
	
	if( strcmp( name, ".rodata1" ) == 0 && do_rodata1 == 1) {
	    *(sec_array_ptr + RODATA1_dx) = i;
	    (*(section_aptr + RODATA1_dx)).sh_addr = section->sh_addr;
	    (*(section_aptr + RODATA1_dx)).sh_offset = section->sh_offset;
	    (*(section_aptr + RODATA1_dx)).sh_size = section->sh_size;
	    (*(section_aptr + RODATA1_dx)).sh_entsize = section->sh_entsize;
	    (*(section_aptr + RODATA1_dx)).sh_flags = section->sh_flags;
	    (*(section_aptr + RODATA1_dx)).sh_link = section->sh_link;
	    (*(section_aptr + RODATA1_dx)).sh_info = section->sh_info;
	    (*(section_aptr + RODATA1_dx)).sh_addralign = section->sh_addralign;
	}
	
	if( strcmp( name, ".init" ) == 0 && do_init == 1) {
	    *(sec_array_ptr + INIT_dx) = i;
	    (*(section_aptr + INIT_dx)).sh_addr = section->sh_addr;
	    (*(section_aptr + INIT_dx)).sh_offset = section->sh_offset;
	    (*(section_aptr + INIT_dx)).sh_size = section->sh_size;
	    (*(section_aptr + INIT_dx)).sh_entsize = section->sh_entsize;
	    (*(section_aptr + INIT_dx)).sh_flags = section->sh_flags;
	    (*(section_aptr + INIT_dx)).sh_link = section->sh_link;
	    (*(section_aptr + INIT_dx)).sh_info = section->sh_info;
	    (*(section_aptr + INIT_dx)).sh_addralign = section->sh_addralign;
	}
	
	if( strcmp( name, ".fini" ) == 0 && do_fini == 1) {
	    *(sec_array_ptr + FINI_dx) = i;
	    (*(section_aptr + FINI_dx)).sh_addr = section->sh_addr;
	    (*(section_aptr + FINI_dx)).sh_offset = section->sh_offset;
	    (*(section_aptr + FINI_dx)).sh_size = section->sh_size;
	    (*(section_aptr + FINI_dx)).sh_entsize = section->sh_entsize;
	    (*(section_aptr + FINI_dx)).sh_flags = section->sh_flags;
	    (*(section_aptr + FINI_dx)).sh_link = section->sh_link;
	    (*(section_aptr + FINI_dx)).sh_info = section->sh_info;
	    (*(section_aptr + FINI_dx)).sh_addralign = section->sh_addralign;
	}
	
    }
    
    return 1;
}

static int
get_file_header (file, file_no)
     FILE * file;
     int    file_no;
{
    Elf_Internal_Ehdr*	elfhdr_ptr;
    switch(file_no) {
    case 1:	elfhdr_ptr = &elf_header;
        break;
    case 2:	elfhdr_ptr = &elf_header2;
        break;
    default:	return 0;
    }
    
    /* Read in the identity array.  */
    if (fread (elfhdr_ptr->e_ident, EI_NIDENT, 1, file) != 1)
	return 0;

    /* Determine how to read the rest of the header.  */
    switch (elfhdr_ptr->e_ident [EI_DATA]) {
    default: /* fall through */
    case ELFDATANONE: /* fall through */
    case ELFDATA2LSB: byte_get = byte_get_little_endian; break;
    case ELFDATA2MSB: byte_get = byte_get_big_endian; break;
    }

    /* For now we only support 32 bit and 64 bit ELF files.  */
    is_32bit_elf = (elfhdr_ptr->e_ident [EI_CLASS] != ELFCLASS64);

    /* Read in the rest of the header.  */
    if (is_32bit_elf) {
	Elf32_External_Ehdr ehdr32;
	
	if (fread (ehdr32.e_type, sizeof (ehdr32) - EI_NIDENT, 1, file) != 1)
	    return 0;
	
	elfhdr_ptr->e_type      = BYTE_GET (ehdr32.e_type);
	elfhdr_ptr->e_machine   = BYTE_GET (ehdr32.e_machine);
	elfhdr_ptr->e_version   = BYTE_GET (ehdr32.e_version);
	elfhdr_ptr->e_entry     = BYTE_GET (ehdr32.e_entry);
	elfhdr_ptr->e_phoff     = BYTE_GET (ehdr32.e_phoff);
	elfhdr_ptr->e_shoff     = BYTE_GET (ehdr32.e_shoff);
	elfhdr_ptr->e_flags     = BYTE_GET (ehdr32.e_flags);
	elfhdr_ptr->e_ehsize    = BYTE_GET (ehdr32.e_ehsize);
	elfhdr_ptr->e_phentsize = BYTE_GET (ehdr32.e_phentsize);
	elfhdr_ptr->e_phnum     = BYTE_GET (ehdr32.e_phnum);
	elfhdr_ptr->e_shentsize = BYTE_GET (ehdr32.e_shentsize);
	elfhdr_ptr->e_shnum     = BYTE_GET (ehdr32.e_shnum);
	elfhdr_ptr->e_shstrndx  = BYTE_GET (ehdr32.e_shstrndx);
    } else {
	Elf64_External_Ehdr ehdr64;
	
	/* If we have been compiled with sizeof (bfd_vma) == 4, then
	   we will not be able to cope with the 64bit data found in
	   64 ELF files.  Detect this now and abort before we start
	   overwritting things.  */
	if (sizeof (bfd_vma) < 8) {
	    error (_("This instance of elfcompare has been built without support for a\n"));
	    error (_("64 bit data type and so it cannot read 64 bit ELF files.\n"));
	    return 0;
	}

	if (fread (ehdr64.e_type, sizeof (ehdr64) - EI_NIDENT, 1, file) != 1)
	    return 0;

	elfhdr_ptr->e_type      = BYTE_GET (ehdr64.e_type);
	elfhdr_ptr->e_machine   = BYTE_GET (ehdr64.e_machine);
	elfhdr_ptr->e_version   = BYTE_GET (ehdr64.e_version);
	elfhdr_ptr->e_entry     = BYTE_GET8 (ehdr64.e_entry);
	elfhdr_ptr->e_phoff     = BYTE_GET8 (ehdr64.e_phoff);
	elfhdr_ptr->e_shoff     = BYTE_GET8 (ehdr64.e_shoff);
	elfhdr_ptr->e_flags     = BYTE_GET (ehdr64.e_flags);
	elfhdr_ptr->e_ehsize    = BYTE_GET (ehdr64.e_ehsize);
	elfhdr_ptr->e_phentsize = BYTE_GET (ehdr64.e_phentsize);
	elfhdr_ptr->e_phnum     = BYTE_GET (ehdr64.e_phnum);
	elfhdr_ptr->e_shentsize = BYTE_GET (ehdr64.e_shentsize);
	elfhdr_ptr->e_shnum     = BYTE_GET (ehdr64.e_shnum);
	elfhdr_ptr->e_shstrndx  = BYTE_GET (ehdr64.e_shstrndx);
    }
    
    return 1;
}

static int
process_section_contents (file, file2)
     FILE * file;
     FILE * file2;
{
    int	i;
    int	retval=0;
    int	total_bytes;
    size_t	byte_cnt;
    unsigned int	secsize_tot=0;
    char * section_buf_ptr;
    char * resbufptr;
    char md5_resbuf[33];
    char md5_resbuf2[33];
    md5_state_t state;
    md5_byte_t digest[16];
    md5_state_t state2;
    md5_byte_t digest2[16];
    int di;
    
    /*
      First, go though all the sections and then validate the
      sizes are equal. If not, then they definately would not
      compare.
    */
    elog(_("sizeof sh_size is %d\n"), sizeof(s_hdrs[0].sh_size));
    for (i = 0; i < LAST_dx; i ++) {
	/* only process sections we wanted */
	if( name_num_array[i] != 0 ) {
	    elog(_("Processing %s segment; size %d\n"), name_number_text(i), s_hdrs[i].sh_size);
	    if( s_hdrs[i].sh_size != s_hdrs2[i].sh_size ) {
		error (_("%s segment has mis-matched sizes - %lx vs %lx\n"), 
		       name_number_text(i), s_hdrs[i].sh_size, s_hdrs2[i].sh_size);
		retval = 1;
	    } /* end if sh_size */
	    secsize_tot += (unsigned int) s_hdrs[i].sh_size;
	    
	} /* end if name */
    } /* end for */
    
    elog(_("Total section size is %d [0x%x] bytes\n"), secsize_tot, secsize_tot);
    
    if( retval ) 
	return retval;
    
    /*
      Now, we know the sizes are good, calloc a segment for all the
      sections we are using, read in each file, get an MD5 Sum and
      return success (0) if they are the same, or (1) if not.
    */
    section_buf = (char *) calloc( secsize_tot + 1, 1 );
    if( section_buf == NULL ) {
	error (_("Out of memory allocating dump request table."));
	retval = 1;
    } else {
	/*
	  Now we want to cycle through the sections and copy them to the
	  section_buf area. Start with the first file, get the MD5 sum, 
	  then do the next file and compare the final SUMs.
	*/
	section_buf_ptr = section_buf;
	total_bytes = 0;
	
	elog(_("Reading sections from file\n"));
	for (i = 0; i < LAST_dx; i ++) {
	    if( name_num_array[i] != 0 ) {
		elog(_("Reading section %s from file\n"), name_number_text(i));
		if( fseek( file, s_hdrs[i].sh_offset, 0 ) ) {
		    error (_("Unable to seek to start of %s at %x\n"), name_number_text(i), s_hdrs[i].sh_offset);
		    retval = 1;
		}
		if( fread(section_buf_ptr, s_hdrs[i].sh_size, 1, file) != 1 ) {
		    retval = 1;
		    error(_("Failed to read %d (0x%x) bytes from %s section of file\n"), s_hdrs[i].sh_size, s_hdrs[i].sh_size, name_number_text(i));
		}
		byte_cnt = s_hdrs[i].sh_size;
		total_bytes += byte_cnt;

		/* Read our section. Increment pointer and read the next. */
		section_buf_ptr += byte_cnt;
	    }
	}
	
	/* Now do an MD5 Sum on the buffer */
	elog(_("Gettting MD5 for file\n"));
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)section_buf, total_bytes);
	md5_finish(&state, digest);

	resbufptr = md5_resbuf;
	for (di = 0; di < 16; ++di) {
	    sprintf(resbufptr, "%02x", digest[di]);
	    resbufptr += 2;
	}
	elog(_("MD5 on file is: %s\n"), md5_resbuf);
	
	section_buf_ptr = section_buf;
	total_bytes = 0;
	
	elog(_("Reading sections from file\n"));
	for (i = 0; i < LAST_dx; i ++) {
	    if( name_num_array[i] != 0 ) {
		elog(_("Reading section %s from file2\n"), name_number_text(i));
		if( fseek( file2, s_hdrs2[i].sh_offset, 0 ) ) {
		    error (_("Unable to seek to start of %s at %x\n"), name_number_text(i), s_hdrs2[i].sh_offset);
		    retval = 1;
		}
		if( fread(section_buf_ptr, s_hdrs2[i].sh_size, 1, file2) != 1 ) {
		    retval = 1;
		    error(_("Failed to read %d (0x%x) bytes from %s section of file2\n"), s_hdrs[i].sh_size, s_hdrs[i].sh_size, name_number_text(i));
		}
		byte_cnt = s_hdrs[i].sh_size;
		total_bytes += byte_cnt;

		/* Read our section. Increment pointer and read the next. */
		section_buf_ptr += byte_cnt;
	    }
	}
	
	/* Now do an MD5 Sum on the buffer */
	elog(_("Gettting MD5 for file2\n"));
	md5_init(&state2);
	md5_append(&state2, (const md5_byte_t *)section_buf, total_bytes);
	md5_finish(&state2, digest2);

	resbufptr = md5_resbuf2;
	for (di = 0; di < 16; ++di) {
	    sprintf(resbufptr, "%02x", digest2[di]);
	    resbufptr += 2;
	}
	elog(_("MD5 on file is: %s\n"), md5_resbuf2);

	/* Now compare and print the MD5s. Set the retval accordingly. */
	if( (retval = strncmp( (char*)md5_resbuf, (char*)md5_resbuf2, 32 )) != 0 ) {
	    /* dump the MD5s */
	    printf("MD5 Mismatch:\n%s\n%s\n\n", md5_resbuf, md5_resbuf2);
	}
    } /* end if section_buf != NULL */
    
    return retval;
}

static int
process_files (file_name, file_name2)
     char * file_name;
     char * file_name2;
{
    FILE *       file;
    FILE *       file2;
    struct stat  statbuf;
    struct stat  statbuf2;
    unsigned int i;
    int		process_status=0;
    int		retval=0;
    
    if (stat (file_name, & statbuf) < 0) {
	error (_("Cannot stat input file %s.\n"), file_name);
	return retval;
    }

    if (stat (file_name2, & statbuf2) < 0) {
	error (_("Cannot stat input file %s.\n"), file_name2);
	return retval;
    }

    file = fopen (file_name, "rb");
    if (file == NULL) {
	error (_("Input file %s not found.\n"), file_name);
	return retval;
    }

    if (! get_file_header (file,1)) {
	error (_("%s: Failed to read file header\n"), file_name);
	fclose (file);
	return retval;
    }

    file2 = fopen (file_name2, "rb");
    if (file2 == NULL) {
	error (_("Input file %s not found.\n"), file_name2);
	return retval;
    }

    if (! get_file_header (file2,2)) {
	error (_("%s: Failed to read file header\n"), file_name2);
	fclose (file2);
	return retval;
    }

    if (! process_file_header (1)) {
	fclose (file);
	return retval;
    }

    if (! process_file_header (2)) {
	fclose (file2);
	return retval;
    }

    if( process_section_headers (file, 1) != 1) {
	error (_("%s: Failed to process section headers\n"), file_name);
	process_status++;
    }

    if( process_section_headers (file2, 2) != 1) {
	error (_("%s: Failed to process section headers\n"), file_name2);
	process_status++;
    }

    /* only process sections if data is good */
    if( process_status == 0)
	retval = process_section_contents (file, file2);

    elog(_("retval after process_section_contents is %d\n"), retval);
    fclose (file);
    fclose (file2);

    if (section_headers) {
	free (section_headers);
	section_headers = NULL;
    }

    if (string_table) {
	free (string_table);
	string_table = NULL;
	string_table_length = 0;
    }
    return retval;
}

static void
output_version(name)
     char * name;
{
    printf("Version %d.%d of %s\n", VERSION_NO, VERSION_PT, name);
}

static void
usage ()
{
    fprintf (stdout, _("Usage: elfcompare {options} elf-file elf_file\n"));
    fprintf (stdout, _("  Options are:\n"));
    fprintf (stdout, _("  -t or --text              .text Program Text\n"));
    fprintf (stdout, _("  -d or --data              .data Initialized Data\n"));
    fprintf (stdout, _("  -D or --data1             .data1 More Initialized Data\n"));
    fprintf (stdout, _("  -r or --rodata            .rodata Read Only Data\n"));
    fprintf (stdout, _("  -R or --rodata1           .rodata1 More Read Only Data\n"));
    fprintf (stdout, _("  -i or --init              .init Code executed before main()\n"));
    fprintf (stdout, _("  -f or --fini              .fini Code executed after main()\n"));
    fprintf (stdout, _("  -v or --version           Display the version number of elfcompare\n"));
    fprintf (stdout, _("  -H or --help              Display this information\n"));
    exit (0);
}


static void
parse_args (argc, argv)
     int argc;
     char ** argv;
{
    int c;

    while ((c = getopt(argc, argv, "tdDrRifvH")) != EOF) {
	switch (c) {
	case 'H':
	    usage ();
	    break;
	case 't':
	    do_text++;
	    break;
	case 'd':
	    do_data++;
	    break;
	case 'D':
	    do_data1++;
	    break;
	case 'r':
	    do_rodata++;
	    break;
	case 'R':
	    do_rodata1++;
	    break;
	case 'i':
	    do_init++;
	    break;
	case 'f':
	    do_fini++;
	    break;
	case 'v':
	    output_version (program_name);
	    break;
	default:
	case '?':
	    usage ();
	}
    }

    if ( !do_text && !do_data && !do_data1 && !do_rodata && !do_rodata1
	 && !do_init && !do_fini)
	usage ();
    else if (argc < 4) {
	warn (_("Nothing to do.\n"));
	usage();
    }
}


int
main (argc, argv)
     int     argc;
     char ** argv;
{
    int retval;

    parse_args (argc, argv);
    
    retval = process_files (argv [optind], argv [(optind+1)]);
    
    return retval;
}
