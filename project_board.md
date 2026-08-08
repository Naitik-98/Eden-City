# 🏙️ Eden City — Project Board

| | |
|---|---|
| **Overall Progress** | `████████████████████` 100% |
| **Current Week** | Project Complete! |
| **Current Task** | None |
| **Blocked By** | None |
| **Build System** | CMake 3.31 + MinGW GCC 14.2 (CodeBlocks) |

---

## Week 1 — Engine Foundation

| # | Step | Status | Description |
|---|---|---|---|
| 1 | Project Setup | ✅ Done | Folders, Config.h, Block.h, CMakeLists.txt, GLAD/GLM/freeglut |
| 2 | Window | ✅ Done | OpenGL 3.3 context + sky-colored clear |
| 3 | Rendering Pipeline | ✅ Done | ShaderProgram + first cube via VBO/VAO |
| 4 | Voxel World | ✅ Done | Chunk, WorldGenerator, MeshBuilder, face culling |
| 5 | Camera + Input | ✅ Done | FPS camera, WASD, mouse look |
| 6 | Collision | ✅ Done | AABB block collision |
| 7 | Gravity & Jump | ✅ Done | Player physics, grounded check |
| 8 | Crosshair | ✅ Done | 2D HUD overlay — dedicated shader fixed |
| 9 | Debug Overlay | ✅ Done | FPS, coords, wireframe toggle |

---

## Week 2 — Interaction & Polish

| Step | Status | Description |
|---|---|---|
| Textures | ✅ Done | TextureManager + pixel art atlas (1024×1024, 4 tiles) |
| Face Culling | ✅ Done | Cross-chunk boundary culling |
| Raycaster | ✅ Done | Block selection (Camera → World) |
| Break Blocks | ✅ Done | Left-click destroy |
| Place Blocks | ✅ Done | Middle=Crystal, Right=Metal |
| Lighting | ✅ Done | Directional light in shader |

## Week 3 — Eden City Content

| Step | Status | Description |
|---|---|---|
| Floating Islands | ✅ Done | 3D Perlin noise, WORLD_CHUNKS_Y=8 (128 tall) |
| Buildings | ✅ Done | Procedural Metal+Crystal structures |
| Roads & Bridges | ✅ Done | Nearest-neighbor connections |
| Crystal Towers | ✅ Done | Activatable via E key, CrystalTowerEntity |
| Fog | ✅ Done | Distance fog in shader |
| Sky | ✅ Done | Gradient sky (sky.vert/frag) |

## Week 4 — Polish & Presentation

| Step | Status | Description |
|---|---|---|
| Entity System | ✅ Done | Base Entity → Player, CrystalTower |
| Save/Load | ✅ Done | Ctrl+S / auto-load, WorldSerializer |
| Sprint | ✅ Done | Left Shift = 1.6× speed |
| Intro Cinematic | ✅ Done | Black screen → fall from sky → play |
| Menu | ✅ Done | Main menu screen (menu.vert/frag, texture) |
| Audio | ✅ Done | miniaudio integrated, ambient loop |
| Optimization | ✅ Done | Gribb/Hartmann Frustum culling |
| Presentation | ✅ Done | Final polish |

---

## Dev Rule Checklist (before each new step)

- [x] Previous step builds ✅
- [x] Zero compiler warnings (W4/Wall) ✅
- [x] FPS stable (once rendering exists)

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
- ✅ Textures with pixel art atlas
- ✅ Debug overlay from day one
- ✅ Entity system done in Week 4
- ✅ Crosshair uses dedicated shader (position-only VAO)
- ✅ Assets (textures + all shaders) copied to build dir by CMake
