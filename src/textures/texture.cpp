#include "textures/texture.h"

TexturePtr ConstantTexture::create(const Vec3D& color) {
  return std::make_unique<ConstantTexture>(color);
}

TexturePtr CheckerTexture::create(TexturePtr t1, TexturePtr t2) {
  return std::make_unique<CheckerTexture>(std::move(t1), std::move(t2));
}

TexturePtr PerlinTexture::create(float scale, const Vec3D& color) {
  return std::make_unique<PerlinTexture>(scale, color);
}
