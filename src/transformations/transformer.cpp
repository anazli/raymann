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

void Transformer::setBoundingBox(const BoundingBox& box) {
  SceneElementDecorator::setBoundingBox(box);
}

BoundingBox& Transformer::boundingBox() {
  return SceneElementDecorator::boundingBox();
}

const BoundingBox& Transformer::boundingBox() const {
  return SceneElementDecorator::boundingBox();
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
  BoundingBox b = boundingBox();
  Point3f p1 = b.minPoint();
  Point3f p2 = Point3f(b.minPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3f p3 = Point3f(b.minPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3f p4 = Point3f(b.minPoint().x(), b.maxPoint().y(), b.maxPoint().z());
  Point3f p5 = Point3f(b.maxPoint().x(), b.minPoint().y(), b.minPoint().z());
  Point3f p6 = Point3f(b.maxPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3f p7 = Point3f(b.maxPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3f p8 = b.maxPoint();
  setBoundingBox(BoundingBox{});
  std::vector<Point3f> v{p1, p2, p3, p4, p5, p6, p7, p8};
  for (Point3f& elem : v) {
    boundingBox().addPoint(m_transformMatrix * Vec4f(elem));
  }
}
