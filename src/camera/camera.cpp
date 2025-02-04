#include "camera/camera.h"

void BaseCamera::computePixelSize() {
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

void BaseCamera::setParamVectors(const Point3f &from, const Point3f &to,
                                 const Vec3f &up) {
  m_from = from;
  m_to = to;
  m_up = up;
}
void BaseCamera::setHSize(int hs) { m_hsize = hs; }

void BaseCamera::setVSize(int vs) { m_vsize = vs; }

void BaseCamera::setFieldOfView(float fv) { m_field_of_view = fv; }

float BaseCamera::fieldOfView() const { return m_field_of_view; }

int BaseCamera::hSize() const { return m_hsize; }

int BaseCamera::vSize() const { return m_vsize; }

Point3f BaseCamera::getFromPoint() const { return m_from; }

Point3f BaseCamera::getToPoint() const { return m_to; }

Vec3f BaseCamera::getUpVector() const { return m_up; }

void BaseCamera::setTransform(const Mat4f &m) {
  m_transform = m;
  m_transformInv = m_transform.inverse();
}

Mat4f BaseCamera::transform() const { return m_transform; }

float BaseCamera::pixelSize() const { return m_pixel_size; }

BaseCamera::BaseCamera() : m_hsize(0), m_vsize(0), m_field_of_view(0.f) {}

BaseCamera::BaseCamera(int hs, int vs, float fv)
    : m_hsize(hs), m_vsize(vs), m_field_of_view(fv) {
  computePixelSize();
}

/*==================================================================
 *		Simple Camera
 *=================================================================*/

Camera::Camera(int hs, int vs, float fv) : BaseCamera(hs, vs, fv) {}

Ray Camera::getRay(int pixel_x, int pixel_y) const {
  auto fpixel_x = static_cast<float>(pixel_x);
  auto fpixel_y = static_cast<float>(pixel_y);
  Ray ray;
  auto xoffset = (fpixel_x + 0.5f) * m_pixel_size;
  auto yoffset = (fpixel_y + 0.5f) * m_pixel_size;
  auto world_x = m_half_width - xoffset;
  auto world_y = m_half_height - yoffset;
  auto pixel = m_transformInv * Vec4f(world_x, world_y, -1.0f, 1.0f);
  ray.setOrigin(m_transformInv * Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
  auto dir =
      getUnitVectorOf(Point3f(pixel.x(), pixel.y(), pixel.z()) - ray.origin());
  ray.setDirection(dir);
  return ray;
}
