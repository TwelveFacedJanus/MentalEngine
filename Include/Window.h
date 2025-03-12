#pragma once
#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ObjectManager.h"
#include "Ui.h"
#include "EngineCamera.h"
#include "Object.h"
class Window : public UI
{
public:
    Window(const char* title, int width = 1920, int height = 1080);
    ~Window();
    void run();

private:
    GLFWwindow* window;

    void initializeGLFW();
    void createWindow(const char* title, int width, int height);
    void initializeGLEW();
    void initializeOpenGLSettings();
    void initializeImGui(); // Initialize ImGui
    void processInput();
    void render();
    GLuint getFBOTexture() const;

    ObjectManager objm;
    GLuint framebuffer, texture;
    int fboWidth = 800, fboHeight = 600; // Framebuffer dimensions

    GLuint shaderProgram;
    EngineCamera ec;
};

#endif