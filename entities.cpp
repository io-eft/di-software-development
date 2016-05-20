#include "entities.hpp"
#include "node.hpp"
#include "myfunctions.hpp"
#include "graph.hpp"
#include "edgelist.hpp"
#include "list.hpp"

Person::Person(int id, char* fn, char* ln, char* g, char* bd, char* cd, char* lip, char* bu): Node(id), firstName(fn), lastName(ln), gender(g),
    birthday(bd), creationDate(cd), locationIP(lip), browserUsed(bu), worksAt(NULL), studiesAt(NULL), worksSince(-1), studiesSince(-1)
{
    age = 2014 - getNumber(birthday.substr(4).c_str());
    interests = new Graph(2, 2);
    //likesByPerson = new List();
    likesByPerson = new Graph(2, 2);
    commentsToPerson = new Graph(2, 2);
}

Person::Person(int id, std::string fn, std::string ln, std::string g, std::string bd, std::string cd, std::string lip, std::string bu): Node(id), firstName(fn),
    lastName(ln), gender(g), birthday(bd), creationDate(cd), locationIP(lip), browserUsed(bu), worksAt(NULL), studiesAt(NULL), worksSince(-1), studiesSince(-1)
{
    age = 0;
    if(birthday != "")
        age = 2014 - getNumber(birthday.substr(4).c_str());
    interests = new Graph(2, 2);
    likesByPerson = new Graph(2, 2);
    commentsToPerson = new Graph(2, 2);
}

Person::Person() {}

Person::Person(const Person& other): Node(other), firstName(other.firstName), lastName(other.lastName), gender(other.gender), birthday(other.birthday), creationDate(other.creationDate),
    locationIP(other.locationIP), browserUsed(other.browserUsed), worksAt(other.worksAt), studiesAt(other.studiesAt), age(other.age), worksSince(other.worksSince), studiesSince(other.studiesSince)
{
    this->interests = new Graph(2,2);
    List* l = other.interests->getAllItems();
    for(int i = 0; i < l->numberOfItems(); i++)
    {
        Interest* inter = (Interest*)l->nodeAt(i);
        this->interests->addItem(inter);
    }
    this->likesByPerson = new Graph(2,2);
    this->commentsToPerson = new Graph(2,2);
}

Person::~Person()
{
    //std::cout << NodeInt::getId() << std::endl;
    //std:: cout << "Deleting Person: ";
    if(interests != NULL)
    {
        interests->clearGraph();
        delete interests;
    }
    if(likesByPerson != NULL)
        delete likesByPerson;
    if(commentsToPerson != NULL)
        delete commentsToPerson;
}

void Person::addInterest(Interest* i)
{
    interests->addItem(i);
}

void Person::addWork(Organisation* o, int year)
{
    if(year > worksSince)
    {
        worksAt = o;
        worksSince = year;
    }
}

void Person::addUniversity(Organisation* o, int year)
{
    if(year > studiesSince)
    {
        studiesAt = o;
        studiesSince = year;
    }
}

void Person::addResidence(Place* p)
{
    livesAt = p;
}

Graph* Person::getInterests()
{
    return interests;
}

Organisation* Person::getWork()
{
    return worksAt;
}

Organisation* Person::getUniversity()
{
    return studiesAt;
}

Place* Person::getResidence()
{
    return livesAt;
}

std::string Person::getFirstName()
{
    return firstName;
}

std::string Person::getLastName()
{
    return lastName;
}
std::string Person::getGender()
{
    return gender;
}

std::string Person::getBirthday()
{
    return birthday;
}

std::string Person::getCreationDate()
{
    return creationDate;
}

std::string Person::getLocationIP()
{
    return locationIP;
}

std::string Person::getBrowserUsed()
{
    return browserUsed;
}

int Person::getAge()
{
    return age;
}

Interest::Interest() {}

Interest::Interest(int id, std::string n, std::string u): Node(id), name(n), url(u) {}

Interest::Interest(int id, char* n, char* u): Node(id), name(n), url(u) {}

Interest::~Interest() {}

std::string Interest::getName()
{
    return name;
}

std::string Interest::getURL()
{
    return url;
}

Organisation::Organisation() {}

Organisation::Organisation(int id, char* t, char* n, char* u): Node(id), type(t), name(n), url(u) {}

Organisation::Organisation(int id, std::string t, std::string n, std::string u): Node(id), type(t), name(n), url(u) {}

Organisation::~Organisation() {}

std::string Organisation::getType()
{
    return type;
}

