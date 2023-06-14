#include "renderers/renderer.h"

#include <algorithm>

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
    Vec3f refract_col = refractedColor(world, ray, rec);
    return surf_col + refl_col + refract_col;
  }
  return Vec3f(0.f, 0.f, 0.f);
}

Vec3f PhongModel::lighting(const SceneElementPtr& world, const Ray& ray) {
  Vec3f normal = m_closestHit->normal(m_closestHit->getRecord().point(ray));
  Point3f point = m_closestHit->getRecord().point(ray);
  Point3f over_point =
      point + (m_closestHit->getRecord().inside ? normal : normal) * 0.02f;
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
  Vec3f black(0.f, 0.f, 0.f);
  if (closest_refl) {
    if (rec <= 0) {
      return black;
    }
    if (closest_refl->getMaterial()->getProperties().getPropertyAsFloat(
            Props::REFLECTION) <= 0.f) {
      return black;
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
  return black;
}

Vec3f PhongModel::refractedColor(const SceneElementPtr& world, const Ray& r,
                                 int rec) {
  SceneElementPtr closest_refract = findClosestHit(world, r);
  Vec3f black(0.f, 0.f, 0.f);
  if (closest_refract->getMaterial()->getProperties().getPropertyAsFloat(
          Props::TRANSPARENCY) <= 0.f ||
      rec == 0) {
    return black;
  }

  determineRefractionIndices(world, r);
  findClosestHit(world, r);
  findRefractionIndicesForClosestHit(world);
  IntersectionRecord record = m_closestHit->getRecord();
  Vec3f normal_vec = record.inside ? -m_closestHit->normal(record.point(r))
                                   : m_closestHit->normal(record.point(r));
  m_closestHit->getRecord().under_point_from_refrac_surf =
      record.point(r) - normal_vec * EPS;
  float ratio = 1.f / 1.5f;
  float cosi = dot(record.eye(r), normal_vec);
  float sin2_t = ratio * ratio * (1.f - cosi * cosi);
  if (sin2_t > 1.f) return black;  // total reflection
  float cos_t = sqrt(1.f - sin2_t);
  Vec3f direction = normal_vec * (ratio * cosi - cos_t) - record.eye(r) * ratio;
  Ray refracted = Ray(record.under_point_from_refrac_surf, direction);

  return computeColor(world, refracted, rec - 1) *
         m_closestHit->getMaterial()->getProperties().getPropertyAsFloat(
             Props::TRANSPARENCY);
}

void PhongModel::determineRefractionIndices(const SceneElementPtr& world,
                                            const Ray& r) {
  std::map<size_t, std::pair<size_t, float>> intersections =
      intersectionsSorted(world);
  std::map<size_t, std::pair<size_t, float>>::const_iterator iter;
  std::list<SceneElementPtr> container;
  for (const auto& [key, value] : intersections) {
    SceneElementPtr current_elem = findSceneElementById(value.first, world);
    float n1 = 0, n2 = 0;
    if (container.empty()) {
      n1 = 1.f;
    } else {
      n1 = container.back()->getMaterial()->getProperties().getPropertyAsFloat(
          Props::REFRACTIVE_INDEX);
    }

    bool is_in = false;
    std::remove_if(container.begin(), container.end(),
                   [&](const SceneElementPtr& elem) {
                     if (elem.get() == current_elem.get()) {
                       is_in = true;
                       return true;
                     } else {
                       return false;
                     }
                   });
    if (!is_in) {
      container.emplace_back(current_elem);
    }

    if (container.empty()) {
      n2 = 1.f;
    } else {
      n2 = container.back()->getMaterial()->getProperties().getPropertyAsFloat(
          Props::REFRACTIVE_INDEX);
    }
    m_refract_index_collection[key] = std::make_pair(n1, n2);
    current_elem->getRecord().n1 = n1;
    current_elem->getRecord().n2 = n2;
  }
}

std::map<size_t, std::pair<float, float>> PhongModel::getContainer() const {
  return m_refract_index_collection;
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

std::map<size_t, std::pair<size_t, float>> PhongModel::intersectionsSorted(
    const SceneElementPtr& world) const {
  std::map<size_t, std::pair<size_t, float>> ret;
  size_t count = 0;
  WorldIterator iter(world->getWorldList());
  if (iter.first()) {
    while (iter.notDone()) {
      ret[count] = std::make_pair(iter.currentElement()->getId(),
                                  iter.currentElement()->getRecord().t1);
      count++;
      ret[count] = std::make_pair(iter.currentElement()->getId(),
                                  iter.currentElement()->getRecord().t2);
      iter.advance();
      count++;
    }
  }

  return ret;
}

SceneElementPtr PhongModel::findSceneElementById(const size_t& id,
                                                 const SceneElementPtr& world) {
  WorldIterator it(world->getWorldList());
  std::list<SceneElementPtr>::const_iterator ret_iter = std::find_if(
      it.begin(), it.end(),
      [&id](const SceneElementPtr& elem) { return elem->getId() == id; });
  if (ret_iter != it.end()) {
    return *ret_iter;
  }
  return *it.begin();
}

void PhongModel::findRefractionIndicesForClosestHit(
    const SceneElementPtr& world) {
  std::map<size_t, std::pair<size_t, float>> intersections =
      intersectionsSorted(world);
  for (const auto& [key, value] : intersections) {
    if (m_closestHit->getRecord().t_min() == value.second) {
      m_closestHit->getRecord().n1 = m_refract_index_collection[key].first;
      m_closestHit->getRecord().n2 = m_refract_index_collection[key].second;
      break;
    }
  }
}

BasicPathTracer::BasicPathTracer(const BaseCamera& cam) : m_cam(cam) {}

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
