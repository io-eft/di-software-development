#include "graph.hpp"
#include "myfunctions.hpp"
#include "edgelist.hpp"
#include "list.hpp"
#include <string.h>
#include <unistd.h>

Graph::GraphNode::GraphNode(int i): items(0), nextOverflow(i), step(i)
{
    tree = new Tree();
}

Graph::GraphNode::~GraphNode()
{
    delete tree;
}

Graph::Graph(int m, int c): buckets(m), bucketSize(c), pointer(0), hashI(0), hashM(m)
{
    table = (GraphNode**)realloc(NULL, m*sizeof(GraphNode*));
    if(table == NULL)
    {
        std::cerr << "Error Allocating Memory!" << std::endl;
        exit(-1);
    }
    for(int i = 0; i < m; i++)
        table[i] = new GraphNode(c);
    allItems = new List();
}

Graph::~Graph()
{
    for(int i = 0; i < buckets; i++)
        delete table[i];
    free(table);
    allItems->clearContent();
    delete allItems;
}

int Graph::hashFunction(int id)
{
    int result = id % (power(2, hashI) * hashM);
    if(result < pointer)
        result = id % (power(2, hashI + 1) * hashM);
    return result;
}

void Graph::addBucket()
{
    buckets++;
    GraphNode** n = (GraphNode**)realloc(table, buckets*sizeof(GraphNode*));
    if(n == NULL)
    {
        std::cerr << "Error allocating memory!" << std::endl;
        for(int i = 0; i < buckets - 1; i++)
            delete table[i];
        free(table);
        exit(-1);
    }
    table = n;
    table[buckets - 1] = new GraphNode(bucketSize);
}

void Graph::addItem(NodeInt* e, bool overFlowActive)
{
    int result = hashFunction(e->getId());
    table[result]->tree->insertItem(e);
    if(table[result]->items == table[result]->nextOverflow)
    {
        if(result != pointer)   //if pointer is not on the table, we certainly will have overflow bucket
            table[result]->nextOverflow = table[result]->nextOverflow + table[result]->step;
        if(overFlowActive)  //if we are in not already in overflow procedure
            overFlow();
    }
        table[result]->items++;
    if(overFlowActive)
        allItems->insertItem(e);
}

void Graph::overFlow()
{
    List l;
    table[pointer]->tree->emptyTree(&l);    //empty the tree to a list
    table[pointer]->items = 0;
    addBucket();
    pointer++;
    if(pointer == hashM*power(2, hashI))
    {
        pointer = 0;
        hashI++;
    }
    while(!l.isEmpty())
    {
        addItem(l.getItem(), false);    //put all items back in the graph
    }
}

void Graph::print()
{
    for(int i = 0; i < buckets; i++)
    {
        std::cout << "Bucket[" << i++ << "]" << std::endl;
        table[i]->tree->printTree();
    }
}

void Graph::destroyGraph()
{
    delete this;
}

void Graph::clearGraph()
{
    for(int i = 0; i < buckets; i++)
    {
        table[i]->items = 0;
        table[i]->tree->clearTree();
    }
    allItems->clearContent();
}

Node* Graph::lookUpItem(int id)
{
    int result = hashFunction(id);
    return (Node*)table[result]->tree->lookUpNode(id);
}

NodeInt* Graph::getItem(int id)
{
    return table[hashFunction(id)]->tree->lookUpNode(id);
}

void Graph::insertEdge(int first, Edge* e)
{
    Node* n = (Node*)table[hashFunction(first)]->tree->lookUpNode(first);
    if(n == NULL)
        delete e;
    else
        n->addEdge(e);
}

int Graph::reachNode1(int first, int second)
{
    if(first == second)
        return 0;
    Node* firstNode = (Node*)table[hashFunction(first)]->tree->lookUpNode(first);   //first node of pair
    Node* secondNode = (Node*)table[hashFunction(second)]->tree->lookUpNode(second);    //second node of pair

    if(firstNode == NULL || secondNode == NULL)
        return INFINITY_REACH_NODE;

    Graph firstVisited(buckets, bucketSize);    //graph for nodes visited by first node
    Graph secondVisited(buckets, bucketSize);   //graph for nodes visited by second node
    NodeDist* fn = new NodeDist(firstNode, 0);
    NodeDist* sn = new NodeDist(secondNode, 0);
    firstVisited.addItem(fn);
    secondVisited.addItem(sn);
    List firstList;     //list for items in queue for first node bfs
    List secondList;    //list for items in queue for 2nd node bfs

    int distance = 1;

    if(firstNode->fillListWithEdges(&firstList, &firstVisited, &secondVisited, distance))   //if you encounter second node on first nodes' edges return 1
        return 1;
    if(secondNode->fillListWithEdges(&secondList, &secondVisited, &firstVisited, distance - 1)) //if you encounter a common node on seconds node edges return 2
        return 2;
    while(!firstList.isEmpty())// && !secondList.isEmpty())
    {   //while there are still items in the first node
        while((firstList.checkTop()) != NULL && (((NodeDist*)firstList.checkTop())->getDistance() == distance))
        {
            fn = (NodeDist*)firstList.getItem();    //get the top item of first list
            if(fn->fillListWithEdges(&firstList, &firstVisited, &secondVisited, distance + 1))
            {   //if you come across a common node, return twice the distance + 1
                delete fn;
                return 2*distance + 1;
            }
            delete fn;
        }
        while((secondList.checkTop() != NULL) && (((NodeDist*)secondList.checkTop())->getDistance() == distance - 1))
        {
            sn = (NodeDist*)secondList.getItem();   //get the top item from the second list
            if(sn->fillListWithEdges(&secondList, &secondVisited, &firstVisited, distance))
            {   //if you come across a common node, return twice the distance + 2
                delete sn;
                return 2*distance + 2;
            }
            delete sn;
        }
        distance++;
    }
    return INFINITY_REACH_NODE;
}

bool Graph::exists(int id)
{
    return table[hashFunction(id)]->tree->exists(id);
}

