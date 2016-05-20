#ifndef __NODE_HPP
#define __NODE_HPP
#include <iostream>
#include "properties.hpp"
#include <string>

class EdgeList;
class Edge;
class List;
class Graph;

class NodeInt
{
    int id;

    public:
	NodeInt();
	NodeInt(int i);
	NodeInt(const NodeInt& other);
	virtual ~NodeInt();

	virtual int getId();
    virtual void print();
    
    void setId(int i);
};

class Node: public NodeInt
{
    //int id;
    Properties* properties;
    EdgeList* contacts;

    public:
        Node();
        Node(int i);
        Node(int i, Properties* p);
        Node(const Node& other);
        ~Node();

        //int getId();
        void print();
	    void addEdge(Edge* e);
	    void printProperties();

	    EdgeList* getContacts();
	    bool fillListWithEdges(List* l, Graph* g, Graph* targerGraph, int dist);
	    void fillListWithEdges(List* l, Graph* g, int dist);
	    int numberOfEdges();
	    
	    void clearEdges();
	    bool hasContact(int id);
	    void printForStalker();
	    double trustOf(int id);
	    void removeContact(int id);
};

class NodeDist: public NodeInt
{
    int distance;
    EdgeList* contacts;

    public:
	NodeDist(Node* n, int i);
	~NodeDist();

	virtual int getDistance();
	virtual EdgeList* getContacts();
	bool fillListWithEdges(List* l, Graph* g, Graph* targerGraph, int dist);
	void fillListWithEdges(List* l, Graph* g, int dist);
	
};

class NodeDistPar: public NodeDist 
{
    int parentId;

    public:
        NodeDistPar(Node* n, int i, int parId);
        ~NodeDistPar();
        
        int getParentId();
        void fillStackWithEdges(List* l, int first);//, int parent, int distance);
};

class Matches: public NodeInt
{
    double matchValue;
    
    public:
        Matches(int id, double mv);
        Matches();
        ~Matches();
        
        double getMatchValue();
        void print();
        
        void setMatchValue(double mv);
};

class Stalkers: public Matches
{
    //double centralityScore;
    std::string forum;
    
    public:
        Stalkers(int id, double cs, std::string f);
        Stalkers();
        ~Stalkers();
        
        //double getCentralityScore();
        std::string getForum();
        
        void setForum(std::string f);
};

class NodeIntInt: public NodeInt
{
    int number;
    
    public:
        NodeIntInt();
        NodeIntInt(int id, int n);
        ~NodeIntInt();
        
        int getNumber();
        void increaseNumber();
};

class NodeClique: public Node
{
    int total;
    int current;
    Node** members;
    
    public:
        NodeClique(int id, int m);
        NodeClique();
        NodeClique(const NodeClique& other, int id = 0);
        ~NodeClique();
        
        Node** getMembers();
        
        bool addMember(Node* n);
        bool isConnectedTo(NodeClique* nc);
        void print();
        int getCurrent();
        bool isReady();
};

class NodeTrust: public NodeInt
{
    EdgeList* contacts;
    double trust;
    
    public:
        NodeTrust();
        NodeTrust(int id, Edge* e);
        NodeTrust(int id, Edge* e, double t);
        ~NodeTrust();
        
        double getTrust();
        void setTrust(double t);
        EdgeList* getContacts();
        double trustOf(int id);
};

class NodeEdge: public Node
{
    int first;
    int second;
    
    public:
        NodeEdge();
        NodeEdge(int id, int a, int b);
        ~NodeEdge();
        
        int getFirst();
        int getSecond();
        bool isConnectedTo(NodeEdge* other);
        void print();
};

class Community: public NodeInt
{
    Graph* members;
    
    public:
        Community();
        Community(int id);
        ~Community();
        
        void addMember(int id);
        void addClique(NodeClique* nq);
        void print();
};

#endif // __NODE__HPP
