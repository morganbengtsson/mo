#include <iostream>
#include <lodepng.h>
#include <mos/render/texture_2d.hpp>

namespace mos {

Texture2D::Texture2D(const unsigned int width, const unsigned int height,
                     const Format &format, const Wrap &wrap,
                     const bool mipmaps, const bool compress)
    : Texture(width, height, format, wrap, mipmaps, compress) {}

Texture2D::~Texture2D() {}

SharedTexture Texture2D::load(const std::string &path, const bool mipmaps,
                              const bool compress, const Texture2D::Wrap &wrap) {
  if (path.empty() || path.back() == '/') {
    return SharedTexture();
  }
  return std::make_shared<Texture2D>(path, mipmaps, compress, wrap);
}

Texture2D::Texture2D(const std::string &path, const bool mipmaps,
                     const bool compress, const Texture2D::Wrap &wrap)
    : Texture({path}, Format::SRGBA, wrap, mipmaps, compress) {}
}
