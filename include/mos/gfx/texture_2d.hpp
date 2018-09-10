#pragma once

#include <atomic>
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <mos/gfx/texture.hpp>

namespace mos {
namespace gfx {

class Texture_2D;
using Shared_texture_2D = std::shared_ptr<Texture_2D>;

/** Texture in two dimension. Contains chars as data. */
class Texture_2D final : public Texture {
public:
  template<class T>
  Texture_2D(T begin, T end, unsigned int width, unsigned int height,
            const Format &format = Format::SRGBA, const Wrap &wrap = Wrap::REPEAT,
            const bool mipmaps = true)
      : Texture({Data(begin, end)}, width, height, format, wrap, mipmaps) {}

   Texture_2D(unsigned int width, unsigned int height,
            const Format &format = Format::SRGBA,
             const Wrap &wrap = Wrap::REPEAT,
            bool mipmaps = true);

  virtual ~Texture_2D();

  /** Load from file */
  static Shared_texture_2D load(const std::string &path,
                              bool color_data = true,
                              bool mipmaps = true,
                              const Wrap &wrap = Wrap::REPEAT);

  /** Create from file. */
  Texture_2D(const std::string &path,
            bool color_data = true,
            bool mipmaps = true,
            const Texture::Wrap &wrap = Texture::Wrap::REPEAT);
};
}
}
