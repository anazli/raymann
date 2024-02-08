#include "composite/iterator.h"

WorldIterator::WorldIterator(const SceneElementContainer &world)
    : m_sceneElementContainer(world) {}

bool WorldIterator::first() {
  m_current = m_sceneElementContainer.begin();
  if (*m_current)
    return true;
  else
    return false;
}

SceneElementContainer::const_iterator WorldIterator::begin() const {
  return m_sceneElementContainer.begin();
}

SceneElementContainer::const_iterator WorldIterator::end() const {
  return m_sceneElementContainer.end();
}

void WorldIterator::advance() { m_current++; }

bool WorldIterator::notDone() {
  return m_current != m_sceneElementContainer.end();
}

SceneElementRawPtr WorldIterator::currentElement() {
  return (*m_current).get();
}

SceneElementContainer::iterator &WorldIterator::getIterator() {
  return m_current;
}
