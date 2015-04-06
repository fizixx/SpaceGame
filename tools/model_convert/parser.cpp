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

#include "parser.h"

#include <cctype>
#include <iostream>

#include <SFML/System/Vector2.hpp>

Parser::Parser(const char* buffer, size_t maxLength)
  : m_buffer(buffer), m_end(m_buffer + maxLength), m_ptr(buffer) {
}

Parser::~Parser() {
}

bool Parser::parse(ModelData* modelDataOut) {
#if 0
  std::string label;

  while (m_ptr < m_end) {
    if (readLabel(&label)) {
      std::cout << "label: " << label << std::endl;
      if (label == "GEOMOBJECT") {
        readObject(modelDataOut);
      } else {
        skipBlock();
      }
    } else {
      ++m_ptr;
    }
  }
#endif  // 0

  processTree(Root, modelDataOut, 0);

  return true;
}

bool Parser::readLabel(std::string* labelOut) {
  // If the first character is not a *, then it's not a label.
  if (*m_ptr != '*')
    return false;

  // Advance past the inital *.
  ++m_ptr;

  std::string result;
  while (m_ptr < m_end) {
    if (std::isspace(*m_ptr))
      break;
    result.push_back(*m_ptr++);
  }

  labelOut->swap(result);

  skipWhitespace();

  return true;
}

void Parser::skipWhitespace() {
  while (m_ptr < m_end) {
    if (!std::isspace(*m_ptr))
      return;
    ++m_ptr;
  }
}

bool Parser::skipOpenBrace() {
  if (*m_ptr != '{')
    return false;
  ++m_ptr;
  skipWhitespace();
  return true;
}

bool Parser::skipCloseBrace() {
  if (*m_ptr != '}')
    return false;
  ++m_ptr;
  skipWhitespace();
  return true;
}

bool Parser::skipBlock() {
  if (skipOpenBrace()) {
    while (m_ptr < m_end) {
      if (skipCloseBrace())
        return true;
      ++m_ptr;
    }
  }

  skipWhitespace();

  return false;
}

void Parser::readObject(ModelData* modelDataOut) {
  if (!skipOpenBrace())
    return;

  // Add a new object to read data into.
  modelDataOut->objects.push_back(ModelObject());
  ModelObject* obj = &modelDataOut->objects.back();

  std::string label;
  while (m_ptr < m_end) {
    if (skipCloseBrace())
      break;

    if (readLabel(&label)) {
      if (label == "MESH") {
        readMesh(obj);
      } else {
        skipBlock();
      }
    } else {
      ++m_ptr;
    }
  }
}

void Parser::readMesh(ModelObject* objOut) {
  if (!skipOpenBrace())
    return;

  std::string label;
  while (m_ptr < m_end) {
    if (skipCloseBrace())
      break;

    if (readLabel(&label)) {
      if (label == "MESH_VERTEX_LIST") {
        readMeshVertexList(objOut);
      } else {
        skipBlock();
      }
    } else {
      ++m_ptr;
    }
  }

  skipCloseBrace();
}

void Parser::readMeshVertexList(ModelObject* objOut) {
  skipOpenBrace();

  while (m_ptr < m_end) {
    if (skipCloseBrace())
      return;

    std::string label;
    if (readLabel(&label)) {
      if (label == "MESH_VERTEX") {
        int index;
        if (!readInt(&index))
          return;

        sf::Vector2f pos;
        if (!readFloat(&pos.x))
          return;

        if (!readFloat(&pos.y))
          return;

        float dummy;
        if (!readFloat(&dummy))
          return;

        objOut->vertices.push_back(pos);
      } else {
        skipBlock();
      }
    } else {
      ++m_ptr;
    }
  }

  skipWhitespace();
}

bool Parser::readInt(int* valueOut) {
  std::string str;
  while (m_ptr < m_end) {
    if (!std::isdigit(*m_ptr) && *m_ptr != '-')
      break;
    str.push_back(*m_ptr++);
  }

  try {
    int result = std::stol(str);
    *valueOut = result;
  } catch (std::exception&) {
    return false;
  }

  skipWhitespace();

  return true;
}

bool Parser::readFloat(float* valueOut) {
  std::string str;
  while (m_ptr < m_end) {
    if (!std::isdigit(*m_ptr) && *m_ptr != '.' && *m_ptr != '-')
      break;
    str.push_back(*m_ptr++);
  }

  try {
    float result = std::stof(str);
    *valueOut = result;
  } catch (std::exception&) {
    return false;
  }

  skipWhitespace();

  return true;
}

void Parser::skipColon() {
  if (*m_ptr == ':') {
    ++m_ptr;
  }

  skipWhitespace();
}

void Parser::skipWord() {
  while (m_ptr != m_end) {
    if (!std::isalpha(*m_ptr))
      break;
    ++m_ptr;
  }
  
  skipWhitespace();
}

void Parser::processTree(BranchType branchType, ModelData* modelDataOut,
                         int level) {
  std::string label;
  while (m_ptr < m_end) {
    if (skipCloseBrace())
      return;

    if (readLabel(&label)) {
#if 0
      for (size_t i = 0; i < level; ++i)
        std::cout << "  ";
      std::cout << label << std::endl;
#endif  // 0

      if (branchType == Root && label == "GEOMOBJECT") {
        // Add a new object.
        modelDataOut->objects.push_back(ModelObject());
        processTree(GeometryObject, modelDataOut, level + 1);
      } else if (branchType == GeometryObject && label == "MESH") {
        processTree(Mesh, modelDataOut, level + 1);
      } else if (branchType == Mesh && label == "MESH_VERTEX_LIST") {
        processTree(MeshVertexList, modelDataOut, level + 1);
      } else if (branchType == Mesh && label == "MESH_FACE_LIST") {
        processTree(MeshFaceList, modelDataOut, level + 1);
      } else if (branchType == MeshVertexList && label == "MESH_VERTEX") {
        readMeshVertex(&modelDataOut->objects.back());
      } else if (branchType == MeshFaceList && label == "MESH_FACE") {
        readMeshFace(&modelDataOut->objects.back());
      } else if (branchType == MeshFaceList && label == "MESH_MTLID") {
        readMaterialId(&modelDataOut->objects.back());
      } else {
        skipBlock();
      }
    } else {
      ++m_ptr;
    }
  }
}

void Parser::readMeshVertex(ModelObject* objOut) {
  int index;
  sf::Vector2f pos;
  float dummy;

  if (!readInt(&index))
    return;

  if (!readFloat(&pos.x))
    return;

  if (!readFloat(&pos.y))
    return;

  if (!readFloat(&dummy))
    return;

  objOut->vertices.push_back(pos);
}

void Parser::readMeshFace(ModelObject* objOut) {
  int index;
  int A, B, C;
  int AB, BC, CA;

  if (!readInt(&index))
    return;
  skipColon();

  skipWord();
  skipColon();
  if (!readInt(&A))
    return;

  skipWord();
  skipColon();
  if (!readInt(&B))
    return;

  skipWord();
  skipColon();
  if (!readInt(&C))
    return;

  skipWord();
  skipColon();
  if (!readInt(&AB))
    return;

  skipWord();
  skipColon();
  if (!readInt(&BC))
    return;

  skipWord();
  skipColon();
  if (!readInt(&CA))
    return;

  // Add the face.
  objOut->faces.emplace_back(A, B, C, 0);
}

void Parser::readMaterialId(ModelObject* objOut) {
  int materialId;
  
  if (!readInt(&materialId))
    return;

  // Set the material id in the last face we added.
  objOut->faces.back().matId = materialId;
}