void Graph::printList()
{
    allItems->printAll();
}

int Graph::numberOfItems()
{
    return allItems->numberOfItems();
}

ResultSet::ResultSet(int m, int c)
{
    connectedNodes = new List();
    prevVisited = new Graph(m, c);
}

ResultSet::~ResultSet()
{
    delete connectedNodes;
    delete prevVisited;
}

ResultSet* Graph::reachNodeN(int nodeId)
{
    Node* n = (Node*)table[hashFunction(nodeId)]->tree->lookUpNode(nodeId);
    ResultSet* result = new ResultSet(buckets, bucketSize); //create a new result set
    if(n == NULL)
        return result;
    NodeDist* nod = new NodeDist(n, 0);
    result->prevVisited->addItem(nod);  //add the first item in the graph
    n->fillListWithEdges(result->connectedNodes, result->prevVisited, 1);   //and the node with his edges

    return result;  //return the resultset
}

int next(ResultSet* rs, Pair* p)
{   //get the next item from the result set
    NodeDist* n = (NodeDist*)rs->connectedNodes->getItem();
    if(n == NULL)   //if the list is empty, return 0
        return 0;
    n->fillListWithEdges(rs->connectedNodes, rs->prevVisited, n->getDistance() + 1);    //else, fill the queue with the edges of the top result
    p->ID = n->getId(); //set the correct prices
    p->distance = n->getDistance();
    delete n;   //free the memory
    return 1;   //and return 1
}

int Graph::numberOfCCs()
{
    Graph visited(buckets, bucketSize);
    List nodes;
    if(allItems->numberOfItems() == 0)
        return 0;
    NodeDist* nod = new NodeDist((Node*)allItems->at(0), 0);
    visited.addItem(nod);
    nod->fillListWithEdges(&nodes, &visited, 0);
    int ccs = 1;
    while(!nodes.isEmpty())
    {
        NodeDist* n = (NodeDist*)nodes.getItem();
        n->fillListWithEdges(&nodes, &visited, 0);
        delete n;
    }
    int index = 1;
    while(visited.numberOfItems() < allItems->numberOfItems())
    {
        ccs++;
        while(visited.exists(allItems->at(index)->getId()))
            index++;
        nod = new NodeDist((Node*)allItems->at(index), 0);
        //std::cout << "In number of CC";
        //nod->print();
        visited.addItem(nod);
        nod->fillListWithEdges(&nodes, &visited, 0);
        while(!nodes.isEmpty())
        {
            NodeDist* n = (NodeDist*)nodes.getItem();
            //n->print();
            n->fillListWithEdges(&nodes, &visited, 0);
            delete n;
        }

    }
    return ccs;
}

int Graph::maxCC()
{
    Graph visited(buckets, bucketSize);
    List nodes;
    if(allItems->numberOfItems() == 0)
        return 0;
    NodeDist* nod = new NodeDist((Node*)allItems->at(0), 0);
    visited.addItem(nod);
    nod->fillListWithEdges(&nodes, &visited, 0);
    while(!nodes.isEmpty())
    {
        NodeDist* n = (NodeDist*)nodes.getItem();
        n->fillListWithEdges(&nodes, &visited, 0);
        delete n;
    }
    int maxItems = visited.numberOfItems();
    int totalItems = maxItems;
    int itemsSoFar = maxItems;
    int index = 1;
    while(visited.numberOfItems() < allItems->numberOfItems())
    {
        while(visited.exists(allItems->at(index)->getId()))
            index++;
        nod = new NodeDist((Node*)allItems->at(index), 0);
        visited.addItem(nod);
        nod->fillListWithEdges(&nodes, &visited, 0);
        while(!nodes.isEmpty())
        {
            NodeDist* n = (NodeDist*)nodes.getItem();
            n->fillListWithEdges(&nodes, &visited, 0);
            delete n;
        }
        totalItems = visited.numberOfItems();
        int currItems = totalItems - itemsSoFar;
        if(currItems > maxItems)
            maxItems = currItems;
        itemsSoFar = totalItems;
    }
    return maxItems;
}

double Graph::density()
{
    int nodes = allItems->numberOfItems();
    double edges = 0;
    for(int i = 0; i < nodes; i++)
    {
        edges += allItems->nodeAt(i)->numberOfEdges();
    }
    return 2*edges / (nodes*(nodes - 1));
}

int Graph::diameter()
{
    int diam = 0;
    int totalNodes = allItems->numberOfItems();
    for(int i = 0; i < totalNodes; i++)
    {
        int first = allItems->at(i)->getId();
        for(int j = i + 1; j < totalNodes; j++)
        {
            int second = allItems->at(j)->getId();
            int temp = reachNode1(first, second);
            if(temp > diam && temp != INFINITY_REACH_NODE)
                diam = temp;
        }
    }
    return diam;
}

double Graph::averagePathLength()
{
    double result = 0;
    double paths = 0;
    int totalNodes = allItems->numberOfItems();
    for(int i = 0; i < totalNodes; i++)
    {
        int first = allItems->at(i)->getId();
        for(int j = i + 1; j < totalNodes; j++)
        {
            int second = allItems->at(j)->getId();
            double temp = reachNode1(first, second);

            if(temp != INFINITY_REACH_NODE)
            {
                result += temp;
                paths++;
            }
        }
    }
    if(paths != 0)
        return result / paths;
    else return -1;
}

double Graph::closenessCentrality(Node* n)
{
    double result = 0;
    int first = n->getId();
    int totalNodes = allItems->numberOfItems();

    for(int i = 0; i < totalNodes; i++)
    {
        int second = allItems->at(i)->getId();

        if(second == first)
            continue;

        double temp = reachNode1(first, second);
        if(temp != INFINITY_REACH_NODE)
            result += 1 / temp;
    }

    if(totalNodes > 1)
        result = result / (totalNodes - 1);
    return result;
}

