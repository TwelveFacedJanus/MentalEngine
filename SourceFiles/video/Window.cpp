#include "../../Include/Window.h"

Window::Window(const char* title, int width, int height)
    : window(nullptr)
{
    initializeGLFW();
    createWindow(title, width, height);
    initializeGLEW();
    initializeOpenGLSettings();
    initializeImGui(); // Initialize ImGui
    objm = ObjectManager();
    // Create a triangle object
    Object triangle = Object(ObjectType::Triangle, "Simple Triangle");
    //Object cube = initialize_cube();
    // Add the triangle to the ObjectManager
    objm.addObject(triangle);
    //objm.addObject(cube);

    // Create FBO and texture
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fboWidth, fboHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Window::~Window()
{
    if (window)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Window::run() {
    while (!glfwWindowShouldClose(window)) {
        processInput();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        // Render your OpenGL content
        render();
        //MainMenu();
        //ProjectStructureTree();
        //FileSystem();
        // Start a new ImGui frame and render it
        UI::default_frame(getFBOTexture());
        UI::FileSystem();
        UI::MainMenu();
        UI::ProjectStructureTree();
        UI::render_imgui();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::initializeGLFW()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::createWindow(const char* title, int width, int height)
{
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }
    glfwMakeContextCurrent(window);
}

void Window::initializeGLEW()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failed to initialize GLEW.");
    }
}

void Window::initializeOpenGLSettings() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glViewport(0, 0, 800, 600); // Ensure this matches your window size
}

void Window::initializeImGui()
{
    // Initialize ImGui with the current window
    UI::initializeImGui(window);
}

void Window::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void Window::render() {
    // Render to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, fboWidth, fboHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set clear color for FBO
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Render the triangle
    objm.render();

    // Switch back to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 800, 600); // Reset viewport to window size
    glClearColor(0.9f, 0.0f, 0.0f, 1.0f); // Set clear color for default framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

GLuint Window::getFBOTexture() const {
    return texture;
}