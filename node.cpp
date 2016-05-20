#include "node.hpp"
#include "edgelist.hpp"
#include "graph.hpp"
#include "list.hpp"

NodeInt::NodeInt() {}

NodeInt::NodeInt(int i): id(i) {}

NodeInt::NodeInt(const NodeInt& other): id(other.id) {}

Node::Node() {}

Node::Node(int i): NodeInt(i)
{
    contacts = new EdgeList();
    properties = NULL;
}

Node::Node(int i, Properties* p): NodeInt(i)
{
    contacts = new EdgeList();
    properties = p;
}

Node::Node(const Node& other): NodeInt(other)
{
    this->contacts = new EdgeList();
    properties = NULL;
}

NodeInt::~NodeInt() {}

NodeDist::~NodeDist() {}

Node::~Node()
{
    //contacts->clearContent();
    if(contacts != NULL)
        delete contacts;
    if(properties != NULL)
        delete properties;
}

int NodeInt::getId()
{
    return id;
}

void NodeInt::print()
{
    std::cout << id << std::endl;
}

void Node::print()
{
    std::cout << "id: " << NodeInt::getId() << std::endl;
    if(properties != NULL)
        properties->printProperties();
}
/*
int Node::getId()
{
    return NodeInt::getId();
}*/

void Node::addEdge(Edge* e)
{
    //contacts->insertOrderedItem(e);
    contacts->insertItem(e);
}

void Node::printProperties()
{
    if(properties != NULL)
        properties->printProperties();
}

EdgeList* Node::getContacts()
{
    return contacts;
}

NodeDist::NodeDist(Node* n, int i): NodeInt(n->getId()), distance(i)
{
    contacts = n->getContacts();
}

int NodeDist::getDistance()
{
    return distance;
}

EdgeList* NodeDist::getContacts()
{
    return contacts;
}

bool Node::fillListWithEdges(List* l, Graph* g, Graph* targetGraph, int dist)
{
    return contacts->fillList(l, g, targetGraph, dist);
}

bool NodeDist::fillListWithEdges(List* l, Graph* g, Graph* targetGraph, int dist)
{
    return contacts->fillList(l, g, targetGraph, dist);
}

void Node::fillListWithEdges(List* l, Graph* g, int dist)
{
    contacts->fillList(l, g, dist);
}

void NodeDist::fillListWithEdges(List* l, Graph* g, int dist)
{
    contacts->fillList(l, g, dist);
}

int Node::numberOfEdges()
{
    return contacts->numberOfItems();
}


NodeDistPar::NodeDistPar(Node* n, int i, int parId): NodeDist(n, i), parentId(parId) {}

NodeDistPar::~NodeDistPar() {}

int NodeDistPar::getParentId()
{
    return parentId;
}

void NodeDistPar::fillStackWithEdges(List* l, int first)
{
    NodeDist::getContacts()->fillStack(l, first, parentId, NodeInt::getId(), NodeDist::getDistance() + 1);
}

Matches::Matches(int id, double mv): NodeInt(id), matchValue(mv) {}

Matches::Matches(): NodeInt(0), matchValue(0) {}

Matches::~Matches() {}

double Matches::getMatchValue()
{
    return matchValue;
}

void Matches::print()
{
    std::cout << "Id:" << NodeInt::getId() << ", Match Rating: " << matchValue << std::endl;
}

void NodeInt::setId(int i)
{
    id = i;
}

void Matches::setMatchValue(double mv)
{
    matchValue = mv;
}

void Node::clearEdges()
{
    //contacts->clearContent();
    delete contacts;
    contacts = new EdgeList();
}

Stalkers::Stalkers() {}

Stalkers::Stalkers(int id, double cs, std::string f): Matches(id, cs), forum(f) {}

Stalkers::~Stalkers() {}

std::string Stalkers::getForum()
{
    return forum;
}

void Stalkers::setForum(std::string f)
{
    forum = f;
}

bool Node::hasContact(int id)
{
    return contacts->exists(id);
}

NodeIntInt::NodeIntInt() {}

NodeIntInt::NodeIntInt(int id, int n): NodeInt(id), number(n) {}

NodeIntInt::~NodeIntInt() {}

int NodeIntInt::getNumber()
{
    return number;
}

void NodeIntInt::increaseNumber()
{
    number++;
}

void Node::printForStalker()
{
    std::cout << this->getId() << " knows: " << std::endl;
    contacts->printForStalker();
}

NodeClique::NodeClique() {}

NodeClique::NodeClique(int id, int m): Node(id), total(m), current(0)
{
    members = new Node*[m];
    for(int i = 0; i < m; i++)
        members[i] = NULL;
}

NodeClique::NodeClique(const NodeClique& other, int id): Node(id), total(other.total), current(other.current)
{
//    if(id != 0)
  //  std::cout << "clique id: " << id << std::endl;
    this->members = new Node*[total];
    for(int i = 0; i < total; i++)
    {
        this->members[i] = other.members[i];
      //  if(id > 0)
        //    std::cout << this->members[i]->getId() << std::endl;
    }
    //std::cout << "Old Current: " << other.current << ". New: " << this->current << std::endl;
}

