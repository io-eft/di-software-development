#include "edge.hpp"

Edge::Edge() {}

Edge::Edge(Node* i): pointsTo(i), properties(NULL), weight(0) {}

Edge::Edge(Node* i, Properties* p): pointsTo(i), properties(p), weight(0) {}

Edge::~Edge()
{
    if(properties != NULL)
        delete properties;
}

int Edge::getId()
{
    return pointsTo->getId();
}

void Edge::printEdgeProperties()
{
    properties->printProperties();
}

Node* Edge::getNode()
{
    return pointsTo;
}

void Edge:: addWeight(double d)
{
    weight = d;
}

double Edge::getWeight()
{
    return weight;
}
