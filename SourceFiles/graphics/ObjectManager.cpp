#include "../../Include/ObjectManager.h"




ObjectManager::ObjectManager() {
    // Initialize the member variable, not a local variable
    componentTree = std::vector<Object>();
}

void ObjectManager::addObject(Object obj) {
    componentTree.push_back(obj);
    std::cout << "Object added: " << obj.name << std::endl;
}

void ObjectManager::render() {
    static float rotationAngle = 0.0f; // Static variable to keep track of rotation angle
    rotationAngle += 1.0f; // Increment the rotation angle

    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f; // Keep the angle within 0-360 degrees
    }

    for (auto& obj : componentTree) {
        obj.render();
    }
}

#ifdef MENTAL_BETA
void ObjectManager::createObject(ObjectType obj_type) {

}
#endif

ObjectManager::~ObjectManager() {
    for (auto& obj : componentTree) {
        if (obj.shader_program) { glDeleteProgram(obj.shader_program); }
        
    }
}