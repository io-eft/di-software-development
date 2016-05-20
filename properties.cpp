#include "properties.hpp"

Properties::Properties(int i): numberOfProperties(i)
{
    prop = (void**)malloc(i*sizeof(void*));
    if(prop == NULL)
    {
        std::cerr << "Error allocating memory" << std::endl;
        exit(-1);
    }
    type = new int[i];
    for(int j = 0; j < i; j++)
    {
        prop[j] = NULL;
        type[j] = -1;
    }
}

Properties::~Properties()
{
    for(int i = 0; i < numberOfProperties; i++)
    {
        free(prop[i]);
    }
    free(prop);
    delete[] type;
}

void Properties::setStringProperty(char* property, int index)
{
    if(index < 0 || index >= numberOfProperties)
    {
        std::cerr << "Error! Index out of bounds!" << std::endl;
    }
    else
    {
        if(prop[index] != NULL)
            free(prop[index]);
        prop[index] = malloc(sizeof(char)*(strlen(property) + 1));
        if(prop[index] == NULL)
        {
            std::cerr << "Error allocating memory" << std::endl;
            exit(-1);
        }
        strcpy((char*)prop[index], property);
        type[index] = 0;
    }
}

void Properties::setIntegerProperty(int property, int index)
{
    if(index < 0 || index >= numberOfProperties)
    {
        std::cerr << "Error! Index out of bounds!" << std::endl;
    }
    else
    {
        if(prop[index] != NULL)
            free(prop[index]);
        int* i = (int*)malloc(sizeof(int));
        *i = property;
        prop[index] = i;
        type[index] = 1;
    }
}

char* Properties::getStringProperty(int index)
{
    if(index < 0 || index >= numberOfProperties)
    {
        std::cerr << "Error! Index out of bounds!" << std::endl;
        return NULL;
    }
    return (char*)prop[index];
}

int Properties::getIntegerProperty(int index)
{
    if(index < 0 || index >= numberOfProperties)
    {
        std::cerr << "Error! Index out of bounds!" << std::endl;
        return -999;
    }
    int* i = (int*)prop[index];
    return *i;
}

void Properties::printProperties()
{
    for(int i = 0; i < numberOfProperties; i++)
    {
        if(type[i] == 0)
            std::cout << (char*)prop[i] << " ";
        if(type[i] == 1)
            std::cout << *(int*)prop[i] << " ";
    }
    std::cout << std::endl;
}
