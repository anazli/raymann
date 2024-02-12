#pragma once

#include <fstream>
#include <string_view>

#include "geometry/triangle.h"
#include "tools/vec3.h"

class WavefrontReader {
 public:
  WavefrontReader(const std::string_view& file = "");

  void parseInput();

  void openFile(const std::string_view& file);

  std::vector<Vec3f> vertexCollection() const;
  std::vector<Triangle> triangleCollection() const;

 private:
  void parseVertexEntry(const std::string_view& line);
  void parseTriangleEntry(const std::string_view& line);
  void parsePolygonEntry(const std::string_view& line);
  void triangulatePolygon(std::vector<Vec3f> vertices);

  std::ifstream m_inputStream;
  std::vector<Vec3f> m_vertices;
  std::vector<Triangle> m_triangles;
};
