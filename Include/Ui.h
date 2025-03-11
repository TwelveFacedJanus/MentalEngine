#pragma once
#ifndef UI_H_
#define UI_H_

#include <GLFW/glfw3.h>
// ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>
#include <filesystem> // Include the filesystem library
#include <fstream>
#include <sstream>
#include "ObjectManager.h"
namespace fs = std::filesystem; // Alias for convenience

static int InputTextCallback(ImGuiInputTextCallbackData* data);

class UI
{
public:
    UI();

    void initializeImGui(GLFWwindow* window);

    void default_frame(GLuint textureID);

    void render_imgui();
    
    void MainMenu();
    void ProjectStructureTree();
    void FileSystem();
    void loadFileContent();
    void saveFileContent();
    void ProjectStructureTree(ObjectManager& objectManager);

    ~UI();

private:
    GLFWwindow* window;
    std::string selectedFilePath; // Путь к выбранному файлу
    std::string fileContent;      // Содержимое файла
    bool isFileSelected = false;  // Флаг, указывающий, выбран ли файл
    std::filesystem::file_time_type lastModificationTime;
    std::string fileContentBuffer;
};

#endif // UI_H_