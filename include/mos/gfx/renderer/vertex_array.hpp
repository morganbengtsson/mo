#pragma once

#include <mos/gfx/renderer/buffer.hpp>
#include <mos/gfx/cloud.hpp>
#include <mos/gfx/mesh.hpp>

class Vertex_array {
public:
  explicit Vertex_array(const Cloud &cloud,
                        std::unordered_map<unsigned int, Buffer> &array_buffers);
  explicit Vertex_array(const Mesh & mesh,
                        std::unordered_map<unsigned int, Buffer> &array_buffers,
                        std::unordered_map<unsigned int, Buffer> &element_array_buffers);
  ~Vertex_array();
  Vertex_array(Vertex_array &&array) noexcept;
  Vertex_array(const Vertex_array &target) = delete;
  Vertex_array & operator=(const Vertex_array &array) = delete;
  Vertex_array & operator=(Vertex_array &&target) noexcept;
  GLuint id{0};
private:
  void release();
};
