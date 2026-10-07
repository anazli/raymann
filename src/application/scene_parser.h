#pragma once

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "application/config_parser.h"
#include "application/parameters.h"
#include "application/wavefront_reader.h"
#include "camera/camera.h"
#include "color/light.h"
#include "composite/builder.h"
#include "composite/scene_element.h"
#include "materials/dielectric.h"
#include "materials/emissive.h"
#include "materials/isotropic.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/standard.h"
#include "textures/texture.h"

using json = nlohmann::json;

struct SceneData {
  SceneElementPtr world;
  PointLight light;
};

class SceneParser {
 public:
  static SceneData createFromJSON(const std::string& path) {
    json j = JsonLoader::load(path);

    // Parse light settings
    auto l = j["light"];
    auto light_pos =
        Point3f(l["position"][0], l["position"][1], l["position"][2]);
    auto light_intensity =
        Vec3f(l["intensity"][0], l["intensity"][1], l["intensity"][2]);
    PointLight light(light_pos, light_intensity);

    // Create scene components
    SceneDirector scene_director;
    EntityFactory entity_factory;
    WorldBuilder world_builder;

    // Parse scene elements
    auto elements = j["elements"];
    for (const auto& elem : elements) {
      DataContainer data;

      // Parse primitive type
      std::string prim_type = elem["primitive"];
      App primitive_type = getPrimitiveType(prim_type);
      data.setProperty(App::PRIMITIVE_TYPE, primitive_type);

      // Parse material type
      std::string mat_type = elem["material"];
      App material_type = getMaterialType(mat_type);
      data.setProperty(App::MATERIAL_TYPE, material_type);

      // Parse texture type
      std::string tex_type = elem.value("texture", "constant");
      App texture_type = getTextureType(tex_type);
      data.setProperty(App::TEXTURE_TYPE, texture_type);

      // Parse color/intensity
      if (elem.contains("color")) {
        auto color = elem["color"];
        data.setProperty(App::COLOR, Vec3f(color[0], color[1], color[2]));
      }

      // Parse material-specific properties
      if (elem.contains("refractive_index")) {
        data.setProperty(App::REFRACTIVE_INDEX,
                         elem["refractive_index"].get<float>());
      }
      if (elem.contains("fuzz")) {
        data.setProperty(App::FUZZ, elem["fuzz"].get<float>());
      }

      // Parse transformation matrix
      if (elem.contains("transform")) {
        auto trans = elem["transform"];
        Mat4f matrix = parseTransformation(trans);
        data.setProperty(App::TRANSFORMATION_MATRIX, matrix);
      }

      // Parse quad-specific properties
      if (primitive_type == App::QUAD) {
        if (elem.contains("origin")) {
          auto origin = elem["origin"];
          data.setProperty(App::QUAD_ORIGIN,
                           Point3f(origin[0], origin[1], origin[2]));
        }
        if (elem.contains("u_axis")) {
          auto u = elem["u_axis"];
          data.setProperty(App::QUAD_UAXIS, Vec3f(u[0], u[1], u[2]));
        }
        if (elem.contains("v_axis")) {
          auto v = elem["v_axis"];
          data.setProperty(App::QUAD_VAXIS, Vec3f(v[0], v[1], v[2]));
        }
      }

      // Create the scene element
      scene_director.createSceneElement(entity_factory, data);
    }

    // Build world
    scene_director.createWorld(world_builder, light);
    auto world = scene_director.getSceneProduct();

    return SceneData{world, light};
  }

  static SceneData createFromConfig(const std::string& config_path) {
    const json config = JsonLoader::load(config_path);
    const auto scene_config = config.value("scene", json{});

    if (scene_config.empty()) {
      const auto config_directory =
          std::filesystem::path(config_path).parent_path();
      return createFromJSON((config_directory / "scene.json").string());
    }

    const std::string type = scene_config.value("type", "json");
    const auto config_directory =
        std::filesystem::path(config_path).parent_path();
    if (type == "json") {
      const auto scene_path = scene_config.value("file", "scene.json");
      return createFromJSON(
          (config_directory / scene_path).lexically_normal().string());
    }

    if (type == "wavefront") {
      const auto model_path =
          config_directory / scene_config.value("file", "model.obj");
      WavefrontReader reader(model_path);

      const auto light_config = scene_config.value("light", json::object());
      const auto position =
          light_config.value("position", json{0.f, 4.f, -4.f});
      const auto intensity =
          light_config.value("intensity", json{1.f, 1.f, 1.f});
      const PointLight light(
          Point3f(position[0].get<float>(), position[1].get<float>(),
                  position[2].get<float>()),
          Vec3f(intensity[0].get<float>(), intensity[1].get<float>(),
                intensity[2].get<float>()));
      const auto material_config =
          scene_config.value("material", json::object());
      const auto material = createWavefrontMaterial(material_config);

      reader.addMaterial(std::move(material));
      reader.addLightForModel(light);
      reader.parseInput();
      return SceneData{reader.getStructure(), light};
    }

    throw std::runtime_error("Unsupported scene type: " + type);
  }

