#pragma once

#include <mat3.h>

#include <fstream>
#include <nlohmann/json.hpp>

#include "container/canvas.h"
#include "renderers/path_tracer.h"
#include "renderers/phong_model.h"
#include "stochastic/stochastic_method.h"

using json = nlohmann::json;

namespace JsonLoader {
inline json load(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) throw std::runtime_error("Could not open file: " + path);
  return json::parse(file);
}
}  // namespace JsonLoader

class ConfigParser {
 public:
  static Canvas createFromJSON(const std::string& path) {
    json j = JsonLoader::load(path);

    // Configure Camera
    auto c = j["camera"];
    auto from = Point3f(c["from"][0], c["from"][1], c["from"][2]);
    auto to = Point3f(c["to"][0], c["to"][1], c["to"][2]);
    auto up = Vec3f(c["up"][0], c["up"][1], c["up"][2]);
    auto fov = c["fov"];
    auto cnv = j["canvas"];
    auto width = cnv["width"];
    auto height = cnv["height"];
    auto camera = Camera(width, height, fov);
    camera.setTransform(view_transform(from, to, up));

    // Configure renderer
    auto r = j["render_settings"];
    auto samples_per_pixel = r["samples"];
    auto material_depth = r["depth"];
    BaseRendererPtr renderer;
    if (r["method"] == "PhongModel") {
      renderer = std::make_unique<PhongModel>();
    } else if (r["method"] == "PathTracer") {
      renderer =
          std::make_unique<PathTracer>(std::make_unique<BruteForceSampler>(
              camera, samples_per_pixel, material_depth));
    }
    auto background_color =
        Vec3f(r["background"][0], r["background"][1], r["background"][2]);
    renderer->setBackgroundColor(background_color);

    // Get number of threads (default to 4 if not specified)
    int num_threads = cnv.value("threads", 4);

    // Configure canvas
    auto canvas = Canvas(std::move(camera), std::move(renderer), num_threads);
    canvas.setFileName(cnv["output"]);
    return canvas;
  }
};