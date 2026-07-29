# Eden City — Task List

## Step 1: Project Setup ✅

- [x] Create folder structure (include/, src/, assets/shaders/, lib/)
- [x] Create Config.h (all game constants)
- [x] Create Block.h (BlockType enum, Block struct, color mapping)
- [x] Create .gitignore (VS/C++ + CMake patterns)
- [x] Create README.md
- [x] Create main.cpp (minimal skeleton — verifies Config.h + Block.h compile)
- [x] Replace .vcxproj/.sln with CMakeLists.txt (CMake + MinGW build system)
- [x] Download & integrate GLAD (OpenGL 3.3 Core) into lib/glad/
- [x] Download & integrate GLM (header-only) into lib/glm/
- [x] Download & integrate freeglut (prebuilt x64) into lib/freeglut/
- [x] Verify project builds with zero warnings (CMake + MinGW GCC 14)
- [x] Verify exe runs and prints correct output


---

## Step 2: OpenGL Window 🔜

- [ ] Create Game.h + Game.cpp (owns all subsystems, main loop)
- [ ] Create Renderer.h + Renderer.cpp (minimal: clear sky color)
- [ ] GLUT init in main.cpp: glutInit, glutCreateWindow, glutMainLoop
- [ ] OpenGL 3.3 Core context via glutInitContextVersion
- [ ] gladLoadGLLoader() after GLUT creates context
- [ ] Clear color = Config::SKY_R/G/B
- [ ] Window title = "Eden City"
- [ ] Verify window opens and clears to dark blue sky


---

## Step 3: Rendering Pipeline ⬜

- [ ] Create ShaderProgram.h + ShaderProgram.cpp
- [ ] Write full basic.vert (position + color + MVP)
- [ ] Write full basic.frag (pass-through color)
- [ ] VBO/VAO setup for a colored cube
- [ ] MVP matrices: glm::perspective + glm::lookAt + glm::mat4(1.0)
- [ ] Render first colored cube on screen


---

## Step 4: Voxel World ⬜

- [ ] Chunk.h + Chunk.cpp (16x16x16 Block array, isDirty flag)
- [ ] World.h + World.cpp (owns chunks, isSolidAt(), getBlockAt())
- [ ] WorldGenerator.h + WorldGenerator.cpp (flat terrain)
- [ ] MeshBuilder.h + MeshBuilder.cpp (chunk → VBO/VAO, face culling)
- [ ] Render flat voxel ground


---

## Step 5: FPS Camera + Input ⬜

- [ ] Camera.h + Camera.cpp (yaw/pitch → view matrix via glm::lookAt)
- [ ] InputManager.h + InputManager.cpp (GLUT callbacks → pollable state)
- [ ] WASD movement
- [ ] Mouse look (glutWarpPointer infinite capture)


---

## Step 6: Collision Detection ⬜

- [ ] Player.h + Player.cpp (owns Camera, AABB 0.6x1.8)
- [ ] Per-axis AABB vs World::isSolidAt()
- [ ] Cannot walk through blocks


---

## Step 7: Gravity & Jumping ⬜

- [ ] Euler integration: vel.y -= gravity * dt
- [ ] Jump impulse when grounded
- [ ] Terminal velocity clamp


---

## Step 8: Crosshair HUD ⬜

- [ ] Orthographic projection overlay in Renderer
- [ ] Two short lines at screen center


---

## Step 9: Debug Overlay ⬜

- [ ] Debug.h + Debug.cpp
- [ ] F3 toggle: FPS, position, chunk coords
- [ ] F4 toggle: wireframe mode

