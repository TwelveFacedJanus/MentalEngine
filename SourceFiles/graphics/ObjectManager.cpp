#include "../../Include/ObjectManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string loadShaderSource(const std::string& filePath) {
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        std::cerr << "ERROR: Could not open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}

GLuint compileShader(const char* shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for shader compile errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

Object initialize_triangle() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Left
         0.5f, -0.5f, 0.0f, // Right
         0.0f,  0.5f, 0.0f  // Top
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first, then bind and set VBO, then configure vertex attributes
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load shaders from files
    std::string vertexShaderSource = loadShaderSource("shaders/vertex_shader.glsl");
    std::string fragmentShaderSource = loadShaderSource("shaders/fragment_shader.glsl");

    if (vertexShaderSource.empty() || fragmentShaderSource.empty()) {
        std::cerr << "ERROR: Failed to load shader source code." << std::endl;
        return Object(); // Return an empty object if shader loading fails
    }
    std::cout << "Shaders has been loaded\n";

    GLuint shaderProgram = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
    std::cout << "Shader program has been created\n";
    // Create and return the Object
    Object triangle;
    triangle.name = "Triangle";
    triangle.obj_type = ObjectType::Triangle; // Assuming Node2D is the correct type
    triangle.VAO = VAO;
    triangle.VBO = VBO;
    triangle.EBO = 0; // Not used for this triangle
    triangle.shaderProgram = shaderProgram; // Add shader program to the object

    return triangle;
}

void render_triangle(Object* obj) {
    glUseProgram(obj->shaderProgram); // Use the shader program
    glBindVertexArray(obj->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

ObjectManager::ObjectManager() {
    // Initialize the member variable, not a local variable
    componentTree = std::vector<Object>();
}

void ObjectManager::addObject(Object obj) {
    componentTree.push_back(obj);
    std::cout << "Object added: " << obj.name << std::endl;
}

void ObjectManager::render() {
    for (auto& obj : componentTree) {
        if (obj.obj_type == ObjectType::Triangle) {;
            render_triangle(&obj);
        }
    }
}

#ifdef MENTAL_BETA
void ObjectManager::createObject(ObjectType obj_type) {

}
#endif

ObjectManager::~ObjectManager() {
    for (auto& obj : componentTree) {
        glDeleteProgram(obj.shaderProgram);
    }
}