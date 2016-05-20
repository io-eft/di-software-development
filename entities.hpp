#ifndef __ENTITIES_HPP
#define __ENTITIES_HPP
#include "node.hpp"
#include <string>

class Interest;
class Organisation;
class Place;
class Reply;

class Person: public Node
{
    std::string firstName;
    std::string lastName;
    int age;
    std::string gender;
    std::string birthday;
    std::string creationDate;
    std::string locationIP;
    std::string browserUsed;
    
    Graph* interests;
    
    Organisation* worksAt;
    int worksSince;
    Organisation* studiesAt;
    int studiesSince;
    
    Place* livesAt;
    
    Graph* likesByPerson;
    
    Graph* commentsToPerson;
    
    public:
        Person(int id, char* fn, char* ln, char* g, char* bd, char* cd, char* lip, char* bu);
        Person(int id, std::string fn, std::string ln, std::string g, std::string bd, std::string cd, std::string lip, std::string bu);
        Person();
        Person(const Person& other);
        ~Person();
        
        std::string getFirstName();
        std::string getLastName();
        std::string getGender();
        std::string getBirthday();
        std::string getCreationDate();
        std::string getLocationIP();
        std::string getBrowserUsed();
        int getAge();
        
        void addInterest(Interest* i);
        void addWork(Organisation* o, int year);
        void addUniversity(Organisation* o, int year);
        void addResidence(Place* p);
        
        Organisation* getWork();
        Organisation* getUniversity();
        
        Place* getResidence();
        
        Graph* getInterests();
        bool hasInterest(Interest* i);
        
        bool isStalker(int k);
        void addLikeToPost(int id);
        void clearInterests();
        void clearLikes();
        void clearWork();
        void clearUniversity();
        void clearPlace();
        
        void addCommentToPerson(int id);
        
        double getTotalReplies();
        double getTotalLikes();
        double getRepliesTo(int id);
        double getLikesTo(int id);
};

class Interest: public Node
{
    std::string name;
    std::string url;
    
    public:
        Interest();
        Interest(int id, std::string n, std::string u);
        Interest(int id, char* n, char* u);
        ~Interest();
        
        std::string getName();
        std::string getURL();
};

class Organisation: public Node
{
    std::string type;
    std::string name;
    std::string url;
    
    public:
        Organisation();
        Organisation(int id, char* t, char* n, char* u);
        Organisation(int id, std::string t, std::string n, std::string u);
        ~Organisation();
        
        std::string getType();
        std::string getName();
        std::string getURL();
        
        bool isCompany();
        bool isUniversity();
};

class Place: public Node
{
    std::string name;
    std::string url;
    std::string type;
    
    public:
        Place();
        Place(int id, char* n, char* u, char* t);
        Place(int id, std::string n, std::string u, std::string t);
        ~Place();
        
        std::string getName();
        std::string getURL();
        std::string getType();
};

class Post: public Node
{
    int author;
    //Graph* replies;
    
    public:
        Post();
        Post(int id, int aid);
        Post(int id);
        ~Post();
        
        int getAuthor();
        //Graph* getReplies();
        
        void addAuthor(int aid);
        //void addReply(Reply* r);
};

class Forum: public NodeInt
{
    std::string name;
    Graph* members;
    Graph* posts;
    
    public:
        Forum(int id, char* n);
        Forum(int id, std::string n);
        Forum();
        ~Forum();
        
        std::string getName();
        Graph* getMembers();
        Graph* getPosts();
};

class Comment: public NodeInt
{
    int replyTo;
    int originalAuthor;
    
    public:
        Comment();
        Comment(int id,int rt, int aid);
        ~Comment();
        
        int getReply();
        int getAuthorOfOriginal();
};

#endif // __ENTITIES__HPP
