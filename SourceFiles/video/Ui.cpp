#include "../../Include/Ui.h"


UI::UI() : window(nullptr) {}

void UI::initializeImGui(GLFWwindow* window) {
    this->window = window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Disable viewports for now

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::default_frame(GLuint textureID) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create a docking space
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Docking Space", nullptr, window_flags);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::End();

    ImGuiWindowFlags window_flags_gl =
        ImGuiWindowFlags_AlwaysUseWindowPadding |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::ShowDemoWindow();
    // Render other ImGui windows
    ImGui::Begin("Hello, ImGui!");
    ImGui::Text("This is a test window.");
    if (ImGui::Button("Press me")) {
        std::cout << "Button pressed!" << std::endl;
    }
    ImGui::End();

    // Add a window to display the FBO texture
    ImGui::Begin("OpenGL Viewport", nullptr, window_flags_gl);
    ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    ImGui::Image((ImTextureID)(intptr_t)textureID, ImGui::GetWindowSize());
    ImGui::End();
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

    ImGui::End();

}


UI::~UI() {
    // Очистка ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
