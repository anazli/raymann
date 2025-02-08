#pragma once

#include <memory>

#include "composite/scene_element.h"
#include "textures/texture.h"

class PrimitiveBuilder {
 public:
  void reset();
  void setData(const DataContainer& data);
  void buildPrimitive();
  void buildTexture();
  void buildMaterial();
  void buildTransformation();
  SceneElementPtr getProduct() const;

 private:
  DataContainer m_input_data;
  SceneElementPtr m_current_element;
  TexturePtr m_current_texture;
  MaterialPtr m_current_material;
  Transformation m_transformation;
};

class WorldBuilder {
 public:
  void reset();
  void createWorld();
  void addLight(const PointLight& light);
  void addElement(SceneElementPtr element);
  void addWorld(SceneElementPtr world);
  SceneElementPtr getProduct() const;
  SceneElementPtr getProductWithBVHierarchy();

 private:
  SceneElementPtr m_world;
};

// TODO: Add world to another, at some point...
class SceneDirector {
 public:
  void createSceneElement(PrimitiveBuilder& builder, const DataContainer& data);
  void createWorld(WorldBuilder& builder, const PointLight& light);
  SceneElementPtr getCurrentElement() const;
  SceneElementPtr getSceneProduct() const;

 private:
  std::vector<SceneElementPtr> m_element_container;
  SceneElementPtr
      m_current_element;  // TODO: to be removed after refactoring (path tracer)
  SceneElementPtr m_scene_product;
};
