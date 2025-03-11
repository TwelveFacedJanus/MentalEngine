#pragma once
#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include "Object.h"
#include <vector>
#include <iostream>

class ObjectManager
{
public:
    ObjectManager();
#ifdef MENTAL_BETA
    void createObject(ObjectType obj_type);
#endif
    void addObject(Object obj);
    void render();
    ~ObjectManager();
private:
    std::vector<Object> componentTree;
};

#endif //OBJECT_MANAGER_H_