/*double Graph::betweennessCentrality(Node* n)
{
    if(!this->exists(n->getId()))
        return 0;

    double result = 0;
    int target = n->getId();
    int nodes = allItems->numberOfItems();

    for(int i = 0; i < nodes; i++)
    {
        Node* first = allItems->nodeAt(i);
        if(first->getId() == target)
            continue;

        int distanceToTarget = this->reachNode1(first->getId(), target);
        if(distanceToTarget == INFINITY_REACH_NODE)
            continue;

        for(int j = i + 1; j < nodes; j++)
        {
            int second = allItems->at(j)->getId();
            if(second == target)
                continue;

            int distance = reachNode1(first->getId(), second);
            int targetToSecond = reachNode1(target, second);
            if(distance < distanceToTarget + targetToSecond)
                continue;
            if(distance == INFINITY_REACH_NODE || distance == 1 || distance <= distanceToTarget)
                continue;

            result += calculateBetweenness(first, second, target, distance, distanceToTarget);
        }
    }
    return result / ((nodes-1)*(nodes-2)/2);
}

double Graph::calculateBetweenness(Node* first, int second, int target, int distance, int distanceToTarget)
{
    double validPaths = 0, allPaths = 0;
    int stepsTaken = 0, foundAt = 0;
    bool passed = false;

    List l;
    EdgeList* edgelist = first->getContacts();
    for(int i = 0; i < edgelist->numberOfItems(); i++)
    {
        NodeDistPar* nd = new NodeDistPar(edgelist->nodeAt(i), 1, first->getId());
        l.insertOnTop(nd);
    }
    while(!l.isEmpty())
    {
        NodeDistPar* nd = (NodeDistPar*)l.getItem();
        if(nd->getId() == second)
        {
            allPaths++;
            if(passed)
                validPaths++;
        }
        if(nd->getDistance() <= distanceToTarget)
        {
            //foundAt = 0;
            passed = false;
        }
        if(nd->getId() == target)
        {
            passed = true;
            //foundAt = nd->getDistance();
        }

        if(nd->getDistance() < distance)
            nd->fillStackWithEdges(&l, first->getId());
        delete nd;
    }
    if(allPaths != 0)
        return validPaths / allPaths;
    else
        return 0;
}*/

void Graph::degreeDistribution()
{
    int maxDegree = 0;
    int nodes = allItems->numberOfItems();
    for(int i = 0; i < nodes; i++)
    {
        int temp = allItems->nodeAt(i)->numberOfEdges();
        if(temp > maxDegree)
            maxDegree = temp;
    }
    std::ofstream myfile;
    myfile.open("degrees.dat");
    if(myfile.is_open())
    {
        for(int i = 0; i < maxDegree; i++)
        {
            int degree = 0;
            for(int j = 0; j < nodes; j++)
            {
                if(allItems->nodeAt(j)->numberOfEdges() == i)
                    degree++;
            }
            myfile << degree << std::endl;
            //if(degree != 0)
                //std::cout << "Degree(" << i << "): " << degree << std::endl;
        }
        myfile.close();
    }
    else
        std::cout << "Failed to open file!" << std::endl;
    if(fork() == 0)
    {
        execl("./gnuscript.sh", "./gnuscript.sh", NULL);
        std::cout << "Exec Failed! Exiting.." << std::endl;
        exit(-1);
    }
}

double Graph::compareGraphs(Graph* g, int minimum)
{
    double matches = 0;

    int firstNodes = this->allItems->numberOfItems();
    int secondNodes = g->allItems->numberOfItems();

    if(firstNodes < minimum || secondNodes < minimum)
        return -1;

    for(int i = 0; i < firstNodes; i++)
    {
        if(g->exists(this->allItems->at(i)->getId()))
            matches++;
    }
    if(matches < minimum)
        return -1;

    return matches / (firstNodes + secondNodes - matches);

}

Matches* Graph::matchSuggestion(Node* n, int k, int h, int x, int limit)
{
    Graph* places = new Graph(this->hashM, bucketSize);
    Graph* interests = new Graph(this->hashM, bucketSize);
    Graph* organisations = new Graph(this->hashM, bucketSize);

    loadInterests("./files/tag.csv", interests);
    loadPlaces("./files/place.csv", places);
    loadOrganisations("./files/organisation.csv", organisations);

    loadPersonHasInterest("./files/person_hasInterest_tag.csv", this, interests);
    loadPersonToOrganisation("./files/person_studyAt_organisation.csv", this, organisations, false);
    loadPersonToOrganisation("./files/person_workAt_organisation.csv", this, organisations, true);
    loadPersonToPlaces("./files/person_isLocatedIn_place.csv", this, places);

    Matches* suggestions = new Matches[limit];
    if(n == NULL)
        return suggestions;
    if(!this->exists(n->getId()))
        return suggestions;

    Person* p = (Person*)n;

    ResultSet* rs = this->reachNodeN(n->getId());
    Pair pair;

    List results;// = new List();

    int nAge = p->getAge();
    Organisation* nWork = p->getWork();
    Organisation* nUniv = p->getUniversity();
    std::string nGender = p->getGender();
    Place* nPlace = p->getResidence();
    Graph* nInterests = p->getInterests();

    while(next(rs, &pair))
    {
        if(pair.distance == 1)
            continue;
        if(pair.distance > h)
            break;

        Person* candidate = (Person*)this->lookUpItem(pair.ID);

        if(candidate->getAge() > nAge + x || candidate->getAge() < nAge - x)
            continue;

        if(nGender == candidate->getGender())
            continue;

        bool matchingCandidate = false;
\
        if(nWork != NULL && ((candidate->getWork() != NULL && nWork->getId() == candidate->getWork()->getId()) || (candidate->getUniversity() != NULL && nWork->getId() == candidate->getUniversity()->getId())))
            matchingCandidate = true;

        if(nUniv != NULL && ((candidate->getWork() != NULL && nUniv->getId() == candidate->getWork()->getId()) || (candidate->getUniversity() != NULL && nUniv->getId() == candidate->getUniversity()->getId())))
            matchingCandidate = true;

        if(nPlace != NULL && candidate->getResidence() != NULL && nPlace->getId() == candidate->getResidence()->getId())
            matchingCandidate = true;

        if(!matchingCandidate)
            continue;

        double matchingRating = nInterests->compareGraphs(candidate->getInterests(), k);

        if(matchingRating != -1)
        {
            Matches* m = new Matches(candidate->getId(), matchingRating);
            results.insertMatch(m);
        }

    }
    delete rs;
    for(int i = 0; i < limit; i++)
    {
        Matches* m = (Matches*)results.getItem();
        if(m != NULL)
        {
            suggestions[i].setId(m->getId());
            suggestions[i].setMatchValue(m->getMatchValue());
            delete m;
        }
    }

    clearInterests();
    clearWork();
    clearUniversity();
    clearPlace();

    delete places;
    delete organisations;
    delete interests;

    return suggestions;
}

