#pragma once

#include "composite/scene_element.h"
#include "tools/tools.h"

class StochasticPdf {
 public:
  virtual ~StochasticPdf() = default;
  virtual float value(const Vec3D &direction) const = 0;
  virtual Vec3D generate() const = 0;
  virtual void setFromW(const Vec3D &w);
};

using StochasticPdfPtr = std::shared_ptr<StochasticPdf>;

class SpherePdf : public StochasticPdf {
 public:
  ~SpherePdf() override = default;
  float value(const Vec3D &direction) const override;
  Vec3D generate() const override;
};

class CosPdf : public StochasticPdf {
 public:
  CosPdf(const Vec3D &w = Vec3D());
  ~CosPdf() override = default;
  float value(const Vec3D &direction) const override;
  Vec3D generate() const override;
  void setFromW(const Vec3D &w) override;

 private:
  OrthoNormalBasis onb;
};

class PrimitivePdf : public StochasticPdf {
 public:
  PrimitivePdf(const SceneElementRawPtr elem, const Point3D &origin);
  ~PrimitivePdf() override = default;
  float value(const Vec3D &direction) const override;
  Vec3D generate() const override;

 private:
  Point3D m_origin;
  SceneElementRawPtr m_element;
};

class CombinedPdf : public StochasticPdf {
 public:
  CombinedPdf(StochasticPdfPtr pdf1, StochasticPdfPtr pdf2, float ratio = 0.5f);
  ~CombinedPdf() override = default;
  float value(const Vec3D &direction) const override;
  Vec3D generate() const override;

 private:
  StochasticPdfPtr m_firstPdf;
  StochasticPdfPtr m_secondPdf;
  float m_ratio;
};
