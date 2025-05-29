#include "common.hpp"

struct Tileset : Texture {
    v2i tileSize;
};

struct DampedCamera2D : Camera2D {
    f32 speed;

    void Use() { BeginMode2D(*(Camera2D *)this); }
    void End() { EndMode2D(); }
};

global struct {
    Camera2D guiCam;
    Shader   defaultShader;
    Texture  testBlock;
    f32      speed;
} State;

void Init() {
    auto bla = Damped<v2>{};
    auto bla = Damped<f32>{};
    auto ble = Array<v2>(50);
    InitWindow(800, 450, "Hello world!");
    // DisableCursor();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    State.defaultShader = LoadShader(0, 0);
    State.testBlock     = LoadTexture("data/test_block.png");
    State.guiCam        = {
               .offset = {400, 225},
               .target = {400, 225},
               .zoom   = 1.0f,
    };
    State.speed = 200.0f;
}

void Update() {
    // TODO normalize vector
    if (IsKeyDown(KEY_A)) State.guiCam.offset.x += State.speed * GetFrameTime();
    if (IsKeyDown(KEY_W)) State.guiCam.offset.y += State.speed * GetFrameTime();
    if (IsKeyDown(KEY_S)) State.guiCam.offset.y -= State.speed * GetFrameTime();
    if (IsKeyDown(KEY_D)) State.guiCam.offset.x -= State.speed * GetFrameTime();
    if (IsKeyPressed(KEY_E)) State.guiCam.zoom *= 2.0f;
    if (IsKeyPressed(KEY_Q)) State.guiCam.zoom /= 2.0f;

    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);

        BeginMode2D(State.guiCam);
        {
            DrawTexture(State.testBlock, 100, 100, WHITE);
        }
        EndMode2D();
    }
    EndDrawing();
}

void Close() {
    CloseWindow();
}
