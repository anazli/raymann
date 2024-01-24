#include "renderers/path_tracer.h"

#include "composite/world.h"

/*BasicPathTracer::BasicPathTracer(const BaseCameraPtr& cam) : m_cam(cam) {}

void BasicPathTracer::visitSceneElementLeaf(SceneElement& elementLeaf,
                                        const Ray& ray) {}

void BasicPathTracer::visitSceneElementComposite(
    const SceneElementPtr& elementComp, const Ray& ray) {
  for (int i = 0; i < 1; ++i) {
    float u = float(m_x + drand48()) / float(m_cam->hSize());
    float v = float(m_y + drand48()) / float(m_cam->vSize());
    m_out_color = computeColor(elementComp, m_cam->getRay(u, v), 0);
  }
  // m_out_color = m_out_color / 5.f;
  //  m_out_color = Vec3f(sqrt(m_out_color.x()), sqrt(m_out_color.y()),
  //                      sqrt(m_out_color.z()));
}

Vec3f BasicPathTracer::computeColor(const SceneElementPtr& world,
                                    const Ray& ray, int rec) {
  findClosestHit(world, ray);
  if (m_closestHit.object) {
    Ray scattered;
    Vec3f attenuation;
    if (rec < 30 && m_closestHit.object->getMaterial()->scatter(
                        ray, m_closestHit, attenuation, scattered)) {
      // std::cout << "trexw" << std::endl;
      return attenuation;  // * computeColor(world, scattered, rec + 1);
    } else {
      return Vec3f(0.f, 0.f, 0.f);
    }
  }
  Vec3 unit_direction = getUnitVectorOf(ray.direction());
  float t = 0.5f * (unit_direction.y() + 1.f);
  return (1.f - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.2f, 0.7f, 1.0f);
}

IntersectionRecord BasicPathTracer::findClosestHit(const SceneElementPtr& world,
                                                   const Ray& r) {
  WorldIterator it(world->getChildren());
  m_tmin = MAXFLOAT;
  if (it.first()) {
    while (it.notDone()) {
      IntersectionRecord record;
      if (it.currentElement()->intersect(r, record)) {
        if (record.t_min() > 0.f && record.t_min() < m_tmin) {
          m_closestHit = record;
          m_closestHit.object = it.currentElement();
          m_tmin = m_closestHit.t_min();
        }
      }
      it.advance();
    }
  }
  return m_closestHit;
}*/
