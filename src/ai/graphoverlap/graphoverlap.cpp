
#include <unistd.h>
#include <iostream>
#include <queue>
#include <debug.h>
#include <graph.h>

using namespace ombt;

static bool debug = false;
#define IFDBG() if (debug) 

static void
usage(const std::string &cmd)
{
    std::cout << "usage: " << cmd << " graphfile1 graphfile2" << std::endl;
}

static void
showtransitions(const Graph &g, const std::string &gname)
{
    if (!debug) return;
    std::cout << std::endl;
    std::cout << gname << " transitions: " << std::endl;
    const Graph::Vertices gv = g.getVertices();
    Graph::Vertices::const_iterator vit = gv.begin();
    Graph::Vertices::const_iterator vitend = gv.end();
    for ( ; vit!=vitend; ++vit)
    {
        Graph::VertexId v1 = vit->first;
        std::cout << gname << " vertex: (" << v1 << "," << g.label(v1) << ")" << std::endl;
        Graph::Vertices::const_iterator vit2 = gv.begin();
        Graph::Vertices::const_iterator vit2end = gv.end();
        for ( ; vit2!=vit2end; ++vit2)
        {
            Graph::VertexId v2 = vit2->first;
            if (g.P(v1, v2))
                std::cout << "(" << v1 << "," << g.label(v1) << ") ==>> (" << v2 << "," << g.label(v2) << ")" << std::endl;
        }
    }
}

static void
showfromset(const Graph &g, const std::string &gname)
{
    if (!debug) return;
    std::cout << std::endl;
    std::cout << gname << " from-vertex-set: " << std::endl;
    const Graph::Vertices gv = g.getVertices();
    Graph::Vertices::const_iterator vit = gv.begin();
    Graph::Vertices::const_iterator vitend = gv.end();
    for ( ; vit!=vitend; ++vit)
    {
        Graph::VertexId vid = vit->first;
        std::cout << gname << " vertex: (" << vid << "," << g.label(vid) << ")" << std::endl;
        std::set<Graph::VertexId> fromvids = g.getFromVertices(vid);
        std::set<Graph::VertexId>::const_iterator fvit = fromvids.begin();
        std::set<Graph::VertexId>::const_iterator fvitend = fromvids.end();
        for ( ; fvit!=fvitend; ++fvit)
        {
            std::cout << "(" << vid << "," << g.label(vid) << ") ==>> (" << *fvit << "," << g.label(*fvit) << ")" << std::endl;
        }
    }
}

static void
showtoset(const Graph &g, const std::string &gname)
{
    if (!debug) return;
    std::cout << std::endl;
    std::cout << gname << " to-vertex-set: " << std::endl;
    const Graph::Vertices gv = g.getVertices();
    Graph::Vertices::const_iterator vit = gv.begin();
    Graph::Vertices::const_iterator vitend = gv.end();
    for ( ; vit!=vitend; ++vit)
    {
        Graph::VertexId vid = vit->first;
        std::cout << gname << " vertex: (" << vid << "," << g.label(vid) << ")" << std::endl;
        std::set<Graph::VertexId> tovids = g.getToVertices(vid);
        std::set<Graph::VertexId>::const_iterator tvit = tovids.begin();
        std::set<Graph::VertexId>::const_iterator tvitend = tovids.end();
        for ( ; tvit!=tvitend; ++tvit)
        {
            std::cout << "(" << vid << "," << g.label(vid) << ") <<== (" << *tvit << "," << g.label(*tvit) << ")" << std::endl;
        }
    }
}

struct PQItem {
public:
    PQItem(Graph::VertexId vid = -1, long priority = -1, 
           bool reverse = false):
           vid_(vid), priority_(priority), reverse_(reverse) { }
    PQItem(const PQItem &pqi):
           vid_(pqi.vid_), priority_(pqi.priority_), 
           reverse_(pqi.reverse_) { }
    ~PQItem() { }
    PQItem &operator=(const PQItem &pqi) {
        if (this != &pqi) {
            vid_ = pqi.vid_;
            priority_ = pqi.priority_;
            reverse_ = pqi.reverse_;
        }
        return(*this);
    }
    bool operator<(const PQItem &rhs) const {
        if (reverse_)
            return(rhs.priority_ < priority_);
        else
            return(priority_ < rhs.priority_);
    }

