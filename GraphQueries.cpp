/*
 * GraphQueries.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myfunctions.hpp"
#include "GraphQueries.h"
#include "defines.h"

/* Creates a graph based on schema's relation person_knows_person
 * bucketsNumber: size of hashtable
 * bucketSize: size of cells in a bucket
 * loads also from data file as attributes anything might be useful
 * mustn't save any query/metric estimation
 * return value: the loaded graph*/
Graph* loadGraph(int bucketsNumber, int bucketSize){
    Graph* g = new Graph(bucketsNumber, bucketSize);
    loadPersonsInGraph("./files/person.csv", g);
    loadPersonKnowsPerson("./files/person_knows_person.csv", g);
    return g;
}

/* Get the matches structure from query 1, the requested
 * pair number and returns the person
 * id. If there is no <pairNumber> result return -1
 */
int getMatch(int pairNumber,Matches* match){
  //TODO
  return match[pairNumber].getId();
}

/* Gets the stalker structure from query 2, a
 * position and returns the stalker's person id
 * at current position. If there is no <position> result return -1
*/
int getStalkerID(int position,Stalkers* stalk){
  return stalk[position].getId();
}

/************************************/
/* Implement for metrics purposes */
/************************************/
void degreeDistribution(Graph* g){
  return g->degreeDistribution();
}

int diameter(Graph* g){
  return g->diameter();
}

double averagePathLength(Graph* g){
  //TODO
  return g->averagePathLength();
}

int numberOfCCs(Graph* g){
  //TODO
  return g->numberOfCCs();
}

int maxCC(Graph* g){
  //TODO
  return g->maxCC();
}

double density(Graph* g){
  //TODO
  return g->density();
}

double closenessCentrality(Node* node,Graph* g){
  //TODO
  return g->closenessCentrality(node);
}

double betweennessCentrality(Node* node,Graph* g){
  //TODO
  return g->betweennessCentrality(node);
}

/************************************/
/* Implement for queries purposes */
/************************************/
Matches* matchSuggestion(Node* node, int k, int h, int x, int limit, Graph* g){
  return g->matchSuggestion(node, k, h, x, limit);
}

Graph* getTopStalkers(int k, int x, int centralityMode, Graph* g, Stalkers* st, List* forums){
  return g->getTopStalkers(k, x, centralityMode, st, forums);
}

void findTrends(int k, Graph* g, char** womenTrends, char** menTrends){
  g->findTrends(k, womenTrends, menTrends);
  return;
}

Graph* buildTrustGraph(int forum, Graph* g){
  //TODO
  return g->buildTrustGraph(forum);
}

double estimateTrust(Node* a, Node* b, Graph* trustGraph){
  //TODO
  return trustGraph->estimateTrust(a, b);
}



















