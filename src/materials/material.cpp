#include "materials/material.h"

Material::Material(TexturePtr tex, const MaterialProperties& prop)
    : BaseMaterial(tex, prop) {}