    Graph::VertexId vid_;
    long priority_;
    bool reverse_;
};

struct PQItem2 {
public:
    PQItem2(Graph::VertexId vid = -1, std::string label = "", 
            long numberOfEdges = -1, bool reverse = false):
            vid_(vid), label_(label), 
            numberOfEdges_(numberOfEdges), reverse_(reverse) { }
    PQItem2(const PQItem2 &pqi):
            vid_(pqi.vid_), label_(pqi.label_), 
            numberOfEdges_(pqi.numberOfEdges_), 
            reverse_(pqi.reverse_) { }
    ~PQItem2() { }
    PQItem2 &operator=(const PQItem2 &pqi2) {
        if (this != &pqi2) {
            vid_ = pqi2.vid_;
            label_ = pqi2.label_;
            numberOfEdges_ = pqi2.numberOfEdges_;
            reverse_ = pqi2.reverse_;
        }
        return(*this);
    }
    bool operator<(const PQItem2 &rhs) const {
        if (reverse_)
        {
            if ((rhs.label_ < label_) ||
                ((rhs.label_ == label_) &&
                 (rhs.numberOfEdges_ < numberOfEdges_)))
                return(true);
            else
                return(false);
        }
        else
        {
            if ((label_ < rhs.label_) ||
                ((label_ == rhs.label_) &&
                 (numberOfEdges_ < rhs.numberOfEdges_)))
                return(true);
            else
                return(false);
        }
    }

    Graph::VertexId vid_;
    std::string label_;
    long numberOfEdges_;
    bool reverse_;
};

static void
printpq(const std::string &msg, Graph &g, std::priority_queue<PQItem> &pq)
{
    std::cout << std::endl;
    std::cout << msg << std::endl;
    while (!pq.empty())
    {
        std::cout << " (VID:";
        std::cout << "(" << pq.top().vid_ << "," << g.label(pq.top().vid_) << ")";
        std::cout << ",PQ:", 
        std::cout << pq.top().priority_;
        std::cout << ")";
        pq.pop();
    }
    std::cout << std::endl;
}

