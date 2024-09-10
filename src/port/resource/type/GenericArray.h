#pragma once

#include <cstdint>
#include <Resource.h>
#include <libultraship/libultra/types.h>

namespace SF64 {

struct Vec2f {
    float x, y;
    Vec2f(float x, float y) : x(x), y(y) {
    }
};

struct Vec3f {
    float x, y, z;
    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {
    }
};

struct Vec3s {
    int16_t x, y, z;
    Vec3s(int16_t x, int16_t y, int16_t z) : x(x), y(y), z(z) {
    }
};

struct Vec3i {
    int32_t x, y, z;
    Vec3i(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z) {
    }
};

struct Vec3iu {
    uint32_t x, y, z;
    Vec3iu(uint32_t x, uint32_t y, uint32_t z) : x(x), y(y), z(z) {
    }
};

struct Vec4f {
    float x, y, z, w;
    Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
    }
};

struct Vec4s {
    int16_t x, y, z, w;
    Vec4s(int16_t x, int16_t y, int16_t z, int16_t w) : x(x), y(y), z(z), w(w) {
    }
};

enum class ArrayType {
    u8,
    s8,
    u16,
    s16,
    u32,
    s32,
    u64,
    f32,
    f64,
    Vec2f,
    Vec3f,
    Vec3s,
    Vec3i,
    Vec3iu,
    Vec4f,
    Vec4s,
};

class GenericArray : public Ship::Resource<uint8_t> {
  public:
    using Resource::Resource;

    GenericArray() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    uint8_t* GetPointer();
    size_t GetPointerSize();

    std::vector<uint8_t> mData;
    size_t mSize;
};
} // namespace SF64
