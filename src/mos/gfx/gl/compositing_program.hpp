#pragma once

#include <glad/glad.h>

struct Compositing_program : public Program {
  Compositing_program();
  GLint color_sampler;
  GLint bloom_sampler;
  GLint bloom_strength;
};
