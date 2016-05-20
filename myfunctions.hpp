#ifndef __MYFUNCTIONS_HPP
#define __MYFUNCTIONS_HPP
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

class Graph;
class List;

int power(int base, int p);

void loadPersonsInGraph(char* fileName, Graph* g);

int getNumber(const char* c);

std::string getString(int n);

void loadPersonKnowsPerson(char* fileName, Graph* g);

void loadInterests(char* fileName, Graph* g);

void loadPersonHasInterest(char* fileName, Graph* pers, Graph* inter);

void loadOrganisations(char* fileName, Graph* org);

void loadPlaces(char* fileName, Graph* places);

void loadPersonToPlaces(char* fileName, Graph* pers, Graph* places);

void loadPersonToOrganisation(char* fileName, Graph* pers, Graph* organ, bool isWork);

void loadPostsAndCreators(char* fileName, Graph* posts);

void loadLikesToPosts(char* fileName, Graph* posts, Graph* g);

void loadForumMembers(char* fileName, Graph* original, Graph* forum, int forumId);

void loadForumMembers(std::ifstream data, Graph* original, Graph* forum, int forumId);

void loadForumPosts(char* fileName, Graph* posts, int forumId);

void loadPostHasComment(char* fileName, Graph* posts, Graph* comments);
    
void loadPostHasCreator(char* fileName, Graph* posts);

void loadCommentHasCreator(char* fileName, Graph* persons, Graph* comments);

void loadForumsForStalkers(char* fileName, Graph* stalkers, Graph* forums);

void loadForumInfoForStalkers(char* fileName, Graph* ids, List* forums);

#endif // __MYFUNCTIONS_HPP
