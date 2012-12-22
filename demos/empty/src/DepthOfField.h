#ifndef JADI_DEPTH_OF_FIELDH
#define JADI_DEPTH_OF_FIELDH

#include <jadi/Jadi.h>
#include <vector>

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
                                        gl_FragColor.rgb = vec3(1.0, 0.0, 0.0);
                                      }
                                      );

struct Particle {
  Vec3 position;
  Vec3 velocity;
  Vec3 forces;
};

class DepthOfField {
 public:
  DepthOfField();
  ~DepthOfField();
  void setup();
  void update();
  void draw(const float* pm, const float* vm, const float* nm);
 private:
  void setupShaders();
  void setupBuffers();
  
 public:
  std::vector<Particle> particles;
  GLuint debug_prog;
  GLuint vbo;
  GLuint vao;
  size_t num_vertices;
};
#endif
