# Project Plan

## Goal

Build a minimal cross-platform C++20 3D application that:

* Runs on Windows first
* Compiles on Linux without architectural changes
* Uses OpenGL 4.5
* Uses GLFW for windowing and input
* Uses GLAD for OpenGL loading
* Uses GLM for math
* Has no UI framework
* Renders:

  * One cube
  * One sphere (ball)
* Supports free-fly FPS-style camera navigation with mouse look

This project establishes a clean rendering and platform foundation for later physics work.

---

## Architectural Decisions (Fixed)

Camera mode:

* Free-fly FPS camera
* Mouse controls yaw and pitch
* WASD movement
* Optional QE for vertical movement

Scene layout:

* Objects placed separately in space
* Cube at world origin (0, 0, 0)
* Sphere at (3, 1, 0)

Coordinate system:

* Right-handed
* Y up
* -Z forward (OpenGL convention)

Cube size:

* Unit cube (1.0 edge length)
* Centered at origin

Sphere radius:

* 0.5

---

## High-Level Architecture

Strict separation between layers.

src/
core/
rendering/
platform/
main.cpp

core/

* Camera
* Transform math
* Future simulation state
* No OpenGL or GLFW

rendering/

* OpenGL initialization
* Shader compilation
* Mesh creation
* Draw calls

platform/

* GLFW window creation
* Input handling
* Frame timing

The core layer must remain fully platform-independent.

---

## Toolchain (Windows First)

Recommended:

* Visual Studio 2022
* Clang-cl toolset
* CMake

Dependencies managed via CMake FetchContent (preferred for portability):

* GLFW
* GLM
* GLAD

Avoid platform-specific APIs.

---

## Toolchain (Linux Later)

* GCC or Clang
* CMake
* System OpenGL drivers

No code changes required.

---

## Build System Structure

Root:

* CMakeLists.txt
* src/

CMake requirements:

* C++20
* No compiler-specific extensions
* Separate targets for external libraries
* Enable warnings

No global include directories.

---

## Rendering Plan

### Step 1: Window and Context

* Create GLFW window
* Request OpenGL 4.5 core profile
* Enable depth testing
* Load GLAD
* Disable VSync initially (optional for debugging)

### Step 2: Shader System

Minimal shader abstraction:

* Load vertex + fragment shader from file
* Compile
* Link
* Uniform setters

No reflection system.

### Step 3: Meshes

Cube:

* Hardcoded vertex positions
* Indexed drawing
* Positions + normals

Sphere:

* UV sphere generation
* Configurable resolution (start low, e.g., 16x16)
* Indexed drawing

Each mesh stores:

* VAO
* VBO
* EBO

No dynamic vertex updates.

### Step 4: Camera System (FPS)

Camera state:

* vec3 position
* float yaw
* float pitch
* vec3 front
* vec3 right
* vec3 up

Controls:

* Mouse delta updates yaw/pitch
* Clamp pitch to avoid flip
* WASD moves along local axes
* QE moves along world Y

Projection:

* Perspective
* 60 degree FOV
* Near 0.1
* Far 100.0

Camera logic lives in core/.

---

## Frame Loop Structure

while (running)

* Poll events
* Compute delta time
* Update camera from input
* Clear color + depth
* Compute view matrix
* Compute projection matrix
* Draw cube
* Draw sphere
* Swap buffers

No simulation yet.

---

## Rendering Pipeline

Uniforms per frame:

* mat4 view
* mat4 projection

Uniform per object:

* mat4 model
* vec3 objectColor

Lighting:

Initial:

* Flat color (for validation)

Then:

* Add simple directional light
* Basic Lambert shading

No shadows.

---

## Cross-Platform Constraints

* Do not include windows.h
* Do not use Win32 input
* Do not use platform timers
* Use std::chrono
* Use GLFW exclusively for input and windowing

All file paths relative.

---

## Compiler Settings

Windows (Clang-cl):

* /O2
* /std:c++20

Linux (GCC/Clang):

* -O3
* -std=c++20

No fast-math initially.

---

## Validation Milestones

Milestone 1
Window opens and clears screen.

Milestone 2
Cube renders correctly.

Milestone 3
Sphere renders correctly.

Milestone 4
Mouse look works smoothly.

Milestone 5
WASD movement works.

Milestone 6
Builds and runs on Linux without source modification.

---

## Quality Constraints

* No global variables except application state container
* No memory leaks
* No per-frame allocations
* No inheritance hierarchy
* No virtual functions
* Clean separation between rendering and core

---

## Next Phase After Completion

* Add instanced rendering
* Introduce simulation state container
* Introduce fixed timestep loop
* Begin gravity implementation

The goal of this first project is architectural correctness, not visual complexity.
