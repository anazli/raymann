#include "application/wavefront_reader.h"

#include <algorithm>
#include <sstream>

#include "application/error.h"

using std::isspace;
using std::istringstream;
using std::string;
using std::string_view;
using std::vector;

bool isVertexEntry(const string_view &line) {
  return !line.empty() && line[0] == 'v' && isspace(line[1]);
}

bool isTriangleEntry(const string_view &line) {
  return !line.empty() && line[0] == 'f' && line.length() == 7;
}

bool isPolygonEntry(const string_view &line) {
  return !line.empty() && line[0] == 'f' && line.length() == 11;
}

bool isGroupEntry(const string_view &line) {
  return !line.empty() && line[0] == 'g' && isspace(line[1]);
}

WavefrontReader::WavefrontReader(const string_view &file) { openFile(file); }

void WavefrontReader::parseInput() {
  if (m_inputStream.is_open()) {
    string line;
    while (getline(m_inputStream, line)) {
      if (isVertexEntry(line)) {
        parseVertexEntry(line);
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

void WavefrontReader::addBuilder(BuilderPtr builder) {
  m_builder = std::move(builder);
  m_builder->createWorld();
  m_finalProduct = m_builder->getProduct();
}

vector<Vec3f> WavefrontReader::vertexCollection() const { return m_vertices; }

vector<Triangle> WavefrontReader::triangleCollection() const {
  return m_triangles;
}

SceneElementPtr WavefrontReader::getStructure() const { return m_finalProduct; }

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

void WavefrontReader::parseTriangleEntry(const string_view &line) {
  istringstream ss(line.data());
  string v, i, j, k;
  if (ss >> v >> i >> j >> k) {
    try {
      Point3f p1 = Point3f(m_vertices[stol(i) - 1]);
      Point3f p2 = Point3f(m_vertices[stol(j) - 1]);
      Point3f p3 = Point3f(m_vertices[stol(k) - 1]);
      if (m_builder) {
        m_builder->processSceneElement(new Triangle({p1, p2, p3}));
        m_builder->addElement();
      } else {
        m_triangles.push_back(Triangle({p1, p2, p3}));
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
  if (m_builder->getProduct() &&
      m_builder->getProduct().get() != m_finalProduct.get()) {
    m_finalProduct->add(m_builder->getProduct());
  }
  m_builder->createWorld();
}

void WavefrontReader::triangulatePolygon(vector<Vec3f> vertices) {
  for (int i = 1; i < vertices.size() - 1; ++i) {
    Point3f p1 = Point3f(vertices[0]);
    Point3f p2 = Point3f(vertices[i]);
    Point3f p3 = Point3f(vertices[i + 1]);
    if (m_builder) {
      m_builder->processSceneElement(new Triangle({p1, p2, p3}));
      m_builder->addElement();
    } else {
      m_triangles.push_back(Triangle({p1, p2, p3}));
    }
  }
}
