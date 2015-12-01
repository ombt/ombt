// tokenizer for elf editor

// headers
#include "tokenize.h"

// definitions
static const int StartOfString = 1;
static const int SkipWS = 2;
static const int GetToken = 3;
static const int EndOfString = 4;
static const int MaxTokens = 20;
static char tbuf[BUFSIZ];
static char *tokens[MaxTokens];

// check if a character is a delimiter
static int isdelimiter(char c, const char *delimiters)
{
	for (const char *pd = delimiters; *pd != '\0'; pd++)
	{
		if (*pd == c)
			return(1);
	}
	return(0);
}

// initialize tokenizer
void
tokenize(const char *input, const char *delimiters)
{
	int it;

	// sanity checks
	MustBeTrue(input != NULL);
	MustBeTrue(delimiters != NULL && *delimiters != '\0');

	// initialize
	for (it=0; it<MaxTokens; it++)
	{
		tokens[it] = NULL;
	}

	// copy over the buffer
	strcpy(tbuf, input);

	// parse out the tokens
	int state = StartOfString;
	char *ps = tbuf;
	char *pe = tbuf + strlen(ps);
	for (it=-1; ps <= pe; )
	{
		switch (state)
		{
		case StartOfString:
			if (*ps == '\0')
			{
				state = EndOfString;
			}
			else if (isdelimiter(*ps, delimiters))
			{
				state = SkipWS;
			}
			else
			{
				it++;
				tokens[it] = ps;
				state = GetToken;
			}
			break;
		case SkipWS:
			if (*ps == '\0')
			{
				state = EndOfString;
			}
			else if (isdelimiter(*ps, delimiters))
			{
				ps++;
			}
			else
			{
				it++;
				tokens[it] = ps;
				state = GetToken;
			}
			break;
		case GetToken:
			if (*ps == '\0')
			{
				state = EndOfString;
			}
			else if (isdelimiter(*ps, delimiters))
			{
				*ps = '\0';
				ps++;
				state = SkipWS;
			}
			else
			{
				ps++;
			}
			break;
		case EndOfString:
			ps++;
			break;
		default:
			MustBeTrue(0);
		}
	}
	return;
}

// get token
char *
gettoken(int it)
{
	MustBeTrue(0 < it && it <= MaxTokens);
	return(tokens[it-1]);
}
