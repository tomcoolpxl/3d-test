#pragma once
#include <glm/glm.hpp>

struct RigidBody {
    glm::vec3 position  {3.0f, 4.0f, 0.0f};  // start above scene
    glm::vec3 velocity  {0.0f, 0.0f, 0.0f};
    float     invMass   {1.0f};               // 1/kg; 0 = infinite mass (static)
    glm::vec3 accumForce{0.0f, 0.0f, 0.0f};

    void applyForce(glm::vec3 f) { accumForce += f; }
    void clearForces()           { accumForce = {0,0,0}; }
};
