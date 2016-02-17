// semantic routines for oty files.

// headers
#include "otyscan.h"

// dump tokens
void
dumpTokens(char *info, char *delimiters)
{
	char *ptoken = NULL;
	char *pcmd = strtok(info, delimiters); 
	fprintf(stdout, "start %s semantics ... \n", pcmd);
	for (ptoken = strtok(NULL, delimiters);
	     ptoken != NULL; 
	     ptoken = strtok(NULL, delimiters))
	{
		fprintf(stdout, "token is ... %s\n", ptoken);
	}
	fprintf(stdout, "end %s semantics ... \n", pcmd);
	return;
}

// semantic routines
int
msgstartSemantics()
{
	fprintf(stdout, "STARTING A MSG ... \n");
	return(OK);
}

int
msgendSemantics()
{
	fprintf(stdout, "ENDING A MSG ... \n");
	return(OK);
}

int
msgidSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
timeSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
classSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
actionSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
outpriorSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
freqSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
prototypeSemantics(char *info)
{
	dumpTokens(info, TAB_NL_TILDE);
	return(OK);
}

int
fieldSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
fieldupSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
npvalSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
widthSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

int
enumSemantics(char *info)
{
	dumpTokens(info, TAB_NL);
	return(OK);
}