void Graph::clearEdges()
{
    int nodes = allItems->numberOfItems();
    for(int i = 0; i < nodes; i++)
        allItems->nodeAt(i)->clearEdges();
}

List* Graph::getAllItems()
{
    return allItems;
}

void Graph::findTrends(int k, char** womenTrends, char** menTrends)//, Graph* interests)
{
    Graph* interests = new Graph(this->hashM, this->bucketSize);
    loadInterests("./files/tag.csv", interests);
    loadPersonHasInterest("./files/person_hasInterest_tag.csv", this, interests);
    List womenInterests;
    List menInterests;
    List* allInterests = interests->getAllItems();

    Graph women(this->hashM, this->bucketSize);
    Graph men(this->hashM, this->bucketSize);

    int nodes = this->allItems->numberOfItems();
    for(int i = 0; i < nodes; i++)
    {
        Person* p = new Person(*(Person*)allItems->nodeAt(i));
        if(p->getGender() == "female")
            women.addItem(p);
        else
            men.addItem(p);
    }

    women.clearEdges();
    men.clearEdges();

    nodes = allInterests->numberOfItems();

    for(int i = 0; i < nodes; i++)
    {
        Interest* inter = (Interest*)allInterests->nodeAt(i);
        women.importEdges(this, inter);
        men.importEdges(this, inter);

        int ccEdges = women.maxCC();
        if(ccEdges > 1)
        {
            Matches* wm = new Matches(inter->getId(), ccEdges);
            womenInterests.insertMatch(wm);
        }

        ccEdges = men.maxCC();
        if(ccEdges > 1)
        {
            Matches* mm = new Matches(inter->getId(), ccEdges);
            menInterests.insertMatch(mm);
        }

        women.clearEdges();
        men.clearEdges();
    }

    //womenInterests.printMatches();
    //menInterests.printMatches();

    for(int i = 0; i < k; i++)
    {
        Interest* j = NULL;
        NodeInt* id = womenInterests.at(i);
        if(id != NULL)
            j = (Interest*)interests->lookUpItem(id->getId());
        if(j != NULL)
        {
            womenTrends[i] = new char[j->getName().length() + 1];
            strcpy(womenTrends[i], j->getName().c_str());
        }
        else
            womenTrends[i] == NULL;

        id = menInterests.at(i);
        if(id != NULL)
            j = (Interest*)interests->lookUpItem(id->getId());
        if(j != NULL)
        {
            menTrends[i] = new char[j->getName().length() + 1];
            strcpy(menTrends[i], j->getName().c_str());
        }
        else
            menTrends[i] == NULL;
    }
    this->clearInterests();
    delete interests;
}

void Graph::clearInterests()
{
    for(int i = 0; i < allItems->numberOfItems(); i++)
    {
        Person* p = (Person*)allItems->at(i);
        p->clearInterests();
    }
}

void Graph::importEdges(Graph* otherG, Interest* inter)
{
    int nodes = allItems->numberOfItems();
    for(int i = 0; i < nodes; i++)
    {
        Person* p = (Person*)allItems->nodeAt(i);
        Person* otherP = (Person*)otherG->lookUpItem(p->getId());
        if(!p->hasInterest(inter))
            continue;
        if(otherP == NULL)
            continue;

        EdgeList* otherE = otherP->getContacts();
        int edgeNodes = otherE->numberOfItems();
        for(int j = 0; j < edgeNodes; j++)
        {
            Person* edgePoint = (Person*)otherE->nodeAt(j);
            Person* ep;
            if((ep = (Person*)this->lookUpItem(edgePoint->getId())) != NULL && edgePoint->hasInterest(inter))
            {
                Edge* ed = new Edge(ep);
                p->addEdge(ed);
            }
        }
    }
}

void Graph::importEdges(Graph* otherG)
{
    int nodes = allItems->numberOfItems();
    for(int i = 0; i < nodes; i++)
    {
        Person* p = (Person*)allItems->nodeAt(i);
        Person* otherP = (Person*)otherG->lookUpItem(p->getId());

        if(otherP == NULL)
            continue;

        EdgeList* otherE = otherP->getContacts();
        int edgeNodes = otherE->numberOfItems();
        for(int j = 0; j < edgeNodes; j++)
        {
            Person* edgePoint = (Person*)otherE->nodeAt(j);
            Person* newEdgeP;
            if((newEdgeP = (Person*)this->lookUpItem(edgePoint->getId())) != NULL)
            {
                Edge* ed = new Edge(newEdgeP);
                p->addEdge(ed);
            }
        }
    }
}