static void
graphOverlap(
    Graph &g1, Graph::VertexId g1vid, std::set<Graph::VertexId> &g1visited,
    Graph &g2, Graph::VertexId g2vid, std::set<Graph::VertexId> &g2visited,
    Graph &g1only, Graph &g2only, Graph &g1g2overlap)
{
    // have we been here before?
    if ((g1visited.find(g1vid) != g1visited.end()) ||
        (g2visited.find(g2vid) != g2visited.end())) return;

    // first check -- labels must be the same
    if (g1.label(g1vid) != g2.label(g2vid)) return;

    // second check -- number of transitions must be the same.
    std::set<Graph::VertexId> g1toset = g1.getToVertices(g1vid);
    std::set<Graph::VertexId> g2toset = g2.getToVertices(g1vid);
    if (g1toset.size() != g2toset.size()) return;

    // passed two simple tests
    IFDBG() std::cout << std::endl;
    IFDBG() std::cout << "Simple tests passed:" << std::endl;
    IFDBG() std::cout << "VID: G1:" << g1vid << " G2: " << g2vid << std::endl;
    IFDBG() std::cout << "LABEL: G1:" << g1.label(g1vid) << " G2: " << g2.label(g2vid) << std::endl;
    IFDBG() std::cout << "EDGES: G1:" << g1.getToVertices(g1vid).size() << " G2: " << g2.getToVertices(g2vid).size() << std::endl;

    // fill up two priority queues for further checking.
    std::priority_queue<PQItem2> g1pq2;
    std::set<Graph::VertexId>::iterator g1tosetit =  g1toset.begin();
    std::set<Graph::VertexId>::iterator g1tosetitend =  g1toset.end();
    for ( ; g1tosetit != g1tosetitend; ++g1tosetit)
    {
        Graph::VertexId vid = *g1tosetit;
        g1pq2.push(PQItem2(vid, g1.label(vid), 
                   g1.getToVertices(vid).size()));
    }
    std::priority_queue<PQItem2> g2pq2;
    std::set<Graph::VertexId>::iterator g2tosetit =  g2toset.begin();
    std::set<Graph::VertexId>::iterator g2tosetitend =  g2toset.end();
    for ( ; g2tosetit != g2tosetitend; ++g2tosetit)
    {
        Graph::VertexId vid = *g2tosetit;
        g2pq2.push(PQItem2(vid, g2.label(vid), 
                   g2.getToVertices(vid).size()));
    }

    // next level of checks
    std::priority_queue<PQItem2> localg1pq2;
    std::priority_queue<PQItem2> localg2pq2;
    for (localg1pq2=g1pq2, localg2pq2=g2pq2; 
        !localg1pq2.empty() && !localg2pq2.empty(); 
         localg1pq2.pop(), localg2pq2.pop())
    {
        if (localg1pq2.top().label_ != localg2pq2.top().label_)
        {
            IFDBG() std::cout << std::endl;
            IFDBG() std::cout << "NO MATCH for :" << g1vid << "," << g2vid << std::endl;
            return;
        }
    }
    IFDBG() std::cout << std::endl;
    IFDBG() std::cout << "MATCH for :" << g1vid << "," << g2vid << std::endl;
    g1visited.insert(g1vid);
    g2visited.insert(g2vid);
    g1g2overlap.insertVertex(g1vid, g1.label(g1vid), false);

    // traverse children
    for (localg1pq2=g1pq2, localg2pq2=g2pq2; 
        !localg1pq2.empty() && !localg2pq2.empty(); localg1pq2.pop(), localg2pq2.pop())
    {
        Graph::VertexId tog1vid = localg1pq2.top().vid_;
        Graph::VertexId tog2vid = localg2pq2.top().vid_;
        g1g2overlap.insertVertex(tog1vid, g1.label(tog1vid), false);
        g1g2overlap.insertEdge(tog1vid, g1vid);
        graphOverlap(g1, tog1vid, g1visited,
                              g2, tog2vid, g2visited,
                              g1only, g2only, g1g2overlap);
    }
}

static void
graphOverlap(
    Graph &g1, const std::priority_queue<PQItem> g1pq,
    Graph &g2, const std::priority_queue<PQItem> g2pq,
    Graph &finalg1only, Graph &finalg2only, Graph &finalg1g2overlap)
{
    long finalg1g2ovsz = finalg1g2overlap.getVertices().size();

    for (std::priority_queue<PQItem> localg1pq(g1pq);
         !localg1pq.empty(); localg1pq.pop())
    {
        Graph::VertexId g1vid = localg1pq.top().vid_;
        if (localg1pq.top().priority_ < finalg1g2ovsz)
        {
            // size of overlap is greater than the 
            // number of connected vertices from 
            // current vertex. stop.
            break;
        }

        for (std::priority_queue<PQItem> localg2pq(g2pq);
             !localg2pq.empty(); localg2pq.pop())
        {
            Graph::VertexId g2vid = localg2pq.top().vid_;
            if (localg2pq.top().priority_ < finalg1g2ovsz)
            {
                // size of overlap is greater than the 
                // number of connected vertices from 
                // current vertex. stop.
                break;
            }

            Graph g1only;
            Graph g2only;
            Graph g1g2overlap;
            std::set<Graph::VertexId> g1visited;
            std::set<Graph::VertexId> g2visited;
            graphOverlap(g1, g1vid, g1visited,
                                  g2, g2vid, g2visited,
                                  g1only, g2only, g1g2overlap);

            long g1g2ovsz = g1g2overlap.getVertices().size();
            if (g1g2ovsz > finalg1g2ovsz)
            {
                // new overlap size greater than current known.
                finalg1only = g1only;
                finalg2only = g2only;
                finalg1g2overlap = g1g2overlap;
                finalg1g2ovsz = finalg1g2overlap.getVertices().size();
            }
        }
    }
}

