#include "../Include/Application.h"

// Object Class Implementation

Object::Object(ObjectTypes type) : obj_type(type)
{
    if (type == Triangle)
    {
        initializeTriangle();
    }
    else if (type == Rectangle)
    {
        initializeRectangle();
    }
}

void Object::render()
{
    if (obj_type == Triangle)
    {
        renderTriangle();
    }
    else if (obj_type == Rectangle)
    {
        renderRectangle();
    }
}

Object::~Object()
{
    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO)
    {
        glDeleteBuffers(1, &VBO);
    }
    if (EBO)
    {
        glDeleteBuffers(1, &EBO);
    }
}

void Object::initializeTriangle()
{
    // Define vertices for a triangle
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f, // Bottom-left
         0.5f, -0.5f, 0.0f, // Bottom-right
         0.0f,  0.5f, 0.0f  // Top-center
    };

    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //generateAndBindVAO(VAO);

    // Generate and bind VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Object::renderTriangle()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Object::initializeRectangle()
{
    // Define vertices for a rectangle (two triangles)
    std::vector<float> vertices = {
        // Positions
         0.5f,  0.5f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, // Bottom-right
        -0.5f, -0.5f, 0.0f, // Bottom-left
        -0.5f,  0.5f, 0.0f  // Top-left
    };

    // Define indices for the rectangle (two triangles)
    std::vector<unsigned int> indices = {
        0, 1, 3, // First triangle
        1, 2, 3  // Second triangle
    };

    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Generate and bind EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Object::renderRectangle()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 6 indices for two triangles
    glBindVertexArray(0);
}

// Application Class Implementation

Application::Application(const char* title, int width, int height)
    : window(nullptr)
{
    initializeGLFW();
    createWindow(title, width, height);
    initializeGLEW();
    initializeOpenGLSettings();
}

Application::~Application()
{
    if (window)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Application::run()
{
    Object triangle(Triangle); // Create a Triangle object
    Object rectangle(Rectangle); // Create a Rectangle object

    while (!glfwWindowShouldClose(window))
    {
        processInput();
        render(triangle); // Render the triangle
        render(rectangle); // Render the rectangle
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::initializeGLFW()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::createWindow(const char* title, int width, int height)
{
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }
    glfwMakeContextCurrent(window);
}

void Application::initializeGLEW()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failed to initialize GLEW.");
    }
}

void Application::initializeOpenGLSettings()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glViewport(0, 0, 800, 600);
}

void Application::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void Application::render(Object& object)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    object.render(); // Render the object (e.g., Triangle or Rectangle)
}