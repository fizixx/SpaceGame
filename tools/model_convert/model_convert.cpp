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

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <base/files/file_utils.h>

#include "models/model_data.h"
#include "parser.h"

template <typename T>
void writeToVector(T value, std::vector<char>* data) {
  union {
    T asType;
    char asChars[sizeof(T)];
  };

  asType = value;
  for (size_t i = 0; i < sizeof(asChars); ++i) {
    data->push_back(asChars[i]);
  }
}

int main(int argc, char* argv[]) {
  std::string inFile;
  std::string outFile;

  if (argc < 3)
    return 1;

  if (argc > 1)
    inFile = std::string{argv[1]};

  if (argc > 2)
    outFile = std::string{argv[2]};

  std::cout << "inFile: " << inFile << std::endl;
  std::cout << "outfile: " << outFile << std::endl;

  std::vector<char> fileData;
  base::readFileToVector(inFile, &fileData);

  ModelData modelData;
  Parser parser(fileData.data(), fileData.size());
  if (!parser.parse(&modelData)) {
    std::cerr << "Could not parse model file (" << inFile << ")" << std::endl;
    return 1;
  }


  // Write the model data to disk.
  std::vector<char> out;
  // Write the number of objects.
  writeToVector<int32_t>(modelData.objects.size(), &out);
  for (const auto& obj : modelData.objects) {
    writeToVector<int32_t>(obj.vertices.size(), &out);
    for (const auto& vertex : obj.vertices) {
      writeToVector<float>(vertex.x, &out);
      writeToVector<float>(vertex.y, &out);
    }
    writeToVector<int32_t>(obj.faces.size(), &out);
    for (const auto& face : obj.faces) {
      writeToVector<float>(face.a, &out);
      writeToVector<float>(face.b, &out);
      writeToVector<float>(face.c, &out);
      writeToVector<float>(face.matId, &out);
    }
  }

  base::writeVectorToFile(outFile, out);

  return 0;
}
