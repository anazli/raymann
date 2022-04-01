#include "world/world.h"

#include <algorithm>

using std::list;
using std::sort;
using std::vector;

World::~World() {
  /*list<Traceable *>::iterator it;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it)
    delete (*it);*/
}

bool World::intersect(const Ray &r) {
  bool has_intersection = false;
  list<std::shared_ptr<Traceable>>::iterator it;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it)
    if ((*it)->intersect(r)) has_intersection = true;
  return has_intersection;
}

void World::add(std::shared_ptr<Traceable> item) {
  m_traceable_list.push_back(item);
  item->setParent(this);
}

void World::remove(std::shared_ptr<Traceable> item, bool del) {
  m_traceable_list.remove(item);
  item->setParent(nullptr);
  if (del) item.reset();
}

Traceable &World::closestHit() {
  list<std::shared_ptr<Traceable>>::const_iterator it;
  Traceable *temp = nullptr;
  float min_hit = MAXFLOAT;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it) {
    if ((*it)->record().t_min() > 0.0f && (*it)->record().t_min() < min_hit) {
      temp = (it->get());
      min_hit = temp->record().t_min();
    }
  }
  if (temp == nullptr) {
    temp = m_traceable_list.front().get();  // just return the first one.
  }
  return *temp;
}

bool compare(const float &f1, const float &f2) { return f1 < f2; }

vector<float> World::intersectionsSorted() const {
  vector<float> ret;
  list<std::shared_ptr<Traceable>>::const_iterator it;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it) {
    ret.push_back((*it)->record().t1);
    ret.push_back((*it)->record().t2);
  }
  sort(ret.begin(), ret.end(), compare);
  return ret;
}

void World::createSphere(std::shared_ptr<SphereBuilder> b) {
  b->buildSphere();
  add(b->getSphere());
}
