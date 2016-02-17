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

// trace flag 
extern int dotrace;

// tokenizer class
class StabTokenizer {
public:
	// internal data types
	enum Type { Unknown, Alphabeta, NoWsName, Name, String, Integer, 
		HexInteger, Real, Char, EndOfString };

	// constructors and destructor
	StabTokenizer(const char *ps):
		type(Unknown), string(NULL), pstart(NULL), pend(NULL), len(0),
		matchlen(0), value(NULL), cursor(NULL), newcursor(NULL)
	{
		// initialize REs
		if (!done)
		{
			wsre = "^[ \t]*";
			MustBeTrue((cwsre = regcmp(wsre, NULL)) != NULL);

			alphare = "^[ \t]*([a-zA-Z]+)$0";
			MustBeTrue((calphare = regcmp(alphare, NULL)) != NULL);

			intre = "^[ \t]*([+-]{0,1}[0-9]+)$0";
			MustBeTrue((cintre = regcmp(intre, NULL)) != NULL);

			hexintre = "^[ \t]*([A-F0-9]+)$0";
			MustBeTrue((chexintre = regcmp(hexintre, NULL)) != NULL);

			// nameconre = "[^;:\'\"]*([;:\'\"])$0";
#if 0
			namere = "^([a-zA-Z0-9_ ]+)$0$";
			MustBeTrue((cnamere = regcmp(namere, NULL)) != NULL);
	
			nameconre = "^[a-zA-Z0-9_ ]+([^a-zA-Z0-9_ ])$0";
			MustBeTrue((cnameconre = regcmp(nameconre, NULL)) != NULL);
#else
			namere = "^([^;:\'\"]+)$0$";
			MustBeTrue((cnamere = regcmp(namere, NULL)) != NULL);

			nameconre = "^[^;:\'\"]+([;:\'\"])$0";
			MustBeTrue((cnameconre = regcmp(nameconre, NULL)) != NULL);

			nowsnamere = "^([^ ;:\'\"]+)$0$";
			MustBeTrue((cnowsnamere = regcmp(nowsnamere, NULL)) != NULL);

			nowsnameconre = "^[^ ;:\'\"]+([ ;:\'\"])$0";
			MustBeTrue((cnowsnameconre = regcmp(nowsnameconre, NULL)) != NULL);
#endif

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
			done = 1;
		}

		// copy string to be parsed
		MustBeTrue(ps != NULL && *ps != '\0');
		len = strlen(ps);
		string = new char [len+1];
		MustBeTrue(string != NULL);
		*string = '\0';
		strcpy(string, ps);
		cursor = newcursor = pstart = string;
		pend = string+len;
		//
		// not a very robust way to do things ... changing
		// BUFSIZ to 10*BUFSIZ, but it'll do for here ...
		//
		value = new char [10*BUFSIZ];
		MustBeTrue(value != NULL);
		*value = '\0';
	}
	~StabTokenizer() {
		if (string != NULL)
			delete [] string;
		string = pstart = pend = cursor = newcursor = NULL;
		type = Unknown;
		matchlen = len = 0;
		if (value != NULL) 
			delete [] value;
		value = NULL;
	}

	// check for equality with a string
	int isEqual(const char *str) const {
		if ((type == EndOfString || type == Unknown) ||
		    (str == NULL || *str == '\0') ||
		    (value == NULL || *value == '\0'))
			return(0);
		else
			return(strcmp(str, value) == 0);
	}