std::string Organisation::getName()
{
    return name;
}

std::string Organisation::getURL()
{
    return url;
}

bool Organisation::isCompany()
{
    if(type == "company")
        return true;
    return false;
}

bool Organisation::isUniversity()
{
    if(type == "university")
        return true;
    return false;
}

Place::Place() {}

Place::Place(int id, char* n, char* u, char* t): Node(id), name(n), url(u), type(t) {}

Place::Place(int id, std::string n, std::string u, std::string t): Node(id), name(n), url(u), type(t) {}

Place::~Place() {}

std::string Place::getType()
{
    return type;
}

std::string Place::getName()
{
    return name;
}

std::string Place::getURL()
{
    return url;
}

bool Person::hasInterest(Interest* i)
{
    return interests->exists(i->getId());
}

Post::Post(): Node(0), author(0) {}//replies(NULL) {}

Post::Post(int id, int aid): Node(id), author(aid)
{
    //replies = new Graph(2, 2);
}

Post::Post(int id): Node(id), author(0)
{
    //replies = new Graph(2, 2);
}

Post::~Post()
{
    //if(replies != NULL)
      //  delete replies;
}

int Post::getAuthor()
{
    return author;
}
/*
Graph* Post::getReplies()
{
    return replies;
}*/

void Post::addAuthor(int aid)
{
    author = aid;
}

bool Person::isStalker(int k)
{
    List* lbp = likesByPerson->getAllItems();
    /*if(lbp->numberOfItems() > 0){
    this->printForStalker();
    std::cout << "And likes " << std::endl;
    lbp->printForStalker();
    }*/
    for(int i = 0; i < lbp->numberOfItems(); i++)
    {
        NodeIntInt* nd = (NodeIntInt*)lbp->at(i);
        if(this->getId() == nd->getId())
            continue;

        if(nd->getNumber() < k)
            continue;

        if(!Node::hasContact(nd->getId()))
            return true;
    }
    return false;
}

void Person::addLikeToPost(int id)
{
    NodeIntInt* nd = (NodeIntInt*)likesByPerson->getItem(id);
    if(nd != NULL)
        nd->increaseNumber();
    else
    {
        nd = new NodeIntInt(id, 1);
        likesByPerson->addItem(nd);
    }
}

void Person::addCommentToPerson(int id)
{
    NodeIntInt* nd = (NodeIntInt*)commentsToPerson->getItem(id);
    if(nd != NULL)
        nd->increaseNumber();
    else
    {
        nd = new NodeIntInt(id, 1);
        commentsToPerson->addItem(nd);
    }
}

Forum::Forum(): members(NULL), posts(NULL) {}

Forum::Forum(int id, char* n): NodeInt(id), name(n)
{
    members = new Graph(3, 3);
    posts = new Graph(3, 3);
}

Forum::Forum(int id, std::string n): NodeInt(id), name(n)
{
    members = new Graph(3, 3);
    posts = new Graph(3, 3);
}

Forum::~Forum()
{
    if(members != NULL)
    {
        members->clearGraph();
        delete members;
    }
    if(posts != NULL)
    {
        posts->clearGraph();
        delete posts;
    }
}

void Person::clearInterests()
{
    if(interests != NULL)
        interests->clearGraph();
    delete interests;
    interests = new Graph(2, 2);
}

void Person::clearLikes()
{
    delete likesByPerson;
    likesByPerson = new Graph(2, 2);
}

void Person::clearWork()
{
    worksAt = NULL;
}

void Person::clearUniversity()
{
    studiesAt = NULL;
}

void Person::clearPlace()
{
    livesAt = NULL;
}

Comment::Comment() {}

Comment::Comment(int id, int rt, int aid): NodeInt(id), replyTo(rt), originalAuthor(aid) {}

Comment::~Comment() {}

int Comment::getReply()
{
    return replyTo;
}

int Comment::getAuthorOfOriginal()
{
    return originalAuthor;
}

double Person::getTotalReplies()
{
    return commentsToPerson->getTotalNumber();
}

double Person::getTotalLikes()
{
    return likesByPerson->getTotalNumber();
}

double Person::getRepliesTo(int id)
{
    NodeIntInt* n = (NodeIntInt*)commentsToPerson->lookUpItem(id);
    if(n != NULL)
        return (double)n->getNumber();
    else return 0;
}

double Person::getLikesTo(int id)
{
    NodeIntInt* n = (NodeIntInt*)likesByPerson->lookUpItem(id);
    if(n != NULL)
        return (double)n->getNumber();
    else return 0;
}
