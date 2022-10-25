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
      m_closest_hit = closestHit(r);
    }
  return has_intersection;
}

Vec3f World::colorAt(const Ray &ray) {
  if (m_closest_hit) {
    Vec3f color = m_closest_hit->lighting(shared_from_this(), ray);
    return color;
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
