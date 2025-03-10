#include "../../Include/Ui.h"


UI::UI() : window(nullptr) {}

void UI::initializeImGui(GLFWwindow* window) {
    this->window = window;

    // ��������� ��������� ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // �������� ���������� � ����������
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // �������� ������
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable moving frames from GLFW window.

    // ��������� ����� ImGui
    ImGui::StyleColorsDark();

    // ��������� �������� ��� GLFW � OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // ���������� ����������
    std::cout << "ImGui Version: " << ImGui::GetVersion() << std::endl;
}
void UI::default_frame() {
    // ������ ������ ����� ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // �������� �������������� ������-������������
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    // ����� ��� ������-������������
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    // ������ ������������� ���� ��� ������-������������
    ImGui::Begin("Docking Space", nullptr, window_flags);

    // �������� ������-������������
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    // ���������� ������������� ����
    ImGui::End();

    // Render the main menu
    //MainMenu();
    ProjectStructureTree();
    FileSystem();

    // ������ ���� ImGui
    ImGui::Begin("Hello, ImGui!", nullptr);
    ImGui::Text("Its just ImGui.");
    if (ImGui::Button("Press me"))
    {
        std::cout << "������ ������!" << std::endl;
    }
    ImGui::End();

    // Show ImGui demo window (for testing)
    ImGui::ShowDemoWindow();
}

void UI::render_imgui() {
    // ��������� ImGui
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

    ImGui::End();

}


UI::~UI() {
    // ������� ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
