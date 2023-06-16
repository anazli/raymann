#include "renderers/path_tracer.h"

/*BasicPathTracer::BasicPathTracer(const BaseCamera& cam) : m_cam(cam) {}

void BasicPathTracer::visitSceneElement(SceneElement& elementLeaf,
                                        const Ray& ray) {}

void BasicPathTracer::visitSceneElementComposite(
    const SceneElementPtr& elementComp, const Ray& ray) {
  for (int i = 0; i < 1; ++i) {
    float u = float(m_x + drand48()) / float(m_cam.hSize());
    float v = float(m_y + drand48()) / float(m_cam.vSize());
    m_out_color = computeColor(elementComp, ray, 5);
  }
  // m_out_color = m_out_color / 2.f;
  // m_out_color = Vec3f(sqrt(m_out_color.x()), sqrt(m_out_color.y()),
  //                    sqrt(m_out_color.z()));
}

Vec3f BasicPathTracer::computeColor(const SceneElementPtr& world,
                                    const Ray& ray, int rec) {
  findClosestHit(world, ray);
  if (m_closestHit) {
    m_closestHit->getRecord().normal =
        m_closestHit->normal(m_closestHit->getRecord().point(ray));
    Ray scattered;
    Vec3f attenuation;
    if (rec < 10 &&
        m_closestHit->getMaterial()->scatter(ray, m_closestHit->getRecord(),
                                             attenuation, scattered)) {
      return attenuation * computeColor(world, scattered, rec + 1);
    } else {
      return Vec3f(0.f, 0.f, 0.f);
    }
  }
  Vec3 unit_direction = getUnitVectorOf(ray.direction());
  float t = 0.5f * (unit_direction.y() + 1.f);
  return (1.f - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.2f, 0.7f, 1.0f);
}

SceneElementPtr BasicPathTracer::findClosestHit(const SceneElementPtr& world,
                                                const Ray& r) {
  WorldIterator it(world->getWorldList());
  m_tmin = MAXFLOAT;
  if (it.first()) {
    while (it.notDone()) {
      if (it.currentElement()->intersect(r) &&
          it.currentElement()->getRecord().t_min() > 0.001f &&
          it.currentElement()->getRecord().t_min() < m_tmin) {
        m_closestHit = it.currentElement();
        m_tmin = it.currentElement()->getRecord().t_min();
      }
      it.advance();
    }
  }
  return m_closestHit;
}
*/
