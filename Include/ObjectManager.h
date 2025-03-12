#pragma once
#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include "Object.h"
#include <vector>
#include <iostream>
#include <glm/ext/matrix_float4x4.hpp>

class ObjectManager
{
public:
    ObjectManager();
#ifdef MENTAL_BETA
    void createObject(ObjectType obj_type);
#endif
    void addObject(Object obj);
    void render();
    void set_projection_matrix(glm::mat4 projection);
    ~ObjectManager();
    std::vector<Object> componentTree;

 
};

#endif //OBJECT_MANAGER_H_