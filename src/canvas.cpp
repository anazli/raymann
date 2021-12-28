#include "canvas.h"

#include <fstream>

using std::ofstream;

void Canvas::render() const {
  ofstream out;
  out.open(fileName());
  for (int i = 0; i < width(); ++i) {
    for (int j = 0; j < height(); ++j) {
    }
  }
  out.close();
}
