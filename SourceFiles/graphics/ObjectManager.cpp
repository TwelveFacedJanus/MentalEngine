#include "../../Include/ObjectManager.h"




ObjectManager::ObjectManager() {
    // Initialize the member variable, not a local variable
    componentTree = std::vector<Object>();
    Object root_node = Object(ObjectType::Node2D, "root_node");
    componentTree.push_back(root_node);
}

void ObjectManager::addObject(Object obj) {
    componentTree[0].child_comps.push_back(obj);
    //componentTree.push_back(obj);
    std::cout << "Object added: " << obj.name << std::endl;
}

void ObjectManager::render() {
    for (auto& obj : componentTree) {
        obj.render();
    }
}

void ObjectManager::set_projection_matrix(glm::mat4 projection) {
    for (auto& obj : componentTree) {
        GLuint projectionLoc = glGetUniformLocation(obj.shader_program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
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