#include "../engine/common.hpp"

Shader AnimShader;

struct AnimatedTex {
    Texture tex;
    v2      tileSize;

    f32 animSpeed;
    f32 animLength, animCounter;

    f32 curAnimation, curFrame;

    AnimatedTex(constr path, v2 _tileSize, f32 _animLength, f32 _animSpeed = .1f)
        : tex{LoadTexture(path)}, tileSize{_tileSize}, animLength{_animLength},
          animSpeed{_animSpeed}, curAnimation{1} {}
};

struct BoidSim {
    AnimatedTex *tex;
    u64          count;
    v2          *pos;
    v2          *dir;
    f32         *speed;
    f32          los;
    f32          losMatching;

    BoidSim(AnimatedTex *_tex, f32 _los, f32 _losMatching, u64 _count = 64)
        : tex{_tex}, los{_los}, losMatching{_losMatching}, count{_count} {
        pos   = (v2 *)malloc(sizeof(v2) * _count);
        dir   = (v2 *)malloc(sizeof(v2) * _count);
        speed = (f32 *)malloc(sizeof(f32) * _count);

        for (u64 i = 0; i < _count; i++) {
            pos[i].x = GetRandomValue(50, 750);
            pos[i].y = GetRandomValue(50, 400);

            f32 angle = RandF() * 2.0f * PI;
            dir[i]    = {cosf(angle), sinf(angle)};
            speed[i]  = 100.0f + RandF() * 100.0f;
        }
    }

    void UpdateSim() {
        for (u64 i = 0; i < count; i++) {
            Ray testRay          = {v3{pos[i].x, pos[i].y}, v3{dir[i].x, dir[i].y}};
            v2  nearbyDirSum     = {};
            v2  nearbyPosSum     = {};
            f32 nearbyBoidsCount = 0;
            for (u64 j = 0; j < count; j++) {
                if (j == i) continue;

                // RULE 2 Calculation
                f32 dist = Vector2Distance(pos[i], pos[j]);
                if (dist < losMatching) {
                    f32 iAngle = atan2f(dir[j].y, dir[j].x);
                    nearbyDirSum += dir[j];
                    nearbyPosSum += pos[j];
                    nearbyBoidsCount++;
                }

                // RULE 1: Avoid boids in los
                auto col = GetRayCollisionBox(
                    testRay, BoundingBox{v3{pos[j].x, pos[j].y}, v3{pos[j].x + tex->tex.width,
                                                                    pos[j].y + tex->tex.height}});
                if (col.hit && col.distance < los) {
                    dir[i] =
                        Vector2Normalize(Vector2Lerp(dir[i], dir[j] - dir[i], 1 * GetFrameTime()));
                    break;
                }
            }

            // RULE 2: Average directions of group
            if (nearbyBoidsCount > 0) {
                dir[i] = Vector2Normalize(
                    Vector2Lerp(dir[i], Vector2Normalize(nearbyDirSum), 1 * GetFrameTime()));
            }

            // RULE 3: Move towards center of group
            if (nearbyBoidsCount > 0) {
                v2 posAvg = nearbyPosSum / nearbyBoidsCount;
                v2 dirAvg = Vector2Normalize(pos[i] - posAvg);
                dir[i]    = Vector2Normalize(Vector2Lerp(dir[i], dirAvg, 2 * GetFrameTime()));
            }

            // RULE 4: Follow cursor
            // auto dirToCursor = Vector2Normalize(GetMousePosition() - pos[j]);
            // dir[j] = Vector2Normalize(Vector2Lerp(dir[j], dirToCursor, 1 * GetFrameTime()));

            // Update
            dir[i] = Vector2Rotate(dir[i], RandF() * GetFrameTime());
            speed[i] += f32(GetRandomValue(-20, 20)) * GetFrameTime();
            pos[i] += dir[i] * GetFrameTime() * speed[i];
            pos[i] %= {800, 450};
        }
    }

    void Draw() {
        tex->animCounter += GetFrameTime();
        if (tex->animCounter >= tex->animSpeed) {
            tex->animCounter -= tex->animSpeed;
            tex->curFrame = fmodf((tex->curFrame + 1), tex->animLength);
        }

        for (u64 i = 0; i < count; i++) {
            f32 iAngle   = atan2f(dir[i].y, dir[i].x) + PI;
            u32 iAnimDir = u32((iAngle / (2 * PI)) * 8.0f);

            DrawTexturePro(tex->tex,
                           {(tex->curFrame * tex->tileSize.x) +
                                (tex->curAnimation * tex->tileSize.x * tex->animLength),
                            iAnimDir * tex->tileSize.y, tex->tileSize.x, tex->tileSize.y},
                           {pos[i].x, pos[i].y, tex->tileSize.x / 2, tex->tileSize.y / 2},
                           tex->tileSize / 2, 0, WHITE);
        }
    }
};

global struct {
    Camera2D cam;
    Shader   defaultShader;

    // Boids
    AnimatedTex *boidTex;
    BoidSim     *boids01;
    BoidSim     *boids02;
    BoidSim     *boids03;
} State;

void Init() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(800, 450, "Hello world!");
    // DisableCursor();
    // SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetRandomSeed(1337);

    State.defaultShader = LoadShader("shaders/default.vert", "shaders/default.frag");
    State.cam           = {
                  .offset = {400, 225},
                  .target = {400, 225},
                  .zoom   = 1.0f,
    };

    State.boidTex = new AnimatedTex("data/wyvern.png", v2{256, 256}, 8.0f, .1f);

    State.boids01 = new BoidSim(State.boidTex, 96, 32, 6);
    State.boids02 = new BoidSim(State.boidTex, 96, 32, 6);
    State.boids03 = new BoidSim(State.boidTex, 96, 32, 3);
}

void Update() {
    State.boids01->UpdateSim();
    State.boids02->UpdateSim();
    // State.boids03->UpdateSim();

    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawMsPF(10, 10);

        BeginMode2D(State.cam);
        BeginShaderMode(State.defaultShader);
        {
            State.boids01->Draw();
            State.boids02->Draw();
            State.boids03->Draw();
        }
        EndShaderMode();

        EndMode2D();
    }
    EndDrawing();
}

void Close() {
    CloseWindow();
}
