#include "renderers/path_tracer.h"

#include "composite/world.h"

BruteForceMC::BruteForceMC(const BaseCameraPtr &cam, int samples)
    : BaseRenderer(cam), m_samples(samples) {}

void BruteForceMC::visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                                         const Ray &ray) {}

void BruteForceMC::visitSceneElementComposite(
    const SceneElementRawPtr elementComp, const Ray &ray) {
  if (elementComp->isWorld()) {
    auto col = Vec3f();
    for (int s = 0; s < m_samples; ++s) {
      auto u = static_cast<float>(m_x + drand48()) /
               static_cast<float>(m_camera->hSize());
      auto v = static_cast<float>(m_y + drand48()) /
               static_cast<float>(m_camera->vSize());

      Ray newRay = m_camera->getRay(u, v);
      col = col + computeColor(elementComp, newRay, m_samples);
    }
    m_out_color = col / static_cast<float>(m_samples);
  }
}

Vec3f BruteForceMC::computeColor(const SceneElementRawPtr world, const Ray &ray,
                                 int rec) {
  IntersectionRecord record;
  if (world->intersect(ray, record)) {
    Ray scattered;
    Vec3f attenuation;
    if (rec > 0 && record.object->getMaterial()->scatter(
                       ray, record, attenuation, scattered)) {
      return attenuation * computeColor(world, scattered, rec - 1);
    } else {
      return Vec3f(0.f, 0.f, 0.f);
    }
  }
  return m_background_color;
}
