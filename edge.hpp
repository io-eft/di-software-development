#ifndef __EDGE_HPP
#define __EDGE_HPP
#include <iostream>
#include "node.hpp"

class Edge
{
    Node* pointsTo;
    Properties* properties;
    double weight;

    public:
        Edge();
        Edge(Node* i);
        Edge(Node* i, Properties* p);
        virtual ~Edge();

	int getId();
	void printEdgeProperties();
	Node* getNode();
	void addWeight(double d);
	double getWeight();

};

#endif // __EDGE__HPP
