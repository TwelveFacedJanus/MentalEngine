#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_


#include <GL/glew.h> // Include OpenGL headers
#include <vector>
#include <string>
#include <filesystem>

#include "Shader.h"

typedef enum ObjectType {
	Triangle,
	Node2D,
} ObjectType;

typedef struct Object
{
	const char* name;
	ObjectType obj_type;
	std::vector<Object> child_comps;
	GLuint VAO, VBO, EBO;
	GLuint shader_program;

	std::string vertex_shader_path;
	std::string fragment_shader_path;

	std::filesystem::file_time_type last_vs_mt;
	std::filesystem::file_time_type last_fs_mt;

	Object(ObjectType objt, const char* n) : obj_type(objt), name(n) {
		if (objt == ObjectType::Triangle) {
			this->initialize_triangle();
		}
		else if (objt == ObjectType::Node2D) {
			this->initialize_node2d();
		}
		this->shader_program = 0;
	}

	void initialize_node2d() {
		std::cout << "Node 2D initialized!\n";
	}

	void initialize_triangle(void)
	{
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		this->EBO = 0;
	}

	void initialize_cube(void) {
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

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			0, 1, 5, 5, 4, 0,
			2, 3, 7, 7, 6, 2,
			0, 3, 7, 7, 4, 0,
			1, 2, 6, 6, 5, 1
		};

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		// Bind VAO first, then bind and set VBO, EBO, and configure vertex attributes
		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
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
	}
	void applyShaders(const char* vsp, const char* fsp) {
		std::string vertex_shader_source = loadShaderSource(vsp);
		std::string fragment_shader_source = loadShaderSource(fsp);

		if (vertex_shader_source.empty() || fragment_shader_source.empty())
		{
			std::cerr << "ERROR: Failed to load empty shader.\n";
			return;
		}

		std::cout << "Shaders has been loaded!\n";
		this->shader_program = createShaderProgram(vertex_shader_source.c_str(), fragment_shader_source.c_str());
		std::cout << "Shader program has been created!\n";
		this->vertex_shader_path = vsp;
		this->fragment_shader_path = fsp;
		this->last_vs_mt = std::filesystem::last_write_time(this->vertex_shader_path);
		this->last_fs_mt = std::filesystem::last_write_time(this->fragment_shader_path);
	}


	void render(void) {
		if (!this->child_comps.empty()) {
			for (auto& o : this->child_comps) {
				o.render();
			}
		}
		if (this->shader_program != 0) {
			this->reload_shaders();
			glUseProgram(this->shader_program);
		}
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

	void reload_shaders(void) {
		auto currentVertexShaderModTime = std::filesystem::last_write_time(this->vertex_shader_path);
		auto currentFragmentShaderModTime = std::filesystem::last_write_time(this->fragment_shader_path);

		if (currentVertexShaderModTime != this->last_vs_mt ||
			currentFragmentShaderModTime != this->last_fs_mt) {
			std::string vertexShaderSource = loadShaderSource(this->vertex_shader_path);
			std::string fragmentShaderSource = loadShaderSource(this->fragment_shader_path);

			if (!vertexShaderSource.empty() && !fragmentShaderSource.empty()) {
				GLuint newShaderProgram = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
				if (newShaderProgram) {
					glDeleteProgram(this->shader_program);
					this->shader_program = newShaderProgram;
					this->last_vs_mt = currentVertexShaderModTime;
					this->last_fs_mt = currentFragmentShaderModTime;
					std::cout << "Shaders reloaded for object: " << this->name << std::endl;
				}
			}
		}
	}
	/*
	void add_child_obj(Object obj) {
		this->child_comps.push_back(obj);
	}
	*/
	~Object() {}
} Object;

#endif // OBJECT_H_