Graph* Graph::getTopStalkers(int k, int x, int centralityMode, Stalkers* st, List* forums)
{
    //forums = new List();
    Graph* forumIds = new Graph(3, 3);
    Graph* posts = new Graph(this->hashM, this->bucketSize);
    loadPostsAndCreators("./files/post_hasCreator_person.csv", posts);
    loadLikesToPosts("./files/person_likes_post.csv", posts, this);
    Graph* stalk = new Graph(hashM, bucketSize);
    int nodes = allItems->numberOfItems();
    for(int i = 0; i < nodes; i++)
    {
        Person* p = (Person*)allItems->nodeAt(i);
        if(p->isStalker(x))
        {   //std::cout << "Found a stalker! (" << p->getId() << ")" << std::endl;
            Person* stalker = new Person(*p);
            stalk->addItem(stalker);
        }
    }
    stalk->importEdges(this);

    stalk->calculateTopStalkers(k, centralityMode, st);
    loadForumsForStalkers("./files/forum_hasMember_person.csv", stalk, forumIds);
    loadForumInfoForStalkers("./files/forum.csv", forumIds, forums);
    this->clearLikes();
    delete posts;
    delete forumIds;
    return stalk;
}

void Graph::clearLikes()
{
    for(int i = 0; i < allItems->numberOfItems(); i++)
    {
        Person* p = (Person*)allItems->at(i);
        p->clearLikes();
    }
}

void Graph::clearWork()
{
    for(int i = 0; i < allItems->numberOfItems(); i++)
    {
        Person* p = (Person*)allItems->at(i);
        p->clearWork();
    }
}

void Graph::clearUniversity()
{
    for(int i = 0; i < allItems->numberOfItems(); i++)
    {
        Person* p = (Person*)allItems->at(i);
        p->clearUniversity();
    }
}

void Graph::clearPlace()
{
    for(int i = 0; i < allItems->numberOfItems(); i++)
    {
        Person* p = (Person*)allItems->at(i);
        p->clearPlace();
    }
}

void Graph::calculateTopStalkers(int k, int centralityMode, Stalkers* st)
{
    List l;
    int nodes = allItems->numberOfItems();
    for(int i = 0; i < nodes; i++)
    {
        double cs;
        Person* p = (Person*)allItems->nodeAt(i);
        if(centralityMode == 1)
            cs = closenessCentrality(p);
        else
            cs = betweennessCentrality(p);
        //std::cout << "Stalker: " << p->getId() << ", Centrality: " << cs << std::endl;
        Stalkers* stalk = new Stalkers(p->getId(), cs, "");
        l.insertMatch(stalk);
    }
    for(int i = 0; i < k; i++)
    {
        Stalkers* s = (Stalkers*)l.nodeAt(i);
        if(s == NULL)
            break;

        st[i].setId(s->getId());
        st[i].setMatchValue(s->getMatchValue());
        st[i].setForum(s->getForum());
    }
}

Graph* Graph::buildTrustGraph(int forum)
{
    Graph* result = new Graph(hashM, bucketSize);
    Graph* posts = new Graph(hashM, bucketSize);
    Graph* comments = new Graph(hashM, bucketSize);

    loadForumMembers("./files/forum_hasMember_person.csv", this, result, forum);
    result->importEdges(this);

    loadForumPosts("./files/forum_containerOf_post.csv", posts, forum);

    loadPostHasCreator("./files/post_hasCreator_person.csv", posts);
    loadPostHasComment("./files/comment_replyOf_post.csv", posts, comments);

    loadCommentHasCreator("./files/comment_hasCreator_person.csv", result, comments);
    loadLikesToPosts("./files/person_likes_post.csv", posts, result);

    result->addEdgeWeights();

    delete posts;
    delete comments;

    return result;
}

void Graph::addEdgeWeights()
{
    
    int totalNodes = allItems->numberOfItems();
    for(int i = 0; i < totalNodes; i++)
    {
        Person* p = (Person*)allItems->at(i);
        
        double totalReplies = p->getTotalReplies();
        double totalLikes = p->getTotalLikes();

        EdgeList* el = p->getContacts();

        int edgeNodes = el->numberOfItems();
        for(int j = 0; j < edgeNodes; j++)
        {
            Edge* edge = el->at(j);
            double repliesForEdge = p->getRepliesTo(edge->getId());
            double likesForEdge = p->getLikesTo(edge->getId());

            double weight;
            if(totalLikes == 0 && totalReplies == 0)
                weight = 0;
            else
            {
                if(totalLikes == 0)
                    weight = 0.7 * repliesForEdge / totalReplies;
                if(totalReplies == 0)
                    weight = 0.3 * likesForEdge / totalLikes;
                if(totalLikes != 0 && totalReplies != 0)
                    weight = (0.3 * likesForEdge / totalLikes) + (0.7 * repliesForEdge / totalReplies);
            }
            edge->addWeight(weight);
        }
    }
}

double Graph::getTotalNumber()
{
    double result = 0;
    for(int i = 0; i < allItems->numberOfItems(); i++)
    {
        result += ((NodeIntInt*)allItems->at(i))->getNumber();
    }
    return result;
}

double Graph::estimateTrust(Node* a, Node* b)
{
    double trust = 0;

    int distance = this->reachNode1(a->getId(), b->getId());
    if(distance == INFINITY_REACH_NODE || distance == 0)
        return 0;
    if(distance == 1)
        return a->trustOf(b->getId());

    Graph** g = new Graph*[distance - 1];
    for(int i = 0; i < distance - 1; i++)
        g[i] = new Graph(3, 3);

    EdgeList* elist = a->getContacts();
    for(int i = 0; i < elist->numberOfItems(); i++)
    {
        Edge* e = elist->at(i);
        NodeTrust* nt = new NodeTrust(e->getId(), e);
        g[0]->addItem(nt);
    }
    

    for(int i = 0; i < distance - 2; i++)
    {   
        List* l = g[i]->getAllItems();
        
        for(int j = 0; j < l->numberOfItems(); j++)
        {   
            NodeTrust* nt = (NodeTrust*)l->at(j);
            double currTrust = nt->getTrust();
            EdgeList* elist = nt->getContacts();
            for(int k = 0; k < elist->numberOfItems(); k++)
            {   
                Edge* e = elist->at(k);
                if(previouslyFound(g, e->getId(), i))
                    continue;
                
                NodeTrust* cand = (NodeTrust*)g[i]->lookUpItem(e->getId());
                
                if(cand == NULL)
                {   
                    cand = new NodeTrust(e->getId(), e, e->getWeight()*currTrust);
                    g[i+1]->addItem(cand);
                }
                else
                {   
                    if(currTrust*e->getWeight() > cand->getTrust())
                        cand->setTrust(currTrust*e->getWeight());

                }
            }

        }
    }

    List* l = g[distance - 2]->getAllItems();
    for(int i = 0; i < l->numberOfItems(); i++)
    {
        NodeTrust* nt = (NodeTrust*)l->at(i);
        double cand = nt->getTrust()*nt->trustOf(b->getId()); 
        if(trust < cand)
            trust = cand;
    }

    for(int i = 0; i < distance - 1; i++)
        delete g[i];

    delete[] g;

    return trust;
}

