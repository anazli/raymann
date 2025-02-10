#pragma once

#include <memory>

#include "camera/camera.h"
#include "composite/scene_element.h"

class StochasticSampler;

class BaseRenderer {
 public:
  BaseRenderer() = default;
  virtual ~BaseRenderer() = default;
  virtual void visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                                     const Ray &ray) = 0;
  virtual void visitSceneElementComposite(const SceneElementRawPtr elementComp,
                                          const Ray &ray) = 0;
  virtual Vec3D computeColor(const SceneElementRawPtr world, const Ray &ray,
                             int rec = 5) = 0;
  virtual void attachStochasticMethod(
      std::unique_ptr<StochasticSampler> stMethod);

  Vec3D getColor() const;
  void setPixelInfo(const int &x, const int y);
  float getPixelInfoX() const;
  float getPixelInfoY() const;
  void setBackgroundColor(const Vec3D &color);
  Vec3D backgroundColor() const;
  void addDiffuseLight(SceneElementRawPtr light);
  SceneElementRawPtr getDiffuseLight() const;

 protected:
  Intersection m_closestHit;
  SceneElementRawPtr m_diffuseLight;
  Vec3D m_out_color;
  Vec3D m_background_color;
  float m_tmin = MAXFLOAT;
  float m_x;
  float m_y;
};

using BaseRendererPtr = std::unique_ptr<BaseRenderer>;
using BaseRendererRawPtr = BaseRenderer *;
