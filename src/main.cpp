// =============================================================================
// main.cpp — Eden City Entry Point
// =============================================================================
// Step 2: Hands control to the Game object which sets up GLUT,
// creates an OpenGL 3.3 Core window, and enters the main loop.
//
// main() is intentionally tiny — all real logic lives in Game.
// =============================================================================

#include "Game.h"

int main(int argc, char** argv) {
    Game game;
    game.run(argc, argv);
    return 0;
}
