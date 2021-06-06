#include <json.hpp>
#include <filesystem>
#include <mos/gfx/material.hpp>
#include <mos/util.hpp>
#include <mos/gfx/assets.hpp>
#include <iostream>

namespace mos::gfx {

using namespace nlohmann;

Material::Material(Material::Albedo albedo,
                   Material::Metallic metallic,
                   Material::Roughness roughness,
                   Material::Emission emission,
                   Material::Ambient_occlusion ao,
                   Material::Normal normal,
                   float alpha,
                   float index_of_refraction,
                   float transmission) : albedo(albedo),
                                         metallic(metallic),
                                         roughness(roughness),
                                         emission(emission),
                                         ambient_occlusion(ao),
                                         normal(normal),
                                         alpha(alpha),
                                         index_of_refraction(index_of_refraction),
                                         transmission(transmission)
{

}

Material::Material(Assets &assets, std::string &path) : Material() {
  if (!path.empty()) {
    std::filesystem::path fpath = path;
    if (fpath.extension() == ".material") {
      auto value = json::parse(mos::text(assets.directory() + fpath.generic_string()));

      auto read_texture = [&](const std::string &name, const bool color_data = true) {
        std::string file_name{};
        if (!value[name]["texture"].is_null()) {
                  std::string temp = value[name]["texture"];
          file_name = temp;
        }
        std::string path = file_name.empty() ? "" : value[name]["texture"].get<std::string>();
        return assets.texture(path, color_data, true);
      };

      auto albedo_map = read_texture("albedo", true);
      auto albedo_value =  glm::vec3(value["albedo"]["value"][0], value["albedo"]["value"][1], value["albedo"]["value"][2]);
      albedo = {albedo_value, albedo_map};

      auto normal_map = read_texture("normal", false);
      normal = {normal_map};

      auto emission_map = read_texture("emission", true);
      auto emission_value = glm::vec3(value["emission"]["value"][0], value["emission"]["value"][1], value["emission"]["value"][2]);
      emission = {emission_value, emission_map};

      auto metallic_map = read_texture("metallic", false);
      auto metallic_value = value["metallic"]["value"];
      metallic = {metallic_value, metallic_map};

      auto roughness_map = read_texture("roughness", false);
      auto roughness_value = value["roughness"]["value"];
      roughness = {roughness_value, roughness_map};

      auto ambient_occlusion_map = read_texture("ambient_occlusion", false);
      auto ambient_occlusion_value = value["ambient_occlusion"]["value"];
      ambient_occlusion = {ambient_occlusion_value, ambient_occlusion_map};

      index_of_refraction = value["index_of_refraction"];
      transmission = value["transmission"];
      alpha = value["alpha"];
    } else {
      throw std::runtime_error(path.substr(path.find_last_of('.')) +
          " file format is not supported.");
    }
  }
}
}
