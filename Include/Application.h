#pragma once
#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "Window.h"

enum ObjectTypes {
    Node2D,
    Node3D,
    Rectangle,
    Cube,
    Triangle,
};

class Object
{
public:
    Object(ObjectTypes type);
    void render();
    ~Object();

private:
    ObjectTypes obj_type;
    GLuint VAO = 0; // Vertex Array Object
    GLuint VBO = 0; // Vertex Buffer Object
    GLuint EBO = 0; // Element Buffer Object (for rectangles)

    void initializeTriangle();
    void renderTriangle();
    void initializeRectangle();
    void renderRectangle();
};

class Application
{
public:
    Application(const char* title, int width = 800, int height = 600);
    ~Application();
    void run();

private:
    GLFWwindow* window;

    void initializeGLFW();
    void createWindow(const char* title, int width, int height);
    void initializeGLEW();
    void initializeOpenGLSettings();
    void processInput();
    void render(Object& object);
};

#endif // APPLICATION_H_