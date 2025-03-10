#pragma once
#ifndef UI_H_
#define UI_H_

#include <GLFW/glfw3.h>
// ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>

class UI
{
public:
    UI();

    void initializeImGui(GLFWwindow* window);

    void default_frame();

    void render_imgui();
    
    void MainMenu();
    void ProjectStructureTree();
    void FileSystem();

    ~UI();

private:
    GLFWwindow* window;
};

#endif // UI_H_