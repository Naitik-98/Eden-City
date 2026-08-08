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

## Step 2: OpenGL Window ✅

- [x] Create Game.h + Game.cpp (owns all subsystems, main loop)
- [x] Create Renderer.h + Renderer.cpp (minimal: clear sky color)
- [x] GLUT init in main.cpp: glutInit, glutCreateWindow, glutMainLoop
- [x] OpenGL 3.3 Core context via glutInitContextVersion
- [x] gladLoadGLLoader() after GLUT creates context
- [x] Clear color = Config::SKY_R/G/B
- [x] Window title = "Eden City"
- [x] Verify window opens and clears to dark blue sky


---

## Step 3: Rendering Pipeline ✅

- [x] Create ShaderProgram.h + ShaderProgram.cpp
- [x] Write full basic.vert (position + normal + UV + MVP)
- [x] Write full basic.frag (texture + lighting + fog)
- [x] VBO/VAO setup for a colored cube
- [x] MVP matrices: glm::perspective + glm::lookAt + glm::mat4(1.0)
- [x] Render first colored cube on screen


---

## Step 4: Voxel World ✅

- [x] Chunk.h + Chunk.cpp (16x16x16 Block array, isDirty flag)
- [x] World.h + World.cpp (owns chunks, isSolidAt(), getBlockAt())
- [x] WorldGenerator.h + WorldGenerator.cpp (flat terrain)
- [x] MeshBuilder.h + MeshBuilder.cpp (chunk → VBO/VAO, face culling)
- [x] Render flat voxel ground


---

## Step 5: FPS Camera + Input ✅

- [x] Camera.h + Camera.cpp (yaw/pitch → view matrix via glm::lookAt)
- [x] InputManager.h + InputManager.cpp (GLUT callbacks → pollable state)
- [x] WASD movement
- [x] Mouse look (glutWarpPointer infinite capture)


---

## Step 6: Collision Detection ✅

- [x] Player.h + Player.cpp (owns Camera, AABB 0.6x1.8)
- [x] Per-axis AABB vs World::isSolidAt()
- [x] Cannot walk through blocks


---

## Step 7: Gravity & Jumping ✅

- [x] Euler integration: vel.y -= gravity * dt
- [x] Jump impulse when grounded
- [x] Terminal velocity clamp


---

## Step 8: Crosshair HUD ✅

- [x] Orthographic projection overlay in Renderer
- [x] Two short lines at screen center
- [x] Dedicated crosshair shader (fix: separate from world shader)


---

## Step 9: Debug Overlay ✅

- [x] Debug.h + Debug.cpp
- [x] F3 toggle: FPS, position, chunk coords (title bar)
- [x] F4 toggle: wireframe mode


---

## Week 2: Interaction & Polish ✅

- [x] TextureManager + stb_image + atlas.png (proper pixel art tiles)
- [x] Cross-chunk face culling (MeshBuilder)
- [x] Raycaster (Camera → World block selection)
- [x] Left-click: break blocks
- [x] Middle-click: place Crystal
- [x] Right-click: place Metal
- [x] Directional lighting (shader)
- [x] Distance fog (shader)


---

## Week 3: Eden City Content ✅

- [x] Floating island generation (3D Perlin noise with vertical gradient)
- [x] Procedural buildings (Metal walls + Crystal windows)
- [x] Roads (nearest-neighbor connections)
- [x] Crystal Towers (rare, activatable via E key)
- [x] Distance fog
- [x] Gradient sky (sky.vert/frag, fullscreen quad)
- [x] Config: WORLD_CHUNKS_Y = 8 (128 blocks tall)


---

## Week 4: Polish & Presentation ✅

- [x] Entity base class + CrystalTowerEntity
- [x] WorldSerializer — Ctrl+S save / auto-load on startup
- [x] Sprint (Left Shift, 1.6× speed)
- [x] Intro cinematic (black screen → fall from sky)
- [x] Main menu screen
- [x] Audio (ambient sounds — miniaudio)
- [x] Frustum culling (skip chunks outside camera frustum)
- [x] Final visual polish
