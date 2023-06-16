#include "renderers/phong_model.h"

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
  if (m_closestHit.object) {
    Vec3f surf_col = lighting(world, ray);
    Vec3f refl_col = reflectedColor(world, ray, rec);
    Vec3f refract_col = refractedColor(world, ray, rec);
    return surf_col + refl_col + refract_col;
  }
  return Vec3f(0.f, 0.f, 0.f);
}

Vec3f PhongModel::lighting(const SceneElementPtr& world, const Ray& ray) {
  Vec3f normal = m_closestHit.object->normal(m_closestHit.point(ray));
  Point3f point = m_closestHit.point(ray);
  Point3f over_point = point + (m_closestHit.inside ? normal : normal) * 0.02f;
  Vec3f normal_vec = m_closestHit.inside
                         ? -m_closestHit.object->normal(over_point)
                         : m_closestHit.object->normal(over_point);
  Vec3f lightv = (world->getLight().position() - over_point).normalize();

  Vec3f effective_color =
      m_closestHit.object->getMaterial()->getTexture()->value(
          0, 0, Vec3f(over_point)) *
      world->getLight().intensity();

  Vec3f ret_ambient =
      effective_color *
      m_closestHit.object->getMaterial()->getProperties().getPropertyAsFloat(
          Props::AMBIENT);
  Vec3f ret_diffuse;
  Vec3f ret_specular;

  if (isShadowed(world, over_point)) return ret_ambient;

  float light_normal = dot(lightv, normal_vec);
  if (light_normal > 0.0f) {
    ret_diffuse =
        effective_color *
        m_closestHit.object->getMaterial()->getProperties().getPropertyAsFloat(
            Props::DIFFUSE) *
        light_normal;
    Vec3f reflectv = reflect(-lightv, normal_vec);
    float reflect_dot_eye = dot(reflectv, m_closestHit.eye(ray));
    if (reflect_dot_eye > 0.0f) {
      float factor =
          pow(reflect_dot_eye, m_closestHit.object->getMaterial()
                                   ->getProperties()
                                   .getPropertyAsFloat(Props::SHININESS));
      ret_specular = world->getLight().intensity() *
                     m_closestHit.object->getMaterial()
                         ->getProperties()
                         .getPropertyAsFloat(Props::SPECULAR) *
                     factor;
    }
  }

  return ret_ambient + ret_diffuse + ret_specular;
}

Vec3f PhongModel::reflectedColor(const SceneElementPtr& world, const Ray& ray,
                                 int rec) {
  IntersectionRecord closest_refl = findClosestHit(world, ray);
  Vec3f black(0.f, 0.f, 0.f);
  if (closest_refl.object) {
    if (rec <= 0) {
      return black;
    }
    if (closest_refl.object->getMaterial()->getProperties().getPropertyAsFloat(
            Props::REFLECTION) <= 0.f) {
      return black;
    }
    Vec3f reflectv =
        reflect(ray.direction(),
                (closest_refl.inside
                     ? closest_refl.object->normal(closest_refl.point(ray))
                     : closest_refl.object->normal(closest_refl.point(ray))));
    Point3f over_point =
        closest_refl.point(ray) +
        (closest_refl.inside
             ? closest_refl.object->normal(closest_refl.point(ray))
             : closest_refl.object->normal(closest_refl.point(ray))) *
            EPS1;
    closest_refl.over_point_from_refl_surf = over_point;
    Ray reflect_ray(over_point, reflectv);

    Vec3f color = computeColor(world, reflect_ray, rec - 1);
    return color * closest_refl.object->getMaterial()
                       ->getProperties()
                       .getPropertyAsFloat(Props::REFLECTION);
  }
  return black;
}