bool previouslyFound(Graph**g, int id, int curr)
{
    for(int i = 0; i < curr; i++)
    {
        if(g[i]->exists(id))
            return true;
    }
    return false;
}
/*
void Graph::fillGraphWithCliques(int k, NodeClique* cl, int curr, Graph* cliques, int* numb)
{	std::cout << *numb << ": Here with curr = " << curr << std::endl;
    //if(k == 1)
      //  std::cout << "K IS 1!!!!!!!!!!!!!!!11111!!!!" << std::endl;
    if(k == 0)
    {
        //std::cout << "Adding a clique! - " << cl->getId() << std::endl;
    //    cl->print();
        cliques->addItem(cl);
        return;
    }

    for(int i = curr; i < allItems->numberOfItems() - k + 1; i++)
    {
        NodeClique* cliq;
        bool deleted = false;
      //  if(k == 1)
            cliq = new NodeClique(*cl, ++(*numb));
        //else
          //  cliq = new NodeClique(*cl);

        if(cl->addMember(allItems->nodeAt(i)))
            fillGraphWithCliques(k - 1, cliq, curr + 1, cliques, numb);
        else
        {
            deleted = true;
            delete cliq;
        }
        if(k > 1 && !deleted)
            delete cliq;
    }

}
*/

Graph* Graph::createEdgeGraph()
{
    Graph* result = new Graph(this->hashM, this->bucketSize);
    List* l = this->getAllItems();
    int id = 1;

    for(int i = 0; i < l->numberOfItems(); i++)
    {
        Node* n = l->nodeAt(i);
        int first = n->getId();
        EdgeList* elist = n->getContacts();

        for(int j = 0; j < elist->numberOfItems(); j++)
        {
            int second = elist->at(j)->getId();
            if(first < second)
            {
                NodeEdge* ne = new NodeEdge(id, first, second);
                result->addItem(ne);
                id++;
            }
        }
    }
    result->addEdgesToEdgeGraph();
    return result;
}

void Graph::addEdgesToEdgeGraph()
{
    List* l = this->getAllItems();

    for(int i = 0; i < l->numberOfItems(); i++)
    {
        NodeEdge* first = (NodeEdge*)l->at(i);
        for(int j = i + 1; j < l->numberOfItems(); j++)
        {
            NodeEdge* second = (NodeEdge*)l->at(j);
            if(first->isConnectedTo(second))
            {
                Edge* e1 = new Edge(second);
                Edge* e2 = new Edge(first);
                first->addEdge(e1);
                second->addEdge(e2);
            }
        }
    }
}

void Graph::removeItem(int id)
{
    Node* n = this->lookUpItem(id);
    EdgeList* elist = n->getContacts();
    for(int i = 0; i < elist->numberOfItems(); i++)
        elist->nodeAt(i)->removeContact(id);

    allItems->removeItem(id);
    int result = hashFunction(id);
    table[result]->tree->removeItem(id);
    if(table[result] > 0)
        table[result]->items--;
}

void Graph::addEdgesFromEdgeGraph(List* l)
{
    for(int i = 0; i < l->numberOfItems(); i++)
    {
        NodeEdge* ne = (NodeEdge*)l->nodeAt(i);
        Node* first = this->lookUpItem(ne->getFirst());
        Node* second = this->lookUpItem(ne->getSecond());
        if(first == NULL || second == NULL)
            std::cout << "Error adding edges from edgegraph" << std::endl;
        else
        {
            Edge* e1 = new Edge(second);
            Edge* e2 = new Edge(first);
            first->addEdge(e1);
            second->addEdge(e2);
        }
    }
}

Graph* Graph::copyGraphWithoutEdges()
{
    Graph* result = new Graph(this->hashM, this->bucketSize);
    for(int i = 0; i < this->allItems->numberOfItems(); i++)
    {   
        Node* nint = allItems->nodeAt(i);
        Node* n = new Node(nint->getId());
        result->addItem(n);   
    }
    return result;
}

void Graph::removeEdgeBetweenNodes(int a, int b)
{
    Node* first = this->lookUpItem(a);
    Node* second = this->lookUpItem(b);
    if(a == NULL || b == NULL)
        return;

    first->removeContact(b);
    second->removeContact(a);
}

Graph* Graph::copyGraph()
{
    Graph* result = this->copyGraphWithoutEdges();
    result->importEdges(this);
    return result;
}

int Graph::totalEdges()
{
    int result = 0;
    for(int i = 0; i < allItems->numberOfItems(); i++)
        result += allItems->nodeAt(i)->numberOfEdges();
    return result;
}

Graph* Graph::GirvanNewman(double modularity)
{
    Graph* result = this->copyGraph();
    Graph* edgeGraph = result->createEdgeGraph();
    double mod = result->calculateModularity();
    while(mod < modularity)
    {
        NodeEdge* e = edgeGraph->getHighestBetweennessEdge();
        if(e == NULL)
            break;
        result->removeEdgeBetweenNodes(e->getFirst(), e->getSecond());
        edgeGraph->removeItem(e->getId());
        mod = result->calculateModularity();
    }
    delete edgeGraph;
    return result;
}

