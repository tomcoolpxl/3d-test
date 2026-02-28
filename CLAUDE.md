# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Minimal cross-platform C++20 3D application using OpenGL 4.5, GLFW, GLAD, and GLM. Renders a cube and sphere with an FPS-style free-fly camera. Intended as a clean rendering/platform foundation for future physics work.

## Build Commands

```bash
# Configure (from repo root)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run
./build/3d-test          # Linux
build\Debug\3d-test.exe  # Windows
```

Dependencies (GLFW, GLM, GLAD) are fetched automatically via CMake `FetchContent` — no manual installation required.

## Source Layout

```
src/
  main.cpp         # Entry point, frame loop
  core/            # Platform-independent: Camera, transforms, future sim state
  rendering/       # OpenGL: shader compilation, mesh creation, draw calls
  platform/        # GLFW: window creation, input handling, frame timing
```

## Architecture Rules

**Layer separation is strict:**
- `core/` must have zero OpenGL or GLFW dependencies
- `rendering/` owns all VAO/VBO/EBO/shader state
- `platform/` owns all GLFW interaction and `std::chrono` timing
- No global variables except a single application state container
- No inheritance hierarchy, no virtual functions
- No per-frame heap allocations

**Cross-platform constraints:**
- Never include `windows.h` or use Win32 APIs
- Use `std::chrono` for timing (not platform timers)
- Use GLFW exclusively for input and windowing
- All file paths must be relative

## Key Decisions

**Coordinate system:** Right-handed, Y-up, -Z forward (OpenGL convention)

**Scene:** Cube (unit, centered at origin), Sphere (radius 0.5, at position 3,1,0)

**Camera:** FPS free-fly; WASD for movement, QE for vertical, mouse for yaw/pitch; 60° FOV, near 0.1, far 100.0; pitch clamped to avoid gimbal flip

**Rendering uniforms:**
- Per-frame: `mat4 view`, `mat4 projection`
- Per-object: `mat4 model`, `vec3 objectColor`

**Lighting:** Start with flat color for validation, then add simple Lambert directional shading (no shadows)

**Meshes:** Cube uses hardcoded indexed vertices with positions + normals; Sphere is UV-generated (start 16×16), indexed

**Shader system:** Load vertex + fragment from file, compile, link, uniform setters — no reflection

## Compiler Settings

| Platform | Toolset | Flags |
|----------|---------|-------|
| Windows | Clang-cl (VS2022) | `/O2 /std:c++20` |
| Linux | GCC or Clang | `-O3 -std=c++20` |

No compiler-specific extensions. No fast-math initially.
