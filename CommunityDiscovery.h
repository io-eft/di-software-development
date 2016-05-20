/**
 * CommunityDiscovery.h
 */

#ifndef COMMUNITY_DISCOVERY_H_
#define COMMUNITY_DISCOVERY_H_

#include "header.h"
#include "Graph.h"

Cpm** computeCPMResults(Graph *graph, int *cliqueSize);
Gn** computeGNResults(Graph *graph, double maxModularity);


#endif
