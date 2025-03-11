#include "camera/camera.h"

Camera::Camera(int hs, int vs, float fv)
    : m_hsize(hs), m_vsize(vs), m_field_of_view(fv) {
  computePixelSize();
}

Ray Camera::getRay(int pixel_x, int pixel_y) const {
  auto fpixel_x = static_cast<float>(pixel_x);
  auto fpixel_y = static_cast<float>(pixel_y);
  Ray ray;
  auto xoffset = (fpixel_x + 0.5f) * m_pixel_size;
  auto yoffset = (fpixel_y + 0.5f) * m_pixel_size;
  auto world_x = m_half_width - xoffset;
  auto world_y = m_half_height - yoffset;
  auto pixel = m_transformInv * Vec4D(world_x, world_y, -1.0f, 1.0f);
  ray.setOrigin(Point3D(m_transformInv * Vec4D(0.0f, 0.0f, 0.0f, 1.0f)));
  auto dir =
      getUnitVectorOf(Point3D(pixel.x(), pixel.y(), pixel.z()) - ray.origin());
  ray.setDirection(dir);
  return ray;
}

int Camera::hSize() const { return m_hsize; }

int Camera::vSize() const { return m_vsize; }

void Camera::setTransform(const Mat4D &m) {
  m_transform = m;
  m_transformInv = m_transform.inverse();
}

float Camera::pixelSize() const { return m_pixel_size; }

void Camera::computePixelSize() {
  APP_ASSERT(m_hsize >= 0 && m_vsize >= 0,
             "Camera: negative horizontal or vertical size found!")
  auto half_view = tan(m_field_of_view / 2.0f);
  auto aspect = static_cast<float>(m_hsize) / m_vsize;
  if (aspect >= 1.0f) {
    m_half_width = half_view;
    m_half_height = half_view / aspect;
  } else {
    m_half_width = half_view * aspect;
    m_half_height = half_view;
  }
  m_pixel_size = (m_half_width * 2.0f) / m_hsize;
}
