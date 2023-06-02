#pragma once

#include "materials/material_properties.h"
#include "textures/texture.h"

class BaseMaterial {
 public:
  virtual void setTexture(TexturePtr tex) {}
  virtual TexturePtr getTexture() const { return nullptr; }
  virtual void setProperties(const MaterialProperties& prop) {}
  virtual MaterialProperties getProperties() const {
    return MaterialProperties();
  }

 protected:
  BaseMaterial(TexturePtr tex,
               const MaterialProperties& prop = MaterialProperties())
      : m_tex(tex), m_prop(prop) {}
  TexturePtr m_tex;
  MaterialProperties m_prop;
};

typedef std::shared_ptr<BaseMaterial> BaseMaterialPtr;

class Material : public BaseMaterial {
 public:
  Material(TexturePtr tex,
           const MaterialProperties& prop = MaterialProperties());
  void setTexture(TexturePtr tex) override { m_tex = tex; }
  TexturePtr getTexture() const override { return m_tex; }
  void setProperties(const MaterialProperties& prop) override { m_prop = prop; }
  MaterialProperties getProperties() const override { return m_prop; }
};
