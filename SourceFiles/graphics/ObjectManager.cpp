#include "../../Include/ObjectManager.h"

ObjectManager::ObjectManager() {
	std::unordered_map<char*, Object> componentsTree;
}

void ObjectManager::addObject(Object obj) {
	componentTree[obj.name] = obj;
}

#ifdef MENTAL_BETA
void ObjectManager::createObject(ObjectType obj_type) {
	
}
#endif

ObjectManager::~ObjectManager() {}