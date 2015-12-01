// utility functions

// headers
#include "edelf.h"

// utility functions
int
REequal(const char *p, const char *s)
{
	static int first = 1;
	static int useRE = 0;
	static char savep[BUFSIZ];

	if (first || p != NULL && strcmp(savep, p) != 0)
	{
		first = 0;
		strcpy(savep, p);
		char *remsg = re_comp(p);
		if (remsg == NULL)
		{
			useRE = 1;
		}
		else
		{
			useRE = 0;
			printf("re_comp failed: %s.\n", remsg);
			printf("using strcmp.\n");
		}
	}
	if (useRE)
	{
		return(re_exec(s) == 1);
	}
	else
	{
		return(strcmp(p, s) == 0);
	}
}

void
rmvnlgets(char *s)
{
	gets(s);
	for ( ; *s != '\n' && *s != '\0'; s++) ;
	*s = '\0';
	return;
}

// editing section headers 
int
nametosec(const char *secnm, int startsec)
{
	// convert a section name to a section number
	for (int sec=startsec; sec < pehdr->e_shnum; sec++)
	{
		if (REequal(secnm, pshstrtbl+pshdrs[sec].sh_name))
			return(sec);
	}
	return(NOTOK);
}

void
zap(char *ps, char c, int sz)
{
	for (int i=0; i<sz; i++)
		ps[i] = c;
	return;
}

const char *
mydemangle(const char *s)
{
	static char ds[BUFSIZ];
	MustBeTrue(s != NULL);
#if EXPTOOLS==1
	if (dflag && cplus_demangle((char *)s, ds, BUFSIZ-1) == 0) 
#else
	if (dflag && cplus_demangle(s, ds, BUFSIZ-1) == 0) 
#endif
		return(ds);
	else
		return(s);
}

long
MYatoi(const char *s)
{
	MustBeTrue(s != NULL);
	if (*s == '\0')
		return(0);
	else if ((strncmp(s, "0x", 2) == 0) || (strncmp(s, "0X", 2) == 0))
	{
		MustBeTrue(strspn(s+2, "abcdefABCDEF0123456789") == strlen(s+2));
		return(strtol(s, NULL, 16));
	}
	else if (strspn(s, "0123456789") == strlen(s))
	{
		return(strtol(s, NULL, 10));
	}
	else
	{
		MustBeTrue((strncmp(s, "0x", 2) == 0) || 
			   (strncmp(s, "0X", 2) == 0) ||
			   (strspn(s, "0123456789") == strlen(s)));
	}
	return(0);
}
