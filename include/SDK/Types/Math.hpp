#pragma once
#include "VarTypes.hpp"
#include "ZigZag.hpp"

template <typename T>
class Vector2 {
public:
    Vector2(T x, T y) {
        this->x = x;
        this->y = y;
    }

    T x, y;
};

template <typename T>
class Vector3 {
public:
    Vector3(T x, T y, T z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    T x, y, z;
};

class BlockCoordinates {
public:
    BlockCoordinates(const ZigZag32 x, const VarInt y, const ZigZag32 z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    ZigZag32 x;
    VarInt y;
    ZigZag32 z;
};