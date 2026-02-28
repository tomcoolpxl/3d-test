#pragma once
#include "RigidBody.h"

// Semi-implicit Euler integration
inline void integrate(RigidBody& b, float dt)
{
    if (b.invMass == 0.0f) return;
    b.velocity  += b.accumForce * b.invMass * dt;  // v += a*dt
    b.position  += b.velocity * dt;                 // x += v*dt (new v)
    b.clearForces();
}

// Floor plane collision with restitution.
// sphereRadius: radius of sphere mesh (0.5f for our sphere).
inline void resolveFloor(RigidBody& b, float floorY, float sphereRadius,
                          float restitution)
{
    float bottom = b.position.y - sphereRadius;
    if (bottom < floorY) {
        b.position.y = floorY + sphereRadius;
        if (b.velocity.y < 0.0f)
            b.velocity.y = -b.velocity.y * restitution;
    }
}
