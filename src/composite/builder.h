#pragma once

#include <memory>

#include "composite/scene_element.h"
#include "materials/material.h"
#include "textures/texture.h"

class Builder {
 public:
  virtual void createWorld(const PointLight &light) = 0;
  virtual void addWorld() = 0;
  virtual void addElement() = 0;
  virtual void createSphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                            const float &r = 1.0f) = 0;
  virtual void createPlane() = 0;
  virtual void applyTransformation(const Mat4f &trans) = 0;
  virtual void applyMaterial(TexturePtr tex,
                             const MaterialProperties &prop) = 0;
  virtual void applyLambertianMaterial(TexturePtr tex,
                                       const MaterialProperties &prop) = 0;
  virtual void applyMetalMaterial(const float &f, TexturePtr tex,
                                  const MaterialProperties &prop) = 0;
  virtual void applyDielectricMaterial(const float &ri, TexturePtr tex,
                                       const MaterialProperties &prop) = 0;
  virtual SceneElementPtr getProduct() = 0;
  virtual SceneElementPtr getCurrentElement() const = 0;
};

typedef std::shared_ptr<Builder> BuilderPtr;

class WorldBuilder : public Builder {
 public:
  void createWorld(const PointLight &light) override;
  void addWorld() override;
  void addElement() override;
  void createSphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                    const float &r = 1.0f) override;
  void createPlane() override;
  void applyTransformation(const Mat4f &trans) override;
  void applyMaterial(TexturePtr tex, const MaterialProperties &prop) override;
  void applyLambertianMaterial(TexturePtr tex,
                               const MaterialProperties &prop) override;
  void applyMetalMaterial(const float &f, TexturePtr tex,
                          const MaterialProperties &prop) override;
  void applyDielectricMaterial(const float &ri, TexturePtr tex,
                               const MaterialProperties &prop) override;
  SceneElementPtr getProduct() override;
  SceneElementPtr getCurrentElement() const override;

 private:
  SceneElement *m_currentElement;
  SceneElementPtr m_product;
};
