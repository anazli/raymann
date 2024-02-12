#pragma once

#include <memory>

#include "composite/scene_element.h"
#include "textures/texture.h"

class Builder {
 public:
  virtual ~Builder() = default;
  virtual void createWorld() = 0;
  virtual void addWorld() = 0;
  virtual void addElement() = 0;
  virtual void addLight(const PointLight &light) = 0;
  virtual void processSceneElement(SceneElementRawPtr element) = 0;
  virtual void applyTransformation(const Mat4f &trans) = 0;
  virtual void applyWorldTransformation(const Mat4f &trans) = 0;
  virtual void applyMaterial(TexturePtr tex,
                             const MaterialProperties &prop) = 0;
  virtual void applyLambertianMaterial(TexturePtr tex,
                                       const MaterialProperties &prop) = 0;
  virtual void applyMetalMaterial(const float &f, TexturePtr tex,
                                  const MaterialProperties &prop) = 0;
  virtual void applyDielectricMaterial(const float &ri, TexturePtr tex,
                                       const MaterialProperties &prop) = 0;
  virtual void createBBoxForElement(const BoundingBox &box) = 0;
  virtual SceneElementPtr getProduct() = 0;
  virtual SceneElementPtr getProductBVHierarchy() = 0;
  virtual SceneElementRawPtr getCurrentElement() const = 0;
};

using BuilderPtr = std::unique_ptr<Builder>;

class WorldBuilder : public Builder {
 public:
  ~WorldBuilder() override = default;
  void createWorld() override;
  void addWorld() override;
  void addElement() override;
  void addLight(const PointLight &light) override;
  void processSceneElement(SceneElementRawPtr element) override;
  void applyTransformation(const Mat4f &trans) override;
  void applyWorldTransformation(const Mat4f &trans) override;
  void applyMaterial(TexturePtr tex, const MaterialProperties &prop) override;
  void applyLambertianMaterial(TexturePtr tex,
                               const MaterialProperties &prop) override;
  void applyMetalMaterial(const float &f, TexturePtr tex,
                          const MaterialProperties &prop) override;
  void applyDielectricMaterial(const float &ri, TexturePtr tex,
                               const MaterialProperties &prop) override;
  virtual void createBBoxForElement(const BoundingBox &box) override;
  SceneElementPtr getProduct() override;
  SceneElementPtr getProductBVHierarchy() override;
  SceneElementRawPtr getCurrentElement() const override;

 private:
  SceneElementRawPtr m_currentElement;
  SceneElementRawPtr m_product;
  PointLight m_light;
};