/*NodeEdge* Graph::getHighestBetweennessEdge()
{
    double max = 0;
    double temp = 0;
    int candidate = 0;
    for(int i = 0; i < allItems->numberOfItems(); i++)
    {
        temp = this->betweennessCentrality(allItems->nodeAt(i));
        if(temp > max)
        {
            max = temp;
            candidate = i;
        }
    }
    return (NodeEdge*)allItems->nodeAt(candidate);
}*/

double Graph::calculateModularity()
{
    double result = 0;
    int edges = this->totalEdges();
    if(edges == 0)
        return 0;

    for(int i = 0; i < allItems->numberOfItems(); i++)
    {
        Node* first = allItems->nodeAt(i);
        for(int j = i + 1; j < allItems->numberOfItems(); j++)
        {
            Node* second = allItems->nodeAt(j);
            if(this->reachNode1(first->getId(), second->getId()) != INFINITY_REACH_NODE)
            {
                double firstDegree = first->numberOfEdges();
                double secondDegree = second->numberOfEdges();
                double a = 0;
                if(first->hasContact(second->getId()))
                    a = 1;
                result += a - ((firstDegree * secondDegree) / edges);
            }
        }
    }
    return result / edges;
}

List* Graph::cliquePercolationMethod(int k)
{
    List* comm = new List();

    Graph* allCommunities = new Graph(2, 2);
    int cliqueId = 0;

    for(int i = 0; i < this->allItems->numberOfItems() - k + 1; i++)
    {
        NodeClique* nc = new NodeClique(0, k);
        nc->addMember(allItems->nodeAt(i));
        fillGraphWithCliques(k - 1, nc, i + 1, allCommunities, &cliqueId);
        delete nc;
    }

    allCommunities->addEdgesToCliqueGraph();
    allCommunities->fillListWithCommunities(comm);

    delete allCommunities;
    return comm;
}

void Graph::addEdgesToCliqueGraph()
{
    int total = allItems->numberOfItems();
    for(int i = 0; i < total; i++)
    {
        NodeClique* firstNC = (NodeClique*)allItems->at(i);
        if(firstNC == NULL)
            break;

        for(int j = i + 1; j < total; j++)
        { //std::cout << "here!" << std::endl;
            NodeClique* secondNC = (NodeClique*)allItems->at(j);
            if(secondNC == NULL)
                break;
            //std::cout << "here?" << std::endl;
            if(firstNC->isConnectedTo(secondNC))
            {   //std::cout << "added an edge!" << std::endl;
                Edge* e1 = new Edge(secondNC);
                firstNC->addEdge(e1);
                Edge* e2 = new Edge(firstNC);
                secondNC->addEdge(e2);
            }
        }
    }
}

void Graph::fillGraphWithCliques(int emptySpots, NodeClique* cl, int curr, Graph* cliques, int* numb)
{   
    //std::cout << "I am in here with " << emptySpots << " empty spots" << std::endl;
    if(emptySpots == 0)
    //if(cl->isReady())
    {   //std::cout << "Empty spots are " << emptySpots << " - ";// << std::endl;
        cl->setId(++(*numb));
        //cl->print();
        cliques->addItem(cl);
        return;
    }

    for(int i = curr; i < allItems->numberOfItems() - emptySpots + 1; i++)
    {
        bool deleted = false; 
        NodeClique* cliq = new NodeClique(*cl);
        //std::cout << "About to add " << allItems->nodeAt(i)->getId() << " to a clique" << std::endl;
        if(cliq->addMember(allItems->nodeAt(i)))
        {
            //std::cout << "Added to the clique" << std::endl;
            fillGraphWithCliques(emptySpots - 1, cliq, i + 1, cliques, numb);
        }
        else
        {
            deleted = true;
            delete cliq;
        }
        if(emptySpots > 1 && !deleted)
            delete cliq;
    }

}

void Graph::fillListWithCommunities(List* l)
{
    int communityId = 1;
    Graph visited (hashM, bucketSize);
    List nodes;
    if(allItems->numberOfItems() == 0)
        return;

    NodeDist* nod = new NodeDist(allItems->nodeAt(0), 0);
    visited.addItem(nod);
    nod->fillListWithEdges(&nodes, &visited, 0);
    Community* comm = new Community(communityId++);
    comm->addClique((NodeClique*)allItems->at(0));
    l->insertItem(comm);

    while(!nodes.isEmpty())
    {
        NodeDist* n = (NodeDist*)nodes.getItem();
        comm->addClique((NodeClique*)this->lookUpItem(n->getId()));
        n->fillListWithEdges(&nodes, &visited, 0);
        delete n;
    }
    int index = 1;
    while(visited.numberOfItems() < allItems->numberOfItems())
    {
        comm = new Community(communityId++);
        l->insertItem(comm);
        while(visited.exists(allItems->at(index)->getId()))
            index++;

        nod = new NodeDist(allItems->nodeAt(index), 0);
        visited.addItem(nod);
        nod->fillListWithEdges(&nodes, &visited, 0);
        comm->addClique((NodeClique*)this->lookUpItem(nod->getId()));
        while(!nodes.isEmpty())
        {
            NodeDist* n = (NodeDist*)nodes.getItem();
            n->fillListWithEdges(&nodes, &visited, 0);
            comm->addClique((NodeClique*)this->lookUpItem(nod->getId()));
            delete n;
        }
    }
}

