#include "../engine/common.hpp"

global struct {
    Camera2D cam;
    Shader   defaultShader;
} State;

void Init() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(800, 450, "Hello world!");
    // DisableCursor();
    // SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetRandomSeed(1337);

    State.defaultShader = LoadShader("shaders/default.vert", "shaders/default.frag");
    State.cam           = {
                  .zoom = 1.0f,
    };
}

void Update() {
    BeginDrawing();
    {
        ClearBackground(GRAY);
        DrawMsPF(10, 10);

        BeginMode2D(State.cam);
        BeginShaderMode(State.defaultShader);
        {
        }
        EndShaderMode();
        EndMode2D();
    }
    EndDrawing();
}

void Close() {
    CloseWindow();
}
