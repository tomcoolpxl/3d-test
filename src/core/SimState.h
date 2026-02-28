#pragma once
#include <vector>
#include "Camera.h"
#include "RigidBody.h"

struct SimState {
    Camera                 camera;
    std::vector<RigidBody> bodies;
};
