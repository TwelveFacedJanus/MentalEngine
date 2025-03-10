#include "../../Include/ObjectManager.h"
#include <iostream>
Object initialize_triangle() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Left
         0.5f, -0.5f, 0.0f, // Right
         0.0f,  0.5f, 0.0f  // Top
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first, then bind and set VBO, then configure vertex attributes
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create and return the Object
    Object triangle;
    triangle.name = "Triangle";
    triangle.obj_type = ObjectType::Triangle; // Assuming Node2D is the correct type
    triangle.VAO = VAO;
    triangle.VBO = VBO;
    triangle.EBO = 0; // Not used for this triangle

    return triangle;
}

void render_triangle(Object* obj) {
    glBindVertexArray(obj->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}


ObjectManager::ObjectManager() {
    // Initialize the member variable, not a local variable
    componentTree = std::vector<Object>();
}

void ObjectManager::addObject(Object obj) {
    componentTree.push_back(obj);
    std::cout << "Object added: " << obj.name << std::endl;
}

void ObjectManager::render() {
    std::cout << "Rendering objects: " << componentTree.size() << std::endl;
    for (auto& obj : componentTree) {
        if (obj.obj_type == ObjectType::Triangle) {
            std::cout << "Rendering triangle" << std::endl;
            render_triangle(&obj);
        }
    }
}
#ifdef MENTAL_BETA
void ObjectManager::createObject(ObjectType obj_type) {
	
}
#endif

ObjectManager::~ObjectManager() {}