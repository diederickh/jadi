#ifndef JADI_LIGHT_RAYS_H
#define JADI_LIGHT_RAYS_H
#include <jadi/Jadi.h>

//static const char* OCCLUSION_PASS_VS = GLSL(140

class LightRays {
 public:
  LightRays();
  ~LightRays();
  void setup();
 private:
  void setupBuffers();
  void setupShaders();
 public:  
  GLuint vao;
  GLuint vbo;
  GLuint fbo;


};
#endif
