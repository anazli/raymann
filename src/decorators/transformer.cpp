#include "decorators/transformer.h"

Transformer::Transformer(Traceable* tr) : TraceableDeco(tr) {
  m_transformer.identity();
}

Transformer::Transformer(Traceable* tr, const Mat4f& m)
    : TraceableDeco(tr), m_transformer(m) {}

Transformer::~Transformer() {}

bool Transformer::intersect(const Ray& r) {
  Ray r_transformed = r.transform(m_transformer.inverse());
  return TraceableDeco::intersect(r_transformed);
}

Record& Transformer::record() { return TraceableDeco::record(); }

Vec3f Transformer::normal(const Point3f& p) const {
  Vec4f v4 = p;
  Point3f object_point = m_transformer.inverse() * v4;
  Vec3f object_normal = TraceableDeco::normal(object_point);
  Vec3f world_normal =
      m_transformer.inverse().transpose() * Vec4f(object_normal);
  return world_normal.normalize();
}
