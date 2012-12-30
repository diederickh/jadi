#ifndef JADI_BLOOM_H
#define JADI_BLOOM_H
#include <jadi/Jadi.h>

class Bloom {
 public:
  Bloom();
  ~Bloom();
  void setup(int w, int h);
  void debugDraw();
 private:
  void setupBuffers();
  void setupShaders();
  GLuint createShader(const char* vs, const char* fs);
  void drawTexture(GLuint tex, int x, int y, int w, int h);
 public:  
  GLuint image_prog;
  GLuint bloom_prog;
  GLuint vao;
  GLuint vbo;
  GLuint fbo;
  /*
  GLuint tex_rays; // attachment 0
  GLuint tex_occlusion; // attachment 1
  GLuint tex_shaded; // attachment 2
  GLuint depth_rbo;
  */
  int fbo_w;
  int fbo_h;


};
#endif
