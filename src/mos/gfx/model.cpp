#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <mos/gfx/model.hpp>
#include <mos/util.hpp>
#include <mos/gfx/material.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace mos::gfx {

Model::Model(std::string name,
             Shared_mesh mesh,
             const glm::mat4 transform,
             Material material)
    : mesh(std::move(mesh)),
      material(std::move(material)),
      transform(transform),
      name_(std::move(name)) {}

auto Model::load(const nlohmann::json &json, Assets &assets,  const glm::mat4 &parent_transform) -> Model {
  auto parsed = json;
  if (parsed.is_string()) {
    spdlog::info("Loading: {}", parsed);
    std::string path = parsed;
    parsed = nlohmann::json::parse(mos::text(assets.directory() + path));
  }
  auto name = parsed.value("name", "");
  auto mesh_path = std::string("");
  if (!parsed["mesh"].is_null()) {
    mesh_path = parsed.value("mesh", "");
  }
  std::string material_path;
  if (!parsed["material"].is_null()) {
    material_path = parsed.value("material", "");
  }

  auto mesh = assets.mesh(mesh_path);
  auto transform = parent_transform * jsonarray_to_mat4(parsed["transform"]);
  auto material = Material::load(assets, material_path);

  auto model = Model(name, mesh, transform, material);
  for (auto &m : parsed["children"]) {
    const std::string str_path = m;
    std::filesystem::path path = str_path;
    if (path.extension() == ".model") {
      model.models.push_back(load(path.generic_string(), assets));
    }
  }
  return model;
}

auto Model::name() const -> std::string { return name_; }

auto Model::position() const -> glm::vec3 {
  return glm::vec3(transform[3]);
}

void Model::position(const glm::vec3 &position) {
  transform[3][0] = position[0];
  transform[3][1] = position[1];
  transform[3][2] = position[2];
}
auto Model::centroid() const -> glm::vec3 {
  if (mesh){
    return (transform * glm::translate(glm::mat4(1.0f), mesh->centroid))[3];
  }
  return transform[3];
}
auto Model::radius() const -> float {
  if (mesh){
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform, scale, rotation, translation, skew, perspective);
    return mesh->radius * glm::max(glm::max(scale.x, scale.y), scale.z);
  }
  return 0.0f;
}

void Model::emission(const glm::vec3 & emission)
{
  material.emission.value = emission;
  for (auto & model : models){
    model.emission(emission);
  }
}

void Model::alpha(const float alpha) {
  material.alpha = alpha;
  for (auto & model : models){
    model.alpha(alpha);
  }
}

void Model::ambient_occlusion(const float ambient_occlusion)
{
  material.ambient_occlusion.value = ambient_occlusion;
  for (auto & model : models){
    model.ambient_occlusion(ambient_occlusion);
  }
}

void Model::index_of_refraction(const float index_of_refraction)
{
  material.index_of_refraction = index_of_refraction;
  for (auto & model : models){
    model.index_of_refraction(index_of_refraction);
  }
}

void Model::transmission(const float transmission)
{
  material.transmission = transmission;
  for (auto & model : models){
    model.transmission(transmission);
  }
}

void Model::roughness(const float roughness)
{
  material.roughness.value = roughness;
  for (auto & model : models){
    model.roughness(roughness);
  }
}

void Model::metallic(const float metallic)
{
  material.metallic.value = metallic;
  for (auto & model : models){
    model.metallic(metallic);
  }
}

}
