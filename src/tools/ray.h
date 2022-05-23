#pragma once

#include "camera/camera.h"

class Ray {
 public:
  Ray() {}
  Ray(const Point3f &ori, const Vec3f &dir) : m_ori(ori), m_dir(dir) {}
  Ray(const Camera &c, int pixel_x, int pixel_y) {
    setForPixel(c, pixel_x, pixel_y);
  }

  void setOrigin(const Point3f &ori) { m_ori = ori; }
  void setDirection(const Vec3f &dir) { m_dir = dir; }
  Point3f origin() const { return m_ori; }
  Vec3f direction() const { return m_dir; }
  Point3f position(const float &t) const { return origin() + t * direction(); }
  Ray transform(const Mat4f &m) const {
    Ray ret;

    Vec4f v4(origin());
    v4 = m * v4;
    Point3f new_o = v4;
    ret.setOrigin(new_o);

    v4 = direction();
    v4 = m * v4;
    Vec4f new_d = v4;
    ret.setDirection(new_d);
    return ret;
  }

  void setForPixel(const Camera &c, int pixel_x, int pixel_y) {
    float xoffset = (static_cast<float>(pixel_x) + 0.5f) * c.pixelSize();
    float yoffset = (static_cast<float>(pixel_y) + 0.5f) * c.pixelSize();
    float world_x = c.halfWidth() - xoffset;
    float world_y = c.halfHeight() - yoffset;
    Mat4f inversed = c.transform().inverse();
    Point3f pixel = inversed * Vec4f(world_x, world_y, -1.0f, 1.0f);
    m_ori = inversed * Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
    m_dir = (Point3f(pixel.x(), pixel.y(), pixel.z()) - m_ori).normalize();
  }

 private:
  Point3f m_ori;
  Vec3f m_dir;
};
