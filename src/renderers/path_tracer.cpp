#include "renderers/path_tracer.h"

#include <random>

#include "composite/world.h"

using std::uniform_real_distribution;
std::mt19937 gen1(19843834823u);

BruteForceMC::BruteForceMC(const BaseCameraPtr &cam, int samples)
    : BaseRenderer(cam), m_samples(samples) {}

void BruteForceMC::visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                                         const Ray &ray) {}

void BruteForceMC::visitSceneElementComposite(
    const SceneElementRawPtr elementComp, const Ray &ray) {
  if (elementComp->isWorld()) {
    uniform_real_distribution<float> rand(0.f, 1.f);
    auto col = Vec3f();
    for (int s = 0; s < m_samples; ++s) {
      auto ax = m_x - m_camera->pixelSize() / 2.f;
      auto bx = m_x + m_camera->pixelSize() / 2.f;
      auto u = ax + rand(gen1) * (bx - ax);

      auto ay = m_y - m_camera->pixelSize() / 2.f;
      auto by = m_y + m_camera->pixelSize() / 2.f;
      auto v = ay + rand(gen1) * (by - ay);

      auto newRay = m_camera->getRay(u, v);
      col = col + computeColor(elementComp, newRay, 100);
    }
    col = col / static_cast<float>(m_samples);
    m_out_color = Vec3f(static_cast<float>(sqrt(col.x())),
                        static_cast<float>(sqrt(col.y())),
                        static_cast<float>(sqrt(col.z())));
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
      return attenuation * world->getLight().intensity() *
             computeColor(world, scattered, rec - 1);
    } else {
      return Vec3f();
    }
  }
  return m_background_color;
}
