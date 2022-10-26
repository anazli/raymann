#include "composite/world.h"

#include <algorithm>

using std::list;
using std::shared_ptr;
using std::sort;
using std::vector;

World::World() : m_closest_hit(nullptr) {}

World::~World() {}

bool World::intersect(const Ray &r) {
  bool has_intersection = false;
  list<TraceablePtr>::iterator it;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it)
    if ((*it)->intersect(r) && (*it)->record().t_min() >= 0.0f) {
      has_intersection = true;  // TODO: to be fixed for negative intersections
    }
  return has_intersection;
}

Vec3f World::lighting(const Ray &ray) { return m_closest_hit->lighting(ray); }

Vec3f World::colorAt(const Ray &ray, int rec) {
  m_closest_hit = closestHit(ray);
  if (m_closest_hit) {
    m_closest_hit->setLight(getLight());
    Vec3f surf_col = lighting(ray);
    Vec3f refl_col = reflectedColor(ray, rec);
    return surf_col + refl_col;
  }
  return Vec3f(0.f, 0.f, 0.f);
}

Vec3f World::reflectedColor(const Ray &r, int rec) {
  TraceablePtr closest_refl = closestHit(r);
  if (closest_refl) {
    if (rec <= 0) {
      return Vec3f(0.f, 0.f, 0.f);
    }
    if (closest_refl->getReflection() <= 0.f) {
      return Vec3f(0.f, 0.f, 0.f);
    }
    Vec3f reflectv =
        reflect(r.direction(),
                (closest_refl->record().inside
                     ? closest_refl->normal(closest_refl->record().point(r))
                     : closest_refl->normal(closest_refl->record().point(r))));
    Point3f over_point =
        closest_refl->record().point(r) +
        (closest_refl->record().inside
             ? closest_refl->normal(closest_refl->record().point(r))
             : closest_refl->normal(closest_refl->record().point(r))) *
            EPS1;
    closest_refl->record().over_point_from_refl_surf = over_point;
    Ray reflect_ray(over_point, reflectv);

    Vec3f color = colorAt(reflect_ray, rec - 1);
    return color * closest_refl->getReflection();
  }
  return Vec3f(0.f, 0.f, 0.f);
}

void World::add(TraceablePtr item) {
  item->setParent(shared_from_this());
  m_traceable_list.push_back(item);
}

void World::remove(TraceablePtr item, bool del) {
  m_traceable_list.remove(item);
  item->setParent(nullptr);
  if (del) item.reset();
}

TraceablePtr World::closestHit(const Ray &r) {
  list<TraceablePtr>::const_iterator it;
  float min_hit = MAXFLOAT;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it) {
    if ((*it)->intersect(r) && (*it)->record().t_min() >= 0.0f &&
        (*it)->record().t_min() < min_hit) {
      m_closest_hit = *it;
      min_hit = (*it)->record().t_min();
    }
  }
  return m_closest_hit;
}

vector<float> World::intersectionsSorted() const {
  vector<float> ret;
  list<TraceablePtr>::const_iterator it;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it) {
    ret.push_back((*it)->record().t1);
    ret.push_back((*it)->record().t2);
  }
  sort(ret.begin(), ret.end(), [](float f1, float f2) { return f1 < f2; });
  return ret;
}

bool World::isShadowed(const Point3f &p) {
  Vec3f v = p - getLight().position();
  float distance = v.length();
  Ray r(getLight().position(), v.normalize());
  if (intersect(r)) {
    if (m_closest_hit) {
      if (m_closest_hit->record().t_min() >= 0.0f &&
          m_closest_hit->record().t_min() < distance)
        return true;
    }
  }
  return false;
}
