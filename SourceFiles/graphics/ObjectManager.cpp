#include "../../Include/ObjectManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;

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
    triangle.vertexShaderPath = "shaders/vertex_shader.glsl";
    triangle.fragmentShaderPath = "shaders/fragment_shader.glsl";
    triangle.lastVertexShaderModTime = fs::last_write_time(triangle.vertexShaderPath);
    triangle.lastFragmentShaderModTime = fs::last_write_time(triangle.fragmentShaderPath);

    return triangle;
}

void reloadShaders(Object* obj) {
    auto currentVertexShaderModTime = fs::last_write_time(obj->vertexShaderPath);
    auto currentFragmentShaderModTime = fs::last_write_time(obj->fragmentShaderPath);

    if (currentVertexShaderModTime != obj->lastVertexShaderModTime ||
        currentFragmentShaderModTime != obj->lastFragmentShaderModTime) {
        std::string vertexShaderSource = loadShaderSource(obj->vertexShaderPath);
        std::string fragmentShaderSource = loadShaderSource(obj->fragmentShaderPath);

        if (!vertexShaderSource.empty() && !fragmentShaderSource.empty()) {
            GLuint newShaderProgram = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
            if (newShaderProgram) {
                glDeleteProgram(obj->shaderProgram);
                obj->shaderProgram = newShaderProgram;
                obj->lastVertexShaderModTime = currentVertexShaderModTime;
                obj->lastFragmentShaderModTime = currentFragmentShaderModTime;
                std::cout << "Shaders reloaded for object: " << obj->name << std::endl;
            }
        }
    }
}

Object initialize_cube() {
    // Define the vertices of the cube
    float vertices[] = {
        // Positions          // Colors
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f
    };

    // Define the indices for the cube
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        2, 3, 7, 7, 6, 2,
        0, 3, 7, 7, 4, 0,
        1, 2, 6, 6, 5, 1
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO first, then bind and set VBO, EBO, and configure vertex attributes
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    Object cube;
    cube.name = "Cube";
    cube.obj_type = ObjectType::Cube;
    cube.VAO = VAO;
    cube.VBO = VBO;
    cube.EBO = EBO;
    cube.shaderProgram = shaderProgram;
    cube.vertexShaderPath = "shaders/vertex_shader.glsl";
    cube.fragmentShaderPath = "shaders/fragment_shader.glsl";
    cube.lastVertexShaderModTime = fs::last_write_time(cube.vertexShaderPath);
    cube.lastFragmentShaderModTime = fs::last_write_time(cube.fragmentShaderPath);

    return cube;
}

void render_cube(Object* obj, float rotationAngle) {
    reloadShaders(obj); // Check and reload shaders if necessary

    // Use the shader program
    glUseProgram(obj->shaderProgram);

    // Calculate the rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0.5f, 1.0f, 0.0f));

    // Pass the rotation matrix to the shader
    GLint rotationMatrixLocation = glGetUniformLocation(obj->shaderProgram, "rotationMatrix");
    glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

    // Render the cube
    glBindVertexArray(obj->VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void render_triangle(Object* obj) {
    reloadShaders(obj); // Check and reload shaders if necessary
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
    static float rotationAngle = 0.0f; // Static variable to keep track of rotation angle
    rotationAngle += 1.0f; // Increment the rotation angle

    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f; // Keep the angle within 0-360 degrees
    }

    for (auto& obj : componentTree) {
        if (obj.obj_type == ObjectType::Triangle) {
            render_triangle(&obj);
        }
        else if (obj.obj_type == ObjectType::Cube) {
            render_cube(&obj, rotationAngle); // Pass the rotation angle
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