 private:
  static TexturePtr createWavefrontTexture(const json& material_config) {
    const auto color = material_config.value("color", json{0.7f, 0.7f, 0.7f});
    const auto texture_type =
        getTextureType(material_config.value("texture", "constant"));

    if (texture_type == App::PERLIN_TEXTURE) {
      const auto scale = material_config.value("scale", 0.5f);
      return PerlinTexture::create(
          scale, Vec3f(color[0].get<float>(), color[1].get<float>(),
                       color[2].get<float>()));
    }
    return ConstantTexture::create(Vec3f(
        color[0].get<float>(), color[1].get<float>(), color[2].get<float>()));
  }

  static MaterialPtr createWavefrontMaterial(const json& material_config) {
    auto texture = createWavefrontTexture(material_config);
    const auto material_type =
        getMaterialType(material_config.value("type", "lambertian"));

    switch (material_type) {
      case App::LAMBERTIAN:
        return Lambertian::create(std::move(texture));
      case App::METAL:
        return Metal::create(std::move(texture),
                             material_config.value("fuzz", 0.2f));
      case App::DIELECTRIC:
        return Dielectric::create(
            std::move(texture), material_config.value("refractive_index", 1.f));
      case App::DIFFUSE_LIGHT:
        return EmissiveMaterial::create(std::move(texture));
      case App::ISOTROPIC:
        return Isotropic::create(std::move(texture));
      case App::STANDARD: {
        DataContainer properties;
        const auto property_names = {
            std::pair<App, const char*>(App::AMBIENT, "ambient"),
            std::pair<App, const char*>(App::DIFFUSE, "diffuse"),
            std::pair<App, const char*>(App::SPECULAR, "specular"),
            std::pair<App, const char*>(App::SHININESS, "shininess"),
            std::pair<App, const char*>(App::REFLECTION, "reflection"),
            std::pair<App, const char*>(App::TRANSPARENCY, "transparency"),
            std::pair<App, const char*>(App::REFRACTIVE_INDEX,
                                        "refractive_index")};
        for (const auto& [property, name] : property_names) {
          if (material_config.contains(name)) {
            properties.setProperty(property,
                                   material_config[name].get<float>());
          }
        }
        return StandardMaterial::create(std::move(texture), properties);
      }
      default:
        throw std::runtime_error("Unsupported Wavefront material type: " +
                                 material_config.value("type", "lambertian"));
    }
  }

  static App getPrimitiveType(const std::string& type) {
    if (type == "cube") return App::CUBE;
    if (type == "sphere") return App::SPHERE;
    if (type == "quad") return App::QUAD;
    if (type == "plane") return App::PLANE;
    if (type == "cone") return App::CONE;
    if (type == "cylinder") return App::CYLINDER;
    return App::CUBE;
  }

  static App getMaterialType(const std::string& type) {
    if (type == "lambertian") return App::LAMBERTIAN;
    if (type == "metal") return App::METAL;
    if (type == "dielectric") return App::DIELECTRIC;
    if (type == "diffuse_light") return App::DIFFUSE_LIGHT;
    if (type == "standard") return App::STANDARD;
    if (type == "isotropic") return App::ISOTROPIC;
    return App::LAMBERTIAN;
  }

  static App getTextureType(const std::string& type) {
    if (type == "constant") return App::CONSTANT_TEXTURE;
    if (type == "perlin") return App::PERLIN_TEXTURE;
    return App::CONSTANT_TEXTURE;
  }

  static Mat4f parseTransformation(const json& trans) {
    Mat4f result;
    bool has_transform = false;

    // Apply transformations in order: translate, scale, rotate
    // Using right multiplication (result * new_transform) to match original
    // code order
    if (trans.contains("translate")) {
      auto t = trans["translate"];
      if (!has_transform) {
        result = translation(t[0].get<float>(), t[1].get<float>(),
                             t[2].get<float>());
        has_transform = true;
      } else {
        result = result * translation(t[0].get<float>(), t[1].get<float>(),
                                      t[2].get<float>());
      }
    }

    if (trans.contains("scale")) {
      auto s = trans["scale"];
      if (!has_transform) {
        result = scale(s[0].get<float>(), s[1].get<float>(), s[2].get<float>());
        has_transform = true;
      } else {
        result = result *
                 scale(s[0].get<float>(), s[1].get<float>(), s[2].get<float>());
      }
    }

    if (trans.contains("rotate_x")) {
      if (!has_transform) {
        result = rotationOverX(trans["rotate_x"].get<float>());
        has_transform = true;
      } else {
        result = result * rotationOverX(trans["rotate_x"].get<float>());
      }
    }
    if (trans.contains("rotate_y")) {
      if (!has_transform) {
        result = rotationOverY(trans["rotate_y"].get<float>());
        has_transform = true;
      } else {
        result = result * rotationOverY(trans["rotate_y"].get<float>());
      }
    }
    if (trans.contains("rotate_z")) {
      if (!has_transform) {
        result = rotationOverZ(trans["rotate_z"].get<float>());
        has_transform = true;
      } else {
        result = result * rotationOverZ(trans["rotate_z"].get<float>());
      }
    }

    return result;
  }
};
