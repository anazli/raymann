#include "transformations/transformer.h"

Transformer::Transformer(SceneElementRawPtr tr, const Mat4f& m)
    : SceneElementDecorator(tr, m) {
  transformBox();
  if (tr->isWorld()) {
    WorldIterator it(getChildren());
    if (it.first()) {
      while (it.notDone()) {
        it.currentElement()->setParent(this);
        it.advance();
      }
    }
  }
}

SceneElementType Transformer::elementType() const {
  return SceneElementDecorator::elementType();
}

bool Transformer::intersect(const Ray& r, IntersectionRecord& record) {
  auto r_transformed = r.transform(m_transformMatrix.inverse());
  return SceneElementDecorator::intersect(r_transformed, record);
}

Vec3f Transformer::normal(const Point3f& p) const {
  Point3f world_point(p);
  Point3f local_point = pointFromWorldToObjectSpace(world_point);
  Vec3f local_normal = SceneElementDecorator::normal(local_point);
  return vectorFromObjectToWorldSpace(local_normal);
}

void Transformer::add(SceneElementPtr item) {
  SceneElementDecorator::add(item);
  WorldIterator it(getChildren());
  if (it.first()) {
    while (it.notDone()) {
      it.currentElement()->setParent(this);
      it.advance();
    }
  }
}

void Transformer::remove(SceneElementRawPtr item, bool del) {
  SceneElementDecorator::remove(item, del);
}

bool Transformer::isWorld() const { return SceneElementDecorator::isWorld(); }

void Transformer::accept(BaseRenderer& renderer, const Ray& ray) {
  SceneElementDecorator::accept(renderer, ray);
}

SceneElementContainer Transformer::getChildren() {
  return SceneElementDecorator::getChildren();
}

void Transformer::setParent(SceneElementRawPtr parent) {
  SceneElementDecorator::setParent(parent);
}

SceneElementRawPtr Transformer::getParent() const {
  return SceneElementDecorator::getParent();
}

Point3f Transformer::pointFromWorldToObjectSpace(const Point3f& point) const {
  Point3f p(point);
  if (getParent()) {
    p = getParent()->pointFromWorldToObjectSpace(p);
  }
  return m_transformMatrix.inverse() * Vec4f(p);
}

Vec3f Transformer::vectorFromObjectToWorldSpace(const Vec3f vec) const {
  Vec3f v(vec);
  v = m_transformMatrix.inverse().transpose() * Vec4f(v);
  v = v.normalize();
  if (getParent()) {
    v = getParent()->vectorFromObjectToWorldSpace(v);
  }
  return v;
}

void Transformer::transformBox() {
  Point3f p1 = m_bBox.minPoint();
  Point3f p2 = Point3f(m_bBox.minPoint().x(), m_bBox.minPoint().y(),
                       m_bBox.maxPoint().z());
  Point3f p3 = Point3f(m_bBox.minPoint().x(), m_bBox.maxPoint().y(),
                       m_bBox.minPoint().z());
  Point3f p4 = Point3f(m_bBox.minPoint().x(), m_bBox.maxPoint().y(),
                       m_bBox.maxPoint().z());
  Point3f p5 = Point3f(m_bBox.maxPoint().x(), m_bBox.minPoint().y(),
                       m_bBox.minPoint().z());
  Point3f p6 = Point3f(m_bBox.maxPoint().x(), m_bBox.minPoint().y(),
                       m_bBox.maxPoint().z());
  Point3f p7 = Point3f(m_bBox.maxPoint().x(), m_bBox.maxPoint().y(),
                       m_bBox.minPoint().z());
  Point3f p8 = m_bBox.maxPoint();
  m_bBox = BoundingBox{};
  std::vector<Point3f> v{p1, p2, p3, p4, p5, p6, p7, p8};
  for (Point3f& elem : v) {
    m_bBox.addPoint(m_transformMatrix * Vec4f(elem));
  }
}
