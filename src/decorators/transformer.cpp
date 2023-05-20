#include "decorators/transformer.h"

Transformer::Transformer(Element* tr) : ElementDeco(tr) {
  m_transformer.identity();
}

Transformer::Transformer(Element* tr, const Mat4f& m)
    : ElementDeco(tr), m_transformer(m) {}

Transformer::~Transformer() {}

bool Transformer::intersect(const Ray& r) {
  Ray r_transformed = r.transform(m_transformer.inverse());
  return ElementDeco::intersect(r_transformed);
}

Record& Transformer::record() { return ElementDeco::record(); }

Vec3f Transformer::normal(const Point3f& p) const {
  Vec4f v4 = p;
  Point3f object_point = m_transformer.inverse() * v4;
  Vec3f object_normal = ElementDeco::normal(object_point);
  Vec3f world_normal =
      m_transformer.inverse().transpose() * Vec4f(object_normal);
  return world_normal.normalize();
}
