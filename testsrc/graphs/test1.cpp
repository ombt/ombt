
#include <iostream>
#include "graphs/Graph.h"

using namespace ombt;

static 
void usage(const char *cmd)
{
    std::cout << "usage: " << cmd << " graph1 gra[h2" << std::endl;
}

int
main(int argc, char **argv)
{
    // check args
    if (argc != 3)
    {
        usage(argv[0]);
        return(2);
    }

    // init graphs from files
    int arg = 0;
    Graph g1(argv[++arg]);
    Graph g2(argv[++arg]);

    // print results
    std::cout << std::endl;
    std::cout << "==>> graph 1:" << g1 << std::endl;
    std::cout << "==>> graph 2:" << g2 << std::endl;

    return(0);
}
