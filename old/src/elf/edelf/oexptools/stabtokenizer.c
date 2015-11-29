// data for stab tokenizer

// headers
#include "stabtokenizer.h"

// globals
int StabTokenizer::done = 0;
const char *StabTokenizer::wsre = NULL;
const char *StabTokenizer::alphare = NULL;
const char *StabTokenizer::intre = NULL;
const char *StabTokenizer::hexintre = NULL;
const char *StabTokenizer::namere = NULL;
const char *StabTokenizer::nameconre = NULL;
const char *StabTokenizer::nowsnamere = NULL;
const char *StabTokenizer::nowsnameconre = NULL;
const char *StabTokenizer::realre[4];
const char *StabTokenizer::qmre = NULL;
const char *StabTokenizer::strconre = NULL;
const char *StabTokenizer::cwsre = NULL;
const char *StabTokenizer::calphare = NULL;
const char *StabTokenizer::cintre = NULL;
const char *StabTokenizer::chexintre = NULL;
const char *StabTokenizer::cnamere = NULL;
const char *StabTokenizer::cnameconre = NULL;
const char *StabTokenizer::cnowsnamere = NULL;
const char *StabTokenizer::cnowsnameconre = NULL;
const char *StabTokenizer::crealre[4];
const char *StabTokenizer::cqmre = NULL;
const char *StabTokenizer::cstrconre = NULL;