double Graph::betweennessCentrality(Node* target)
{
    double betweenness = 0;
    int total = allItems->numberOfItems();
    Graph* positions = new Graph(this->hashM, this->bucketSize);
    double* allBetw = new double[total];
    List** p = new List*[total];
    List* stack = new List();
    List* queue = new List();
    double* paths = new double[total];
    double* distance = new double[total];

    for(int i = 0; i < total; i++)
    {
        NodeIntInt* nii = new NodeIntInt(allItems->nodeAt(i)->getId(), i);
        positions->addItem(nii);
        allBetw[i] = 0;
    }

    int targetPosition = ((NodeIntInt*)positions->lookUpItem(target->getId()))->getNumber();
    for(int i = 0; i < total; i++)
    {       
        Node* n = allItems->nodeAt(i);
        int nPos = ((NodeIntInt*)positions->lookUpItem(n->getId()))->getNumber();
        
        for(int j = 0; j < total; j++)
        {
            p[j] = new List();
            paths[j] = 0;
            distance[j] = -1;
        }
        paths[nPos] = 1;
        distance[nPos] = 0;

        queue->insertItem(n);
        while(!queue->isEmpty())
        {
            Node* curr = (Node*)queue->getItem();
            int currPosition = ((NodeIntInt*)positions->lookUpItem(curr->getId()))->getNumber();
            stack->insertOnTop(curr);
            EdgeList* elist = curr->getContacts();
            for(int k = 0; k < elist->numberOfItems(); k++)
            {
                Node* neighbour = elist->nodeAt(k);
                int neigPosition = ((NodeIntInt*)positions->lookUpItem(neighbour->getId()))->getNumber();
                if(distance[neigPosition] < 0)
                {
                    queue->insertItem(neighbour);
                    distance[neigPosition] = distance[currPosition] + 1;
                }

                if(distance[neigPosition] == distance[currPosition] + 1)
                {
                    paths[neigPosition] = paths[neigPosition] + paths[currPosition];
                    p[neigPosition]->insertItem(curr);
                }
            }
        }

        for(int w = 0; w < total; w++)
            distance[w] = 0;
        
        while(!stack->isEmpty())
        {
            Node* curr = (Node*)stack->getItem();
            int currPosition = ((NodeIntInt*)positions->lookUpItem(curr->getId()))->getNumber();
            for(int j = 0; j < p[currPosition]->numberOfItems(); j++)
            {
                int otherPos = ((NodeIntInt*)positions->lookUpItem(p[currPosition]->nodeAt(j)->getId()))->getNumber();

                distance[otherPos] = distance[otherPos] + (paths[otherPos] / paths[currPosition])*(1 + distance[currPosition]);
            }
            if(currPosition != nPos)
                allBetw[currPosition] = allBetw[currPosition] + distance[currPosition];
        }
        for(int w = 0; w < total; w++)
        {
            p[w]->clearContent();
            delete p[w];
        }
    }
    double normaliser = (total - 1)*(total - 2);
    //normaliser = normaliser/2;
    betweenness = allBetw[targetPosition];
    delete[] paths;
    delete queue;
    delete stack;
    delete[] p;
    delete[] allBetw;
    delete positions;
    delete[] distance;
    
    return betweenness/normaliser;
}

NodeEdge* Graph::getHighestBetweennessEdge()
{
    double betweenness = 0;
    int total = allItems->numberOfItems();
    Graph* positions = new Graph(this->hashM, this->bucketSize);
    double* allBetw = new double[total];
    List** p = new List*[total];
    List* stack = new List();
    List* queue = new List();
    double* paths = new double[total];
    double* distance = new double[total];

    for(int i = 0; i < total; i++)
    {
        NodeIntInt* nii = new NodeIntInt(allItems->nodeAt(i)->getId(), i);
        positions->addItem(nii);
        allBetw[i] = 0;
    }

    //int targetPosition = ((NodeIntInt*)positions->lookUpItem(target->getId()))->getNumber();
    for(int i = 0; i < total; i++)
    {       
        Node* n = allItems->nodeAt(i);
        int nPos = ((NodeIntInt*)positions->lookUpItem(n->getId()))->getNumber();
        
        for(int j = 0; j < total; j++)
        {
            p[j] = new List();
            paths[j] = 0;
            distance[j] = -1;
        }
        paths[nPos] = 1;
        distance[nPos] = 0;

        queue->insertItem(n);
        while(!queue->isEmpty())
        {
            Node* curr = (Node*)queue->getItem();
            int currPosition = ((NodeIntInt*)positions->lookUpItem(curr->getId()))->getNumber();
            stack->insertOnTop(curr);
            EdgeList* elist = curr->getContacts();
            for(int k = 0; k < elist->numberOfItems(); k++)
            {
                Node* neighbour = elist->nodeAt(k);
                int neigPosition = ((NodeIntInt*)positions->lookUpItem(neighbour->getId()))->getNumber();
                if(distance[neigPosition] < 0)
                {
                    queue->insertItem(neighbour);
                    distance[neigPosition] = distance[currPosition] + 1;
                }

                if(distance[neigPosition] == distance[currPosition] + 1)
                {
                    paths[neigPosition] = paths[neigPosition] + paths[currPosition];
                    p[neigPosition]->insertItem(curr);
                }
            }
        }

        for(int w = 0; w < total; w++)
            distance[w] = 0;
        
        while(!stack->isEmpty())
        {
            Node* curr = (Node*)stack->getItem();
            int currPosition = ((NodeIntInt*)positions->lookUpItem(curr->getId()))->getNumber();
            for(int j = 0; j < p[currPosition]->numberOfItems(); j++)
            {
                int otherPos = ((NodeIntInt*)positions->lookUpItem(p[currPosition]->nodeAt(j)->getId()))->getNumber();

                distance[otherPos] = distance[otherPos] + (paths[otherPos] / paths[currPosition])*(1 + distance[currPosition]);
            }
            if(currPosition != nPos)
                allBetw[currPosition] = allBetw[currPosition] + distance[currPosition];
        }
        for(int w = 0; w < total; w++)
        {
            p[w]->clearContent();
            delete p[w];
        }
    }
    double normaliser = (total - 1)*(total - 2);
    for(int j = 0; j < total; j++)
        allBetw[j] = allBetw[j]/normaliser;

    int maxPosition = 0;
    for(int i = 0; i < total; i++)
    {
        if(allBetw[i] > betweenness)
        {
            betweenness = allBetw[i];
            maxPosition = i;
        }
    }

    //normaliser = normaliser/2;
    //betweenness = allBetw[targetPosition];
    delete[] paths;
    delete queue;
    delete stack;
    delete[] p;
    delete[] allBetw;
    delete positions;
    delete[] distance;
    
    return (NodeEdge*)allItems->at(maxPosition);   
}