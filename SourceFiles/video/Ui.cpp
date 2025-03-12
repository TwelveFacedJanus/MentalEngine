#include "../../Include/Ui.h"
#include <functional>


static int InputTextCallback(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        // Изменяем размер буфера
        std::string* str = (std::string*)data->UserData;
        str->resize(data->BufTextLen);
        data->Buf = &(*str)[0];
    }
    return 0;
}

UI::UI() : window(nullptr) {}

void UI::initializeImGui(GLFWwindow* window) {
    this->window = window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("fonts/ttf/JetBrainsMono-Regular.ttf", 15);
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Disable viewports for now

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
void UI::default_frame(GLuint textureID) {
    if (isFileSelected) {
        auto currentModificationTime = fs::last_write_time(selectedFilePath);
        if (currentModificationTime != lastModificationTime) {
            loadFileContent(); // Перезагружаем содержимое файла, если оно изменилось
        }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S)) {
        saveFileContent();
    }

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::Begin("Docking Space", nullptr, window_flags);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::End();

    ImGuiWindowFlags window_flags_gl = ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    ImGui::Begin("Viewport Tabs", nullptr, window_flags_gl);

    if (ImGui::BeginTabBar("ViewportTabBar")) {
        if (ImGui::BeginTabItem("OpenGL Viewport")) {
            ImGui::Image((ImTextureID)(intptr_t)textureID, ImGui::GetContentRegionAvail());
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Editor")) {
            if (isFileSelected) {
                ImGui::Text("Editing: %s", selectedFilePath.c_str());

                // Используем динамический буфер с поддержкой изменения размера
                if (ImGui::InputTextMultiline("##Editor", &fileContentBuffer[0], fileContentBuffer.size() + 1, ImVec2(-1.0f, -1.0f),
                    ImGuiInputTextFlags_CallbackResize, InputTextCallback, (void*)&fileContentBuffer)) {
                    fileContent = fileContentBuffer; // Сохраняем изменения в fileContent
                }
            }
            else {
                ImGui::Text("No file selected. Select a file in the Filesystem.");
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();

    ImGui::ShowDemoWindow();
    ImGui::Begin("Hello, ImGui!");
    ImGui::Text("This is a test window.");
    if (ImGui::Button("Press me")) {
        std::cout << "Button pressed!" << std::endl;
    }
    ImGui::End();
}

void UI::saveFileContent() {
    if (isFileSelected) {
        std::ofstream file(selectedFilePath);
        if (file.is_open()) {
            file << fileContent;
            file.close();
            std::cout << "File saved successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to save file!" << std::endl;
        }
    }
}

void UI::render_imgui() {
    // Рендеринг ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::MainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        // File Menu
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                std::cout << "New file created!" << std::endl;
            }
            if (ImGui::MenuItem("Open")) {
                std::cout << "Open file dialog!" << std::endl;
            }
            if (ImGui::MenuItem("Save")) {
                std::cout << "File saved!" << std::endl;
            }
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(window, true); // Close the application
            }
            ImGui::EndMenu();
        }

        // Edit Menu
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo")) {
                std::cout << "Undo action!" << std::endl;
            }
            if (ImGui::MenuItem("Redo")) {
                std::cout << "Redo action!" << std::endl;
            }
            ImGui::EndMenu();
        }

        // View Menu
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Fullscreen")) {
                std::cout << "Toggle fullscreen!" << std::endl;
            }
            ImGui::EndMenu();
        }

        // Help Menu
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                std::cout << "About this application!" << std::endl;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}


void UI::ProjectStructureTree() {
    ImGui::Begin("Components Tree");

    ImGui::End();
}

void UI::FileSystem() {
    ImGui::Begin("Filesystem");

    static fs::path currentDir = fs::current_path();
    ImGui::Text("Current Directory: %s", currentDir.string().c_str());

    if (currentDir.has_parent_path() && ImGui::Button("Up")) {
        currentDir = currentDir.parent_path();
    }

    // Начинаем создание таблицы с 3 колонками: иконка, имя, размер
    if (ImGui::BeginTable("FileSystemTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        // Заголовки колонок
        ImGui::TableSetupColumn("Icon", ImGuiTableColumnFlags_WidthFixed, 20.0f);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableHeadersRow();

        for (const auto& entry : fs::directory_iterator(currentDir)) {
            std::string entryName = entry.path().filename().string();
            bool isDirectory = fs::is_directory(entry.status());

            ImGui::TableNextRow();

            // Колонка с иконкой
            ImGui::TableSetColumnIndex(0);
            if (isDirectory) {
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "[FOLDER]");
            }
            else {
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "[FILE]");
            }

            // Колонка с именем
            ImGui::TableSetColumnIndex(1);
            if (isDirectory) {
                if (ImGui::Selectable(entryName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        currentDir = entry.path();
                    }
                }
            }
            else {
                if (ImGui::Selectable(entryName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
                    selectedFilePath = entry.path().string();
                    isFileSelected = true;
                    loadFileContent(); // Загружаем содержимое файла
                }
            }

            // Колонка с размером
            ImGui::TableSetColumnIndex(2);
            if (!isDirectory) {
                ImGui::Text("%ld bytes", fs::file_size(entry));
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void UI::loadFileContent() {
    if (isFileSelected) {
        std::ifstream file(selectedFilePath);
        if (file.is_open()) {
            fileContent.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            fileContentBuffer = fileContent; // Инициализируем буфер
            file.close();
            lastModificationTime = fs::last_write_time(selectedFilePath); // Обновляем время последнего изменения
        }
        else {
            fileContent = "Failed to open file!";
            fileContentBuffer = fileContent; // Инициализируем буфер
        }
    }
}
void UI::ProjectStructureTree(ObjectManager& objectManager) {
    ImGui::Begin("Components Tree");
    
    // Рекурсивная функция для отображения дерева объектов
    std::function<void(const Object&)> displayObjectTree = [&](const Object& obj) {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (obj.child_comps.empty()) {
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // Узлы без детей будут листьями
            ImGui::TreeNodeEx(obj.name, node_flags);
        }
        else {
            // Используем уникальный ID для узла
            ImGuiID node_id = ImGui::GetID(obj.name);
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)node_id, node_flags, "%s", obj.name);
            if (node_open) {
                for (const auto& child : obj.child_comps) {
                    displayObjectTree(child);
                }
                ImGui::TreePop();
            }
        }
        };

    // Отображаем корневой узел и его детей
    std::vector<Object>& componentTree = objectManager.componentTree;
    for (const auto& obj : componentTree) {
        displayObjectTree(obj);
    }

    ImGui::End();
}



UI::~UI() {
    // Очистка ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
