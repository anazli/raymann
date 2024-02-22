#pragma once

#include <memory>

#include "camera/camera.h"
#include "composite/scene_element.h"

class StochasticMethod;

class BaseRenderer {
 public:
  BaseRenderer() = default;
  virtual ~BaseRenderer() = default;
  virtual void visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                                     const Ray &ray) = 0;
  virtual void visitSceneElementComposite(const SceneElementRawPtr elementComp,
                                          const Ray &ray) = 0;
  virtual Vec3f computeColor(const SceneElementRawPtr world, const Ray &ray,
                             int rec = 5) = 0;
  virtual void attachStochasticMethod(
      std::unique_ptr<StochasticMethod> stMethod);

  Vec3f getColor() const;
  void setPixelInfo(const int &x, const int y);
  float getPixelInfoX() const;
  float getPixelInfoY() const;
  void setBackgroundColor(const Vec3f &color);
  Vec3f backgroundColor() const;

 protected:
  IntersectionRecord m_closestHit;
  Vec3f m_out_color;
  Vec3f m_background_color;
  float m_tmin = MAXFLOAT;
  float m_x;
  float m_y;
};

using BaseRendererPtr = std::unique_ptr<BaseRenderer>;
using BaseRendererRawPtr = BaseRenderer *;
