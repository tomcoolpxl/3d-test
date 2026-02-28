#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct RigidBody {
    glm::vec3 position       {0.0f, 0.0f, 0.0f};
    glm::vec3 velocity       {0.0f, 0.0f, 0.0f};
    glm::quat orientation    {1.0f, 0.0f, 0.0f, 0.0f}; // identity (w,x,y,z)
    glm::vec3 angularVelocity{0.0f, 0.0f, 0.0f};
    float     invMass        {1.0f};   // 1/kg; 0 = static
    float     invInertia     {1.0f};   // scalar; solid sphere: 5*invMass/(2*r²)
    float     radius         {0.5f};
    glm::vec3 accumForce     {0.0f, 0.0f, 0.0f};
    glm::vec3 accumTorque    {0.0f, 0.0f, 0.0f};

    void applyForce (glm::vec3 f) { accumForce  += f; }
    void applyTorque(glm::vec3 t) { accumTorque += t; }
    void clearForces() { accumForce = {}; accumTorque = {}; }
};
