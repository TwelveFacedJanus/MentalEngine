#pragma once
#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

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
    Object(ObjectTypes type) : obj_type(type) {}
private:
    ObjectTypes obj_type;
};

class Application
{
public:
    Application(const char* title, int width = 800, int height = 600)
        : window(nullptr)
    {
        initializeGLFW();
        createWindow(title, width, height);
        initializeGLEW();
        initializeOpenGLSettings();
    }

    ~Application()
    {
        if (window)
        {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    void run()
    {
        while (!glfwWindowShouldClose(window))
        {
            processInput();
            render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

private:
    GLFWwindow* window;

    void initializeGLFW()
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void createWindow(const char* title, int width, int height)
    {
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window.");
        }
        glfwMakeContextCurrent(window);
    }

    void initializeGLEW()
    {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            throw std::runtime_error("Failed to initialize GLEW.");
        }
    }

    void initializeOpenGLSettings()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glViewport(0, 0, 800, 600);
    }

    void processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        // Add your rendering code here
    }
};

#endif // APPLICATION_H_