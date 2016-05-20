#ifndef __EDGELIST_HPP
#define __EDGEList_HPP
#include "edge.hpp"
//#include "list.hpp"

class Graph;
class List;

class EdgeList
{
    struct EdgeListNode
    {
        Edge* content;
        EdgeListNode* next;

        EdgeListNode(Edge* c);
        ~EdgeListNode();
    };

    EdgeListNode* head;
    EdgeListNode* tail;
    EdgeListNode* current;
    int items;

    public:
        EdgeList();
        ~EdgeList();

        void insertItem(Edge* e);
	    void insertOrderedItem(Node* e, Properties* p);
	    void insertOrderedItem(Edge* ed);
        Edge* getItem();
    	Edge* getNext();
        bool isEmpty();
	    void clearContent();

        void printAll();
        bool exists(int i);

	    bool fillList(List* l, Graph* g, Graph* targetGraph, int dist);
	    void fillList(List* l, Graph* g, int dist);
	    void fillList(List* l, Graph* g);
	    void fillStack(List* l, int first, int second, int parent, int distance);
    
	    int numberOfItems();
	    Node* nodeAt(int i);
	    
	    void printForStalker();
	    
	    Edge* at(int i);
	    double trustOf(int id);
        void removeItem(int id);
};
#endif // __EDGELIST_HPP
