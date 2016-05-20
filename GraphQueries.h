/*
 * GraphQueries.h
 *
  */

#ifndef GRAPHQUERIES_H_
#define GRAPHQUERIES_H_

#include "GraphLib.h"

/**
 * Matches
 
struct matches {
    //TODO
};
typedef struct matches Matches;

/**
 * Stalkers
 *
struct stalkers {
    //TODO
};
typedef struct stalkers Stalkers;
*/
//help functions
Graph* loadGraph(int bucketsNumber, int bucketSize);
int getStalkerID(int position,Stalkers* stalk);
int getMatch(int pairNumber,Matches* match);

//metrics functions
void degreeDistribution(Graph* g);
int diameter(Graph* g);
double averagePathLength(Graph* g);
int numberOfCCs(Graph* g);
int maxCC(Graph* g);
double density(Graph* g);
double closenessCentrality(Node* node,Graph* g);
double betweennessCentrality(Node* node,Graph* g);

//queries functions
Matches* matchSuggestion(Node* node, int k, int h, int x, int limit, Graph* g);
Graph* getTopStalkers(int k, int x, int centralityMode, Graph* g, Stalkers* st, List* l);
void findTrends(int k, Graph* g, char** womenTrends, char** menTrends);
Graph* buildTrustGraph(int forum, Graph* g);
double estimateTrust(Node* a, Node* b, Graph* trustGraph);

#endif /* GRAPHQUERIES_H_ */



