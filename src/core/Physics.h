#pragma once
#include <cmath>
#include <glm/gtc/quaternion.hpp>
#include "RigidBody.h"

// Factory: solid sphere body (sets invMass, invInertia, radius)
inline RigidBody makeSphere(glm::vec3 pos, float radius, float mass)
{
    RigidBody b;
    b.position   = pos;
    b.radius     = radius;
    b.invMass    = 1.0f / mass;
    b.invInertia = 5.0f * b.invMass / (2.0f * radius * radius); // I=2/5*m*r²
    return b;
}

// Semi-implicit Euler: linear + angular
inline void integrate(RigidBody& b, float dt)
{
    if (b.invMass == 0.0f) return;

    // Linear
    b.velocity += b.accumForce * b.invMass * dt;
    b.position += b.velocity * dt;

    // Angular
    b.angularVelocity += b.accumTorque * b.invInertia * dt;
    glm::quat spin(0.0f,
                   b.angularVelocity.x * 0.5f * dt,
                   b.angularVelocity.y * 0.5f * dt,
                   b.angularVelocity.z * 0.5f * dt);
    b.orientation = glm::normalize(b.orientation + spin * b.orientation);

    b.clearForces();
}

// Floor collision with restitution + Coulomb friction torque.
inline void resolveFloor(RigidBody& b, float floorY, float restitution,
                          float friction = 0.4f)
{
    if (b.position.y - b.radius >= floorY) return;

    b.position.y = floorY + b.radius;
    if (b.velocity.y >= 0.0f) return;

    // Normal impulse magnitude (infinite-mass floor)
    float jn = -(1.0f + restitution) * b.velocity.y / b.invMass; // > 0
    b.velocity.y = -b.velocity.y * restitution;

    // Friction at contact point r = (0, -radius, 0)
    const glm::vec3 rContact{0.0f, -b.radius, 0.0f};
    glm::vec3 vContact = b.velocity + glm::cross(b.angularVelocity, rContact);
    glm::vec3 vSlip{vContact.x, 0.0f, vContact.z};
    if (glm::length(vSlip) < 1e-5f) return;

    float     denom = b.invMass + b.radius * b.radius * b.invInertia;
    glm::vec3 jt    = -vSlip / denom;  // "stick" impulse

    // Coulomb clamp: |j_t| <= mu * j_n
    float jtLen = glm::length(jt);
    float jtMax = friction * jn;
    if (jtLen > jtMax) jt *= jtMax / jtLen;

    b.velocity        += jt * b.invMass;
    b.angularVelocity += glm::cross(rContact, jt) * b.invInertia;
}

// Sphere-sphere normal impulse (angular terms = 0, see note above).
inline void resolveSpherePair(RigidBody& a, RigidBody& b, float restitution)
{
    glm::vec3 delta = b.position - a.position;
    float dist2 = glm::dot(delta, delta);
    float rSum  = a.radius + b.radius;
    if (dist2 >= rSum * rSum || dist2 < 1e-8f) return;

    float     dist        = std::sqrt(dist2);
    glm::vec3 normal      = delta / dist;       // A → B
    float     penetration = rSum - dist;

    // Position correction weighted by invMass
    float totalInv = a.invMass + b.invMass;
    if (totalInv < 1e-8f) return;
    a.position -= normal * (penetration * a.invMass / totalInv);
    b.position += normal * (penetration * b.invMass / totalInv);

    // Velocity impulse
    float vRelN = glm::dot(a.velocity - b.velocity, normal);
    if (vRelN >= 0.0f) return;  // separating

    float j = -(1.0f + restitution) * vRelN / totalInv;
    a.velocity += normal * (j * a.invMass);
    b.velocity -= normal * (j * b.invMass);
}
