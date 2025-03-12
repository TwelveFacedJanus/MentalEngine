#pragma once
#ifndef ENGINECAMERA_H_
#define ENGINECAMERA_H_

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>

typedef struct EngineCamera {
    glm::vec3 camera_pos;
    glm::vec3 camera_target;
    glm::vec3 camera_direction;
    glm::vec3 up;
    glm::vec3 camera_right;
    glm::vec3 camera_up;
    glm::mat4 view;
    float speed = 0.1f;
    float sensitivity = 0.1f;
    float yaw = -90.0f;   // ”гол поворота вокруг оси Y
    float pitch = 0.0f;   // ”гол поворота вокруг оси X

    EngineCamera() {
        this->camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
        this->camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
        this->camera_direction = glm::normalize(this->camera_pos - this->camera_target);
        this->up = glm::vec3(0.0f, 1.0f, 0.0f);
        this->camera_right = glm::normalize(glm::cross(up, this->camera_direction));
        this->camera_up = glm::cross(this->camera_direction, this->camera_right);
        this->view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void processKeyboard(int key, float deltaTime) {
        float velocity = speed * deltaTime;
        if (key == GLFW_KEY_W)
            camera_pos += camera_direction * velocity;
        if (key == GLFW_KEY_S)
            camera_pos -= camera_direction * velocity;
        if (key == GLFW_KEY_A)
            camera_pos -= camera_right * velocity;
        if (key == GLFW_KEY_D)
            camera_pos += camera_right * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset) {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera_direction = glm::normalize(direction);

        // ќбновл€ем camera_right и camera_up
        camera_right = glm::normalize(glm::cross(camera_direction, up));
        camera_up = glm::normalize(glm::cross(camera_right, camera_direction));
    }

    void updateViewMatrix() {
        view = glm::lookAt(camera_pos, camera_pos + camera_direction, camera_up);
    }
} EngineCamera;

#endif // ENGINE_CAMERA_H_