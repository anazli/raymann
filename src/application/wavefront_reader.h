#pragma once

#include <fstream>
#include <string_view>

#include "composite/builder.h"
#include "composite/scene_element.h"
#include "geometry/triangle.h"
#include "materials/material.h"
#include "tools/vec3.h"

class WavefrontReader {
 public:
  WavefrontReader(std::string_view file = "");

  void parseInput();
  void setFileName(std::string_view file);
  void normalizeVertices();
  void addLightForModel(const PointLight& light);
  void addMaterial(BaseMaterialPtr mat);

  std::vector<Vec3D> vertexCollection() const;
  std::vector<Vec3D> vertexNormalCollection() const;
  std::vector<Triangle> triangleCollection() const;
  SceneElementPtr getStructure() const;
  SceneElementPtr getStructureBVHierarchy() const;

 private:
  void openFile();
  void parseVertexEntry(std::string_view line, std::vector<Vec3D>& vec);
  void parseVertexNormalEntry(std::string_view line);
  void parseTriangleEntry(std::string_view line);
  void parsePolygonEntry(std::string_view line);
  void parseGroupEntry(std::string_view line);
  void triangulatePolygon(std::vector<Vec3D> vertices);

  std::ifstream m_inputStream;
  std::vector<Vec3D> m_vertices;
  std::vector<Vec3D> m_verticesNormalized;
  std::vector<Vec3D> m_verticesNormals;
  std::vector<Triangle> m_triangles;
  SceneElementPtr m_finalProduct;
  SceneElementPtr m_currentGroup;
  PointLight m_light;
  BaseMaterialPtr m_material;
  std::string m_file;
};
