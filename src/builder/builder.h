#pragma once

#include "composite/properties.h"
#include "composite/traceable.h"

class TraceableBuilder {
 public:
  virtual ~TraceableBuilder() {}
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
  // TODO: this member is probably not needed. Properties are passed through the
  // director. Either directly with a constructor here (DI), or remove
  // member/setter and pass it as parameter to the methods. The first makes more
  // sense (with interface class).
  // TODO: Remove all these methods for adding patterns and find better solution
};

typedef std::shared_ptr<TraceableBuilder> TraceableBuilderPtr;
