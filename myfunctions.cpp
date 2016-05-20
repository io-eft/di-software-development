#include "myfunctions.hpp"
#include "entities.hpp"
#include "graph.hpp"
#include "list.hpp"

int power(int base, int p)
{
    int result = 1;
    for(int i = 0; i < p; i++)
        result = result*base;
    return result;
}

int getNumber(const char* c)
{   //get number from a c_string using stringstream
    int number = 0;
    std::stringstream ss(c);
    if(!(ss >> number))
        std::cout << "Wrong input, setting to default (0). " << c << std::endl;
    return number;
}

std::string getString(int n)
{   //turn number to string using stringstream
    std::stringstream ss;
    std::string s;
    ss << n;
    ss >> s;
    return s;
}

void loadPersonsInGraph(char* fileName, Graph* g)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string idStr;
        std::string firstname;
        std::string lastname;
        std::string gender;
        std::string birthday;
        std::string creationdate;
        std::string locationid;
        std::string browserUsed;
        int id;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;

            getline(ss, idStr,'|');
            getline(ss, firstname,'|');
            getline(ss, lastname,'|');
            getline(ss, gender,'|');
            getline(ss, birthday,'|');
            getline(ss, creationdate,'|');
            getline(ss, locationid,'|');
            getline(ss, browserUsed,'|');
            id = getNumber(idStr.c_str());

            //Person* p = new Person(id, firstname.c_str(), lastname.c_str(), gender.c_str(), birthday.c_str(), creationdate.c_str(), locationid.c_str(), browserUsed.c_str());
            Person* p = new Person(id, firstname, lastname, gender, birthday, creationdate, locationid, browserUsed);
            g->addItem(p);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadPersonKnowsPerson(char* fileName, Graph* g)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            Properties* p = new Properties(2);
            p->setStringProperty("knows", 0);
            p->setIntegerProperty(30, 1);

            Node* endNode = g->lookUpItem(id2);
            Edge* e = new Edge(endNode, p);

            //Edge* e = setEdgeProperties(id1, id2, "knows", 30, g);
            g->insertEdge(id1, e);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}


void loadInterests(char* fileName, Graph* g)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string idstr;
        std::string name;
        std::string url;
        int id;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, idstr, '|');
            getline(ss, name, '|');
            getline(ss, url, '|');
            id = getNumber(idstr.c_str());

            Interest* inter = new Interest(id, name, url);
            g->addItem(inter);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadPersonHasInterest(char* fileName, Graph* pers, Graph* inter)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            Interest* in = (Interest*)inter->lookUpItem(id2);
            Person* p = (Person*)pers->lookUpItem(id1);
            p->addInterest(in);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadOrganisations(char* fileName, Graph* org)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string idstr;
        std::string type;
        std::string name;
        std::string url;
        int id;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, idstr, '|');
            getline(ss, type, '|');
            getline(ss, name, '|');
            getline(ss, url, '|');
            id = getNumber(idstr.c_str());

            Organisation* organ = new Organisation(id, type, name, url);
            org->addItem(organ);

            ss.str("");
            ss.clear();
        }

    data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadPersonToOrganisation(char* fileName, Graph* pers, Graph* organ, bool isWork)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        std::string yearstr;
        int id1;
        int id2;
        int year;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            getline(ss, yearstr, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());
            year = getNumber(yearstr.c_str());

            Organisation* org = (Organisation*)organ->lookUpItem(id2);
            Person* p = (Person*)pers->lookUpItem(id1);
            if(isWork)
                p->addWork(org, year);
            else
                p->addUniversity(org, year);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadPlaces(char* fileName, Graph* places)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string idstr;
        std::string type;
        std::string name;
        std::string url;
        int id;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, idstr, '|');
            getline(ss, name, '|');
            getline(ss, url, '|');
            getline(ss, type, '|');
            id = getNumber(idstr.c_str());

            Place* pl = new Place(id, name, url, type);
            places->addItem(pl);

            ss.str("");
            ss.clear();
        }

    data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadPersonToPlaces(char* fileName, Graph* pers, Graph* places)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        std::string yearstr;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            Place* pl = (Place*)places->lookUpItem(id2);
            Person* p = (Person*)pers->lookUpItem(id1);

            p->addResidence(pl);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadPostsAndCreators(char* fileName, Graph* posts)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            Post* p = new Post(id1, id2);

            posts->addItem(p);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadLikesToPosts(char* fileName, Graph* posts, Graph* g)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            Post* p = (Post*)posts->getItem(id2);
            if(p == NULL)
                continue;

            Person* per = (Person*)g->lookUpItem(id1);
            if(per == NULL)
                continue;

            per->addLikeToPost(p->getAuthor());

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadForumMembers(char* fileName, Graph* original, Graph* forum, int forumId)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            if(id1 != forumId)
                continue;

            Person* p = (Person*)original->lookUpItem(id2);
            if(p == NULL)
                continue;

            Person* newP = new Person(*p);
            forum->addItem(newP);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadForumMembers(std::ifstream data, Graph* original, Graph* forum, int forumId)
{
        data.clear();
        data.seekg(0, std::ios::beg);

        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            if(id1 != forumId)
                continue;

            Person* p = (Person*)original->lookUpItem(id2);
            if(p == NULL)
                continue;

            Person* newP = new Person(*p);
            forum->addItem(newP);

            ss.str("");
            ss.clear();
        }

}

void loadForumPosts(char* fileName, Graph* posts, int forumId)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            if(id1 != forumId)
                continue;

            Post* newP = new Post(id2);
            posts->addItem(newP);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadPostHasCreator(char* fileName/*, Graph* persons*/, Graph* posts)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            Post* p = (Post*)posts->lookUpItem(id1);

            if(p != NULL)
                p->addAuthor(id2);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadPostHasComment(char* fileName, Graph* posts, Graph* comments)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            Post* p = (Post*)posts->lookUpItem(id2);

            if(p == NULL)
                continue;
            //std::cout << "Found a comment!" << std::endl;
            Comment* c = new Comment(id1, p->getId(), p->getAuthor());
            comments->addItem(c);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadCommentHasCreator(char* fileName, Graph* persons/*, Graph* posts*/, Graph* comments)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            Comment* c = (Comment*)comments->lookUpItem(id1);

            if(c == NULL)
                continue;

            Person* p = (Person*)persons->lookUpItem(id2);
            if(p != NULL)
                p->addCommentToPerson(c->getAuthorOfOriginal());

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadForumsForStalkers(char* fileName, Graph* stalkers, Graph* forums)
{
    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string id2str;
        int id1;
        int id2;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, id2str, '|');
            id1 = getNumber(id1str.c_str());
            id2 = getNumber(id2str.c_str());

            if(!stalkers->exists(id2))
                continue;

            if(forums->exists(id1))
                continue;

            NodeInt* f = new NodeInt(id1);
            forums->addItem(f);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}

void loadForumInfoForStalkers(char* fileName, Graph* ids, List* forums)
{
        std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
        std::string name;
        //std::string date;
        int id1;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            getline(ss, name, '|');
            id1 = getNumber(id1str.c_str());

            if(!ids->exists(id1))
                continue;

            Forum* f = new Forum(id1, name);
            forums->insertItem(f);

            ss.str("");
            ss.clear();
        }

        data.close();
    }
    else
        std::cout << "Failed to open " << fileName << std::endl;
}
