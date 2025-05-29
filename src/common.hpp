#pragma once
#include <raylib/raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <cstdint>
#include <raylib/raygui.h>

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
