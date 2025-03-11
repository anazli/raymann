#pragma once

#include <memory>

#include "composite/scene_element.h"
#include "geometry/primitive.h"
#include "textures/texture.h"

class EntityFactory {
 public:
  void setData(const DataContainer& data);
  PrimitivePtr createPrimitive();
  MaterialPtr createMaterial();

 private:
  TexturePtr createTexture();

  DataContainer m_input_data;
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

class SceneDirector {
 public:
  void createSceneElement(EntityFactory& factory, const DataContainer& data);
  void createWorld(WorldBuilder& builder, const PointLight& light);
  SceneElementPtr getCurrentElement() const;
  SceneElementPtr getSceneProduct() const;

 private:
  std::vector<SceneElementPtr> m_element_container;
  SceneElementPtr m_current_scene_element;
  SceneElementPtr m_scene_product;
};
