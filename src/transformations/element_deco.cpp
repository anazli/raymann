#include "transformations/element_deco.h"

ElementDeco::ElementDeco(SceneElement* tr) : m_element(tr) {}

ElementDeco::~ElementDeco() { delete m_element; }

bool ElementDeco::intersect(const Ray& r, IntersectionRecord& record) {
  return m_element->intersect(r, record);
}

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

void ElementDeco::accept(BaseRenderer& renderer, const Ray& ray) {
  m_element->accept(renderer, ray);
}

void ElementDeco::setMaterial(BaseMaterialPtr mat) {
  m_element->setMaterial(mat);
}

BaseMaterialPtr ElementDeco::getMaterial() const {
  return m_element->getMaterial();
}

void ElementDeco::setParent(std::shared_ptr<SceneElement> t) {
  m_element->setParent(t);
}

std::shared_ptr<SceneElement> ElementDeco::getParent() const {
  return m_element->getParent();
}
