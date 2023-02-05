#pragma once

#include <memory>

#include "composite/properties.h"
#include "composite/traceable.h"
#include "textures/texture.h"

class Builder {
 public:
  virtual void createWorld(const PointLight &light) = 0;
  virtual void addWorld(const PointLight &light) = 0;
  virtual void addElement() = 0;
  virtual void createSphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                            const float &r = 1.0f) = 0;
  virtual void createPlane() = 0;
  virtual void applyTransformation(const Mat4f &trans) = 0;
  virtual void applyMaterial(TexturePtr tex, const Properties &prop) = 0;
  virtual TraceablePtr getProduct() = 0;
  virtual TraceablePtr getCurrentElement() const = 0;
};

typedef std::shared_ptr<Builder> BuilderPtr;

class WorldBuilder : public Builder {
 public:
  WorldBuilder();
  void createWorld(const PointLight &light) override;
  void addWorld(const PointLight &light) override;
  void addElement() override;
  void createSphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                    const float &r = 1.0f) override;
  void createPlane() override;
  void applyTransformation(const Mat4f &trans) override;
  void applyMaterial(TexturePtr tex, const Properties &prop) override;
  TraceablePtr getProduct() override;
  TraceablePtr getCurrentElement() const override;

 private:
  Traceable *m_currentTraceable;
  TraceablePtr m_product;
};
