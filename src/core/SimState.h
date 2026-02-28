#pragma once
#include "Camera.h"
#include "RigidBody.h"

struct SimState {
    Camera    camera;
    RigidBody sphereBody;
    // future: more bodies, constraints, etc.
};