	// check next token type
	int isString() {
		// end-of-string or String ?
		Spaces spaces;
		if (dotrace) fprintf(stdout, "%sisString() entry, cursor = %s\n", spaces(), cursor);
		if (iseos()) return(0);
		if (type == String)
		{
			if (dotrace) fprintf(stdout, "%sisString() = %s\n", spaces(), value);
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
					if (dotrace) fprintf(stdout, "%sisString() = %s\n", spaces(), value);
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
		if (dotrace) fprintf(stdout, "%sisName() entry, cursor = %s\n", spaces(), cursor);
		if (iseos()) return(0);
		if (type == Name)
		{
			if (dotrace) fprintf(stdout, "%sisName() = %s\n", spaces(), value);
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
				if (*value == ':' || *value == ';')
					newcursor--;
				endname = newcursor;
				strncpy(value, startname, endname-startname);
				value[endname-startname] = '\0';
				cursor=startname;
				type = Name;
				matchlen = endname-startname;
				if (dotrace) fprintf(stdout, "%sisName() = %s\n", spaces(), value);
				return(matchlen);
			}
		}

		// check if the first scan failed, we might have a 
		// string of ONLY a name that does not terminate
		// with a non-name-alllowed character.
		if (cursor == startname)
		{
			// check if the entire string is a name.
			if ((newcursor = regex(cnamere, cursor, value)) != NULL)
			{
				endname = newcursor;
				strncpy(value, startname, endname-startname);
				value[endname-startname] = '\0';
				cursor=startname;
				type = Name;
				matchlen = endname-startname;
				if (dotrace) fprintf(stdout, "%sisName() = %s\n", spaces(), value);
				return(matchlen);
			}
		}

		// reset cursor to where we started, but failed.
		cursor = startname;
		return(0);
	}
	int isNoWsName() {
		// end-of-string or no-ws-name ?
		Spaces spaces;
		if (dotrace) fprintf(stdout, "%sisNoWsName() entry, cursor = %s\n", spaces(), cursor);
		if (iseos()) return(0);
		if (type == NoWsName)
		{
			if (dotrace) fprintf(stdout, "%sisNoWsName() = %s\n", spaces(), value);
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
		while ((newcursor = regex(cnowsnameconre, cursor, value)) != NULL)
		{
			if (*value == ':' && *newcursor == ':')
			{
				// double colons are allowed in names.
				cursor = newcursor+1;
			}
			else
			{
				// we found the end
				if (*value == ':' || *value == ';')
					newcursor--;
				endname = newcursor;
				strncpy(value, startname, endname-startname);
				value[endname-startname] = '\0';
				cursor=startname;
				type = Name;
				matchlen = endname-startname;
				if (dotrace) fprintf(stdout, "%sisNoWsName() = %s\n", spaces(), value);
				return(matchlen);
			}
		}

		// check if the first scan failed, we might have a 
		// string of ONLY a name that does not terminate
		// with a non-name-alllowed character.
		if (cursor == startname)
		{
			// check if the entire string is a name.
			if ((newcursor = regex(cnowsnamere, cursor, value)) != NULL)
			{
				endname = newcursor;
				strncpy(value, startname, endname-startname);
				value[endname-startname] = '\0';
				cursor=startname;
				type = Name;
				matchlen = endname-startname;
				if (dotrace) fprintf(stdout, "%sisNoWsName() = %s\n", spaces(), value);
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
		if (dotrace) fprintf(stdout, "%sisReal() entry, cursor = %s\n", spaces(), cursor);
		if (iseos()) return(0);
		if (type == Real)
		{
			if (dotrace) fprintf(stdout, "%sisReal() = %s\n", spaces(), value);
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
				if (dotrace) fprintf(stdout, "%sisReal() = %s\n", spaces(), value);
				return(matchlen);
			}
		}

		// not a real
		return(0);
	}
	int isAlphabeta() {
		Spaces spaces;
		if (dotrace) fprintf(stdout, "%sisAlphabeta() entry, cursor = %s\n", spaces(), cursor);
		if (iseos()) return(0);
		if (type == Alphabeta)
		{
			if (dotrace) fprintf(stdout, "%sisAlphabeta() = %s\n", spaces(), value);
			return(matchlen);
		}
		type = Unknown;
		matchlen = 0;
		newcursor = regex(calphare, cursor, value);
		if (newcursor == NULL) return(0);
		type = Alphabeta;
		matchlen = newcursor-cursor;
		if (dotrace) fprintf(stdout, "%sisAlphabeta() = %s\n", spaces(), value);
		return(matchlen);
	}
	int isInteger() {
		Spaces spaces;
		if (dotrace) fprintf(stdout, "%sisInteger() entry, cursor = %s\n", spaces(), cursor);
		if (iseos()) return(0);
		if (type == Integer)
		{
			if (dotrace) fprintf(stdout, "%sisInteger() = %s\n", spaces(), value);
			return(matchlen);
		}
		type = Unknown;
		matchlen = 0;
		newcursor = regex(cintre, cursor, value);
		if (newcursor == NULL) return(0);
		type = Integer;
		matchlen = newcursor-cursor;
		if (dotrace) fprintf(stdout, "%sisInteger() = %s\n", spaces(), value);
		return(matchlen);
	}
	int isHexInteger() {
		Spaces spaces;
		if (dotrace) fprintf(stdout, "%sisHexInteger() entry, cursor = %s\n", spaces(), cursor);
		if (iseos()) return(0);
		if (type == HexInteger)
		{
			if (dotrace) fprintf(stdout, "%sisHexInteger() = %s\n", spaces(), value);
			return(matchlen);
		}
		type = Unknown;
		matchlen = 0;
		newcursor = regex(chexintre, cursor, value);
		if (newcursor == NULL) return(0);
		type = HexInteger;
		matchlen = newcursor-cursor;
		if (dotrace) fprintf(stdout, "%sisHexInteger() = %s\n", spaces(), value);
		return(1);
	}
	int isChar() {
		Spaces spaces;
		if (dotrace) fprintf(stdout, "%sisChar() entry, cursor = %s\n", spaces(), cursor);
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
		if (dotrace) fprintf(stdout, "%sgetValue() = %s\n", spaces(), value);
			return(OK);
		}
		else
		{
			*token = '\0';
		if (dotrace) fprintf(stdout, "%sgetValue() - type mismatch\n", spaces());
			return(NOTOK);
		}
	}
	long getHexInteger() const {
		Spaces spaces;
		MustBeTrue(type == HexInteger);
		if (dotrace) fprintf(stdout, "%sgetHexInteger() = %s\n", spaces(), value);
		return(strtol(value, NULL, 16));
	}
	long getInteger() const {
		Spaces spaces;
		MustBeTrue(type == Integer);
		if (dotrace) fprintf(stdout, "%sgetInteger() = %s\n", spaces(), value);
		return(strtol(value, NULL, 10));
	}
	char getChar() const {
		Spaces spaces;
		MustBeTrue(type == Char);
		if (dotrace) fprintf(stdout, "%sgetChar() = %c\n", spaces(), *value);
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
			if (dotrace)
			{
				// fprintf(stdout, "%scursor string ... %s\n", spaces(), cursor);
				// fprintf(stdout, "%snewcursor string ... %s\n", spaces(), newcursor);
				fprintf(stdout, "%saccepting token ... %s\n", spaces(), value);
			}
			type = Unknown;
			cursor = newcursor;
			iseos();
			break;
		}
		return;
	}

protected:
	// internal regular expressions
	static int done;
	static const char *wsre;
	static const char *alphare;
	static const char *intre;
	static const char *hexintre;
	static const char *namere;
	static const char *nameconre;
	static const char *nowsnamere;
	static const char *nowsnameconre;
	static const char *realre[4];
	static const char *qmre;
	static const char *strconre;
	static const char *cwsre;
	static const char *calphare;
	static const char *cintre;
	static const char *chexintre;
	static const char *cnamere;
	static const char *cnameconre;
	static const char *cnowsnamere;
	static const char *cnowsnameconre;
	static const char *crealre[4];
	static const char *cqmre;
	static const char *cstrconre;

	// internal data
	char *string;
	char *pstart, *pend, *cursor, *newcursor;
	char *value;
	Type type;
	int matchlen, len;
};
#endif
