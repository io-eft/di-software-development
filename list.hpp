#ifndef __LIST_HPP
#define __List_HPP
#include "node.hpp"

class List
{
    struct ListNode
    {
        NodeInt* content;
        ListNode* next;

        ListNode(NodeInt* c);
        ~ListNode();
    };

    ListNode* head;
    ListNode* tail;
    ListNode* current;
    int items;

    public:
        List();
        ~List();

    void insertItem(NodeInt* e);
	void insertOrderedItem(NodeInt* e);
	void insertOnTop(NodeInt* e);
    NodeInt* getItem();
	NodeInt* getNext();
    bool isEmpty();
	void clearContent();

    void printAll();
	NodeInt* checkTop();
	NodeInt* at(int i);
	Node* nodeAt(int i);
	int numberOfItems();
	
	void insertMatch(Matches* m);
	void printMatches();
	
	void printForStalker();
	void insertOrderedByQuantity(NodeIntInt* m, int limit);
	void deleteTail();
	void removeItem(int id);
	int findPosition(int id);
};
#endif // __LIST_HPP


