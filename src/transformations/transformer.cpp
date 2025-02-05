#include "transformations/transformer.h"

#include "composite/iterator.h"

Transformer::Transformer(SceneElementRawPtr tr, const Mat4D& m)
    : SceneElementDecorator(tr, m) {
  m_inverseTransf = m_transformMatrix.inverse();
  m_inverseTransfTransp = m_inverseTransf.transpose();
  m_elementType = tr->elementType();
  m_bBox = tr->boundingBox();
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
  auto r_transformed = r.transform(m_inverseTransf);
  return SceneElementDecorator::intersect(r_transformed, record);
}

Vec3D Transformer::normal(const Point3D& p) const {
  // This is meant for normal calculation when world is transformed.
  //  The vectorFromObjectToWorldSpace from SceneElement is called here
  //  which does inversion and leads to big performance overhead. TODO:
  //  Refactor. Disabled for the moment.
  //  Point3D world_point(p);
  //  Point3D local_point = pointFromWorldToObjectSpace(world_point);
  //  Vec3D local_normal = SceneElementDecorator::normal(local_point);
  //  return vectorFromObjectToWorldSpace(local_normal);
  Vec4D v4 = p;
  Point3D object_point = m_inverseTransf * v4;
  Vec3D object_normal = SceneElementDecorator::normal(object_point);
  Vec3D world_normal = m_inverseTransfTransp * Vec4D(object_normal);
  return getUnitVectorOf(world_normal);
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

SceneElementContainer::iterator Transformer::remove(
    SceneElementRawPtr item, SceneElementPtr removedElem) {
  return SceneElementDecorator::remove(item, removedElem);
}

bool Transformer::isWorld() const { return SceneElementDecorator::isWorld(); }

void Transformer::accept(BaseRenderer& renderer, const Ray& ray) {
  SceneElementDecorator::accept(renderer, ray);
}

SceneElementContainer Transformer::getChildren() const {
  return SceneElementDecorator::getChildren();
}

SceneElementContainer& Transformer::getChildren() {
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

float Transformer::pdf(const Point3D& origin, const Vec3D& direction) {
  return SceneElementDecorator::pdf(origin, direction);
}

Vec3D Transformer::random(const Point3D& origin) {
  return SceneElementDecorator::random(origin);
}

Point3D Transformer::pointFromWorldToObjectSpace(const Point3D& point) const {
  Point3D p(point);
  if (getParent()) {
    p = getParent()->pointFromWorldToObjectSpace(p);
  }
  return m_inverseTransf * Vec4D(p);
}

Vec3D Transformer::vectorFromObjectToWorldSpace(const Vec3D vec) const {
  Vec3D v(vec);
  v = m_inverseTransfTransp * Vec4D(v);
  v.normalize();
  if (getParent()) {
    v = getParent()->vectorFromObjectToWorldSpace(v);
  }
  return v;
}

void Transformer::transformBox() {
  BoundingBox b = boundingBox();
  Point3D p1 = b.minPoint();
  Point3D p2 = Point3D(b.minPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3D p3 = Point3D(b.minPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3D p4 = Point3D(b.minPoint().x(), b.maxPoint().y(), b.maxPoint().z());
  Point3D p5 = Point3D(b.maxPoint().x(), b.minPoint().y(), b.minPoint().z());
  Point3D p6 = Point3D(b.maxPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3D p7 = Point3D(b.maxPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3D p8 = b.maxPoint();
  setBoundingBox(BoundingBox{});
  std::vector<Point3D> v{p1, p2, p3, p4, p5, p6, p7, p8};
  for (Point3D& elem : v) {
    boundingBox().addPoint(m_transformMatrix * Vec4D(elem));
  }
}
