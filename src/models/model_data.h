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

#ifndef MODELS_MODEL_DATA_H_
#define MODELS_MODEL_DATA_H_

#include <cstdint>
#include <vector>

#include <SFML/System/Vector2.hpp>

struct ModelFace {
  uint32_t a{0};
  uint32_t b{0};
  uint32_t c{0};
  uint32_t matId{0};

  ModelFace(uint32_t a, uint32_t b, uint32_t c, uint32_t matId)
    : a(a), b(b), c(c), matId(matId) {}
};

struct ModelObject {
  std::vector<sf::Vector2f> vertices;
  std::vector<ModelFace> faces;
};

struct ModelData {
  std::vector<ModelObject> objects;
};

#endif  // MODELS_MODEL_DATA_H_
