#include "decorators/element_deco.h"

ElementDeco::ElementDeco(SceneElement* tr) : m_element(tr) {}

ElementDeco::~ElementDeco() { delete m_element; }

bool ElementDeco::intersect(const Ray& r) { return m_element->intersect(r); }

Vec3f ElementDeco::normal(const Point3f& p) const {
  return m_element->normal(p);
}

void ElementDeco::add(std::shared_ptr<SceneElement> item) {
  m_element->add(item);
}

void ElementDeco::remove(std::shared_ptr<SceneElement> item, bool del) {
  m_element->remove(item, del);
}

bool ElementDeco::isWorld() const { return m_element->isWorld(); }

void ElementDeco::accept(BaseRendererPtr& renderer, const Ray& ray) {
  m_element->accept(renderer, ray);
}
