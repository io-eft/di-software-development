/*
 * GraphLib.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GraphLib.h"
#include "defines.h"

/************************************/
/* Implement for test main purposes */
/************************************/


/**
 * Creates a properties object
 * number: the number of properties
 * return value: a properties object
 */
Properties* createProperties(int number) {
    Properties* p = new Properties(number);
    return p;
}

/**
 * Sets a string property "property" in position "index"
 * property: the type of the property
 * index: the position of the property
 * p: the properties object
 */
void setStringProperty(char* property, int index, Properties* p) {
    p->setStringProperty(property, index);
}

/**
 * Sets an integer property "property" in position "index"
 * property: the value of the property
 * index: the position of the property
 * p: the properties object
 */
void setIntegerProperty(int property, int index, Properties* p) {
    p->setIntegerProperty(property, index);
}

/**
 * Gets a string property "property" from position "index"
 * index: the position of the requested property
 * p: the properties object
 * return value: the type of the property
 */
char* getStringProperty(int index, Properties* p) {
    return p->getStringProperty(index);
}

/**
 * Gets an integer property "property" from position "index"
 * index: the position of the requested property
 * p: the properties object
 * return value: the value of the property
 */
int getIntegerProperty(int index, Properties* p) {
    p->getIntegerProperty(index);
}

/**
 * Creates a node with specific properties
 * id: the id of the node
 * p: the properties of the node
 * return value: the requested node
 */
Node* createNode(int id, Properties* p) {
    Node* n = new Node(id, p);
    return n;
}

/**
 * Creates an edge with specific properties
 * startID: the id of the start node
 * endID: the id of the end node
 * p: the properties of the edge
 * return value: the requested edge
 */
Edge* createEdge(Node* endID, Properties* p) {
    Edge* e = new Edge(endID, p);
    return e;
}


/************************************/
/* Implement for part 1 */
/************************************/

/* Creates an empty graph
 * m: size of hashtable
 * c: size of cells in a bucket
 * return value: an empty graph*/
Graph* createGraph(int m, int c) {
    Graph* g = new Graph(m, c);
    return g;
}

/* Destroys a graph
 * g: the graph
 * return value: success/fail status */
int destroyGraph(Graph* g) {
    //g->destroyGraph();
    delete g;
    return SUCCESS;
}

/* Inserts a node in the graph
 * n: the node to insert
 * g: the graph
 * return value: success/fail status */
int insertNode(/* const */ Node* /* const */ n, Graph* g) {
    g->addItem(n);
    return SUCCESS;
}

/* Inserts an edge in the graph
 * startID: the id of the start node
 * e: the edge we want to add to the start node
 * g: the graph
 * return value: success/fail status */
int insertEdge(int startID, /* const */ Edge* /* const */ e, Graph* g) {
    Node* n = g->lookUpItem(startID);
    if(n == NULL)
        return -1;

    n->addEdge(e);
    return SUCCESS;
}

/* Retrieves a node from the graph
 * nodeID: the if of node to retrieve
 * g: the graph
 * return value: the requested node */
Node* lookupNode(int nodeID, /* const */ Graph* /* const */ g) {
    return g->lookUpItem(nodeID);
}

/* Finds shortest path distance between two nodes in an undirected graph with no edge weights
 * startID: the id of the start node
 * endID: the id of the end node
 * g: the graph
 * return value: the shortest path */
int reachNode1(int startID, int endID, /* const */ Graph* /* const */ g) {
    return g->reachNode1(startID, endID);
}

/* Finds shortest path distance between all reachable nodes from a given node
 * in an undirected graph with no edge weights
 * startID: the id of the start node
 * endID: the id of the end node
 * g: the graph
 * return value: a resultSet */
ResultSet* reachNodeN(int startID, /* const */ Graph* /* const */ g) {
    return g->reachNodeN(startID);
}
/* Checks if another result is available
 * resultSet: the ResultSet returned by a reachNodeN call
 * pair: a Pair instance to hold the next result, the space for the Pair is
 * allocated by the caller
 * return value: 0 when the no more record is available, 1 when
 */

Edge* createEdge(int id1, int id2, Properties* p, Graph* g)
{
    Node* n = g->lookUpItem(id2);
    Edge* e = new Edge(n);
    return e;
}