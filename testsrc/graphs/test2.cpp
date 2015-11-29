
#include <iostream>
#include <graphs/Graph.h>
#include <graphs/Algorithms.h>

using namespace ombt;

static 
void usage(const char *cmd)
{
    std::cout << "usage: " << cmd << " graph" << std::endl;
}

int
main(int argc, char **argv)
{
    if (argc != 2)
    {
        usage(argv[0]);
        return(2);
    }

    int arg = 0;
    Graph g1(argv[++arg]);
    const Graph::Vertices &vertices = g1.getVertices();

    std::cout << std::endl;
    std::cout << "==>> graph:" << g1 << std::endl;

    std::cout << std::endl;
    std::cout << "==>> DFS of graph:" << std::endl;
    depthFirstTraversal(g1, true);

    std::cout << std::endl;
    std::cout << "==>> DFS of graph (version 2):" << std::endl;
    depthFirstTraversal2(g1, true);

    Graph::Vertices::const_iterator vit = vertices.begin();
    Graph::Vertices::const_iterator vitend = vertices.end();
    for ( ; vit != vitend; ++vit)
    {
        Graph::VertexId vid = vit->first;

        std::cout << std::endl;
        std::cout << "==>> DFS of graph for ID:" << vid << std::endl;
        depthFirstTraversal(g1, vid, true);
    }

    std::cout << std::endl;
    std::cout << "==>> BFS of graph:" << std::endl;
    breadthFirstTraversal(g1, true);

    std::cout << std::endl;
    std::cout << "==>> BFS of graph (version 2):" << std::endl;
    breadthFirstTraversal2(g1, true);

    vit = vertices.begin();
    vitend = vertices.end();
    for ( ;vit != vitend; ++vit)
    {
        Graph::VertexId vid = vit->first;

        std::cout << std::endl;
        std::cout << "==>> BFS of graph for ID:" << vid << std::endl;
        breadthFirstTraversal(g1, vid, true);
    }

    return(0);
}
