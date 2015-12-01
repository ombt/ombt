#ifndef __STABTOKENIZER_H
#define __STABTOKENIZER_H
// stab string tokenizer class definition

// headers 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "spaces.h"

// tokenizer class
class StabTokenizer {
public:
	// internal data types
	enum Type { Unknown, Name, String, Integer, 
		HexInteger, Real, Char, EndOfString };

	// constructors and destructor
	StabTokenizer(const char *ps):
		wsre(NULL), intre(NULL), hexintre(NULL), nameconre(NULL),
		cwsre(NULL), cintre(NULL), chexintre(NULL), cnameconre(NULL),
		type(Unknown), pstart(NULL), pend(NULL), len(0),
		matchlen(0), value(NULL), cursor(NULL), newcursor(NULL)
	{
		// initialize REs
		wsre = "^[ \t]*";
		MustBeTrue((cwsre = regcmp(wsre, NULL)) != NULL);

		intre = "^[ \t]*([+-]{0,1}[0-9]+)$0";
		MustBeTrue((cintre = regcmp(intre, NULL)) != NULL);

		hexintre = "^[ \t]*([A-F0-9]+)$0";
		MustBeTrue((chexintre = regcmp(hexintre, NULL)) != NULL);

		// nameconre = "[^;:\'\"]*([;:\'\"])$0";
		nameconre = "^[a-zA-Z0-9_ ]+([^a-zA-Z0-9_ ])$0";
		MustBeTrue((cnameconre = regcmp(nameconre, NULL)) != NULL);

		realre[0] = "^[ \t]*([-+]{0,1}[0-9]+(\\.[0-9]*){0,1}([eEqQ][-+]{0,1}[0-9]+){0,1})$0";
		MustBeTrue((crealre[0] = regcmp(realre[0], NULL)) != NULL);

		realre[1] = "^[ \t]*([-+]{0,1}INF)$0";
		MustBeTrue((crealre[1] = regcmp(realre[1], NULL)) != NULL);

		realre[2] = "^[ \t]*(QNAN)$0";
		MustBeTrue((crealre[2] = regcmp(realre[2], NULL)) != NULL);

		realre[3] = "^[ \t]*(SNAN)$0";
		MustBeTrue((crealre[3] = regcmp(realre[3], NULL)) != NULL);

		qmre = "^([\"'])$0";
		MustBeTrue((cqmre = regcmp(qmre, NULL)) != NULL);

		strconre = "^[^\\\"']*([\\\"'])$0";
		MustBeTrue((cstrconre = regcmp(strconre, NULL)) != NULL);

		// copy string to be parsed
		MustBeTrue(ps != NULL && *ps != '\0');
		len = strlen(ps);
		string = new char [len+1];
		MustBeTrue(string != NULL);
		strcpy(string, ps);
		cursor = newcursor = pstart = string;
		pend = string+len;
		value = new char [BUFSIZ];
		MustBeTrue(value != NULL);
	}
	~StabTokenizer() {
		if (string != NULL)
			delete [] string;
		string = pstart = pend = cursor = newcursor = NULL;
		type = Unknown;
		matchlen = len = 0;
		if (cwsre != NULL) 
			free((void *)cwsre);
		cwsre = NULL;
		if (cintre != NULL) 
			free((void *)cintre);
		cintre = NULL;
		if (chexintre != NULL) 
			free((void *)chexintre);
		chexintre = NULL;
		if (cnameconre != NULL) 
			free((void *)cnameconre);
		cnameconre = NULL;
		for (int i=0; i<4; i++)
		{
			if (crealre[i] != NULL) 
				free((void *)crealre[i]);
			crealre[i] = NULL;
		}
		if (cqmre != NULL) 
			free((void *)cqmre);
		cqmre = NULL;
		if (cstrconre != NULL) 
			free((void *)cstrconre);
		cstrconre = NULL;
		if (value != NULL) 
			delete [] value;
		value = NULL;
	}

