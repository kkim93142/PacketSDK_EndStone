#pragma once
#include <cstdint>

#include <cstdint>

using ZigZag32 = int32_t;
using ZigZag64 = int64_t;

//
//
// namespace Zigzag32 {
//     constexpr uint32_t encode(const int32_t n) noexcept {
//         return (static_cast<uint32_t>(n) << 1) ^ static_cast<uint32_t>(n >> 31);
//     }
//
//     constexpr int32_t decode(const uint32_t n) noexcept {
//         return static_cast<int32_t>((n >> 1) ^ (~(n & 1) + 1));
//     }
//
//     inline void encodeInto(const int32_t v, uint32_t& out) noexcept { out = encode(v); }
//     inline void decodeInto(const uint32_t v, int32_t& out) noexcept { out = decode(v); }
// }
//
//
// namespace Zigzag64 {
//     constexpr uint64_t encode(int64_t n) noexcept {
//         return (static_cast<uint64_t>(n) << 1) ^ static_cast<uint64_t>(n >> 63);
//     }
//
//     constexpr int64_t decode(uint64_t n) noexcept {
//         return static_cast<int64_t>((n >> 1) ^ (~(n & 1) + 1));
//     }
//
//     inline void encodeInto(int64_t v, uint64_t& out) noexcept { out = encode(v); }
//     inline void decodeInto(uint64_t v, int64_t& out) noexcept { out = decode(v); }
// }