#pragma once

#include <memory>

#include "application/error.h"
#include "tools/tools.h"

class BaseCamera {
 public:
  virtual ~BaseCamera() = default;
  virtual Ray getRay(int pixel_x, int pixel_y) const = 0;
  virtual void computePixelSize();
  void setParamVectors(const Point3f &from, const Point3f &to, const Vec3f &up);
  void setHSize(int hs);
  void setVSize(int vs);
  void setFieldOfView(float fv);
  float fieldOfView() const;
  int hSize() const;
  int vSize() const;
  void setTransform(const Mat4f &m);
  Mat4f transform() const;
  float pixelSize() const;

 protected:
  BaseCamera();
  BaseCamera(int hs, int vs, float fv);
  int m_hsize;
  int m_vsize;
  float m_field_of_view;
  Point3f m_from;
  Point3f m_to;
  Vec3f m_up;
  Mat4f m_transform;
  float m_pixel_size;
};

using BaseCameraPtr = std::unique_ptr<BaseCamera>;

class Camera : public BaseCamera {
 public:
  Camera() = default;
  Camera(int hs, int vs, float fv);
  ~Camera() override = default;

  Ray getRay(int pixel_x, int pixel_y) const override;
  void computePixelSize() override;

 private:
  float halfWidth() const;
  float halfHeight() const;
  Point3f getFromPoint() const;
  Point3f getToPoint() const;
  Vec3f getUpVector() const;

  float m_half_width;
  float m_half_height;
};
