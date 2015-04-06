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

#ifndef TOOLS_MODEL_CONVERT_PARSER_H_
#define TOOLS_MODEL_CONVERT_PARSER_H_

#include <string>

#include <base/macros.h>

#include "models/model_data.h"

class Parser {
public:
  Parser(const char* buffer, size_t maxLength);
  ~Parser();

  // Parse the given data into the ModelData.
  bool parse(ModelData* modelDataOut);

private:
  enum BranchType {
    Root,
    GeometryObject,
    Mesh,
    MeshVertexList,
    MeshFaceList,
  };

  bool readLabel(std::string* labelOut);
  bool readToLabel(const std::string& label);
  void skipWhitespace();
  bool skipOpenBrace();
  bool skipCloseBrace();
  bool skipBlock();
  void readObject(ModelData* modelDataOut);
  void readMesh(ModelObject* objOut);
  void readMeshVertexList(ModelObject* objOut);
  bool readInt(int* valueOut);
  bool readFloat(float* valueOut);
  void skipColon();
  void skipWord();

  void processTree(BranchType branchType, ModelData* modelDataOut, int level);
  void readMeshVertex(ModelObject* objOut);
  void readMeshFace(ModelObject* objOut);
  void readMaterialId(ModelObject* objOut);

  // The entire buffer we are reading through.
  const char* m_buffer;

  // The size of the buffer.
  const char* m_end;

  // The current location we are in the buffer.
  const char* m_ptr;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Parser);
};

#endif  // TOOLS_MODEL_CONVERT_PARSER_H_
