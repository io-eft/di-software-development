#ifndef __MYJOB__HPP
#define __MYJOB__HPP
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <fstream>
#include "myfunctions.hpp"

class List;
class Graph;

class MyJob
{
    
    std::ifstream data;
    int forumId;
    List* resultList;
    pthread_mutex_t* listLock;
    pthread_cond_t* jobsDone;
    int maxForums;
    //std::ifstream data;
    
    public:
        //std::ifstream data;
    	MyJob(char* fn, int fId, List* rl, pthread_mutex_t* ll, pthread_cond_t* jd, int mf);
	    virtual ~MyJob();
	
	    virtual void work();
	    
	    int getForumId();
	    std::ifstream getIfstream();
};

class MyOtherJob: public MyJob
{
    Graph* originalGraph;
    Graph* result;
    char* fileName;
    
    public:
        MyOtherJob(char* fn, int fId, Graph* og, Graph* rg);
        ~MyOtherJob();
        
        void work();
        char* getFileName();
};

#endif

