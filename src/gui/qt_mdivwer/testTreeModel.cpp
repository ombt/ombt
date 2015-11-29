#include <iostream>

#include "TreeModel.h"

int
main(int argc, char *argv[])
{
    for (int arg=0; arg<argc; ++arg)
    {
        std::string filename(argv[arg]);
        TreeModel model(filename);
    }
    return(0);
}
