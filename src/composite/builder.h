#pragma once

#include <memory>

#include "composite/scene_element.h"
#include "textures/texture.h"

/*struct BuilderData {
  PrimitiveType primitive_type;
  MaterialType material_type;
  DataContainer material_properties;
};

class PrimitiveBuilder {
 public:
  virtual void createPrimitive(const PrimitiveType &type) = 0;
  virtual void createPrimitive(SceneElementRawPtr primitive) = 0;
};*/

class Builder {
 public:
  virtual ~Builder() = default;
  virtual void createWorld() = 0;
  virtual void addWorldToProduct() = 0;
  virtual void addElementToProduct() = 0;
  virtual void addLight(const PointLight &light) = 0;
  virtual void createPrimitive(const AppParameters &type) = 0;
  virtual void createPrimitive(SceneElementRawPtr primitive) = 0;
  virtual void applyTransformation(const Mat4D &trans) = 0;
  virtual void applyWorldTransformation(const Mat4D &trans) = 0;
  virtual void applyMaterial(TexturePtr tex, const DataContainer &prop) = 0;
  virtual void applyLambertianMaterial(
      TexturePtr tex, const DataContainer &prop = DataContainer{}) = 0;
  virtual void applyEmissiveMaterial(
      TexturePtr tex, const DataContainer &prop = DataContainer{}) = 0;
  virtual void applyMetalMaterial(
      TexturePtr tex, const DataContainer &prop = DataContainer{}) = 0;
  virtual void applyDielectricMaterial(
      TexturePtr tex, const DataContainer &prop = DataContainer{}) = 0;
  virtual void createBBoxForElement(const BoundingBox &box) = 0;
  virtual SceneElementPtr getProduct() = 0;
  virtual SceneElementPtr getProductBVHierarchy() = 0;
  const virtual SceneElementPtr getCurrentElement() const = 0;
};

using BuilderPtr = std::unique_ptr<Builder>;

class WorldBuilder : public Builder {
 public:
  ~WorldBuilder() override = default;
  void createWorld() override;
  void addWorldToProduct() override;
  void addElementToProduct() override;
  void addLight(const PointLight &light) override;
  void createPrimitive(const AppParameters &type) override;
  void createPrimitive(SceneElementRawPtr primitive) override;
  void applyTransformation(const Mat4D &trans) override;
  void applyWorldTransformation(const Mat4D &trans) override;
  void applyMaterial(TexturePtr tex, const DataContainer &prop) override;
  void applyLambertianMaterial(
      TexturePtr tex, const DataContainer &prop = DataContainer{}) override;
  void applyEmissiveMaterial(
      TexturePtr tex, const DataContainer &prop = DataContainer{}) override;
  void applyMetalMaterial(TexturePtr tex,
                          const DataContainer &prop = DataContainer{}) override;
  void applyDielectricMaterial(
      TexturePtr tex, const DataContainer &prop = DataContainer{}) override;
  virtual void createBBoxForElement(const BoundingBox &box) override;
  SceneElementPtr getProduct() override;
  SceneElementPtr getProductBVHierarchy() override;
  const SceneElementPtr getCurrentElement() const override;

 private:
  SceneElementPtr m_currentElement;
  SceneElementPtr m_product;
  PointLight m_light;
};
