#pragma once

#include <memory>

#include "builder/shape_builder.h"

class SceneDirector {
 public:
  virtual std::shared_ptr<Traceable> create() = 0;

 protected:
  SceneDirector();
  SceneDirector(std::shared_ptr<TraceableBuilder> builder,
                const Properties &prop);
  std::shared_ptr<TraceableBuilder> m_builder;
  Properties m_properties;
};

typedef std::shared_ptr<SceneDirector> SceneDirectorPtr;

class StandardSphere : public SceneDirector {
 public:
  StandardSphere(std::shared_ptr<TraceableBuilder> builder,
                 const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class CheckerSphere : public SceneDirector {
 public:
  CheckerSphere(std::shared_ptr<TraceableBuilder> builder,
                const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class RingSphere : public SceneDirector {
 public:
  RingSphere(std::shared_ptr<TraceableBuilder> builder, const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class GradientSphere : public SceneDirector {
 public:
  GradientSphere(std::shared_ptr<TraceableBuilder> builder,
                 const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class StandardPlane : public SceneDirector {
 public:
  StandardPlane(std::shared_ptr<TraceableBuilder> builder,
                const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class CheckerPlane : public SceneDirector {
 public:
  CheckerPlane(std::shared_ptr<TraceableBuilder> builder,
               const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class RingPlane : public SceneDirector {
 public:
  RingPlane(std::shared_ptr<TraceableBuilder> builder, const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};
