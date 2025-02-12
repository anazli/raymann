#include "renderers/phong_model.h"

void PhongModel::visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                                       const Ray& ray) {
  /*if (elementLeaf.intersect(ray) && elementLeaf.getRecord().min_hit > 0. &&
      elementLeaf.getRecord().min_hit < m_tmin) {
    m_closestHit = std::make_shared<SceneElement>(elementLeaf);
    m_tmin = m_closestHit->getRecord().min_hit;
  }*/
}

void PhongModel::visitSceneElementComposite(
    const SceneElementRawPtr elementComp, const Ray& ray) {
  if (elementComp->isWorld()) {
    m_out_color = computeColor(elementComp, ray);
  }
}

Vec3D PhongModel::computeColor(const SceneElementRawPtr world, const Ray& ray,
                               int rec) {
  m_closestHit = Intersection{};
  if (world->intersect(ray, m_closestHit)) {
    if (m_closestHit.primitive) {
      auto surf_col = lighting(world, ray);
      auto refl_col = reflectedColor(world, ray, rec);
      // auto refract_col = refractedColor(world, ray, rec);
      return surf_col + refl_col /*+ refract_col*/;
    }
  }
  return m_background_color;
}

Vec3D PhongModel::lighting(const SceneElementRawPtr world, const Ray& ray) {
  auto normal = m_closestHit.primitive->normal(m_closestHit.getHitPoint(ray));
  auto point = m_closestHit.getHitPoint(ray);
  m_closestHit.omega = -ray.direction();
  auto over_point =
      point + (m_closestHit.inside ? Vec3D(normal) : Vec3D(normal)) * 0.02f;
  auto normal_vec = m_closestHit.inside
                        ? -m_closestHit.primitive->normal(over_point)
                        : m_closestHit.primitive->normal(over_point);
  auto lightv = getUnitVectorOf(world->getLight().position() - over_point);

  auto effective_color =
      m_closestHit.primitive->getMaterial()->getTexture()->value(
          0, 0, Vec3D(over_point)) *
      world->getLight().intensity();

  auto ret_ambient =
      effective_color * m_closestHit.primitive->getMaterial()
                            ->getProperties()
                            .getPropertyAs<float>(AppParameters::AMBIENT)
                            .value();
  auto ret_diffuse = Vec3D{};
  auto ret_specular = Vec3D{};

  if (isShadowed(world, over_point)) return ret_ambient;

  auto light_normal = dot(lightv, normal_vec);
  if (light_normal > 0.0f) {
    ret_diffuse = effective_color *
                  m_closestHit.primitive->getMaterial()
                      ->getProperties()
                      .getPropertyAs<float>(AppParameters::DIFFUSE)
                      .value() *
                  light_normal;
    auto reflectv = reflect(-lightv, Vec3D(normal_vec));
    auto reflect_dot_eye = dot(reflectv, m_closestHit.omega);
    if (reflect_dot_eye > 0.0f) {
      float factor = pow(reflect_dot_eye,
                         m_closestHit.primitive->getMaterial()
                             ->getProperties()
                             .getPropertyAs<float>(AppParameters::SHININESS)
                             .value());
      ret_specular = world->getLight().intensity() *
                     m_closestHit.primitive->getMaterial()
                         ->getProperties()
                         .getPropertyAs<float>(AppParameters::SPECULAR)
                         .value() *
                     factor;
    }
  }

  return ret_ambient + ret_diffuse + ret_specular;
}

Vec3D PhongModel::reflectedColor(const SceneElementRawPtr world, const Ray& ray,
                                 int rec) {
  auto closestReflectedHit = Intersection{};
  auto black = Vec3D(0.f, 0.f, 0.f);
  if (world->intersect(ray, closestReflectedHit)) {
    if (closestReflectedHit.primitive) {
      if (rec <= 0) {
        return black;
      }
      if (closestReflectedHit.primitive->getMaterial()
              ->getProperties()
              .getPropertyAs<float>(AppParameters::REFLECTION)
              .value() <= 0.f) {
        return black;
      }
      auto reflectv = reflect(
          ray.direction(), (closestReflectedHit.inside
                                ? Vec3D(closestReflectedHit.primitive->normal(
                                      closestReflectedHit.getHitPoint(ray)))
                                : Vec3D(closestReflectedHit.primitive->normal(
                                      closestReflectedHit.getHitPoint(ray)))));
      auto over_point = closestReflectedHit.getHitPoint(ray) +
                        (closestReflectedHit.inside
                             ? Vec3D(closestReflectedHit.primitive->normal(
                                   closestReflectedHit.getHitPoint(ray)))
                             : Vec3D(closestReflectedHit.primitive->normal(
                                   closestReflectedHit.getHitPoint(ray)))) *
                            EPS1;
      closestReflectedHit.over_point_from_refl_surf = over_point;
      auto reflect_ray = Ray(over_point, reflectv);

      auto color = computeColor(world, reflect_ray, rec - 1);
      return color * closestReflectedHit.primitive->getMaterial()
                         ->getProperties()
                         .getPropertyAs<float>(AppParameters::REFLECTION)
                         .value();
    }
  }
  return black;
}

