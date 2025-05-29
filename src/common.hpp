#pragma once
#include <raylib/raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <concepts>
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

template <typename T>
concept VectorType = requires(f32 scalar, T t) {
    { scalar * t } -> std::same_as<T>;
};

template <VectorType T>
struct Damped {
    T _y;
};

template <typename T>
struct Array {
    T  *data;
    u32 count, len;

    Array<T>(u32 _len) : len{_len}, count{0}, data{malloc(sizeof(T) * _len)} {};
    Push(T val) {
        if (count + 1 > len) return;
        data[count++] = val;
    }
    Reset() { count = 0; }
    T Pop() {
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