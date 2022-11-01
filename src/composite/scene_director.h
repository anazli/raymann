#pragma once

#include <memory>

#include "builder/shape_builder.h"

class SceneDirector {
 public:
  virtual std::shared_ptr<Traceable> create(
      std::shared_ptr<TraceableBuilder> builder, const Properties &prop) = 0;
};

typedef std::shared_ptr<SceneDirector> SceneDirectorPtr;

class StandardSphere : public SceneDirector {
 public:
  std::shared_ptr<Traceable> create(std::shared_ptr<TraceableBuilder> builder,
                                    const Properties &prop) override;
};

class CheckerSphere : public SceneDirector {
 public:
  std::shared_ptr<Traceable> create(std::shared_ptr<TraceableBuilder> builder,
                                    const Properties &prop) override;
};

class RingSphere : public SceneDirector {
 public:
  std::shared_ptr<Traceable> create(std::shared_ptr<TraceableBuilder> builder,
                                    const Properties &prop) override;
};

class GradientSphere : public SceneDirector {
 public:
  std::shared_ptr<Traceable> create(std::shared_ptr<TraceableBuilder> builder,
                                    const Properties &prop) override;
};

class StandardPlane : public SceneDirector {
 public:
  std::shared_ptr<Traceable> create(std::shared_ptr<TraceableBuilder> builder,
                                    const Properties &prop) override;
};

class CheckerPlane : public SceneDirector {
 public:
  std::shared_ptr<Traceable> create(std::shared_ptr<TraceableBuilder> builder,
                                    const Properties &prop) override;
};

class RingPlane : public SceneDirector {
 public:
  std::shared_ptr<Traceable> create(std::shared_ptr<TraceableBuilder> builder,
                                    const Properties &prop) override;
};
