#pragma once

#include "composite/properties.h"
#include "composite/traceable.h"

class TraceableBuilder {
 public:
  ~TraceableBuilder() {}
  virtual void addTraceable(Traceable* tr) {}
  virtual void addTransformer() {}
  virtual void addMaterial() {}
  virtual void addStripePattern() {}
  virtual void addGradientPattern() {}
  virtual void addRingPattern() {}
  virtual void addCheckerPattern() {}
  virtual std::shared_ptr<Traceable> getTraceable() const { return nullptr; };

  void setProperties(const Properties& prop) { m_prop = prop; }

 protected:
  Properties m_prop;
};

typedef std::shared_ptr<TraceableBuilder> TraceableBuilderPtr;
