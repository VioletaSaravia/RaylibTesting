#pragma once
#include <raylib/raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <concepts>
#include <cstdint>
#include <raylib/raygui.h>

// ===== MATH =====

using i8   = int8_t;
using i16  = int16_t;
using i32  = int32_t;
using i64  = int64_t;
using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;
using f32  = float_t;
using f64  = double_t;
using cstr = char *;

using v2   = Vector2;
using v3   = Vector3;
using v4   = Vector4;
using mat4 = Matrix;

v2 operator+(v2 lhs, v2 rhs) {
    return v2{lhs.x + rhs.x, lhs.y + rhs.y};
}
v2 operator-(v2 lhs, v2 rhs) {
    return v2{lhs.x - rhs.x, lhs.y - rhs.y};
}
v2 operator*(f32 lhs, v2 rhs) {
    return v2{lhs * rhs.x, lhs * rhs.y};
}
v2 operator*(v2 lhs, f32 rhs) {
    return v2{rhs * lhs.x, rhs * lhs.y};
}

struct v2i {
    i32 x, y;
};

#define global static
#define internal static
#define persist static

template <typename T>
concept VectorType = requires(f32 scalar, T t) {
    { scalar * t } -> std::same_as<T>;
};

template <VectorType T>
struct Damped {
    T _y;
};

// ===== MEMORY =====

template <typename T>
struct Array {
    T  *data;
    u32 count, len;

    Array<T>(u32 _len) : len{_len}, count{0}, data{malloc(sizeof(T) * _len)} {};
    void Push(T val) {
        if (count + 1 > len) return;
        data[count++] = val;
    }
    void Reset() { count = 0; }
    T    Pop() {
        if (count == 0) return;
        count--;
        return data[count + 1];
    }
};

struct Arena {
    u8 *data;
    u32 used, size;

    u8 *Alloc(u32 _size) {
        if (used + _size >= size) return nullptr;

        u8 *result = &data[used];
        used += _size;
        return result;
    }
};

// ===== KEYBINDS =====

enum KeybindSource { None, Mouse, Kb, Pad1, Pad2, Pad3, Pad4, PadAny, SourceCount };

struct Keybind {
    KeybindSource source;
    union {
        struct {
            KeyboardKey mods[2];
            KeyboardKey key[2];
        } kb;
        struct {
            KeyboardKey mods[2];
            MouseButton btns[2];
        } mouse;
        struct {
            GamepadButton mods[2];
            GamepadButton btns[2];
        } pad;
    };
};

enum Action { MoveUp, MoveDown, MoveLeft, MoveRight, Accept, Cancel, ActionCount };

constexpr u32 MaxKeybinds = 2;

global Keybind Mappings[Action::ActionCount][MaxKeybinds] = {
    {{.source = ::Kb, .kb = {.key = KEY_W}},
     {.source = ::Pad1, .pad = {.btns = {GAMEPAD_BUTTON_LEFT_FACE_UP}}}}, // MoveUp
    {{.source = ::Kb, .kb = {.key = KEY_S}},
     {.source = ::Pad1, .pad = {.btns = {GAMEPAD_BUTTON_LEFT_FACE_DOWN}}}}, // MoveDown
    {{.source = ::Kb, .kb = {.key = KEY_A}},
     {.source = ::Pad1, .pad = {.btns = {GAMEPAD_BUTTON_LEFT_FACE_LEFT}}}}, // MoveLeft
    {{.source = ::Kb, .kb = {.key = KEY_D}},
     {.source = ::Pad1, .pad = {.btns = {GAMEPAD_BUTTON_LEFT_FACE_RIGHT}}}}, // MoveRight
    {{.source = ::Kb, .kb = {.key = KEY_ENTER}},
     {.source = ::Pad1, .pad = {.btns = {GAMEPAD_BUTTON_RIGHT_FACE_DOWN}}}}, // Accept
    {{.source = ::Kb, .kb = {.key = KEY_ESCAPE}},
     {.source = ::Pad1, .pad = {.btns = {GAMEPAD_BUTTON_RIGHT_FACE_RIGHT}}}}, // Cancel
};


bool IsActionDown(Action action) {
    auto mapping = Mappings[action];

    for (u32 i = 0; i < MaxKeybinds; i++) {
        auto cur = mapping[i];
        switch (cur.source) {
        case ::None: continue;
        case ::Kb:
            if (IsKeyDown(cur.kb.mods[0]) && IsKeyDown(cur.kb.mods[1]) &&
                IsKeyDown(cur.kb.key[0]) && IsKeyDown(cur.kb.key[1]))
                return true;
            break;

        case ::Mouse:
            if (IsKeyDown(cur.mouse.mods[0]) && IsKeyDown(cur.mouse.mods[1]) &&
                IsMouseButtonDown(cur.mouse.btns[0]) && IsMouseButtonDown(cur.mouse.btns[1]))
                return true;
            break;

        case ::Pad1:
        case ::Pad2:
        case ::Pad3:
        case ::Pad4:
            auto gamepad = cur.source - 3;
            if (!IsGamepadAvailable(gamepad)) continue;
            if (IsGamepadButtonDown(gamepad, cur.pad.mods[0]) &&
                IsGamepadButtonDown(gamepad, cur.pad.mods[1]) &&
                IsGamepadButtonDown(gamepad, cur.pad.btns[0]) &&
                IsGamepadButtonDown(gamepad, cur.pad.btns[1]))
                return true;
            break;
        }

        // TODO
        // case ::PadAny:
    }

    return false;
}

