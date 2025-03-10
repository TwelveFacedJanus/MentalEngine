#pragma once
#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Object.h"
#include "ObjectManager.h"
#include "Ui.h"


class Window : public UI
{
public:
    Window(const char* title, int width = 800, int height = 600);
    ~Window();
    void run();

private:
    GLFWwindow* window;
    ObjectManager objm;
    GLuint shaderProgram;

    void initializeGLFW();
    void createWindow(const char* title, int width, int height);
    void initializeGLEW();
    void initializeOpenGLSettings();
    void initializeImGui(); // Initialize ImGui
    void processInput();
    void render();
};

#endif