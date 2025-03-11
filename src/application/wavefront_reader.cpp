#include "application/wavefront_reader.h"

#include <algorithm>
#include <iterator>
#include <sstream>

#include "acceleration/bvh.h"
#include "application/error.h"
#include "composite/world.h"
#include "geometry/primitive.h"

using std::isspace;
using std::istringstream;
using std::string;
using std::string_view;
using std::vector;

bool isVertexEntry(string_view line) {
  return !line.empty() && line[0] == 'v' && isspace(line[1]);
}

bool isVertexNormalEntry(string_view line) {
  return !line.empty() && line[0] == 'v' && line[1] == 'n' && isspace(line[2]);
}

bool isTriangleEntry(string_view line) {
  std::istringstream iss{line.data()};
  auto n = std::distance(std::istream_iterator<std::string>{iss},
                         std::istream_iterator<std::string>{});
  return !line.empty() && line[0] == 'f' && n == 4;
}

bool isPolygonEntry(string_view line) {
  std::istringstream iss{line.data()};
  auto n = std::distance(std::istream_iterator<std::string>{iss},
                         std::istream_iterator<std::string>{});
  return !line.empty() && line[0] == 'f' && n == 6;
}

bool isGroupEntry(string_view line) {
  return !line.empty() && line[0] == 'g' && isspace(line[1]);
}

