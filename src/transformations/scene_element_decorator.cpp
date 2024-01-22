#include "transformations/scene_element_decorator.h"

SceneElementDecorator::SceneElementDecorator(SceneElement* tr, const Mat4f& m)
    : m_element(tr), m_transformMatrix(m) {}

SceneElementDecorator::~SceneElementDecorator() { delete m_element; }

bool SceneElementDecorator::intersect(const Ray& r,
                                      IntersectionRecord& record) {
  return m_element->intersect(r, record);
}

Vec3f SceneElementDecorator::normal(const Point3f& p) const {
  return m_element->normal(p);
}

void SceneElementDecorator::add(std::shared_ptr<SceneElement> item) {
  m_element->add(item);
}

void SceneElementDecorator::remove(SceneElement* item, bool del) {
  m_element->remove(item, del);
}

bool SceneElementDecorator::isWorld() const { return m_element->isWorld(); }

void SceneElementDecorator::accept(BaseRenderer& renderer, const Ray& ray) {
  m_element->accept(renderer, ray);
}

void SceneElementDecorator::setMaterial(BaseMaterialPtr mat) {
  m_element->setMaterial(mat);
}

BaseMaterialPtr SceneElementDecorator::getMaterial() const {
  return m_element->getMaterial();
}

void SceneElementDecorator::setParent(SceneElementRawPtr parent) {
  m_element->setParent(parent);
}

SceneElementRawPtr SceneElementDecorator::getParent() const {
  return m_element->getParent();
}
