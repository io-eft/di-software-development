#include "edgelist.hpp"
#include "graph.hpp"
#include "list.hpp"

EdgeList::EdgeListNode::EdgeListNode(Edge* c): content(c), next(NULL) {}

EdgeList::EdgeListNode::~EdgeListNode()
{
    if(content != NULL)
        delete content;
}

EdgeList::EdgeList(): head(NULL), tail(NULL), current(NULL), items(0) {}

EdgeList::~EdgeList()
{
    while(head != NULL)
    {
        EdgeListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

bool EdgeList::isEmpty()
{
    if(head == NULL)
        return true;
    else
        return false;
}

void EdgeList::insertItem(Edge* e)
{
    EdgeListNode* n = new EdgeListNode(e);
    if(head == NULL)
    {
        head = n;
        tail = n;
        current = head;
    }
    else
    {
        tail->next = n;
        tail = n;
    }
    items++;
}

Edge* EdgeList::getItem()
{
    if(head == NULL)
        return NULL;
    EdgeListNode* temp = head;
    head = head->next;
    Edge* result = temp->content;
    temp->content = NULL;
    delete temp;
    items--;
    return result;
}

void EdgeList::printAll()
{
    EdgeListNode* temp = head;
    while(temp != NULL)
    {
        //temp->content->print();
        temp = temp->next;
    }
}

void EdgeList::insertOrderedItem(Node* e, Properties* p)
{   //inserts a new edge in an ordered way
    Edge* ed = new Edge(e, p);
    EdgeListNode* n = new EdgeListNode(ed);
    if(head == NULL)
    {
        head = n;
        tail = n;
    }
    else
    {
        if(e->getId() < head->content->getId())
        {
            n->next = head;
            head = n;
        }
        else
        {
            EdgeListNode* temp = head;
            while(temp->next != NULL && e->getId() > temp->next->content->getId())
                temp = temp->next;

            n->next = temp->next;
            temp->next = n;
            if(tail == temp)
                tail = n;
        }
    }
    items++;
}

void EdgeList::insertOrderedItem(Edge* ed)
{   //inserts a new edge in an ordered way
    EdgeListNode* n = new EdgeListNode(ed);
    if(head == NULL)
    {
        head = n;
        tail = n;
    }
    else
    {
        if(ed->getId() < head->content->getId())
        {
            n->next = head;
            head = n;
        }
        else
        {
            EdgeListNode* temp = head;
            while(temp->next != NULL && ed->getId() > temp->next->content->getId())
                temp = temp->next;

            n->next = temp->next;
            temp->next = n;
            if(tail == temp)
                tail = n;
        }
    }
    items++;
}

Edge* EdgeList::getNext()
{
    Edge* temp = current->content;
    if(current == NULL)
        current = head;
    else
        current = current->next;
    return temp;
}

void EdgeList::clearContent()
{
    current = head;
    while(current != NULL)
    {
        current->content = NULL;
        current = current->next;
    }
    items = 0;
    head = NULL;
}

bool EdgeList::fillList(List* l, Graph* g, Graph* targetGraph, int dist)    //for reachnode1
{   //fill list with nodes you haven't traveled on before, return true if you find a node traveled from the other direction
    EdgeListNode* temp = head;
    while(temp != NULL)
    {
        Node* n = temp->content->getNode();
        if(targetGraph->exists(n->getId()))
            return true;
        if(!g->exists(n->getId()))
        {
            NodeInt* ni = new NodeInt(n->getId());
            g->addItem(ni);
            NodeDist* nd = new NodeDist(n, dist);
            l->insertItem(nd);
        }
        temp = temp->next;
    }
    return false;
}

void EdgeList::fillList(List* l, Graph* g, int dist)    //for reachnoden
{   //fill with nodes you haven't traveled to before
    EdgeListNode* temp = head;
    while(temp != NULL)
    {
        Node* n = temp->content->getNode();
        if(!g->exists(n->getId()))
        {
            NodeInt* ni = new NodeInt(n->getId());
            g->addItem(ni);
            NodeDist* nd = new NodeDist(n, dist);
            l->insertItem(nd);
        }
        temp = temp->next;
    }
}

int EdgeList::numberOfItems()
{
    return items;
}

Node* EdgeList::nodeAt(int i)
{
    if(i < 0 || i > items)
        return NULL;
    EdgeListNode* temp = head;
    for(int j = 0; j < i; j++)
        temp = temp->next;
    return temp->content->getNode();
}

void EdgeList::fillStack(List* l, int first, int second, int parent, int distance)
{
    EdgeListNode* temp = head;
    while(temp != NULL)
    {
        Node* n = temp->content->getNode();
        if(n->getId() != first && n->getId() != second)
        {
            NodeDistPar* ndp = new NodeDistPar(n, distance, parent);
            l->insertOnTop(ndp);
        }
        temp = temp->next;
    }
}

bool EdgeList::exists(int id)
{
    EdgeListNode* temp = head;
    while(temp != NULL)
    {
        if(temp->content->getId() == id)
            return true;
        temp = temp->next;
    }
    return false;
}

void EdgeList::printForStalker()
{
    EdgeListNode* temp = head;
    while(temp != NULL)
    {
        std::cout << temp->content->getId() << ", ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

Edge* EdgeList::at(int i)
{
    if(i < 0 || i >= items)
        return NULL;
    EdgeListNode* temp = head;
    for(int j = 0; j < i; j++)
        temp = temp->next;
    return temp->content;
}

double EdgeList::trustOf(int id)
{
    EdgeListNode* temp = head;
    while(temp != NULL)
    {
        if(temp->content->getId() == id)
            return temp->content->getWeight();
        temp = temp->next;
    }
    return 0;
}

void EdgeList::removeItem(int id)
{
    if(head == NULL)
        return;

    EdgeListNode* temp = head;
    if(head->content->getId() == id)
    {
        head = head->next;
        if(tail == temp)
            tail = NULL;
        if(current == temp)
            current = NULL;        
    }
    else
    {
        EdgeListNode* parent;
        do
        {
            parent = temp;
            temp = temp->next;
        }while(temp != NULL && temp->content->getId() != id);
        if(temp != NULL)
            parent->next = temp->next;
        if(tail == temp)
            tail = parent;
        if(current == temp)
            current = parent;
    }

    items--;
    if(temp != NULL)
        delete temp;
}