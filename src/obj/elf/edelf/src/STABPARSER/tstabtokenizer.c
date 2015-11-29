#include <stdio.h>
#include <string.h>
#include "stabtokenizer.h"

int
main(int argc, char **argv)
{
	char value[BUFSIZ];
	StabTokenizer st(argv[1]);
	while (!st.iseos())
	{
		if (st.isString())
		{
			st.getValue(StabTokenizer::String, value);
			printf("<%s> is a String ...\n", value);
		}
		else if (st.isName())
		{
			st.getValue(StabTokenizer::Name, value);
			printf("<%s> is a Name ...\n", value);
		}
		else if (st.isReal())
		{
			st.getValue(StabTokenizer::Real, value);
			printf("<%s> is a Real ...\n", value);
		}
		else if (st.isInteger())
		{
			st.getValue(StabTokenizer::Integer, value);
			printf("<%s> is a Integer ...\n", value);
		}
		else if (st.isHexInteger())
		{
			st.getValue(StabTokenizer::HexInteger, value);
			printf("<%s> is a HexInteger ...\n", value);
		}
		else if (st.isChar())
		{
			st.getValue(StabTokenizer::Char, value);
			printf("<%s> is a Char ...\n", value);
		}
		st.accept();
	}
	return(0);
}
