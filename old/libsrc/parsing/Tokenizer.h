#ifndef __OMBT_TOKENIZER
#define __OMBT_TOKENIZER

// string tokenizer

// headers
#include <string>
#include <vector>
#include <atomic/BaseObject.h>

namespace ombt {

// tokenizer class
class Tokenizer: public BaseObject
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

}

#endif
