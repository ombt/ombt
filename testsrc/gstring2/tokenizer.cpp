// simple class to tokenize a string

#include "tokenizer.h"

// ctors and dtor
Tokenizer::Tokenizer() { }
Tokenizer::Tokenizer(const Tokenizer &src) { }
Tokenizer::~Tokenizer() { }

// assignment
Tokenizer &
Tokenizer::operator=(const Tokenizer &rhs)
{
	return(*this);
}

// tokenizing functor
int
Tokenizer::operator()(const std::string &line, 
                      std::vector<std::string> &tokens,
                      const std::string &delimiters, bool trim_front)
{
    static std::string whitespace = "\t ";
    std::string::size_type lastpos = 0;

    if (trim_front)
    	lastpos = line.find_first_not_of(whitespace, 0);

    std::string::size_type pos = line.find_first_of(delimiters, lastpos);

    while (std::string::npos != pos || std::string::npos != lastpos)
    {
        tokens.push_back(line.substr(lastpos, pos - lastpos));
        lastpos = line.find_first_not_of(delimiters, pos);
        pos = line.find_first_of(delimiters, lastpos);
    }

    return(0);
}

