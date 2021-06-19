#include <json.hpp>
#include <mos/gfx/font.hpp>
#include <mos/util.hpp>
#include <filesystem>

namespace mos::gfx {
using namespace nlohmann;
Font::Font(Font::Char_map characters,
           Shared_texture_2D texture,
           const float height,
           const float base)
    : texture(std::move(texture)),
      characters(std::move(characters)),
      height_(height),
      base_(base) {
  texture->wrap = Texture_2D::Wrap::Clamp;
}

Font::Font(const std::string &path) {
  std::filesystem::path fpath = path;
  auto doc = json::parse(text(fpath.generic_string()));
  for (auto &c : doc["symbols"]) {
    Character character{
        .id = c["id"],
        .position = {c["x"], c["y"]},
        .offset = {c["xoffset"], c["yoffset"]},
        .advance = c["xadvance"],
        .width = c["width"],
        .height = c["height"],
    };
    characters.insert(Char_pair(character.id, character));
  }
  base_ = doc["config"]["base"];
  height_ = doc["config"]["charHeight"];
  std::string texture_name = doc["config"]["textureFile"];
  texture = Texture_2D::load(fpath.parent_path().generic_string() + "/" + texture_name);
}

auto Font::height() const -> float { return height_; }

auto Font::base() const -> float { return base_; }
}
