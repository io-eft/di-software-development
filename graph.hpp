#ifndef __GRAPH_HPP
#define __GRAPH_HPP
#include "tree.hpp"
#include "edge.hpp"
#include "defines.h"
#include <stdlib.h>
#include "entities.hpp"
#include "mythreads.hpp"

struct ResultSet;
class List;

class Graph
{
    struct GraphNode
    {
        Tree* tree;
        int items;
        int nextOverflow;
        int step;

        GraphNode(int i);
        ~GraphNode();
    };

    int buckets;
    int bucketSize;
    int pointer;
    int hashI;
    int hashM;
    GraphNode** table;

    void addBucket();
    void overFlow();
    int hashFunction(int id);

    List* allItems;
    
    void calculateTopStalkers(int k, int centralityMode, Stalkers* st);
    void clearInterests();
    void clearLikes();
    void clearWork();
    void clearUniversity();
    void clearPlace();
    void addEdgeWeights();
    
    //void fillClique(int k, NodeClique* clique, int curr);
    void fillGraphWithCliques(int k, NodeClique* cl, int curr, Graph* cliques, int* numb);
    void addEdgesToCliqueGraph();
    
    public:
        Graph(int m, int c);
        ~Graph();

        void addItem(NodeInt* e, bool overFlowActive = true);
        void print();
	    void insertEdge(int first, Edge* e);
	    void removeItem(int id);

        Node* lookUpItem(int id);
        NodeInt* getItem(int id);

        void destroyGraph();

	    int reachNode1(int first, int second);
	    ResultSet* reachNodeN(int n);
	    bool exists(int id);
	    void printList();
	    int numberOfItems();
	    int numberOfCCs();
	    int maxCC();
	    double density();
	    void clearGraph();
	    void clearEdges();

        void degreeDistribution();
	    int diameter();
	    double averagePathLength();
	    double closenessCentrality(Node* n);
	    double betweennessCentrality(Node* n);
	    double calculateBetweenness(Node* first, int second, int target, int distance, int distanceToTarget);
	    
	    double compareGraphs(Graph* g, int minimum);
	    Matches* matchSuggestion(Node* n, int k, int h, int x, int limit);
	
	    void findTrends(int k, char** womenTrends, char** menTrends);//, Graph* interests);
	    //void findTrends(int k, std::string* womenTrends, std::string* menTrends, Graph* interests);
	    List* getAllItems();
	    void importEdges(Graph* otherG, Interest* inter);
	    void importEdges(Graph* otherG);
	    Graph* getTopStalkers(int k, int x, int centralityMode/*, Graph* g*/, Stalkers* st, List* forum);
	    Graph* buildTrustGraph(int forum);//, Graph* forums);
	    double estimateTrust(Node* a, Node* b);
	    double getTotalNumber();
	    List* cliquePercolationMethod(int k);
	    
	    Graph** getTopNForums(char* fileName, int number);
        Graph* createEdgeGraph();
        void addEdgesToEdgeGraph();
        void addEdgesFromEdgeGraph(List* l);
        Graph* copyGraphWithoutEdges();
        void removeEdgeBetweenNodes(int a, int b);
        Graph* copyGraph();
        Graph* GirvanNewman(double modularity);
        double calculateModularity();
        int totalEdges();
        NodeEdge* getHighestBetweennessEdge();
        void fillListWithCommunities(List* l);
};

struct ResultSet
{
    List* connectedNodes;
    Graph* prevVisited;

    //public:
	ResultSet(int c, int m);
	~ResultSet();
};

struct Pair
{
    int ID;
    int distance;
};

int next(ResultSet* rs, Pair* p);
bool previouslyFound(Graph** g, int id, int curr);

#endif // __GRAPH_HPP
