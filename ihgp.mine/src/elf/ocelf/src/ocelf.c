// elf object file comparer

// headers 
#include "ocelf.h"

// externs
extern char *optarg;
extern int optind;

// globals
int aflag = 0;
int tflag = 0;
int sflag = 0;
int fflag = 0;
int Sflag = 1;
int Pflag = 0;
int Yflag = 0;
int Tflag = 0;
int Bflag = 0;
long maxBytesToPrint = 100;
long offsetfldwidth = 0;

// usage msg
void
usage(const char *cmd)
{
	printf("\nusage: %s [-?sfTYBatV] [-M bytes] [-S or -P] elf_file1 elf_file2 \n\n", cmd);
	printf("\t-? - print usage message\n");
	printf("\t-s - silent mode, return code indicates status\n");
	printf("\t-f - fast mode, returns upon first difference\n");
	printf("\t-S - compare file sections (default).\n");
	printf("\t-P - compare program segments.\n");
	printf("\t-T - include string tables in comparisons.\n");
	printf("\t-Y - include symbol tables in comparisons.\n");
	printf("\t-B - include .bss sections in comparisons.\n");
	printf("\t-a - list all differences.\n");
	printf("\t-t - translate to ASCII character.\n");
	printf("\t-M - maximum number of bytes to print per difference (default = 100).\n");
	printf("\t-V - print version information.\n");
	printf("\t-? - print usage message\n\n");
	printf("return code of 0 - files are the same.\n");
	printf("return code of 1 - files are the different.\n");
	printf("return code of >1 - an error occurred.\n\n");
	printf("can compare segments or sections, but not both together.\n");
	printf("by default, only changes to .init, .text, .fini, \n");
	printf(".rodata.*, and .data.* will mark two files as\n");
	printf("different. changes to other sections are reported, but \n");
	printf("ignored. use the -T or -Y options to add checking for \n");
	printf("differences in string and symbol tables, respectively.\n");
	printf("by default, 100 bytes are printed for each difference found.\n");
	printf("to print all differences, use the -a option, or\n");
	printf("use the -M option to give the number of bytes to print.\n");
	return;
}

// version information
void
version(const char *cmd)
{
	printf("%s: Version 1.2, Author: M.A. Rumore (mrumore@alcatel-lucent.com)\n",
		cmd);
	return;
}

// main entry point
main(int argc, char ** argv)
{
	// get command line options
	for (int c=0; (c=getopt(argc, argv, "?VtasfSPYTBM:")) != EOF; )
	{
		switch (c)
		{
		case 'a':
			// print all differences found.
			aflag = 1;
			break;
		case 't':
			// translate to ascii
			tflag = 1;
			break;
		case 's':
			// silent mode, do not list differences.
			sflag = 1;
			break;
		case 'M':
			// maximum bytes to print
			maxBytesToPrint = atoi(optarg);
			if (maxBytesToPrint < 0)
			{
				printf("maximum byte to print must be >= 0\n");
				usage(argv[0]);
				return(2);
			}
			break;
		case 'Y':
			// also check symbol tables
			Yflag = 1;
			break;
		case 'B':
			// also check .bss sections
			Bflag = 1;
			break;
		case 'T':
			// also check string tables
			Tflag = 1;
			break;
		case 'S':
			// compare file sections
			Sflag = 1;
			Pflag = 0;
			break;
		case 'P':
			// compare program segments
			Pflag = 1;
			Sflag = 0;
			break;
		case 'f':
			// fast mode, exit when first difference is detected.
			fflag = 1;
			break;
		case '?':
			usage(argv[0]);
			return(0);
		case 'V':
			version(argv[0]);
			return(0);
		default:
			ERRORD("invalid command option.", c, EINVAL);
			return(2);
		}
	}

	// verify two files were given
	if (optind+2 != argc)
	{
		printf("incorrect number of files given.\n");
		usage(argv[0]);
		return(2);
	}

	// compare files
	int status = NOTOK;
	try {
		// open files for read
		int fd1 = NULL;
		MustBeTrue((fd1 = open(argv[optind], O_RDONLY)) != NOTOK);
		int fd2 = NULL;
		MustBeTrue((fd2 = open(argv[optind+1], O_RDONLY)) != NOTOK);

		// get size of files
		struct stat sbuf1, sbuf2;
		MustBeTrue(fstat(fd1, &sbuf1) == OK);
		MustBeTrue(fstat(fd2, &sbuf2) == OK);
		long filesz = sbuf1.st_size;
		if (filesz < sbuf2.st_size) 
			filesz = sbuf2.st_size;
		offsetfldwidth = log10((double)(++filesz))+1;

		// now compare files for real
		status = compare(fd1, fd2);
	}
	catch (const char *pe)
	{
		printf("EXCEPTION CAUGHT: %s\n", pe);
		ERROR(pe, errno);
		return(2);
	}
	catch (char *pe)
	{
		printf("EXCEPTION CAUGHT: %s\n", pe);
		ERROR(pe, errno);
		return(2);
	}

	// all done
	if (IsError(status))
	{
		if (!sflag) printf("Error status (status=0x%x, ret=2).\n", status);
		return(2);
	}
	else if (IsIdentical(status))
	{
		if (!sflag) printf("Files are identical (status=0x%x, ret=0).\n", status);
		return(0);
	}
	else if (IsDifferent(status))
	{
		if (!sflag) printf("Files are different (status=0x%x, ret=1).\n", status);
		return(1);
	}
	else
	{
		if (!sflag) printf("Unknown error (status=0x%x, ret=2).\n", status);
		return(2);
	}
}
