#ifndef __PROPERTIES_HPP
#define __PROPERTIES_HPP
#include <iostream>
#include <stdlib.h>
#include <string.h>

class Properties
{
    int numberOfProperties;
    int* type;
    void** prop;

    public:
        Properties(int i);
        ~Properties();

	void setStringProperty(char* property, int index);
	void setIntegerProperty(int property, int index);
	char* getStringProperty(int index);
	int getIntegerProperty(int index);
	void printProperties();
};

#endif // __PROPERTIES__HPP
