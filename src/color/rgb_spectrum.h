#pragma once
#include "color/spectrum.h"

class RGBSpectrum : public Spectrum {
 public:
  RGBSpectrum(float value = 0.f, int samplesSize = 60);

 private:
  static constexpr float m_lowerLim = 400.f;
  static constexpr float m_upperLim = 700.f;
};
