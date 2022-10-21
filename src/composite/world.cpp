#include "composite/world.h"

#include <algorithm>

using std::list;
using std::shared_ptr;
using std::sort;
using std::vector;

World::~World() {}

bool World::intersect(const Ray &r) {
  bool has_intersection = false;
  list<shared_ptr<Traceable>>::iterator it;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it)
    if ((*it)->intersect(r) && (*it)->record().t_min() >= 0.0f)
      has_intersection = true;  // TODO: to be fixed for negative intersections
  return has_intersection;
}

Vec3f World::color_at(const Ray &ray, const int &rec) {
  if (intersect(ray)) {
    Traceable &t = closestHit(ray);
    Vec3f surf_col = t.lighting(getLight(), ray);
    Vec3f refl_col = t.reflectedColor(ray, rec);
    if (isShadowed(t.record().over_point_from_surf)) {
      return t.record().ambient + refl_col;
    }
    return surf_col + refl_col;
  }
  return Vec3f(0.f, 0.f, 0.f);
}

void World::add(shared_ptr<Traceable> item) {
  m_traceable_list.push_back(item);
  item->setParent(this);
}

void World::remove(std::shared_ptr<Traceable> item, bool del) {
  m_traceable_list.remove(item);
  item->setParent(nullptr);
  if (del) item.reset();
}

Traceable &World::closestHit(const Ray &r) {
  list<shared_ptr<Traceable>>::const_iterator it;
  Traceable *ret = nullptr;
  float min_hit = MAXFLOAT;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it) {
    if ((*it)->intersect(r) && (*it)->record().t_min() >= 0.0f &&
        (*it)->record().t_min() < min_hit) {
      ret = (it->get());
      min_hit = ret->record().t_min();
    }
  }
  if (!ret) {
    ret = m_traceable_list.front()
              .get();  // There is no Hit -> Black color, so any member would
                       // be ok, just return the first one.
  }
  return *ret;
}

vector<float> World::intersectionsSorted() const {
  vector<float> ret;
  list<shared_ptr<Traceable>>::const_iterator it;
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
  Traceable &t = closestHit(r);
  if (t.record().t_min() >= 0.0f && t.record().t_min() < distance) return true;
  return false;
}

Record &World::record() { return rec; }
