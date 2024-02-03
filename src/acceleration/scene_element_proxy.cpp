#include "acceleration/scene_element_proxy.h"

#include "application/error.h"
#include "geometry/cone.h"
#include "geometry/cylinder.h"

SceneElementProxy::SceneElementProxy(SceneElementPtr element)
    : m_sceneElement(element) {
  if (m_sceneElement) {
    applyDefaultBoundsForElementType();
    if (m_sceneElement->transformationMatrix() != Mat4f()) {
      transformBox();
    }
  }
}

SceneElementProxy::SceneElementProxy(SceneElementPtr element,
                                     const Point3f &minPoint,
                                     const Point3f &maxPoint)
    : m_sceneElement(element),
      SceneElement(BoundingBoxProperties(minPoint, maxPoint)) {
  if (m_sceneElement && m_sceneElement->transformationMatrix() != Mat4f()) {
    transformBox();
  }
}

bool SceneElementProxy::intersect(const Ray &r, IntersectionRecord &record) {
  return false;
}

void SceneElementProxy::add(SceneElementPtr item) {
  APP_ASSERT(item, "Item is null");
  if (m_sceneElement->isWorld()) {
    addPoint(item->boundingBoxProperties().minPoint());
    addPoint(item->boundingBoxProperties().maxPoint());
    if (item->containsElement()) {
      item->getElementOfBoundingBox()->setParent(m_sceneElement.get());
      m_sceneElement->add(item->getElementOfBoundingBox());
    } else {
      item->setParent(m_sceneElement.get());
      m_sceneElement->add(item);
    }
  }
}

bool SceneElementProxy::containsElement() const {
  return m_sceneElement != nullptr;
}

SceneElementPtr SceneElementProxy::getElementOfBoundingBox() const {
  return m_sceneElement;
}

void SceneElementProxy::setElementOfBoundingBox(SceneElementPtr element) {
  m_sceneElement = element;
}

void SceneElementProxy::addPoint(const Point3f &point) {
  if (point.x() < m_bBoxProps.minPoint().x())
    m_bBoxProps.minPoint().setX(point.x());
  if (point.y() < m_bBoxProps.minPoint().y())
    m_bBoxProps.minPoint().setY(point.y());
  if (point.z() < m_bBoxProps.minPoint().z())
    m_bBoxProps.minPoint().setZ(point.z());

  if (point.x() > m_bBoxProps.maxPoint().x())
    m_bBoxProps.maxPoint().setX(point.x());
  if (point.y() > m_bBoxProps.maxPoint().y())
    m_bBoxProps.maxPoint().setY(point.y());
  if (point.z() > m_bBoxProps.maxPoint().z())
    m_bBoxProps.maxPoint().setZ(point.z());
}

bool SceneElementProxy::containsPoint(const Point3f &point) const {
  // three-way comparison doesn't work
  return point.x() >= m_bBoxProps.minPoint().x() &&
         point.x() <= m_bBoxProps.maxPoint().x() &&

         point.y() >= m_bBoxProps.minPoint().y() &&
         point.y() <= m_bBoxProps.maxPoint().y() &&

         point.z() >= m_bBoxProps.minPoint().z() &&
         point.z() <= m_bBoxProps.maxPoint().z();
}

bool SceneElementProxy::containsBoundingBox(
    const BoundingBoxProperties &prop) const {
  return containsPoint(prop.minPoint()) && containsPoint(prop.maxPoint());
}

void SceneElementProxy::applyDefaultBoundsForElementType() {
  SceneElementType t = m_sceneElement->elementType();
  switch (t) {
    case SceneElementType::SPHERE:
      m_bBoxProps = BoundingBoxProperties(Point3f(-1.f, -1.f, -1.f),
                                          Point3f(1.f, 1.f, 1.f));
      break;
    case SceneElementType::CONE: {
      Cone *cone = dynamic_cast<Cone *>(m_sceneElement.get());
      if (cone) {
        if (!cone->isClosed()) {
          m_bBoxProps = BoundingBoxProperties(
              Point3f(-limit::infinity(), -limit::infinity(),
                      -limit::infinity()),
              Point3f(limit::infinity(), limit::infinity(), limit::infinity()));
        } else {
          float a = fabs(cone->minimumY());
          float b = fabs(cone->maximumY());
          float lim = std::max(a, b);
          m_bBoxProps =
              BoundingBoxProperties(Point3f(-lim, cone->minimumY(), -lim),
                                    Point3f(lim, cone->maximumY(), lim));
        }
      }
      break;
    }
    case SceneElementType::CYLINDER: {
      Cylinder *cyl = dynamic_cast<Cylinder *>(m_sceneElement.get());
      if (cyl) {
        if (!cyl->isClosed()) {
          m_bBoxProps =
              BoundingBoxProperties(Point3f(-1.f, -limit::infinity(), -1.f),
                                    Point3f(1.f, limit::infinity(), 1.f));
        } else {
          m_bBoxProps =
              BoundingBoxProperties(Point3f(-1.f, cyl->minimumY(), -1.f),
                                    Point3f(1.f, cyl->maximumY(), 1.f));
        }
      }
      break;
    }
    case SceneElementType::CUBE:
      m_bBoxProps = BoundingBoxProperties(Point3f(-1.f, -1.f, -1.f),
                                          Point3f(1.f, 1.f, 1.f));
      break;
    case SceneElementType::TRIANGLE:
      break;
    case SceneElementType::PLANE:
      m_bBoxProps = BoundingBoxProperties(
          Point3f(-limit::infinity(), 0.f, -limit::infinity()),
          Point3f(limit::infinity(), 0.f, limit::infinity()));
      break;
    default:
      break;
  }
}

void SceneElementProxy::transformBox() {
  Point3f p1 = m_bBoxProps.minPoint();
  Point3f p2 = Point3f(m_bBoxProps.minPoint().x(), m_bBoxProps.minPoint().y(),
                       m_bBoxProps.maxPoint().z());
  Point3f p3 = Point3f(m_bBoxProps.minPoint().x(), m_bBoxProps.maxPoint().y(),
                       m_bBoxProps.minPoint().z());
  Point3f p4 = Point3f(m_bBoxProps.minPoint().x(), m_bBoxProps.maxPoint().y(),
                       m_bBoxProps.maxPoint().z());
  Point3f p5 = Point3f(m_bBoxProps.maxPoint().x(), m_bBoxProps.minPoint().y(),
                       m_bBoxProps.minPoint().z());
  Point3f p6 = Point3f(m_bBoxProps.maxPoint().x(), m_bBoxProps.minPoint().y(),
                       m_bBoxProps.maxPoint().z());
  Point3f p7 = Point3f(m_bBoxProps.maxPoint().x(), m_bBoxProps.maxPoint().y(),
                       m_bBoxProps.minPoint().z());
  Point3f p8 = m_bBoxProps.maxPoint();
  m_bBoxProps = BoundingBoxProperties{};
  std::vector<Point3f> v{p1, p2, p3, p4, p5, p6, p7, p8};
  for (Point3f &elem : v) {
    addPoint(m_sceneElement->transformationMatrix() * Vec4f(elem));
  }
}
