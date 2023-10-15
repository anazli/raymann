#include "transformations/transformer.h"

Transformer::Transformer(SceneElement* tr) : ElementDeco(tr) {
  m_transformer.identity();
}

Transformer::Transformer(SceneElement* tr, const Mat4f& m)
    : ElementDeco(tr), m_transformer(m) {}

bool Transformer::intersect(const Ray& r, IntersectionRecord& record) {
  auto r_transformed = r.transform(m_transformer.inverse());
  return ElementDeco::intersect(r_transformed, record);
}

Vec3f Transformer::normal(const Point3f& p) const {
  Vec4f v4 = p;
  Point3f object_point = m_transformer.inverse() * v4;
  Vec3f object_normal = ElementDeco::normal(object_point);
  Vec3f world_normal =
      m_transformer.inverse().transpose() * Vec4f(object_normal);
  return world_normal.normalize();
}

void Transformer::add(std::shared_ptr<SceneElement> item) {
  ElementDeco::add(item);
}

void Transformer::remove(std::shared_ptr<SceneElement> item, bool del) {
  ElementDeco::remove(item, del);
}

bool Transformer::isWorld() const { return ElementDeco::isWorld(); }

void Transformer::accept(BaseRenderer& renderer, const Ray& ray) {
  ElementDeco::accept(renderer, ray);
}

void Transformer::setMaterial(BaseMaterialPtr mat) {
  ElementDeco::setMaterial(mat);
}

BaseMaterialPtr Transformer::getMaterial() const {
  return ElementDeco::getMaterial();
}

void Transformer::setParent(std::shared_ptr<SceneElement> t) {
  SceneElement::setParent(t);
}

std::shared_ptr<SceneElement> Transformer::getParent() const {
  return ElementDeco::getParent();
}
