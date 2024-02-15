#include "application/wavefront_reader.h"

#include <algorithm>
#include <iterator>
#include <sstream>

#include "acceleration/bvh.h"
#include "application/error.h"
#include "composite/world.h"

using std::isspace;
using std::istringstream;
using std::string;
using std::string_view;
using std::vector;

bool isVertexEntry(const string_view &line) {
  return !line.empty() && line[0] == 'v' && isspace(line[1]);
}

bool isVertexNormalEntry(const string_view &line) {
  return !line.empty() && line[0] == 'v' && line[1] == 'n' && isspace(line[2]);
}

bool isTriangleEntry(const string_view &line) {
  std::istringstream iss{line.data()};
  int n = std::distance(std::istream_iterator<std::string>{iss},
                        std::istream_iterator<std::string>{});
  return !line.empty() && line[0] == 'f' && n == 4;
}

bool isPolygonEntry(const string_view &line) {
  std::istringstream iss{line.data()};
  int n = std::distance(std::istream_iterator<std::string>{iss},
                        std::istream_iterator<std::string>{});
  return !line.empty() && line[0] == 'f' && n == 6;
}

bool isGroupEntry(const string_view &line) {
  return !line.empty() && line[0] == 'g' && isspace(line[1]);
}

void handleStringsWithSlash(string &str) {
  size_t first(0);
  size_t last(0);
  while (first != std::string::npos && last != std::string::npos) {
    first = str.find('/');
    if (first != std::string::npos) {
      last = str.find('/', first + 1);
      if (last != std::string::npos) {
        str.replace(first, last - first + 1, last - first, ' ');
      }
    }
  }
}

WavefrontReader::WavefrontReader(const string_view &file) {
  openFile(file);
  m_finalProduct = std::make_shared<World>();
}

void WavefrontReader::parseInput() {
  if (m_inputStream.is_open()) {
    string line;
    while (getline(m_inputStream, line)) {
      if (isVertexEntry(line)) {
        parseVertexEntry(line);
      } else if (isVertexNormalEntry(line)) {
        parseVertexNormalEntry(line);
      } else if (isTriangleEntry(line)) {
        parseTriangleEntry(line);
      } else if (isPolygonEntry(line)) {
        parsePolygonEntry(line);
      } else if (isGroupEntry(line)) {
        parseGroupEntry(line);
      }
    }
  }
  m_inputStream.close();
}

void WavefrontReader::openFile(const string_view &file) {
  if (!file.empty()) {
    m_inputStream.open(file.data());
  }
}

void WavefrontReader::addLightForModel(const PointLight &light) {
  m_light = light;
  m_finalProduct->setLight(light);
}

void WavefrontReader::addMaterial(TexturePtr tex,
                                  const MaterialProperties &prop) {
  m_material = std::make_shared<Material>(std::move(tex), prop);
}

vector<Vec3f> WavefrontReader::vertexCollection() const { return m_vertices; }

std::vector<Vec3f> WavefrontReader::vertexNormalCollection() const {
  return m_verticesNormals;
}

vector<Triangle> WavefrontReader::triangleCollection() const {
  return m_triangles;
}

SceneElementPtr WavefrontReader::getStructure() const { return m_finalProduct; }

SceneElementPtr WavefrontReader::getStructureBVHierarchy() const {
  BVHierarchy bvh;
  bvh.divideWorld(m_finalProduct);
  return m_finalProduct;
}

void WavefrontReader::parseVertexEntry(const string_view &line) {
  istringstream ss(line.data());
  string v, x, y, z;
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

void WavefrontReader::parseVertexNormalEntry(const std::string_view &line) {
  istringstream ss(line.data());
  string v, x, y, z;
  if (ss >> v >> x >> y >> z) {
    try {
      m_verticesNormals.push_back(Vec3f(stof(x), stof(y), stof(z)));
    } catch (...) {
      APP_MSG(line.data());
      APP_ASSERT(false,
                 "Error while parsing the line above in wavefront file!");
    }
  }
}

void WavefrontReader::parseTriangleEntry(const string_view &line) {
  if (m_verticesNormals.empty()) {
    if (std::count_if(line.begin(), line.end(),
                      [](char c) { return isspace(c); }) == 3) {
      istringstream ss(line.data());
      string v, i, j, k;
      if (ss >> v >> i >> j >> k) {
        try {
          auto p1 = Point3f(m_vertices[stol(i) - 1]);
          auto p2 = Point3f(m_vertices[stol(j) - 1]);
          auto p3 = Point3f(m_vertices[stol(k) - 1]);
          SceneElementPtr tr = std::make_shared<Triangle>(
              std::initializer_list<Point3f>{p1, p2, p3});
          tr->setMaterial(m_material);
          if (m_currentGroup) {  // if we don't parse any g entry
            m_currentGroup->add(tr);
          } else {
            m_finalProduct->add(tr);
          }
          m_triangles.push_back(Triangle({p1, p2, p3}));
        } catch (...) {
          APP_MSG(line.data());
          APP_ASSERT(false,
                     "Error while parsing the line above in wavefront file!");
        }
      }
    }
  } else {
    try {
      string str(line);
      handleStringsWithSlash(str);
      istringstream ss(str);
      string v, i, j, k, h, g, o;
      if (ss >> v >> i >> j >> k >> h >> g >> o) {
        auto p1 = Point3f(m_vertices[stol(i) - 1]);
        auto p2 = Point3f(m_vertices[stol(k) - 1]);
        auto p3 = Point3f(m_vertices[stol(g) - 1]);

        auto n1 = Vec3f(m_verticesNormals[stol(j) - 1]);
        auto n2 = Vec3f(m_verticesNormals[stol(h) - 1]);
        auto n3 = Vec3f(m_verticesNormals[stol(o) - 1]);

        SceneElementPtr tr =
            std::make_shared<SmoothTriangle>(p1, p2, p3, n1, n2, n3);
        tr->setMaterial(m_material);
        if (m_currentGroup) {  // if we don't parse any g entry
          m_currentGroup->add(tr);
        } else {
          m_finalProduct->add(tr);
        }
      }
    } catch (...) {
      APP_MSG(line.data());
      APP_ASSERT(false,
                 "Error while parsing the line above in wavefront file!");
    }
  }
}

void WavefrontReader::parsePolygonEntry(const string_view &line) {
  istringstream ss(line.data());
  string v, i, j, k, f, g;
  if (ss >> v >> i >> j >> k >> f >> g) {
    try {
      vector<Vec3f> polygonVertices;
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

void WavefrontReader::parseGroupEntry(const std::string_view &line) {
  m_currentGroup = std::make_shared<World>();
  m_currentGroup->setLight(m_light);
  m_finalProduct->add(m_currentGroup);
}

void WavefrontReader::triangulatePolygon(vector<Vec3f> vertices) {
  for (int i = 1; i < vertices.size() - 1; ++i) {
    Point3f p1 = Point3f(vertices[0]);
    Point3f p2 = Point3f(vertices[i]);
    Point3f p3 = Point3f(vertices[i + 1]);
    SceneElementPtr tr =
        std::make_shared<Triangle>(std::initializer_list<Point3f>{p1, p2, p3});
    tr->setMaterial(m_material);
    if (m_currentGroup) {
      m_currentGroup->add(tr);
    } else {
      m_finalProduct->add(tr);
    }
    m_triangles.push_back(Triangle({p1, p2, p3}));
  }
}
