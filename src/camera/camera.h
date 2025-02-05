#pragma once

#include <memory>

#include "application/error.h"
#include "tools/tools.h"

class BaseCamera {
 public:
  virtual ~BaseCamera() = default;
  virtual Ray getRay(int pixel_x, int pixel_y) const = 0;
  void computePixelSize();
  void setParamVectors(const Point3D &from, const Point3D &to, const Vec3D &up);
  void setHSize(int hs);
  void setVSize(int vs);
  void setFieldOfView(float fv);
  float fieldOfView() const;
  int hSize() const;
  int vSize() const;
  Point3D getFromPoint() const;
  Point3D getToPoint() const;
  Vec3D getUpVector() const;
  void setTransform(const Mat4D &m);
  Mat4D transform() const;
  float pixelSize() const;

 protected:
  BaseCamera();
  BaseCamera(int hs, int vs, float fv);

  int m_hsize;
  int m_vsize;
  float m_field_of_view;
  Point3D m_from;
  Point3D m_to;
  Vec3D m_up;
  Mat4D m_transform;
  Mat4D m_transformInv;
  float m_pixel_size;
  float m_half_width;
  float m_half_height;
};

using BaseCameraPtr = std::shared_ptr<BaseCamera>;

class Camera : public BaseCamera {
 public:
  Camera() = default;
  Camera(int hs, int vs, float fv);
  ~Camera() override = default;

  Ray getRay(int pixel_x, int pixel_y) const override;
};
