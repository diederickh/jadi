#ifndef JADI_DEPTH_OF_FIELDH
#define JADI_DEPTH_OF_FIELDH

#include <jadi/Jadi.h>
#include <vector>
#include "Particles.h"

static const char* DOF_DEBUG_VS = GLSL(120, 
  uniform mat4 u_pm;
  uniform mat4 u_vm;
  uniform mat4 u_mm;
  attribute vec4 a_pos;
  attribute vec3 a_norm;
  varying vec3 v_norm;
  
  void main() {
    v_norm = a_norm;
    gl_Position = u_pm * u_vm * u_mm * a_pos;
  }
);

static const char* DOF_DEBUG_FS = GLSL(120, 
  varying vec3 v_norm;
  void main() {
    gl_FragColor.a = 1.0;
    gl_FragColor.rgb = v_norm;
  }
);

static const char* DOF_DEPTH_VS = GLSL(120,
  uniform mat4 u_pm;
  uniform mat4 u_vm;
  uniform mat4 u_mm;
  attribute vec4 a_pos;
  attribute vec3 a_norm;
  varying vec4 v_pos;
  varying vec3 v_norm;
  const float n = 0.1;
  const float f = 500.0f;

  void main() {
    gl_Position = u_pm * u_vm * u_mm * a_pos;
    v_pos = gl_Position;
    v_norm = a_norm;
  }
);

static const char* DOF_DEPTH_FS = GLSL(120,
  varying vec3 v_norm;
  varying vec4 v_pos;
  void main() {
    gl_FragColor.a = 1.0;
    gl_FragColor.rgb = vec3(v_pos.z, v_pos.z, v_pos.z);
  }
);

static const char* DOF_IMAGE_VS = GLSL(120,
  attribute vec4 a_pos;
  attribute vec2 a_tex;
  varying vec2 v_tex;
  void main() {
    gl_Position = a_pos;
    v_tex = a_tex;
  }
);

static const char* DOF_IMAGE_FS = GLSL(120,
  uniform sampler2D u_tex;
  varying vec2 v_tex;
  void main() {
    vec4 col = texture2D(u_tex, v_tex);
    gl_FragColor = col;
  }
);

class DepthOfField {
 public:
  DepthOfField();
  ~DepthOfField();
  void setup(int w, int h);
  void update();
  void beginDepthPass();
  void endDepthPass();
  void debugDraw();
  void draw(const float* pm, const float* vm, const float* nm, std::vector<Particle>& particles);
 private:
  void setupShaders();
  void setupBuffers();
  void setupFBO();
  GLuint createProgram(const char* vs, const char* fs);
  GLuint createTexture(int w, int h, GLenum iformat, GLenum eformat);
  void drawTexture(GLuint tex, int x, int y, int w, int h);
 public:

  int fbo_w;
  int fbo_h;
  Mat4 mm;

  GLuint debug_prog;
  GLuint depth_prog;
  GLuint vbo;
  GLuint vao;
  size_t num_vertices;
  GLuint fbo;
  GLuint depth_rbo;
  GLuint depth_tex;

  GLuint image_prog;
  GLuint image_vao;
  GLuint image_vbo;
};
#endif
