#pragma once

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string_view>

#include "application/error.h"
#include "geometry/triangle.h"
#include "tools/vec3.h"

bool isVertexEntry(const std::string_view& line) {
  return !line.empty() && line[0] == 'v' && std::isspace(line[1]);
}

bool isTriangleEntry(const std::string_view& line) {
  return !line.empty() && line[0] == 'f' && line.length() == 7;
}

bool isPolygonEntry(const std::string_view& line) {
  return !line.empty() && line[0] == 'f' && line.length() == 11;
}

class WavefrontReader {
 public:
  WavefrontReader(const std::string_view& file = "") { openFile(file); }

  void parseInput() {
    if (m_inputStream.is_open()) {
      std::string line;
      while (std::getline(m_inputStream, line)) {
        if (isVertexEntry(line)) {
          parseVertexEntry(line);
        } else if (isTriangleEntry(line)) {
          parseTriangleEntry(line);
        } else if (isPolygonEntry(line)) {
          parsePolygonEntry(line);
        }
      }
    }
    m_inputStream.close();
  }

  void openFile(const std::string_view& file) {
    if (!file.empty()) {
      m_inputStream.open(file.data());
    }
  }

  std::vector<Vec3f> vertexCollection() const { return m_vertices; }
  std::vector<Triangle> triangleCollection() const { return m_triangles; }

 private:
  void parseVertexEntry(const std::string_view& line) {
    std::istringstream ss(line.data());
    std::string v, x, y, z;
    if (ss >> v >> x >> y >> z) {
      try {
        m_vertices.push_back(Vec3f(stof(x), stof(y), stof(z)));
      } catch (...) {
        APP_MSG(line.data());
        APP_ASSERT(false,
                   "Error while parsing the line above in wavefront file!");
      }
    }
  }

  void parseTriangleEntry(const std::string_view& line) {
    std::istringstream ss(line.data());
    std::string v, i, j, k;
    if (ss >> v >> i >> j >> k) {
      try {
        Point3f p1 = Point3f(m_vertices[stol(i) - 1]);
        Point3f p2 = Point3f(m_vertices[stol(j) - 1]);
        Point3f p3 = Point3f(m_vertices[stol(k) - 1]);
        m_triangles.push_back(Triangle({p1, p2, p3}));
      } catch (...) {
        APP_MSG(line.data());
        APP_ASSERT(false,
                   "Error while parsing the line above in wavefront file!");
      }
    }
  }

  void parsePolygonEntry(const std::string_view& line) {
    std::istringstream ss(line.data());
    std::string v, i, j, k, f, g;
    if (ss >> v >> i >> j >> k >> f >> g) {
      try {
        std::vector<Vec3f> polygonVertices;
        polygonVertices.push_back(m_vertices[stol(i) - 1]);
        polygonVertices.push_back(m_vertices[stol(j) - 1]);
        polygonVertices.push_back(m_vertices[stol(k) - 1]);
        polygonVertices.push_back(m_vertices[stol(f) - 1]);
        polygonVertices.push_back(m_vertices[stol(g) - 1]);
        triangulatePolygon(polygonVertices);
      } catch (...) {
        APP_MSG(line.data());
        APP_ASSERT(false,
                   "Error while parsing the line above in wavefront file!");
      }
    }
  }

  void triangulatePolygon(std::vector<Vec3f> vertices) {
    for (int i = 1; i < vertices.size() - 1; ++i) {
      Point3f p1 = Point3f(vertices[0]);
      Point3f p2 = Point3f(vertices[i]);
      Point3f p3 = Point3f(vertices[i + 1]);
      m_triangles.push_back(Triangle({p1, p2, p3}));
    }
  }

  std::ifstream m_inputStream;
  std::vector<Vec3f> m_vertices;
  std::vector<Triangle> m_triangles;
};
