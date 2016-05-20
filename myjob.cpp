#include "myjob.hpp"
#include "node.hpp"
#include "graph.hpp"
#include "list.hpp"

MyJob::MyJob(char* fn, int fId, List* rl, pthread_mutex_t* ll, pthread_cond_t* jd, int mf): forumId(fId), resultList(rl), listLock(ll), jobsDone(jd), maxForums(mf)
{
    data.open(fn);
    if(!data.is_open())
    {
        std::cerr << "Failed to open the file!" << std::endl;
    }
}

MyJob::~MyJob()
{
    if(data.is_open())
        data.close();
}

void MyJob::work()
{
    int result = 0;
    /*std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {*/
    data.clear();
    data.seekg(0, std::ios::beg);


        std::string line;
        std::string id1str;
        int id1;
        std::stringstream ss;

        getline(data, line);
        while(getline(data, line))
        {
            ss.str("");
            ss.clear();
            ss << line;
            getline(ss, id1str, '|');
            id1 = getNumber(id1str.c_str());

            if(id1 == forumId)
                result++;

            ss.str("");
            ss.clear();
        }
      //  data.close();
    //}
    NodeIntInt* nii = new NodeIntInt(forumId, result);
    //std::cout << "ForumId: " << forumId << ", size: " << result << std::endl;

    pthread_mutex_lock(listLock);
    resultList->insertOrderedByQuantity(nii, maxForums);
    pthread_cond_broadcast(jobsDone);

    pthread_mutex_unlock(listLock);
}

char* MyOtherJob::getFileName()
{
    return fileName;
}

int MyJob::getForumId()
{
    return forumId;
}

MyOtherJob::MyOtherJob(char* fn, int fId, Graph* og, Graph* rg): MyJob(fn, fId, NULL, NULL, NULL, 0), originalGraph(og), result(rg)
{
    fileName = (char*)malloc(sizeof(char*)*(strlen(fn) + 1));
    if(fileName == NULL)
    {
        std::cerr << "Malloc failed!" << std::endl;
        exit(-1);
    }
    strcpy(fileName, fn);
 //   result = new Graph(5, 5);
}

MyOtherJob::~MyOtherJob()
{
    free(fileName);
}

void MyOtherJob::work()
{
    //std::cout << "Loading members of " << MyJob::getForumId() <<
    //loadForumMembers(data, originalGraph, result, MyJob::getForumId());
    loadForumMembers(getFileName(), originalGraph, result, MyJob::getForumId());
    result->importEdges(originalGraph);
    //std::cout << "In work, diameter: " << result->diameter() << std::endl;
}
