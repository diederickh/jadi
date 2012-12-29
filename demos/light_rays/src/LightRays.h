#ifndef JADI_LIGHT_RAYS_H
#define JADI_LIGHT_RAYS_H
#include <jadi/Jadi.h>

/*
static const char* OCCLUSION_PASS_VS = GLSL(120,
  attribute vec4 a_pos;
);
*/

class LightRays {
 public:
  LightRays();
  ~LightRays();
  void setup();
  void beginOcclusionPass();
  void endOcclusionPass();
 private:
  void setupBuffers();
  void setupShaders();
 public:  
  GLuint vao;
  GLuint vbo;
  GLuint fbo;
  GLuint tex_occlusion;

};
#endif
