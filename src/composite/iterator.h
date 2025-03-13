#pragma once

#include "composite/scene_element.h"

class WorldIterator {
 public:
  WorldIterator(const SceneElementContainer& world);
  bool first();
  SceneElementContainer::const_iterator begin() const;
  SceneElementContainer::const_iterator end() const;
  void advance();
  bool notDone();
  SceneElementRawPtr currentElement();
  SceneElementContainer::iterator& getIterator();

 private:
  SceneElementContainer::iterator m_current;
  SceneElementContainer m_sceneElementContainer;
};