static void
graphOverlap(Graph &g1, Graph &g2, 
    Graph &finalg1only, Graph &finalg2only, Graph &finalg1g2overlap)
{
    std::priority_queue<PQItem> g1topq;
    std::priority_queue<PQItem> g1frompq;
    std::priority_queue<PQItem> g1revtopq;
    std::priority_queue<PQItem> g1revfrompq;

    std::cout << std::endl;

    long visited;
    const Graph::Vertices g1v = g1.getVertices();
    Graph::Vertices::const_iterator g1vit = g1v.begin();
    Graph::Vertices::const_iterator g1vitend = g1v.end();
    for ( ; g1vit!=g1vitend; ++g1vit)
    {
        Graph::VertexId g1vid = g1vit->first;
	std::cout << std::endl;
        std::cout << "G1 Vertex: (" << g1vid << "," << g1.label(g1vid) << ")";
        std::cout << std::endl;

        visited = 0;
        g1.connectedViaTo(g1vid, visited, true);
        std::cout << "Vertices visited Via To-Links: " << visited;
        std::cout << std::endl;
        g1topq.push(PQItem(g1vid, visited));
        g1revtopq.push(PQItem(g1vid, visited, true));

        visited = 0;
        g1.connectedViaFrom(g1vid, visited, true);
        std::cout << "Vertices visited Via From-Links: " << visited;
        std::cout << std::endl;
        g1frompq.push(PQItem(g1vid, visited));
        g1revfrompq.push(PQItem(g1vid, visited, true));
    }

    std::priority_queue<PQItem> dummypq;
    dummypq = g1topq;
    printpq("G1 To PQ:", g1, dummypq);
    dummypq = g1revtopq;
    printpq("G1 Reverse To PQ:", g1, dummypq);
    dummypq = g1frompq;
    printpq("G1 From PQ:", g1, dummypq);
    dummypq = g1revfrompq;
    printpq("G1 Reverse From PQ:", g1, dummypq);

    std::priority_queue<PQItem> g2topq;
    std::priority_queue<PQItem> g2frompq;
    std::priority_queue<PQItem> g2revtopq;
    std::priority_queue<PQItem> g2revfrompq;

    const Graph::Vertices g2v = g2.getVertices();
    Graph::Vertices::const_iterator g2vit = g2v.begin();
    Graph::Vertices::const_iterator g2vitend = g2v.end();
    for ( ; g2vit!=g2vitend; ++g2vit)
    {
        Graph::VertexId g2vid = g2vit->first;
	std::cout << std::endl;
        std::cout << "G2 Vertex: (" << g2vid << "," << g2.label(g2vid) << ")";
        std::cout << std::endl;

        visited = 0;
        g2.connectedViaTo(g2vid, visited, true);
        std::cout << "Vertices visited Via To-Links: " << visited;
        std::cout << std::endl;
        g2topq.push(PQItem(g2vid, visited));
        g2revtopq.push(PQItem(g2vid, visited, true));

        visited = 0;
        g2.connectedViaFrom(g2vid, visited, true);
        std::cout << "Vertices visited Via From-Links: " << visited;
        std::cout << std::endl;
        g2frompq.push(PQItem(g2vid, visited));
        g2revfrompq.push(PQItem(g2vid, visited, true));
    }

    dummypq = g2topq;
    printpq("G2 To PQ:", g1, dummypq);
    dummypq = g2revtopq;
    printpq("G2 Reverse To PQ:", g1, dummypq);
    dummypq = g2frompq;
    printpq("G2 From PQ:", g2, dummypq);
    dummypq = g2revfrompq;
    printpq("G2 Reverse From PQ:", g2, dummypq);

    std::cout << std::endl << "G1 TO-PQ, G2 TO-PQ" << std::endl;
    finalg1only.clear();
    finalg2only.clear();
    finalg1g2overlap.clear();
    graphOverlap(g1, g1topq, g2, g2topq, 
                          finalg1only, finalg2only, finalg1g2overlap);
    if (finalg1g2overlap.empty())
        std::cout << "==>> G1-G2-overlap is empty!" << std::endl;
    else
        std::cout << "==>> G1-G2-overlap graph:" << finalg1g2overlap << std::endl;

    std::cout << std::endl << "G1 TO-PQ, G2 FROM-PQ" << std::endl;
    finalg1only.clear();
    finalg2only.clear();
    finalg1g2overlap.clear();
    graphOverlap(g1, g1topq, g2, g2frompq,
                          finalg1only, finalg2only, finalg1g2overlap);
    if (finalg1g2overlap.empty())
        std::cout << "==>> G1-G2-overlap is empty!" << std::endl;
    else
        std::cout << "==>> G1-G2-overlap graph:" << finalg1g2overlap << std::endl;

    std::cout << std::endl << "G1 FROM-PQ, G2 TO-PQ" << std::endl;
    finalg1only.clear();
    finalg2only.clear();
    finalg1g2overlap.clear();
    graphOverlap(g1, g1frompq, g2, g2topq,
                          finalg1only, finalg2only, finalg1g2overlap);
    if (finalg1g2overlap.empty())
        std::cout << "==>> G1-G2-overlap is empty!" << std::endl;
    else
        std::cout << "==>> G1-G2-overlap graph:" << finalg1g2overlap << std::endl;

    std::cout << std::endl << "G1 FROM-PQ, G2 FROM-PQ" << std::endl;
    finalg1only.clear();
    finalg2only.clear();
    finalg1g2overlap.clear();
    graphOverlap(g1, g1frompq, g2, g2frompq,
                          finalg1only, finalg2only, finalg1g2overlap);
    if (finalg1g2overlap.empty())
        std::cout << "==>> G1-G2-overlap is empty!" << std::endl;
    else
        std::cout << "==>> G1-G2-overlap graph:" << finalg1g2overlap << std::endl;
}

