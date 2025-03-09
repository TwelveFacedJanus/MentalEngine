#pragma once
#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include <unordered_map>
#include "Object.h"

class ObjectManager
{
public:
	ObjectManager();
#ifdef MENTAL_BETA
	void createObject(ObjectType obj_type);
#endif
	void addObject(Object obj);
	~ObjectManager();
private:
	std::unordered_map<char*, Object> componentTree;
};


#endif //OBJECT_MANAGER_H_