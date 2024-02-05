#pragma once

#include "composite/world.h"

class BaseIterator {
 public:
  virtual ~BaseIterator() = default;
  virtual bool first() = 0;
  virtual SceneElementContainer::const_iterator begin() const = 0;
  virtual SceneElementContainer::const_iterator end() const = 0;
  virtual void advance() = 0;
  virtual bool notDone() = 0;
  virtual SceneElementRawPtr currentElement() = 0;

 protected:
};

class WorldIterator : public BaseIterator {
 public:
  WorldIterator(const SceneElementContainer &world);
  bool first() override;
  SceneElementContainer::const_iterator begin() const override;
  SceneElementContainer::const_iterator end() const override;
  void advance() override;
  bool notDone() override;
  SceneElementRawPtr currentElement() override;

 private:
  SceneElementContainer::const_iterator m_current;
  SceneElementContainer m_sceneElementContainer;
};
