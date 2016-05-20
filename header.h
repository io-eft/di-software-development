/**
 *   Header.h
 */


#ifndef HEADER_H_
#define HEADER_H_

#define  NUMBER_OF_TOP_N_FORUMS 6


typedef struct forum{
        int size;
        char *name;
}Forum;

typedef struct community{
        int numberOfMembers;
        int *members;
}community;

typedef struct algorithmResults{
        int numberOfCommunities;
        community *communities;
}algorithmResults;

typedef struct cpm{

        char *forum;
        algorithmResults clique3;
        algorithmResults clique4;
}Cpm;

typedef struct gn{

        char *forum;
        algorithmResults results;
}Gn;

typedef enum {False, True} boolean;


#endif
