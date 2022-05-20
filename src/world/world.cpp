#include "world/world.h"

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
    if ((*it)->intersect(r)) has_intersection = true;
  return has_intersection;
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

Traceable &World::closestHit() {
  list<shared_ptr<Traceable>>::const_iterator it;
  Traceable *ret = nullptr;
  float min_hit = MAXFLOAT;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it) {
    if ((*it)->record().t_min() > 0.0f && (*it)->record().t_min() < min_hit) {
      ret = (it->get());
      min_hit = ret->record().t_min();
    }
  }
  if (ret == nullptr) {
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

bool World::isShadowed(const PointLight &l, const Point3f &p) {
  Vec3f v = l.position() - p;
  float distance = v.length();
  Vec3f direction = v.normalize();
  Ray r(p, direction);
  intersect(r);
  Traceable &t = closestHit();
  if (t.record().t_min() > 0 && t.record().t_min() < distance) return true;
  return false;
}
