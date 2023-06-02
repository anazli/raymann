#include "renderers/renderer.h"

#include "composite/world.h"

PhongModel::PhongModel() {}

void PhongModel::visitSceneElement(SceneElement& elementLeaf, const Ray& ray) {
  /*if (elementLeaf.intersect(ray) && elementLeaf.getRecord().t_min() > 0. &&
      elementLeaf.getRecord().t_min() < m_tmin) {
    m_closestHit = std::make_shared<SceneElement>(elementLeaf);
    m_tmin = m_closestHit->getRecord().t_min();
  }*/
}

void PhongModel::visitSceneElementComposite(const SceneElementPtr& elementComp,
                                            const Ray& ray) {
  if (elementComp->isWorld()) {
    m_out_color = computeColor(elementComp, ray);
  }
}

Vec3f PhongModel::computeColor(const SceneElementPtr& world, const Ray& ray,
                               int rec) {
  findClosestHit(world, ray);
  if (m_closestHit && m_closestHit->intersect(ray)) {
    Vec3f surf_col = lighting(world, ray);
    Vec3f refl_col = reflectedColor(world, ray, rec);
    return surf_col + refl_col;
  }
  return Vec3f(0.f, 0.f, 0.f);
}

Vec3f PhongModel::lighting(const SceneElementPtr& world, const Ray& ray) {
  Point3f over_point =
      m_closestHit->getRecord().point(ray) +
      (m_closestHit->getRecord().inside
           ? m_closestHit->normal(m_closestHit->getRecord().point(ray))
           : m_closestHit->normal(m_closestHit->getRecord().point(ray))) *
          0.02f;
  Vec3f normal_vec = m_closestHit->getRecord().inside
                         ? -m_closestHit->normal(over_point)
                         : m_closestHit->normal(over_point);
  Vec3f lightv = (world->getLight().position() - over_point).normalize();

  Vec3f effective_color = m_closestHit->getMaterial()->getTexture()->value(
                              0, 0, Vec3f(over_point)) *
                          world->getLight().intensity();

  Vec3f ret_ambient =
      effective_color *
      m_closestHit->getMaterial()->getProperties().getPropertyAsFloat(
          Props::AMBIENT);
  Vec3f ret_diffuse;
  Vec3f ret_specular;

  if (isShadowed(world, over_point)) return ret_ambient;

  float light_normal = dot(lightv, normal_vec);
  if (light_normal > 0.0f) {
    ret_diffuse =
        effective_color *
        m_closestHit->getMaterial()->getProperties().getPropertyAsFloat(
            Props::DIFFUSE) *
        light_normal;
    Vec3f reflectv = reflect(-lightv, normal_vec);
    float reflect_dot_eye = dot(reflectv, m_closestHit->getRecord().eye(ray));
    if (reflect_dot_eye > 0.0f) {
      float factor =
          pow(reflect_dot_eye,
              m_closestHit->getMaterial()->getProperties().getPropertyAsFloat(
                  Props::SHININESS));
      ret_specular =
          world->getLight().intensity() *
          m_closestHit->getMaterial()->getProperties().getPropertyAsFloat(
              Props::SPECULAR) *
          factor;
    }
  }

  return ret_ambient + ret_diffuse + ret_specular;
}

Vec3f PhongModel::reflectedColor(const SceneElementPtr& world, const Ray& r,
                                 int rec) {
  SceneElementPtr closest_refl = findClosestHit(world, r);
  if (closest_refl) {
    if (rec <= 0) {
      return Vec3f(0.f, 0.f, 0.f);
    }
    if (closest_refl->getMaterial()->getProperties().getPropertyAsFloat(
            Props::REFLECTION) <= 0.f) {
      return Vec3f(0.f, 0.f, 0.f);
    }
    Vec3f reflectv = reflect(
        r.direction(),
        (closest_refl->getRecord().inside
             ? closest_refl->normal(closest_refl->getRecord().point(r))
             : closest_refl->normal(closest_refl->getRecord().point(r))));
    Point3f over_point =
        closest_refl->getRecord().point(r) +
        (closest_refl->getRecord().inside
             ? closest_refl->normal(closest_refl->getRecord().point(r))
             : closest_refl->normal(closest_refl->getRecord().point(r))) *
            EPS1;
    closest_refl->getRecord().over_point_from_refl_surf = over_point;
    Ray reflect_ray(over_point, reflectv);

    Vec3f color = computeColor(world, reflect_ray, rec - 1);
    return color *
           closest_refl->getMaterial()->getProperties().getPropertyAsFloat(
               Props::REFLECTION);
  }
  return Vec3f(0.f, 0.f, 0.f);
}

void PhongModel::checkInside(const Ray& r) {}

bool PhongModel::isShadowed(const SceneElementPtr& world, const Point3f& p) {
  if (m_closestHit) {
    PointLight light = world->getLight().position();
    Vec3f v = p - light.position();
    float distance = v.length();
    Ray r(light.position(), v.normalize());
    SceneElementPtr closest_shad = findClosestHit(world, r);
    if (closest_shad) {
      if (closest_shad->getRecord().t_min() >= 0.0f &&
          closest_shad->getRecord().t_min() < distance)
        return true;
    }
  }
  return false;
}

SceneElementPtr PhongModel::findClosestHit(const SceneElementPtr& world,
                                           const Ray& r) {
  WorldIterator it(world->getWorldList());
  m_tmin = MAXFLOAT;
  if (it.first()) {
    while (it.notDone()) {
      if (it.currentElement()->intersect(r) &&
          it.currentElement()->getRecord().t_min() >= 0.0f &&
          it.currentElement()->getRecord().t_min() < m_tmin) {
        m_closestHit = it.currentElement();
        m_tmin = it.currentElement()->getRecord().t_min();
      }
      it.advance();
    }
  }
  return m_closestHit;
}