/*Vec3D PhongModel::refractedColor(const SceneElementPtr& world, const Ray& ray,
                                 int rec) {
  auto closest_refract = findClosestHit(world, ray);
  auto black = Vec3D(0.f, 0.f, 0.f);
  if (closest_refract.object->getMaterial()->getProperties().getPropertyAsFloat(
          Props::TRANSPARENCY) <= 0.f ||
      rec == 0) {
    return black;
  }

  determineRefractionIndices(world, ray);
  findRefractionIndicesForClosestHit(world, ray);
  auto normal_vec =
      closest_refract.inside
          ? -closest_refract.object->normal(closest_refract.point(ray))
          : closest_refract.object->normal(closest_refract.point(ray));
  closest_refract.under_point_from_refrac_surf =
      closest_refract.point(ray) - normal_vec * EPS;
  auto ratio = m_closestHit.n1 / m_closestHit.n2;
  auto cosi = dot(closest_refract.eye(ray), normal_vec);
  auto sin2_t = ratio * ratio * (1.f - cosi * cosi);
  if (sin2_t > 1.f) return black;  // total reflection
  auto cos_t = static_cast<float>(sqrt(1.f - sin2_t));
  auto direction =
      normal_vec * (ratio * cosi - cos_t) - closest_refract.eye(ray) * ratio;
  auto refracted = Ray(closest_refract.under_point_from_refrac_surf, direction);

  return computeColor(world, refracted, rec - 1) *
         closest_refract.object->getMaterial()
             ->getProperties()
             .getPropertyAsFloat(Props::TRANSPARENCY);
  return Vec3D(0.f, 0.f, 0.f);
}*/

/*void PhongModel::determineRefractionIndices(const SceneElementPtr& world,
                                            const Ray& ray) {
  auto intersections = intersectionsSorted(world, ray);
  std::map<size_t, std::pair<size_t, float>>::const_iterator iter;
  std::list<SceneElementPtr> container;
  for (const auto& [key, value] : intersections) {
    SceneElementPtr current_elem = findSceneElementById(value.first, world);
    auto n1 = 0.f, n2 = 0.f;
    if (container.empty()) {
      n1 = 1.f;
    } else {
      n1 = container.back()->getMaterial()->getProperties().getPropertyAsFloat(
          Props::REFRACTIVE_INDEX);
    }

    auto is_in = false;
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
}*/

/*std::map<size_t, std::pair<float, float>> PhongModel::getContainer() const {
  return m_refract_index_collection;
}*/

void PhongModel::checkInside(const Ray& r) {}

bool PhongModel::isShadowed(const SceneElementRawPtr world, const Point3D& p) {
  if (m_closestHit.primitive) {
    auto light = world->getLight();
    auto v = p - light.position();
    auto distance = v.length();
    auto r = Ray(light.position(), getUnitVectorOf(v));
    auto closestShadowedHit = Intersection{};
    if (world->intersect(r, closestShadowedHit)) {
      if (closestShadowedHit.primitive) {
        if (closestShadowedHit.min_hit > 0.0f &&
            closestShadowedHit.min_hit < distance)
          return true;
      }
    }
  }
  return false;
}

/*std::map<size_t, std::pair<size_t, float>> PhongModel::intersectionsSorted(
    const SceneElementPtr& world, const Ray& ray) const {
  std::map<size_t, std::pair<size_t, float>> ret;
  auto count = 0;
  WorldIterator iter(world->getChildren());
  if (iter.first()) {
    while (iter.notDone()) {
      auto record = IntersectionRecord{};
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
}*/

/*SceneElementPtr PhongModel::findSceneElementById(const size_t& id,
                                                 const SceneElementPtr& world) {
  WorldIterator it(world->getChildren());
  auto ret_iter = std::find_if(
      it.begin(), it.end(),
      [&id](const SceneElementPtr& elem) { return elem->getId() == id; });
  if (ret_iter != it.end()) {
    return *ret_iter;
  }
  return *it.begin();
}*/

/*void PhongModel::findRefractionIndicesForClosestHit(
    const SceneElementPtr& world, const Ray& ray) {
  auto intersections = intersectionsSorted(world, ray);
  for (const auto& [key, value] : intersections) {
    if (m_closestHit.min_hit == value.second) {
      m_closestHit.n1 = m_refract_index_collection[key].first;
      m_closestHit.n2 = m_refract_index_collection[key].second;
      break;
    }
  }
}*/
