#include "game.cpp"

int main() {
    Init();
    while (!WindowShouldClose()) Update();
    Close();

    return 0;
}