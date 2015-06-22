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

#include "utils/math.h"

#include <cmath>

const float kPi = 3.1415f;

float distanceBetween(const ca::Vec2& p1, const ca::Vec2& p2) {
  float xd = p2.x - p1.x;
  float yd = p2.y - p1.y;
  return std::sqrtf(xd * xd + yd * yd);
}

float directionBetween(const ca::Vec2& p1, const ca::Vec2& p2) {
  float dx = p2.x - p1.x;
  float dy = p2.y - p1.y;
  float direction = radToDeg(std::atan2(dy, dx));
  return wrap(direction, 0.f, 360.f);
}
