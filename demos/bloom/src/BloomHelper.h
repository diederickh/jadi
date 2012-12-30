#ifndef JADI_BLOOM_HELPER_H
#define JADI_BLOOM_HELPER_H

#include <jadi/Jadi.h>
#include "Particles.h"

static const char* BH_BASIC_VS = GLSL(120,
  uniform mat4 u_pm;
  uniform mat4 u_vm;
  uniform mat4 u_mm;
  attribute vec4 a_pos;
  attribute vec3 a_norm;
  attribute vec2 a_tex;
  varying vec3 v_norm;
  varying vec2 v_tex;
  void main() {
    v_norm = a_norm;
    v_tex = a_tex;
    gl_Position = u_pm * u_vm * u_mm * a_pos;
  }
);

static const char* BH_BASIC_FS = GLSL(120,
  uniform sampler2D u_tex;
  varying vec3 v_norm;
  varying vec2 v_tex;
  void main() {
    gl_FragColor = vec4(0.5 + v_norm * 0.5, 1.0);
    gl_FragColor = texture2D(u_tex, v_tex);
    gl_FragColor.a = 1.0;
  }
);

class BloomHelper {
 public:
  BloomHelper();
  ~BloomHelper();
  void setup();
  void drawBloomableAreas(const float* pm, const float* vm, std::vector<Particle>& particles);
  void drawShaded(const float* pm, const float* vm, std::vector<Particle>& particles);

 private:
  void draw(const float* pm, const float* vm, std::vector<Particle>& particles, GLuint tex);
  void setupBuffers();
  void setupShaders();

 private:
  GLuint prog;
  GLuint vao;
  GLuint vbo;
  size_t num_vertices;
  GLuint tex_bloom;
  GLuint tex_shaded;
};

#endif