NodeClique::~NodeClique()
{
    delete[] members;
}

bool NodeClique::addMember(Node* n)
{
    if(current == total)
        return false;

    for(int i = 0; i < current; i++)
        if(!members[i]->hasContact(n->getId()))
        {
            //std::cout << "Not a match!" << std::endl;
            return false;
        }
        /*else
        {
            if(current > 1)
                std::cout << "Match of: " << current + 1 << " - id: " << n->getId() << " - " << members[0]->getId() << " - " << members[1]->getId() << std::endl;

        }*/

    /*std::cout << "I have " << current << " items.";
    for(int i = 0; i < current; i++)
        std::cout << " " << members[i]->getId();
    std::cout << ". Added " << n->getId() << std::endl; 
    */
    members[current] = n;
    current++;
   // if(current == total)
     //   std::cout << "Added a third!" << std::endl;
    //std::cout << current << std::endl;
   // if(current > 0)
    //std::cout << "Id: " << this->getId() << " added a new member (" << current << ")" << std::endl;

    return true;
}

int NodeClique::getCurrent()
{
    return current;
}

bool NodeClique::isReady()
{
    if(current == total)
        return true;
    else
        return false;
}

Node** NodeClique::getMembers()
{
    return members;
}

bool NodeClique::isConnectedTo(NodeClique* nc)
{
    //if(current == total)
      //  std::cout << "ID: " << this->getId() << " , current: " << current << std::endl;
    //return true;
    int errors = 0;
    Node** candidate = nc->getMembers();
    if(candidate == NULL)
    {
        std::cout << "WTF?" << std::endl;
        return false;
    }
    for(int i = 0; i < total; i++)
    {
        bool result = false;
        for(int j = 0; j < total; j++)
        {
            if(candidate[i] == NULL)
            {
                std::cout << "WTF CANDIDATE? " << i << std::endl;
                return false;
            }
            if(this->members[j] == NULL)
            {
                std::cout << "WTF MEMBERS? " << i << std::endl;
                return false;
            }
            if(candidate[i]->getId() == this->members[j]->getId())
            {
                result = true;
                break;
            }
        }
        if(result != true)
        {
            if(errors == 1)
                return false;

            errors++;
       }
    }
    return true;
}

void NodeClique::print()
{
    std::cout << "Id: " << NodeInt::getId() << ", " << current << " out of " << total << ".";
    for(int i = 0; i < current; i++)
        std:: cout << " " << members[i]->getId();
    std::cout << std::endl;
}

double Node::trustOf(int id)
{
    return contacts->trustOf(id);
}

NodeTrust::NodeTrust() {}

NodeTrust::NodeTrust(int id, Edge* e): NodeInt(e->getId())
{
    contacts = e->getNode()->getContacts();
    trust = e->getWeight();
}

NodeTrust::NodeTrust(int id, Edge* e, double t): NodeInt(e->getId()), trust(t)
{
    //NodeInt(e->getId());
    contacts = e->getNode()->getContacts();
}

NodeTrust::~NodeTrust() {}

double NodeTrust::getTrust()
{
    return trust;
}

void NodeTrust::setTrust(double t)
{
    trust = t;
}

EdgeList* NodeTrust::getContacts()
{
    return contacts;
}

double NodeTrust::trustOf(int id)
{
    return contacts->trustOf(id);
}

NodeEdge::NodeEdge() {}

NodeEdge::NodeEdge(int id, int a, int b): Node(id), first(a), second(b)
{
    //contacts = new EdgeList();
}

NodeEdge::~NodeEdge()
{
 //   delete contacts;
}

int NodeEdge::getFirst()
{
    return first;
}

int NodeEdge::getSecond()
{
    return second;
}

bool NodeEdge::isConnectedTo(NodeEdge* other)
{
    if(this->first == other->getFirst())
        return true;
    if(this->first == other->getSecond())
        return true;
    if(this->second == other->getFirst())
        return true;
    if(this->second == other->getSecond())
        return true;
    return false;
}

void Node::removeContact(int id)
{
    contacts->removeItem(id);
}

void NodeEdge::print()
{
    std::cout << "Id: " << NodeInt::getId() << ", first: " << first << ", second: " << second << std::endl;
}

Community::Community() {}

Community::Community(int id): NodeInt(id)
{
    members = new Graph(2, 2);
}
Community::~Community()
{
    delete members;
}

void Community::addMember(int id)
{
    if(!members->exists(id))
    {
        NodeInt* nd = new NodeInt(id);
        members->addItem(nd);
    }
}

void Community::addClique(NodeClique* nq)
{
    Node** n = nq->getMembers();
    int total = nq->getCurrent();
    for(int i = 0; i < total; i++)
    {
        if(!members->exists(n[i]->getId()))
        {
            NodeInt* nd = new NodeInt(n[i]->getId());
            members->addItem(nd);
        }
    }
}

void Community::print()
{
    std::cout << "Community id:" << NodeInt::getId();
    List* l = members->getAllItems();
    for(int i = 0; i < l->numberOfItems(); i++)
        std::cout << " " << l->at(i)->getId();
    std::cout << std::endl;
}