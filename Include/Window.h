#pragma once
#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(const char* title, int width = 800, int height = 600);
    ~Window();
    void run();
private:
    GLFWwindow* window;

    void initializeGLFW();
    void createWindow(const char* title, int width, int height);
    void initializeGLEW();
    void initializeOpenGLSettings();
    void processInput();
    void render();
};

#endif