Vec3f PhongModel::refractedColor(const SceneElementPtr& world, const Ray& ray,
                                 int rec) {
  IntersectionRecord closest_refract = findClosestHit(world, ray);
  Vec3f black(0.f, 0.f, 0.f);
  if (closest_refract.object->getMaterial()->getProperties().getPropertyAsFloat(
          Props::TRANSPARENCY) <= 0.f ||
      rec == 0) {
    return black;
  }

  determineRefractionIndices(world, ray);
  findRefractionIndicesForClosestHit(world, ray);
  Vec3f normal_vec =
      closest_refract.inside
          ? -closest_refract.object->normal(closest_refract.point(ray))
          : closest_refract.object->normal(closest_refract.point(ray));
  closest_refract.under_point_from_refrac_surf =
      closest_refract.point(ray) - normal_vec * EPS;
  float ratio = m_closestHit.n1 / m_closestHit.n2;
  float cosi = dot(closest_refract.eye(ray), normal_vec);
  float sin2_t = ratio * ratio * (1.f - cosi * cosi);
  if (sin2_t > 1.f) return black;  // total reflection
  float cos_t = sqrt(1.f - sin2_t);
  Vec3f direction =
      normal_vec * (ratio * cosi - cos_t) - closest_refract.eye(ray) * ratio;
  Ray refracted = Ray(closest_refract.under_point_from_refrac_surf, direction);

  return computeColor(world, refracted, rec - 1) *
         closest_refract.object->getMaterial()
             ->getProperties()
             .getPropertyAsFloat(Props::TRANSPARENCY);
  return Vec3f(0.f, 0.f, 0.f);
}

void PhongModel::determineRefractionIndices(const SceneElementPtr& world,
                                            const Ray& ray) {
  std::map<size_t, std::pair<size_t, float>> intersections =
      intersectionsSorted(world, ray);
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
  }
}

std::map<size_t, std::pair<float, float>> PhongModel::getContainer() const {
  return m_refract_index_collection;
}

void PhongModel::checkInside(const Ray& r) {}

bool PhongModel::isShadowed(const SceneElementPtr& world, const Point3f& p) {
  if (m_closestHit.object) {
    PointLight light = world->getLight().position();
    Vec3f v = p - light.position();
    float distance = v.length();
    Ray r(light.position(), v.normalize());
    IntersectionRecord closest_shad = findClosestHit(world, r);
    if (closest_shad.object) {
      if (closest_shad.t_min() > 0.0f && closest_shad.t_min() < distance)
        return true;
    }
  }
  return false;
}

IntersectionRecord PhongModel::findClosestHit(const SceneElementPtr& world,
                                              const Ray& ray) {
  WorldIterator it(world->getWorldList());
  m_tmin = MAXFLOAT;
  if (it.first()) {
    while (it.notDone()) {
      IntersectionRecord record;
      if (it.currentElement()->intersect(ray, record)) {
        if (record.t_min() > 0.0f && record.t_min() < m_tmin) {
          m_closestHit = record;
          m_closestHit.object = it.currentElement();
          m_tmin = m_closestHit.t_min();
        }
      }
      it.advance();
    }
  }
  return m_closestHit;
}

std::map<size_t, std::pair<size_t, float>> PhongModel::intersectionsSorted(
    const SceneElementPtr& world, const Ray& ray) const {
  std::map<size_t, std::pair<size_t, float>> ret;
  size_t count = 0;
  WorldIterator iter(world->getWorldList());
  if (iter.first()) {
    while (iter.notDone()) {
      IntersectionRecord record;
      if (iter.currentElement()->intersect(ray, record))
        if (record.t1 != -MAXFLOAT) {
          ret[count] =
              std::make_pair(iter.currentElement()->getId(), record.t1);
          count++;
        }
      if (record.t2 != -MAXFLOAT) {
        ret[count] = std::make_pair(iter.currentElement()->getId(), record.t2);
      }
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
    const SceneElementPtr& world, const Ray& ray) {
  std::map<size_t, std::pair<size_t, float>> intersections =
      intersectionsSorted(world, ray);
  for (const auto& [key, value] : intersections) {
    if (m_closestHit.t_min() == value.second) {
      m_closestHit.n1 = m_refract_index_collection[key].first;
      m_closestHit.n2 = m_refract_index_collection[key].second;
      break;
    }
  }
}