	// check next token type
	int isString() {
		// end-of-string or String ?
		Spaces spaces;
		fprintf(stdout, "%sisString() entry, cursor = %s\n", 
			spaces(), cursor);
		if (iseos()) return(0);
		if (type == String)
		{
			fprintf(stdout, "%sisString() = %s\n", 
				spaces(), value);
			return(matchlen);
		}
		type = Unknown;
		matchlen = 0;

		// check for white space
		if ((newcursor = regex(cwsre, cursor)) != NULL)
			cursor = newcursor;

		// check for quotation marks, " or '
		if ((newcursor = regex(cqmre, cursor, value)) == NULL)
			return(0);

		// try to find a string
		char *startqm = cursor;
		char *endqm = cursor;
		cursor = newcursor;
		while ((newcursor = regex(cstrconre, cursor, value)) != NULL)
		{
			switch (*value)
			{
			case '\\':
				// check for end-of-string
				if (*newcursor == '\0')
				{
					cursor = startqm;
					return(0);
				}
				// skip whatever was escaped, 
				cursor = ++newcursor;
				break;
			case '"':
			case '\'':
				if (*value == *startqm)
				{
					// we found the end
					endqm = newcursor;
					strncpy(value, startqm, endqm-startqm);
					value[endqm-startqm] = '\0';
					type = String;
					matchlen = endqm-startqm;
					fprintf(stdout, "%sisString() = %s\n", 
						spaces(), value);
					return(matchlen);
				}
				cursor = newcursor;
				break;
			}
		}

		// reset cursor to where we started, but failed.
		cursor = startqm;
		return(0);
	}
	int isName() {
		// end-of-string or name ?
		Spaces spaces;
		fprintf(stdout, "%sisName() entry, cursor = %s\n", 
			spaces(), cursor);
		if (iseos()) return(0);
		if (type == Name)
		{
			fprintf(stdout, "%sisName() = %s\n", 
				spaces(), value);
			return(matchlen);
		}
		type = Unknown;
		matchlen = 0;

		// skip white-space
		if ((newcursor = regex(cwsre, cursor)) != NULL)
			cursor = newcursor;

		// look for a name
		char *startname = cursor;
		char *endname = cursor;
		while ((newcursor = regex(cnameconre, cursor, value)) != NULL)
		{
			if (*value == ':' && *newcursor == ':')
			{
				// double colons are allowed in names.
				cursor = newcursor+1;
			}
			else
			{
				// we found the end
				if (*value == ':')
					newcursor--;
				endname = newcursor;
				strncpy(value, startname, endname-startname);
				value[endname-startname] = '\0';
				cursor=startname;
				type = Name;
				matchlen = endname-startname;
				fprintf(stdout, "%sisName() = %s\n", 
					spaces(), value);
				return(matchlen);
			}
		}

		// reset cursor to where we started, but failed.
		cursor = startname;
		return(0);
	}
	int isReal() {
		Spaces spaces;
		// end-of-string or String ?
		fprintf(stdout, "%sisReal() entry, cursor = %s\n", 
			spaces(), cursor);
		if (iseos()) return(0);
		if (type == Real)
		{
			fprintf(stdout, "%sisReal() = %s\n", 
				spaces(), value);
			return(matchlen);
		}
		type = Unknown;
		matchlen = 0;

		// search for a real
		for (int re=0; re<4; re++)
		{
			newcursor = regex(crealre[re], cursor, value);
			if (newcursor != NULL)
			{
				type = Real;
				matchlen = newcursor-cursor;
				fprintf(stdout, "%sisReal() = %s\n", 
					spaces(), value);
				return(matchlen);
			}
		}

		// not a real
		return(0);
	}
	int isInteger() {
		Spaces spaces;
		fprintf(stdout, "%sisInteger() entry, cursor = %s\n", 
			spaces(), cursor);
		if (iseos()) return(0);
		if (type == Integer)
		{
			fprintf(stdout, "%sisInteger() = %s\n", 
				spaces(), value);
			return(matchlen);
		}
		type = Unknown;
		matchlen = 0;
		newcursor = regex(cintre, cursor, value);
		if (newcursor == NULL) return(0);
		type = Integer;
		matchlen = newcursor-cursor;
		fprintf(stdout, "%sisInteger() = %s\n", 
			spaces(), value);
		return(matchlen);
	}
	int isHexInteger() {
		Spaces spaces;
		fprintf(stdout, "%sisHexInteger() entry, cursor = %s\n", 
			spaces(), cursor);
		if (iseos()) return(0);
		if (type == HexInteger)
		{
			fprintf(stdout, "%sisHexInteger() = %s\n", 
				spaces(), value);
			return(matchlen);
		}
		type = Unknown;
		matchlen = 0;
		newcursor = regex(chexintre, cursor, value);
		if (newcursor == NULL) return(0);
		type = HexInteger;
		matchlen = newcursor-cursor;
		fprintf(stdout, "%sisHexInteger() = %s\n", 
			spaces(), value);
		return(1);
	}
	int isChar() {
		Spaces spaces;
		fprintf(stdout, "%sisChar() entry, cursor = %s\n", 
			spaces(), cursor);
		if (iseos())
			return(0);
		type = Char;
		value[0] = *cursor;
		value[1] = '\0';
		newcursor = cursor+1;
		matchlen = 1;
		return(matchlen);
	}
	int iseos() {
		if (type == EndOfString) return(1);
		if (cursor >= pend) type = EndOfString;
		return(type == EndOfString);
	}

	// get token value
	Type getType() const {
		return(type);
	}
	int getValue(Type tokentype, char *token) {
		Spaces spaces;
		MustBeTrue(token != NULL);
		if (tokentype == type)
		{
			strcpy(token, value);
			fprintf(stdout, "%sgetValue() = %s\n", 
				spaces(), value);
			return(OK);
		}
		else
		{
			*token = '\0';
			fprintf(stdout, "%sgetValue() - type mismatch\n", 
				spaces());
			return(NOTOK);
		}
	}
	char getChar() const {
		Spaces spaces;
		MustBeTrue(type == Char);
		fprintf(stdout, "%sgetChar() = %c\n", 
			spaces(), *value);
		return(*value);
	}

	// accept the token and move on
	void accept() {
		Spaces spaces;
		switch (type)
		{
		case Unknown:
			break;
		case EndOfString:
			break;
		default:
			fprintf(stdout, "%scursor string ... %s\n", 
				spaces(), cursor);
			fprintf(stdout, "%snewcursor string ... %s\n", 
				spaces(), newcursor);
			fprintf(stdout, "%saccepting token ... %s\n", 
				spaces(), value);
			type = Unknown;
			cursor = newcursor;
			iseos();
			break;
		}
		return;
	}

protected:
	// internal regular expressions
	const char *wsre;
	const char *intre;
	const char *hexintre;
	const char *nameconre;
	const char *realre[4];
	const char *qmre;
	const char *strconre;
	const char *cwsre;
	const char *cintre;
	const char *chexintre;
	const char *cnameconre;
	const char *crealre[4];
	const char *cqmre;
	const char *cstrconre;

	// internal data
	char *string;
	char *pstart, *pend, *cursor, *newcursor;
	char *value;
	Type type;
	int matchlen, len;
};
#endif
