#pragma once

#include <glm/glm.hpp>

struct Camera {
    glm::vec3 position{0.0f, 1.0f, 5.0f};
    float yaw{-90.0f};   // degrees; -90 → looking down -Z
    float pitch{0.0f};   // degrees

    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 right{1.0f, 0.0f,  0.0f};
    glm::vec3 up   {0.0f, 1.0f,  0.0f};

    void updateVectors();
    glm::mat4 viewMatrix() const;

    void processMouseDelta(float dx, float dy, float sensitivity = 0.1f);
    void processMovement(bool fwd, bool back, bool left, bool right,
                         bool ascend, bool descend, float speed, float dt);
};
