#pragma once

#include <vec3.h>

#include <filesystem>
#include <fstream>
#include <string_view>

#include "composite/builder.h"
#include "composite/scene_element.h"
#include "geometry/triangle.h"
#include "materials/material.h"

class WavefrontReader {
 public:
  WavefrontReader() = default;
  WavefrontReader(const std::filesystem::path& file);

  void parseInput();
  void setInputFile(const std::filesystem::path& file);
  void normalizeVertices();
  void addLightForModel(const PointLight& light);
  void addMaterial(MaterialPtr mat);

  std::vector<Vec3f> vertexCollection() const;
  std::vector<Normal3f> vertexNormalCollection() const;
  std::vector<Triangle> triangleCollection() const;
  SceneElementPtr getStructure() const;
  SceneElementPtr getStructureBVHierarchy() const;

 private:
  void openFile();
  void parseVertexEntry(std::string_view line, std::vector<Vec3f>& vec);
  void parseVertexNormalEntry(std::string_view line);
  void parseTriangleEntry(std::string_view line);
  void parsePolygonEntry(std::string_view line);
  void parseGroupEntry(std::string_view line);
  void triangulatePolygon(std::vector<Vec3f> vertices);

  std::ifstream m_inputStream;
  std::vector<Vec3f> m_vertices;
  std::vector<Vec3f> m_verticesNormalized;
  std::vector<Normal3f> m_verticesNormals;
  std::vector<Triangle> m_triangles;
  SceneElementPtr m_finalProduct;
  SceneElementPtr m_currentGroup;
  PointLight m_light;
  MaterialPtr m_material;
  std::filesystem::path m_file;
};
