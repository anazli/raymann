#include "composite/scene_element.h"

#include "iterator.h"
#include "renderers/renderer.h"
#include "scene_element.h"
#include "world.h"

bool SceneElement::intersect(const Ray& r, IntersectionRecord& record) {
  return false;
}

void SceneElement::add(SceneElementPtr item) {}

SceneElementContainer::iterator SceneElement::remove(
    SceneElementRawPtr item, SceneElementPtr removedElem) {}

bool SceneElement::isWorld() const { return false; }

Vec3D SceneElement::normal(const Point3D& p) const { return Vec3D(); }

void SceneElement::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElementLeaf(this, ray);
}

SceneElementContainer SceneElement::getChildren() const {
  return SceneElementContainer();
}

std::vector<std::shared_ptr<SceneElement> >& SceneElement::getChildren() {}

void SceneElement::setMaterial(MaterialPtr mat) { m_material = mat; }

MaterialPtr SceneElement::getMaterial() const { return m_material; }

void SceneElement::setParent(SceneElementRawPtr parent) { m_parent = parent; }

SceneElementRawPtr SceneElement::getParent() const { return m_parent; }

void SceneElement::setLight(const PointLight& light) {}

PointLight SceneElement::getLight() const { return PointLight(); }

void SceneElement::setBoundingBox(const BoundingBox& box) { m_bBox = box; }

BoundingBox& SceneElement::boundingBox() { return m_bBox; }

const BoundingBox& SceneElement::boundingBox() const { return m_bBox; }

float SceneElement::pdf(const Point3D& origin, const Vec3D& direction) {
  return 1.f;
}

Vec3D SceneElement::random(const Point3D& origin) {
  return Vec3D(1.f, 0.f, 0.f);
}

Point3D SceneElement::pointFromWorldToObjectSpace(const Point3D& point) const {
  return Mat4D().inverse() * Vec4D(point);
}

Vec3D SceneElement::vectorFromObjectToWorldSpace(const Vec3D vec) const {
  Vec3D v(vec);
  v = Mat4D().inverse().transpose() * Vec4D(v);
  v.normalize();
  return v;
}

void SceneElement::setTransformation(const Transformation& transformation) {
  m_transformation = transformation;
  m_transformation.transformBoundingBox(m_bBox);
}
SceneElement::SceneElement() {
  m_transformation = Transformation();
  if (isWorld()) {
    WorldIterator it(getChildren());
    if (it.first()) {
      while (it.notDone()) {
        it.currentElement()->setParent(this);
        it.advance();
      }
    }
  }
}
SceneElement::SceneElement(const BoundingBox& props) : m_bBox(props) {
  m_transformation = Transformation();
  if (isWorld()) {
    WorldIterator it(getChildren());
    if (it.first()) {
      while (it.notDone()) {
        it.currentElement()->setParent(this);
        it.advance();
      }
    }
  }
}
