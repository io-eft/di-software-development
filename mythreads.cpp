#include "mythreads.hpp"
#include "graph.hpp"
#include "myqueue.hpp"
#include "list.hpp"
#include <unistd.h>

void* workerThread(void* args)
{
    MyQueue* mq = (MyQueue*)args;   //get the arguments
    while(true) //keep running
    {
        MyJob* mj = mq->popTop();   //get a job
        if(mj == NULL)  //if it's empty
            break;  //stop running
        mj->work(); //else, send it to client
        delete mj;  //delete the job
    }
    //pthread_detach(pthread_self());
    pthread_exit(NULL); //exit
}

Graph** Graph::getTopNForums(char* fileName, int n)
{
    int queueSize = 10;
    int poolSize = 20;
    int nojobs = 0;

    MyQueue* mq = new MyQueue(queueSize);
    pthread_t* ptid = new pthread_t[poolSize];

    pthread_cond_t* jobsDone = new pthread_cond_t;
    pthread_cond_init(jobsDone, NULL);

    for(int i = 0; i < poolSize; i++)
        pthread_create(&ptid[i], NULL, workerThread, (void*) mq);

    List* l = new List();
    pthread_mutex_t* listMutex = new pthread_mutex_t;
    pthread_mutex_init(listMutex, NULL);

    std::ifstream data;
    data.open(fileName);
    if(data.is_open())
    {
        std::string line;
        std::string id1str;
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
            id1 = getNumber(id1str.c_str());

            MyJob* mj = new MyJob("./files/forum_hasMember_person.csv", id1, l, listMutex, jobsDone, n);
            mq->insertEntry(mj);
            nojobs++;

            ss.str("");
            ss.clear();
        }
        data.close();
    }

    pthread_mutex_lock(listMutex);
    while(l->numberOfItems() < nojobs)
        pthread_cond_wait(jobsDone, listMutex);
    pthread_mutex_unlock(listMutex);

    Graph** topForums = new Graph*[n];

    std::cout << "List has " << l->numberOfItems() << " items." << std::endl;

    for(int i = 0; i < n; i++)
    {
        topForums[i] = new Graph(this->hashM, this->bucketSize);
        NodeIntInt* nii = (NodeIntInt*)l->getItem();
        if(nii != NULL)
        {
            MyOtherJob* moj = new MyOtherJob("./files/forum_hasMember_person.csv", nii->getId(), this, topForums[i]);
            mq->insertEntry(moj);
            delete nii;
        }
        else
        {
            std::cout << "Not enough in the list!(" << i << ")" << std::endl;
            break;
        }

    }

    delete l;
    mq->deactivate();
    for(int i = 0; i < poolSize; i++)
    {
        mq->signalWorkersForExit();
        pthread_join(ptid[i], NULL);
    }

    pthread_cond_destroy(jobsDone);
    pthread_mutex_destroy(listMutex);

    delete jobsDone;
    delete listMutex;
    delete mq;
    delete[] ptid;

    return topForums;
}
