#ifndef __GRAPH_H
#define __GRAPH_H

#include <assert.h>

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

#include <debug.h>
#include <tokenizer.h>

namespace ombt {

class Graph {
public:
    // data types
    typedef long VertexId;

    struct Vertex {
        Vertex(): 
            id_(-1), visited_(false), label_("") { }
        Vertex(VertexId id, const std::string &label): 
            id_(id), visited_(false), label_(label) { }
        Vertex(const Vertex &v): 
            id_(v.id_), visited_(v.visited_), label_(v.label_) { }
        ~Vertex() { } 
        Vertex &operator=(const Vertex &v) {
            if (this != &v) {
                id_ = v.id_;
                visited_ = v.visited_;
                label_ = v.label_;
            }
            return(*this);
        }

        VertexId id_;
        bool visited_;
        std::string label_;
    };
    typedef std::map<VertexId, Vertex> Vertices;

    struct Edge {
        Edge(): exists_(false), color_("") { }
        Edge(bool e, const std::string &c): 
            exists_(e), color_(c) { }
        Edge(const Edge &e): 
            exists_(e.exists_), color_(e.color_) { }
        ~Edge() { }
        Edge &operator=(const Edge &e) {
            if (this != &e) {
                exists_ = e.exists_;
                color_ = e.color_;
            }
            return(*this);
        }

        bool exists_;
        std::string color_;
    };
    typedef std::map<VertexId, Edge> VertexEdges;
    typedef std::map<VertexId, VertexEdges> Edges;

public:
    // ctors and dtor
    Graph();
    Graph(const Graph &src);
    Graph(const std::string &filename);
    ~Graph();

    // operations
    Graph &operator=(const Graph &rhs);
    bool P(VertexId v0, VertexId v1) const;
    std::set<VertexId> P(VertexId v1) const;
    std::set<VertexId> getFromVertices(VertexId v1) const;
    std::set<VertexId> getToVertices(VertexId v1) const;
    const Vertices &getVertices() const { return(vertices_); }
    const std::string color(VertexId v0, VertexId v1) const;
    const std::string label(VertexId vid) const;
    Vertex vertex(VertexId vid) const;
    Edge edge(VertexId v0, VertexId v1) const;
    bool connected();
    void connected(VertexId vid, long &visited, bool clear = false);
    void connectedViaTo(VertexId vid, long &visited, bool clear = false);
    void connectedViaFrom(VertexId vid, long &visited, bool clear = false);

    // build graph
    void clear();
    void insertVertex(VertexId vid, const std::string &color, bool check = true);
    void insertEdge(VertexId fromvid, VertexId tovid, const std::string &color = "white");

    // misc
    bool empty() const { return(vertices_.size() == 0); }
    friend std::ostream &operator<<(std::ostream &os, const Graph &g);
    void clearVisited();

private:
    // utilities
    void readfile(const std::string &filename);
    void parse(const std::string &line, std::map<std::string, std::string> &nvs);
    std::set<VertexId> getVertices(VertexId vid, const Edges &edges) const;

private:
    // data
    static VertexId nextVertexId_;
    int numberOfVertices_;
    int numberOfEdges_;
    Vertices vertices_;
    Edges fromedges_;
    Edges toedges_;
};

}

#endif