int
main(int argc, char **argv)
{
    // check args
    for (int c=0; (c=getopt(argc, argv, "d")) != EOF; )
    {
        switch (c)
        {
        case 'd':
            debug = true;
            break;
        }
    }
    if ((argc - optind) != 2)
    {
        usage(argv[0]);
        return(2);
    }

    // init graphs from files
    Graph g1(argv[optind]);
    Graph g2(argv[optind+1]);

    // show transistions
    showtransitions(g1, "G1");
    showfromset(g1, "G1");
    showtoset(g1, "G1");
    showtransitions(g2, "G2");
    showfromset(g2, "G2");
    showtoset(g2, "G2");

    // are graphs at least connected?
    std::cout << std::endl;
    if (g1.connected())
    {
        std::cout << "Graph G1 is connected." << std::endl;
    }
    else
    {
        std::cout << "Graph G1 is NOT connected." << std::endl;
        return(2);
    }
    if (g2.connected())
    {
        std::cout << "Graph G2 is connected." << std::endl;
    }
    else
    {
        std::cout << "Graph G2 is NOT connected." << std::endl;
        return(2);
    }

    // crunch time !!!
    Graph g1only;
    Graph g2only;
    Graph g1g2overlap;
    graphOverlap(g1, g2, g1only, g2only, g1g2overlap);

    // print results
    std::cout << std::endl;
    if (g1only.empty())
        std::cout << "==>> G1-only is empty!" << std::endl;
    else
        std::cout << "==>> G1-only graph:" << g1only << std::endl;
    if (g2only.empty())
        std::cout << "==>> G2-only is empty!" << std::endl;
    else
        std::cout << "==>> G2-only graph:" << g2only << std::endl;
    if (g1g2overlap.empty())
        std::cout << "==>> G1-G2-overlap is empty!" << std::endl;
    else
        std::cout << "==>> G1-G2-overlap graph:" << g1g2overlap << std::endl;

    return(0);
}
