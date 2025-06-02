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
    Camera2D cam;
    Shader   defaultShader;
    Texture  testBlock;
    f32      speed;
} State;

void AddKeybind(Action action, Keybind keybind, u32 slot = 0) {
    Mappings[action][slot] = keybind;
}

void RemoveKeybind(Action action, u32 slot) {
    Mappings[action][slot].source = ::None;
}

void Init() {
    InitWindow(800, 450, "Hello world!");
    // DisableCursor();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    State.defaultShader = LoadShader(0, 0);
    State.testBlock     = LoadTexture("data/test_block.png");
    State.cam           = {
                  .offset = {400, 225},
                  .target = {400, 225},
                  .zoom   = 1.0f,
    };
    State.speed = 200.0f;
}

void Update() {
    if (IsActionDown(Action::MoveRight)) printf("Actions work?\n");
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);

        BeginMode2D(State.cam);
        {
            for (u32 i = 0; i < Action::ActionCount; i++) {
                GuiButton({0, (f32)i * 50, 40, 40}, TextFormat("Action: %d", i));
                GuiButton({50, (f32)i * 50, 40, 40}, TextFormat("%d", Mappings[i][0].source));
                GuiButton({100, (f32)i * 50, 40, 40}, TextFormat("%d", Mappings[i][1].source));
            }
        }
        EndMode2D();
    }
    EndDrawing();
}

void Close() {
    CloseWindow();
}
