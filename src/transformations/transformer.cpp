#include "transformations/transformer.h"

Transformer::Transformer(SceneElementRawPtr tr, const Mat4f& m)
    : SceneElementDecorator(tr, m) {
  WorldIterator it(getChildren());
  if (it.first()) {
    while (it.notDone()) {
      it.currentElement()->setParent(this);
      it.advance();
    }
  }
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

void Transformer::setMaterial(BaseMaterialPtr mat) {
  SceneElementDecorator::setMaterial(mat);
}

BaseMaterialPtr Transformer::getMaterial() const {
  return SceneElementDecorator::getMaterial();
}

void Transformer::setParent(SceneElementRawPtr parent) {
  SceneElementDecorator::setParent(parent);
}

SceneElementRawPtr Transformer::getParent() const {
  return SceneElementDecorator::getParent();
}

Point3f Transformer::pointFromWorldToObjectSpace(Point3f& point) const {
  if (getParent()) {
    point = getParent()->pointFromWorldToObjectSpace(point);
  }
  return m_transformMatrix.inverse() * Vec4f(point);
}

Vec3f Transformer::vectorFromObjectToWorldSpace(Vec3f vec) const {
  vec = m_transformMatrix.inverse().transpose() * Vec4f(vec);
  vec = vec.normalize();
  if (getParent()) {
    vec = getParent()->vectorFromObjectToWorldSpace(vec);
  }
  return vec;
}
