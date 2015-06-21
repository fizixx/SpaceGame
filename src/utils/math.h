// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef UTILS_MATH_H_
#define UTILS_MATH_H_

#include "canvas/math/vec2.h"

extern const float kPi;

// Convert radians to degrees.
inline float radToDeg(float rad) {
  return rad * 180.f / kPi;
}

// Convert degrees to radians.
inline float degToRad(float deg) {
  return deg * kPi / 180.f;
}

// Wrap the given value around the min and max.
template <typename T>
inline T wrap(T value, T min, T max) {
  if (value < min) {
    value += (max - min);
  } else if (value > max) {
    value -= (max - min);
  }
  return value;
}

// Calculate the distance between two points.
float distanceBetween(const ca::Vec2& p1, const ca::Vec2& p2);

// Calculate the direction between two points.
float directionBetween(const ca::Vec2& p1, const ca::Vec2& p2);

#endif  // UTILS_MATH_H_
