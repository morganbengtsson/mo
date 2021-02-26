#pragma once

#include <glad/glad.h>

/** Uniforms for the bounding box shader program. */
struct Box_program : public Program {
  Box_program();
  GLint model_view_projection;
};
