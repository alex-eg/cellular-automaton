#pragma once
#include <cstdint>
#include <limits>
#include <map>

namespace ca {
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;
using f80 = long double;

static_assert(std::numeric_limits<f32>::is_iec559,
              "IEEE 754 floating point support for float not found");
static_assert(std::numeric_limits<f64>::is_iec559,
              "IEEE 754 floating point support for double not found");
static_assert(std::numeric_limits<f80>::is_iec559,
              "IEEE 754 floating point support for long double not found");
} // namespace ca
