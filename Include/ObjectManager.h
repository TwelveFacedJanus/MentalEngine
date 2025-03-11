#pragma once
#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <chrono>
#include "Object.h"

typedef enum ObjectType {
    Rectangle,
    Cube,
    Node2D,
    Shader,
    Triangle
} ObjectType;

typedef struct Object {
    std::string name; // Use std::string for safer memory management
    ObjectType obj_type;
    std::vector<Object*> child_comps;
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    std::filesystem::file_time_type lastVertexShaderModTime;
    std::filesystem::file_time_type lastFragmentShaderModTime;
} Object;

Object initialize_triangle(); // Declaration
void render_triangle(Object* obj); // Declaration

Object initialize_cube(); // Declaration
void render_cube(Object* obj); // Declaration

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