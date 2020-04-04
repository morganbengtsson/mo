#include <glm/gtx/intersect.hpp>
#include <glm/gtx/normal.hpp>
#include <mos/sim/navmesh.hpp>
#include <mos/util.hpp>

namespace mos {
namespace sim {

Navmesh::Navmesh(const nlohmann::json &json, gfx::Assets &assets, const glm::mat4 &parent_transform) {
  auto parsed = json;
  if (parsed.is_string()) {
    std::cout << "Loading Nav_model: " << parsed << std::endl;
        std::string path = parsed;
    parsed = nlohmann::json::parse(mos::text(assets.directory() + path));
  }
  auto mesh_path = std::string("");
  if (!parsed["mesh"].is_null()) {
    mesh_path = parsed.value("mesh", "");
  }
  mesh = assets.mesh(mesh_path);

  //TODO:
  //transform = parent_transform * jsonarray_to_mat4(parsed["transform"]);
}

Navmesh::Optional_vertex
Navmesh::intersects(const glm::vec3 &origin, const glm::vec3 &direction) {
  for (size_t i = 0; i < mesh->triangles.size(); i++) {
    Face face(mesh->vertices[mesh->triangles[i][0]], mesh->vertices[mesh->triangles[i][1]], mesh->vertices[mesh->triangles[i][2]]);
    auto intersection = face.intersects(origin, direction);
    if (intersection) {
      return intersection;
    }
  }
  return std::optional<gfx::Vertex>();
}

Navmesh::Optional_vertex
Navmesh::closest_intersection(const glm::vec3 &origin,
                               const glm::vec3 &direction) {
  Optional_vertex closest;
  for (size_t i = 0; i < mesh->triangles.size(); i++) {
    Face face(mesh->vertices[mesh->triangles[i][0]], mesh->vertices[mesh->triangles[i][1]], mesh->vertices[mesh->triangles[i][2]]);
    auto intersection = face.intersects(origin, direction);
    if (intersection) {
      auto distance = glm::distance(origin, intersection->position);
      if (!closest ||
          distance < glm::distance(origin, closest->position)) {
        closest = intersection;
      }
    }
  }
  return closest;
}

void Navmesh::calculate_normals() {
  for (size_t i = 0; i < mesh->triangles.size(); i++) {
    //TODO: Generalize
    auto &v0 = mesh->vertices[mesh->triangles[i][0]];
    auto &v1 = mesh->vertices[mesh->triangles[i][1]];
    auto &v2 = mesh->vertices[mesh->triangles[i][2]];

    auto normal = glm::triangleNormal(v0.position, v1.position, v2.position);
    v0.normal = normal;
    v1.normal = normal;
    v2.normal = normal;
  }
}

Navmesh::Face::Face(gfx::Vertex &v0, gfx::Vertex &v1, gfx::Vertex &v2)
    : v0_(v0), v1_(v1), v2_(v2) {}

Navmesh::Optional_vertex
Navmesh::Face::intersects(const glm::vec3 &origin, const glm::vec3 &direction) {
  glm::vec2 bary;
  float distance;
  auto intersects =
      glm::intersectRayTriangle(origin, direction, v0_.position, v1_.position, v2_.position, bary, distance);

  if (intersects) {
    auto p = origin + direction * distance;
    auto n = glm::triangleNormal(v0_.position, v1_.position, v2_.position);
    auto t = glm::normalize(v0_.position - v1_.position); // Is this correct?

    //Document this?
    auto a = bary.x / (bary.x + bary.y + distance);
    auto b = bary.y / (bary.x + bary.y + distance);
    auto c = distance / (bary.x + bary.y + distance);

    //n = a * v0_.normal + b * v1_.normal + c * v2_.normal;

    n = glm::normalize(n);

    //auto uv = (v0_.uv + v1_.uv + v2_.uv) / 3.0f;
    auto uv = a * v0_.uv + b * v1_.uv + c * v2_.uv;

    //auto uv_l = bary.x * v0_.uv_lightmap + bary.y * v1_.uv_lightmap + bary.z * v2_.uv_lightmap;
    auto v = gfx::Vertex(p, n, t, uv);
    return std::optional<gfx::Vertex>(v);
  }
  return std::optional<gfx::Vertex>();
}
}
}
