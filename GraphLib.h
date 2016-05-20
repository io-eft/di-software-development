/*
 * GraphLib.h
 *
  */

#ifndef GRAPHLIB_H_
#define GRAPHLIB_H_
#include "graph.hpp"

/*Help functions for test main*/
Properties* createProperties(int number);
void setStringProperty(char* property, int index, Properties* p);
void setIntegerProperty(int property, int index, Properties* p);
char* getStringProperty(int index, Properties* p);
int getIntegerProperty(int index, Properties* p);
Node* createNode(int id, Properties* p);
Edge* createEdge(Node* endID, Properties* p);
Edge* createEdge(int id1, int id2, Properties* p, Graph* g);

/*Functions for implementation of part 1*/
Graph* createGraph(int m, int c);
int destroyGraph(Graph* g);
int insertNode(/* const */ Node* /* const */ n, Graph* g);
int insertEdge(int startID, /* const */ Edge* /* const */ e, Graph* g);
Node* lookupNode(int id, /* const */ Graph* /* const */ g);
int reachNode1(int startID, int endID, /* const */ Graph*  /* const */ g);
ResultSet* reachNodeN(int startID, /* const */ Graph*  /* const */ g);
int next(ResultSet* resultSet, Pair* pair);

#endif /* GRAPHLIB_H_ */
