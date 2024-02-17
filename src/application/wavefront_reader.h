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
  WavefrontReader(const std::string_view& file = "");

  void parseInput();
  void setFileName(const std::string_view& file);
  void normalizeVertices();
  void addLightForModel(const PointLight& light);
  void addMaterial(TexturePtr tex, const MaterialProperties& prop);

  std::vector<Vec3f> vertexCollection() const;
  std::vector<Vec3f> vertexNormalCollection() const;
  std::vector<Triangle> triangleCollection() const;
  SceneElementPtr getStructure() const;
  SceneElementPtr getStructureBVHierarchy() const;

 private:
  void openFile();
  void parseVertexEntry(const std::string_view& line, std::vector<Vec3f>& vec);
  void parseVertexNormalEntry(const std::string_view& line);
  void parseTriangleEntry(const std::string_view& line);
  void parsePolygonEntry(const std::string_view& line);
  void parseGroupEntry(const std::string_view& line);
  void triangulatePolygon(std::vector<Vec3f> vertices);

  std::ifstream m_inputStream;
  std::vector<Vec3f> m_vertices;
  std::vector<Vec3f> m_verticesNormalized;
  std::vector<Vec3f> m_verticesNormals;
  std::vector<Triangle> m_triangles;
  SceneElementPtr m_finalProduct;
  SceneElementPtr m_currentGroup;
  PointLight m_light;
  BaseMaterialPtr m_material;
  std::string_view m_file;
};
