#include "transformations/transformer.h"

Transformer::Transformer(SceneElement* tr, const Mat4f& m)
    : SceneElementDecorator(tr, m) {}

bool Transformer::intersect(const Ray& r, IntersectionRecord& record) {
  auto r_transformed = r.transform(m_transformMatrix.inverse());
  return SceneElementDecorator::intersect(r_transformed, record);
}

Vec3f Transformer::normal(const Point3f& p) const {
  Vec4f v4 = p;
  Point3f object_point = m_transformMatrix.inverse() * v4;
  Vec3f object_normal = SceneElementDecorator::normal(object_point);
  Vec3f world_normal =
      m_transformMatrix.inverse().transpose() * Vec4f(object_normal);
  return world_normal.normalize();
}

void Transformer::add(std::shared_ptr<SceneElement> item) {
  SceneElementDecorator::add(item);
}

void Transformer::remove(SceneElement* item, bool del) {
  SceneElementDecorator::remove(item, del);
}

bool Transformer::isWorld() const { return SceneElementDecorator::isWorld(); }

void Transformer::accept(BaseRenderer& renderer, const Ray& ray) {
  SceneElementDecorator::accept(renderer, ray);
}

void Transformer::setMaterial(BaseMaterialPtr mat) {
  SceneElementDecorator::setMaterial(mat);
}

BaseMaterialPtr Transformer::getMaterial() const {
  return SceneElementDecorator::getMaterial();
}

void Transformer::setParent(SceneElementRawPtr parent) {
  SceneElement::setParent(parent);
}

SceneElementRawPtr Transformer::getParent() const {
  return SceneElementDecorator::getParent();
}