bool IsActionUp(Action action) {
    auto mapping = Mappings[action];

    for (u32 i = 0; i < MaxKeybinds; i++) {
        auto cur = mapping[i];
        switch (cur.source) {
        case ::None: continue;
        case ::Kb:
            if (IsKeyUp(cur.kb.key[0]) && IsKeyUp(cur.kb.key[1])) return true;
            break;

        case ::Mouse:
            if (IsMouseButtonUp(cur.mouse.btns[0]) && IsMouseButtonUp(cur.mouse.btns[1]))
                return true;
            break;

        case ::Pad1:
        case ::Pad2:
        case ::Pad3:
        case ::Pad4:
            auto gamepad = cur.source - 3;
            if (!IsGamepadAvailable(gamepad)) continue;
            if (IsGamepadButtonUp(gamepad, cur.pad.btns[0]) &&
                IsGamepadButtonUp(gamepad, cur.pad.btns[1]))
                return true;
            break;
        }
    }

    return false;
}

bool IsActionPressed(Action action) {
    auto mapping = Mappings[action];

    for (u32 i = 0; i < MaxKeybinds; i++) {
        auto cur = mapping[i];
        switch (cur.source) {
        case ::None: continue;
        case ::Kb:
            if (IsKeyDown(cur.kb.mods[0]) && IsKeyDown(cur.kb.mods[1]) &&
                IsKeyPressed(cur.kb.key[0]) && IsKeyPressed(cur.kb.key[1]))
                return true;
            break;

        case ::Mouse:
            if (IsKeyDown(cur.mouse.mods[0]) && IsKeyDown(cur.mouse.mods[1]) &&
                IsMouseButtonPressed(cur.mouse.btns[0]) && IsMouseButtonPressed(cur.mouse.btns[1]))
                return true;
            break;

        case ::Pad1:
        case ::Pad2:
        case ::Pad3:
        case ::Pad4:
            auto gamepad = cur.source - 3;
            if (!IsGamepadAvailable(gamepad)) continue;
            if (IsGamepadButtonDown(gamepad, cur.pad.mods[0]) &&
                IsGamepadButtonDown(gamepad, cur.pad.mods[1]) &&
                IsGamepadButtonPressed(gamepad, cur.pad.btns[0]) &&
                IsGamepadButtonPressed(gamepad, cur.pad.btns[1]))
                return true;
            break;
        }
    }

    return false;
}

bool IsActionPressedRepeat(Action action) {
    auto mapping = Mappings[action];

    for (u32 i = 0; i < MaxKeybinds; i++) {
        auto cur = mapping[i];
        switch (cur.source) {
        case ::None: continue;
        case ::Kb:
            if (IsKeyDown(cur.kb.mods[0]) && IsKeyDown(cur.kb.mods[1]) &&
                IsKeyPressedRepeat(cur.kb.key[0]) && IsKeyPressedRepeat(cur.kb.key[1]))
                return true;
            break;

        case ::Mouse: break;

        case ::Pad1:
        case ::Pad2:
        case ::Pad3:
        case ::Pad4: break;
        }
    }

    return false;
}

bool IsActionReleased(Action action) {
    auto mapping = Mappings[action];

    for (u32 i = 0; i < MaxKeybinds; i++) {
        auto cur = mapping[i];
        switch (cur.source) {
        case ::None: continue;
        case ::Kb:
            if (IsKeyDown(cur.kb.mods[0]) && IsKeyDown(cur.kb.mods[1]) &&
                IsKeyReleased(cur.kb.key[0]) && IsKeyReleased(cur.kb.key[1]))
                return true;
            break;

        case ::Mouse:
            if (IsKeyDown(cur.mouse.mods[0]) && IsKeyDown(cur.mouse.mods[1]) &&
                IsMouseButtonReleased(cur.mouse.btns[0]) &&
                IsMouseButtonReleased(cur.mouse.btns[1]))
                return true;
            break;

        case ::Pad1:
        case ::Pad2:
        case ::Pad3:
        case ::Pad4:
            auto gamepad = cur.source - 3;
            if (!IsGamepadAvailable(gamepad)) continue;
            if (IsGamepadButtonDown(gamepad, cur.pad.mods[0]) &&
                IsGamepadButtonDown(gamepad, cur.pad.mods[1]) &&
                IsGamepadButtonReleased(gamepad, cur.pad.btns[0]) &&
                IsGamepadButtonReleased(gamepad, cur.pad.btns[1]))
                return true;
            break;
        }
    }

    return false;
}