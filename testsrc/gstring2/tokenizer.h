#ifndef __TOKENIZER_H
#define __TOKENIZER_H

// string tokenizer

// headers
#include <string>
#include <vector>

// tokenizer class
class Tokenizer
{
public:
    // ctors and dtor
    Tokenizer();
    Tokenizer(const Tokenizer &src);
    ~Tokenizer();

    // assignment
    Tokenizer &operator=(const Tokenizer &rhs);

    // tokenizing function
    int operator()(const std::string &line, 
                   std::vector<std::string> &tokens,
                   const std::string &delimiters = " ",
                   bool trim_front = true);
};

#endif
