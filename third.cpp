/*
 ============================================================================
 Name        : SD-2014-Part-1.c
 Version     :
 Copyright   : Your copyright notice
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GraphLib.h"
#include "myfunctions.hpp"
#include "defines.h"
#include "list.hpp"

#define PERSON_PROPERTIES_NUM 3
#define PERSON_REL_PROPERTIES_NUM 2

#define CHECK(text, actual, expected)                         \
  if (actual != expected) {                                   \
    printf("%-30s: Failed | actual = %3d, expected = %3d\n",  \
           text,                                              \
           actual,                                            \
           expected);                                         \
  } else {                                                    \
    printf("%-30s: Success\n", text);                     \
  }


/* Creates a node person and sets its properties */
Node* setPersonProperties(int id, char* name, char* surname, int age);
/* Creates an edge between two persons and sets its properties */
Edge* setEdgeProperties(int startNodeID, int endNodeID, char* type, int weight, Graph* g);
/* Prints a person's properties */
void printPersonProperties(Node* n);
/* Prints an edge's properties */
void printEdgeProperties(int startID, Edge* n);

int main(int argc, char* argv[]){//(void) {

    int m = 2;
    int c = 3;
    int numberOfForums = 3;
    std::cout << "This is the one!" << std::endl;

    /*create empty graph*/
    //Graph* g = createGraph(m, c);

    Graph* g1 = createGraph(m, c);
    //Graph* places = createGraph(m, c);
    //Graph* interests = createGraph(m,c);
    //Graph* organisations = createGraph(m,c);

    std::cout << "Loading Persons" << std::endl;
    loadPersonsInGraph("./files/person.csv", g1);
    std::cout << "Loaded Persons" << std::endl;
    /*std::cout << "Loading Interests" << std::endl;
    loadInterests("./files/tag.csv", interests);
    std::cout << "Loading Places" << std::endl;
    loadPlaces("./files/place.csv", places);
    std::cout << "Loading Organisations" << std::endl;
    loadOrganisations("./files/organisation.csv", organisations);*/

    std::cout << "Loading Person Knows Person" << std::endl;
    loadPersonKnowsPerson("./files/person_knows_person.csv", g1);
    std::cout << "Loaded Person Knows Person" << std::endl;
/*    Graph** forumGraphs = g1->getTopNForums("./files/forum.csv", numberOfForums);

    for(int i = 0; i < numberOfForums; i++)
    {
        std::cout << "Items of " << i << ": " << forumGraphs[i]->numberOfItems() << std::endl;
        delete forumGraphs[i];
    }
    delete[] forumGraphs;
*/
    //std::cout << "Graph items: " << g1->numberOfItems() << std::endl;

    /*Node* bn = g1->lookUpItem(8899);
    if(bn == NULL)
        std::cout << "WTF?" << std::endl;
    else{

    double betw = g1->betweennessCentrality(bn);
    std::cout << "Betweenness: " << betw << " (target = 0.306)" << std::endl; 
    }*/
    int betweennessIds[5] = {1734,   38,     8899,   9900,   75};
    float betweennessIdsRes[5] = {  0.306,  0.053,  0.018,  0.005,  0.000};

    for (int i = 0; i < 5; ++i) {
        int nodeID = betweennessIds[i];
        Node* node = g1->lookUpItem(nodeID);
        double betwCentrty = g1->betweennessCentrality(node);
        std::cout << "Betweenness: " << betwCentrty << " - target = " << betweennessIdsRes[i] << std::endl; 
        //CHECKDOUBLE("Graph betweenness centrality ", betwCentrty, betweennessIdsRes[i]);
    }    

    Graph* gn = g1->GirvanNewman(0.05);
    std::cout << g1->numberOfItems() << std::endl;
    std::cout << gn->numberOfItems() << std::endl;
    std::cout << gn->diameter() << std::endl;
    std::cout << g1->diameter() << std::endl;
    delete gn;
    /*NodeClique nq(4, 4);
    Node* n = g1->lookUpItem(6906);
    if(nq.addMember(n))
        std::cout << "Member Added 1" << std::endl;
    n = g1->lookUpItem(977);
    if(nq.addMember(n))
        std::cout << "Member Added 2" << std::endl;
    n = g1->lookUpItem(4909);
    if(nq.addMember(n))
        std::cout << "Member Added 3" << std::endl;
    n = g1->lookUpItem(8443);
    if(nq.addMember(n))
        std::cout << "Member Added 4" << std::endl;

    nq.print();*/


    //List* l = g1->cliquePercolationMethod(3);
    //delete l;
    /*Graph* trustGraph;
    int forumID = 34680;

    trustGraph = g1->buildTrustGraph(forumID);

    int trustANodeId = 30;
    int trustBNodeId = 9805;
    int trustCNodeId = 9700;
    int trustDNodeId = 25;
    Node *ta = lookupNode(trustANodeId, trustGraph);
    Node *tb = lookupNode(trustBNodeId, trustGraph);
    Node *tc = lookupNode(trustCNodeId, trustGraph);
    Node *td = lookupNode(trustDNodeId, trustGraph);
    double trustAB;
    trustAB = trustGraph->estimateTrust(ta, tb);
    printf("Trust between nodes (%d,%d) is %f\n", trustANodeId, trustBNodeId, trustAB);
    //CHECKDOUBLE("Trust: (30,9805) ", trustAB, 0.134);

    double trustAC;
    trustAC = trustGraph->estimateTrust(ta, tc);
    printf("Trust between nodes (%d,%d) is %f\n", trustANodeId, trustCNodeId, trustAC);

    double trustAD;
    trustAD = trustGraph->estimateTrust(ta, td);
    printf("Trust between nodes (%d,%d) is %f\n", trustANodeId, trustDNodeId, trustAD);
    //CHECKDOUBLE("Trust: (30,9700) ", trustAC, 0.15);
    //std::cout << "List size " << l->numberOfItems() << std::endl;
    delete trustGraph;*/  
    /*Graph* edgeGraph = g1->createEdgeGraph();
    List* l = edgeGraph->getAllItems();
    l->printAll();*/
    //g1->removeEdgeBetweenNodes(25, 30);
    /*std::cout << "Graph items: " << g1->numberOfItems() << std::endl;
    std::cout << "Graph diameter: " << g1->diameter() << std::endl;
    std::cout << "Graph maxCC: " << g1->maxCC() << std::endl;
    std::cout << "Graph CCs: " << g1->numberOfCCs() << std::endl;
    */

  //  List* l = g1->cliquePercolationMethod(3);
    //std::cout << "Done" << std::endl;
    //delete l;


    //g1->removeItem(25);
    /*Graph* edgeGraph = g1->createEdgeGraph();
    std::cout << "Graph items: " << edgeGraph->numberOfItems() << std::endl;
    std::cout << "Graph diameter: " << edgeGraph->diameter() << std::endl;
    std::cout << "Graph maxCC: " << edgeGraph->maxCC() << std::endl;
    Graph* g = g1->copyGraphWithoutEdges();
    *///List* l = edgeGraph->getAllItems();
    //l->printAll();
    /*std::cout << "Graph diameter: " << g->diameter() << std::endl;
    std::cout << "Graph maxCC: " << g->maxCC() << std::endl;
    std::cout << "Number of edges: " << g->totalEdges() << std::endl;
    std::cout << "Graph items: " << g->numberOfItems() << std::endl;
    *///
    /*for(int i = 20; i < 80; i++)
        edgeGraph->removeItem(i);

    
    g->addEdgesFromEdgeGraph(edgeGraph->getAllItems());
    std::cout << "Graph diameter: " << g->diameter() << std::endl;
    std::cout << "Graph items: " << g->numberOfItems() << std::endl;
    std::cout << "Graph maxCC: " << g->maxCC() << std::endl;
    std::cout << "Graph CCs: " << g1->numberOfCCs() << std::endl;

    delete g;
    delete edgeGraph;
    */destroyGraph(g1);
    //CHECK("Number of pairs in the ResultSet", counter, 10);
    
    
    return EXIT_SUCCESS;
}

Node* setPersonProperties(int id, char* name, char* surname, int age) {

    /*create properties*/
    Properties* prop = createProperties(PERSON_PROPERTIES_NUM);
    setStringProperty(name, 0, prop);
    setStringProperty(surname, 1, prop);
    setIntegerProperty(age, 2, prop);

    /*create node*/
    Node* n = createNode(id, prop);

    return n;
}

Edge* setEdgeProperties(int startNodeID, int endNodeID, char* type, int weight, Graph* g) {

    /*create edge properties*/
    Properties* propEdge = createProperties(PERSON_REL_PROPERTIES_NUM);
    setStringProperty(type, 0, propEdge);
    setIntegerProperty(weight, 1, propEdge);

    /*create an edge*/
    Node* endNode = g->lookUpItem(endNodeID);
    Edge* e = createEdge(endNode, propEdge);
    return e;
}

void printPersonProperties(Node* n) {
    n->printProperties();
}

void printEdgeProperties(int startID, Edge* e) {
    e->printEdgeProperties();
}