void handleStringsWithSlash(string &str) {
  size_t first{};
  size_t last{};
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

WavefrontReader::WavefrontReader(string_view file) : m_file(file) {
  m_finalProduct = World::create();
}

void WavefrontReader::parseInput() {
  openFile();
  if (m_inputStream.is_open()) {
    string line;
    while (getline(m_inputStream, line)) {
      if (isVertexEntry(line) && m_verticesNormalized.empty()) {
        parseVertexEntry(line, m_vertices);
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

void WavefrontReader::setFileName(std::string_view file) { m_file = file; }

void WavefrontReader::normalizeVertices() {
  openFile();
  if (m_inputStream.is_open()) {
    string line;
    while (getline(m_inputStream, line)) {
      if (isVertexEntry(line)) {
        parseVertexEntry(line, m_verticesNormalized);
      }
    }
  }
  m_inputStream.close();

  auto minX = limit::max();
  auto maxX = -limit::max();
  auto minY = limit::max();
  auto maxY = -limit::max();
  auto minZ = limit::max();
  auto maxZ = -limit::max();
  for (const Vec3D &elem : m_verticesNormalized) {
    minX = std::min(minX, elem.x());
    maxX = std::max(maxX, elem.x());
    minY = std::min(minY, elem.y());
    maxY = std::max(maxY, elem.y());
    minZ = std::min(minZ, elem.z());
    maxZ = std::max(maxZ, elem.z());
  }
  auto xrange = maxX - minX;
  auto yrange = maxY - minY;
  auto zrange = maxZ - minZ;
  for (Vec3D &elem : m_verticesNormalized) {
    elem.setX((elem.x() - minX) / xrange * 2.f - 1.f);
    elem.setY((elem.y() - minY) / yrange * 2.f - 1.f);
    elem.setZ((elem.z() - minZ) / zrange * 2.f - 1.f);
  }
}

void WavefrontReader::addLightForModel(const PointLight &light) {
  m_light = light;
  m_finalProduct->setLight(light);
}

void WavefrontReader::addMaterial(MaterialPtr mat) { m_material = mat; }

vector<Vec3D> WavefrontReader::vertexCollection() const { return m_vertices; }

std::vector<Normal3D> WavefrontReader::vertexNormalCollection() const {
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

void WavefrontReader::openFile() {
  if (!m_file.empty()) {
    m_inputStream.open(m_file.data());
  }
}

void WavefrontReader::parseVertexEntry(string_view line,
                                       std::vector<Vec3D> &vec) {
  istringstream ss(line.data());
  string v, x, y, z;
  if (ss >> v >> x >> y >> z) {
    try {
      vec.push_back(Vec3D(stof(x), stof(y), stof(z)));
    } catch (...) {
      APP_MSG(line.data());
      APP_ASSERT(false,
                 "Error while parsing the line above in wavefront file!");
    }
  }
}

void WavefrontReader::parseVertexNormalEntry(std::string_view line) {
  istringstream ss(line.data());
  string v, x, y, z;
  if (ss >> v >> x >> y >> z) {
    try {
      m_verticesNormals.push_back(Normal3D(stof(x), stof(y), stof(z)));
    } catch (...) {
      APP_MSG(line.data());
      APP_ASSERT(false,
                 "Error while parsing the line above in wavefront file!");
    }
  }
}

void WavefrontReader::parseTriangleEntry(string_view line) {
  if (m_verticesNormals.empty()) {
    if (std::count_if(line.begin(), line.end(),
                      [](char c) { return isspace(c); }) == 3) {
      istringstream ss(line.data());
      string v, i, j, k;
      if (ss >> v >> i >> j >> k) {
        try {
          Point3D p1, p2, p3;
          if (m_verticesNormalized.empty()) {
            p1 = Point3D(m_vertices[stoi(i) - 1]);
            p2 = Point3D(m_vertices[stoi(j) - 1]);
            p3 = Point3D(m_vertices[stoi(k) - 1]);
          } else {
            p1 = Point3D(m_verticesNormalized[stoi(i) - 1]);
            p2 = Point3D(m_verticesNormalized[stoi(j) - 1]);
            p3 = Point3D(m_verticesNormalized[stoi(k) - 1]);
          }
          auto primitive =
              Triangle::create(std::initializer_list<Point3D>{p1, p2, p3});
          auto scene_element = std::make_shared<SceneElement>();
          scene_element->setMaterial(m_material);
          scene_element->setPrimitive(primitive);
          if (m_currentGroup) {  // if we don't parse any g entry
            m_currentGroup->add(scene_element);
          } else {
            m_finalProduct->add(scene_element);
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
        size_t vertSize = m_vertices.size();
        Point3D p1, p2, p3;
        if (stoi(i) < 0)
          p1 = Point3D(m_vertices[vertSize + stoi(i) - 1]);
        else
          p1 = Point3D(m_vertices[stoi(i) - 1]);
        if (stoi(j) < 0)
          p2 = Point3D(m_vertices[vertSize + stoi(k) - 1]);
        else
          p2 = Point3D(m_vertices[stoi(k) - 1]);
        if (stoi(k) < 0)
          p3 = Point3D(m_vertices[vertSize + stoi(g) - 1]);
        else
          p3 = Point3D(m_vertices[stoi(g) - 1]);

        size_t vertNormSize = m_verticesNormals.size();
        Normal3D n1, n2, n3;
        if (stoi(j) < 0)
          n1 = Normal3D(m_verticesNormals[vertNormSize + stoi(j) - 1]);
        else
          n1 = Normal3D(m_verticesNormals[stoi(j) - 1]);
        if (stoi(h) < 0)
          n2 = Normal3D(m_verticesNormals[vertNormSize + stoi(h) - 1]);
        else
          n2 = Normal3D(m_verticesNormals[stoi(h) - 1]);
        if (stoi(o) < 0)
          n3 = Normal3D(m_verticesNormals[vertNormSize + stoi(o) - 1]);
        else
          n3 = Normal3D(m_verticesNormals[stoi(o) - 1]);

        auto scene_element = std::make_shared<SceneElement>();
        auto primitive = SmoothTriangle::create(p1, p2, p3, n1, n2, n3);
        scene_element->setMaterial(m_material);
        scene_element->setPrimitive(primitive);
        if (m_currentGroup) {  // if we don't parse any g entry
          m_currentGroup->add(scene_element);
        } else {
          m_finalProduct->add(scene_element);
        }
      }
    } catch (...) {
      APP_MSG(line.data());
      APP_ASSERT(false,
                 "Error while parsing the line above in wavefront file!");
    }
  }
}

void WavefrontReader::parsePolygonEntry(string_view line) {
  istringstream ss(line.data());
  string v, i, j, k, f, g;
  if (ss >> v >> i >> j >> k >> f >> g) {
    try {
      vector<Vec3D> polygonVertices;
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

void WavefrontReader::parseGroupEntry(std::string_view line) {
  m_currentGroup = World::create();
  m_currentGroup->setLight(m_light);
  m_finalProduct->add(m_currentGroup);
}

void WavefrontReader::triangulatePolygon(vector<Vec3D> vertices) {
  for (int i = 1; i < vertices.size() - 1; ++i) {
    Point3D p1 = Point3D(vertices[0]);
    Point3D p2 = Point3D(vertices[i]);
    Point3D p3 = Point3D(vertices[i + 1]);
    auto scene_element = std::shared_ptr<SceneElement>();
    auto primitive =
        Triangle::create(std::initializer_list<Point3D>{p1, p2, p3});
    scene_element->setMaterial(m_material);
    scene_element->setPrimitive(primitive);
    if (m_currentGroup) {
      m_currentGroup->add(scene_element);
    } else {
      m_finalProduct->add(scene_element);
    }
    m_triangles.push_back(Triangle({p1, p2, p3}));
  }
}
