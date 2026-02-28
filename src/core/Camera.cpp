#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

void Camera::updateVectors()
{
    float yawR   = glm::radians(yaw);
    float pitchR = glm::radians(pitch);

    glm::vec3 f;
    f.x = std::cos(pitchR) * std::cos(yawR);
    f.y = std::sin(pitchR);
    f.z = std::cos(pitchR) * std::sin(yawR);

    front = glm::normalize(f);
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::viewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::processMouseDelta(float dx, float dy, float sensitivity)
{
    yaw   += dx * sensitivity;
    pitch += dy * sensitivity;
    pitch  = std::clamp(pitch, -89.0f, 89.0f);
    updateVectors();
}

void Camera::processMovement(bool fwd, bool back, bool left, bool right_,
                              bool ascend, bool descend, float speed, float dt)
{
    float v = speed * dt;
    if (fwd)     position += front * v;
    if (back)    position -= front * v;
    if (left)    position -= right  * v;
    if (right_)  position += right  * v;
    if (ascend)  position += glm::vec3(0.0f, 1.0f, 0.0f) * v;
    if (descend) position -= glm::vec3(0.0f, 1.0f, 0.0f) * v;
}
