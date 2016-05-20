#include "list.hpp"

List::ListNode::ListNode(NodeInt* c): content(c), next(NULL) {}

List::ListNode::~ListNode()
{
    if(content != NULL)
        delete content;
}

List::List(): head(NULL), tail(NULL), current(NULL), items(0) {}

List::~List()
{
    while(head != NULL)
    {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

bool List::isEmpty()
{
    if(head == NULL)
        return true;
    else
        return false;
}

void List::insertItem(NodeInt* e)
{
    ListNode* n = new ListNode(e);
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

NodeInt* List::getItem()
{
    if(head == NULL)
        return NULL;
    ListNode* temp = head;
    head = head->next;
    NodeInt* result = temp->content;
    temp->content = NULL;
    delete temp;
    items--;
    return result;
}

void List::printAll()
{
    if(head == NULL)
    {
        std::cout << "Head is null" << std::endl;
        return;
    }
    ListNode* temp = head;
    int i = 1;
    while(temp != NULL)
    {   //std::cout << i++ << ": ";
        temp->content->print();
        temp = temp->next;
    }
}

void List::insertOrderedItem(NodeInt* e)
{   //insert in an ordered way, unique values only
    ListNode* n = new ListNode(e);
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
            ListNode* temp = head;
            while(temp->next != NULL && e->getId() > temp->next->content->getId())
                temp = temp->next;

            if(temp->next != NULL && e->getId() == temp->content->getId())
            {
                n->content = NULL;
                delete n;
                return;
            }

            n->next = temp->next;
            temp->next = n;
            if(tail == temp)
                tail = n;
        }
    }
    items++;
}

NodeInt* List::getNext()
{
    NodeInt* temp = current->content;
    if(current == NULL)
        current = head;
    else
        current = current->next;
    return temp;
}

void List::clearContent()
{
    current = head;
    while(current != NULL)
    {
        current->content = NULL;
        current = current->next;
    }
    items = 0;
}

NodeInt* List::checkTop()
{
    if(head != NULL)
        return head->content;
    else
        return NULL;
}

NodeInt* List::at(int i)
{
    if(i < 0 || i >= items)
        return NULL;
    ListNode* temp = head;
    for(int j = 0; j < i; j++)
        temp = temp->next;
    return temp->content;
}

Node* List::nodeAt(int i)
{
    if(i < 0 || i >= items)
        return NULL;
    ListNode* temp = head;
    for(int j = 0; j < i; j++)
        temp = temp->next;
    return (Node*)temp->content;
}

int List::numberOfItems()
{
    return items;
}

void List::insertOnTop(NodeInt* e)
{
    ListNode* n = new ListNode(e);
    n->next = head;
    head = n;
    if(tail == NULL)
        tail = n;
    items++;
}

void List::insertMatch(Matches* m)
{
    ListNode* n = new ListNode(m);
    if(head == NULL)
    {
        head = n;
        tail = n;
    }
    else
    {
        Matches* candidate = (Matches*)head->content;
        if(m->getMatchValue() > candidate->getMatchValue())
        {
            n->next = head;
            head = n;
        }
        else
        {
            ListNode* temp = head;
            while(temp->next != NULL)
            {
                candidate = (Matches*)temp->next->content;
                if(m->getMatchValue() > candidate->getMatchValue())
                    break;
                temp = temp->next;
            }

            n->next = temp->next;
            temp->next = n;
            if(tail == temp)
                tail = n;
        }
    }
    items++;
}

void List::printMatches()
{
    ListNode* temp = head;
    while(temp != NULL)
    {
        ((Matches*)temp->content)->print();
        temp = temp->next;
    }
}

void List::printForStalker()
{
    ListNode* temp = head;
    while(temp != NULL)
    {
        std::cout << temp->content->getId() << "(" << ((NodeIntInt*)temp->content)->getNumber() << "), ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

void List::insertOrderedByQuantity(NodeIntInt* m, int limit)
{
    std::cout << "Inserting item in the list. Current items in list: " << items << std::endl;
    ListNode* n = new ListNode(m);
    if(head == NULL)
    {
        head = n;
        tail = n;
    }
    else
    {
        NodeIntInt* candidate = (NodeIntInt*)head->content;
        if(m->getNumber() > candidate->getNumber())
        {
            n->next = head;
            head = n;
        }
        else
        {
            ListNode* temp = head;
            while(temp->next != NULL)
            {
                candidate = (NodeIntInt*)temp->next->content;
                if(m->getNumber() > candidate->getNumber())
                    break;
                temp = temp->next;
            }

            n->next = temp->next;
            temp->next = n;
            if(tail == temp)
                tail = n;
        }
    }
    if(items > limit)
        deleteTail();
    items++;
}

void List::deleteTail()
{
    if(head != NULL)
    {
        ListNode* temp = head;
        ListNode* tdb = head->next;
        while(tdb != tail)
        {
            temp = tdb;
            tdb = tdb->next;
        }
        tail = temp;
        tail->next = NULL;
        delete tdb;
    }
}

void List::removeItem(int id)
{
    if(head == NULL)
        return;

    ListNode* temp = head;
    if(head->content->getId() == id)
    {
        head = head->next;
        if(tail == temp)
            tail = NULL;        
    }
    else
    {
        ListNode* parent;
        do
        {
            parent = temp;
            temp = temp->next;
        }while(temp != NULL && temp->content->getId() != id);
        if(temp != NULL)
            parent->next = temp->next;
        if(tail == temp)
            tail = parent;
    }

    items--;
    if(temp != NULL)
    {
        temp->content = NULL;
        delete temp;
    }
}

int List::findPosition(int id)
{
    int position = 0;
    ListNode* temp = head;
    while(temp != NULL)
    {
        if(temp->content->getId() == id)
            return position;
        position++;
        temp = temp->next;
    }
    return -1;
}