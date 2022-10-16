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

class StandardSphere : public SceneDirector {
 public:
  StandardSphere(std::shared_ptr<TraceableBuilder> builder,
                 const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class SphereWithCheckerPattern : public SceneDirector {
 public:
  SphereWithCheckerPattern(std::shared_ptr<TraceableBuilder> builder,
                           const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class SphereWithRingPattern : public SceneDirector {
 public:
  SphereWithRingPattern(std::shared_ptr<TraceableBuilder> builder,
                        const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class SphereWithGradientPattern : public SceneDirector {
 public:
  SphereWithGradientPattern(std::shared_ptr<TraceableBuilder> builder,
                            const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class StandardPlane : public SceneDirector {
 public:
  StandardPlane(std::shared_ptr<TraceableBuilder> builder,
                const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class PlaneWithCheckerPattern : public SceneDirector {
 public:
  PlaneWithCheckerPattern(std::shared_ptr<TraceableBuilder> builder,
                          const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};

class PlaneWithRingPattern : public SceneDirector {
 public:
  PlaneWithRingPattern(std::shared_ptr<TraceableBuilder> builder,
                       const Properties &prop);
  std::shared_ptr<Traceable> create() override;
};
