# 🏙️ Eden City — Project Board

| | |
|---|---|
| **Overall Progress** | `██░░░░░░░░░░░░░░░░░░` 10% |
| **Current Week** | Week 1 — Engine Foundation |
| **Current Task** | Step 2: OpenGL Window |
| **Blocked By** | None |
| **Build System** | CMake 3.31 + MinGW GCC 14.2 (CodeBlocks) |

---

## Week 1 — Engine Foundation

| # | Step | Status | Description |
|---|---|---|---|
| 1 | Project Setup | ✅ Done | Folders, Config.h, Block.h, CMakeLists.txt, GLAD/GLM/freeglut |
| 2 | Window | 🟨 In Progress | OpenGL 3.3 context + sky-colored clear |
| 3 | Rendering Pipeline | ⬜ Not Started | ShaderProgram + first cube via VBO/VAO |
| 4 | Voxel World | ⬜ Not Started | Chunk, WorldGenerator, MeshBuilder, face culling |
| 5 | Camera + Input | ⬜ Not Started | FPS camera, WASD, mouse look |
| 6 | Collision | ⬜ Not Started | AABB block collision |
| 7 | Gravity & Jump | ⬜ Not Started | Player physics, grounded check |
| 8 | Crosshair | ⬜ Not Started | 2D HUD overlay |
| 9 | Debug Overlay | ⬜ Not Started | FPS, coords, wireframe toggle |

---

## Week 2 — Interaction & Polish

| Step | Status | Description |
|---|---|---|
| Textures | ⬜ | TextureManager, texture atlas |
| Face Culling | ⬜ | Cross-chunk boundary culling |
| Raycaster | ⬜ | Block selection (Camera → World) |
| Break Blocks | ⬜ | Left-click destroy |
| Place Blocks | ⬜ | Right-click place |
| Lighting | ⬜ | Directional light in shader |

## Week 3 — Eden City Content

| Step | Status | Description |
|---|---|---|
| Floating Islands | ⬜ | Procedural island generation |
| Buildings | ⬜ | Procedural structures |
| Roads & Bridges | ⬜ | Connections between islands |
| Crystal Towers | ⬜ | Activatable special structures |
| Fog | ⬜ | Distance fog (shader) |
| Sky | ⬜ | Skybox or gradient sky |

## Week 4 — Polish & Presentation

| Step | Status | Description |
|---|---|---|
| Entity System | ⬜ | Base Entity → Player, CrystalTower, Tree |
| Save/Load | ⬜ | Chunk serialization |
| Menu | ⬜ | Main menu screen |
| Audio | ⬜ | Ambient sounds |
| Optimization | ⬜ | Frustum culling, mesh optimization |
| Presentation | ⬜ | Final polish and prep |

---

## Dev Rule Checklist (before each new step)

- [x] Previous step builds ✅
- [x] Zero compiler warnings (W4/Wall)
- [ ] FPS stable (once rendering exists)
- [ ] Git commit made

## Build Instructions

```bash
# Configure (first time only)
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build

# Run
build\bin\EdenCity.exe
```

## Design Notes

- ✅ CMake build system (no Visual Studio files — works with CodeBlocks, VS Code, CLion)
- ✅ Modern OpenGL (VBO/VAO/shaders) — no `glBegin()`/`glEnd()`
- ✅ `Block` has `bool active` field
- ✅ `WorldGenerator` separated from `World`
- ✅ `Chunk` is data-only — `MeshBuilder` generates geometry
- ✅ `MeshBuilder` separated from `Renderer`
- ✅ Colors only in Week 1 (textures Week 2)
- ✅ Debug overlay from day one
- ✅ Entity system planned for Week 4
