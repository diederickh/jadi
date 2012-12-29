#ifndef JADI_LIGHT_RAY_HELPER_H
#define JADI_LIGHT_RAY_HELPER_H

/*
 This class is used in combination with the LigthRays class. 
 Lights rays need a couple of passes. One of these passes is to
 render the occluding objects using a solid color. For this pass
 you don't need any heavy light calculations, just a simple pass through 
 shader is enough. This class is used to:

 - render the light from which we make 'rays' + occluding objects 
 - render the full 'colored/shaded' scene

 */

#include <jadi/Jadi.h>
#include <vector>
#include "Particles.h"

static const char* LRH_SHARED_VS = GLSL(120,
  uniform mat4 u_pm;
  uniform mat4 u_vm;
  uniform mat4 u_mm;
  attribute vec4 a_pos;
  attribute vec3 a_norm;
  varying vec3 v_norm;
  void main() {
    gl_Position = u_pm * u_vm * u_mm * a_pos;
    v_norm = a_norm;
  }
);

static const char* LRH_OCCLUSION_FS = GLSL(120,
  uniform vec3 u_color;
  varying vec3 v_norm;
  void main() {
    gl_FragColor = vec4(u_color, 1.0);
  }
);

static const char* LRH_FINAL_FS = GLSL(120,
  varying vec3 v_norm;
  void main() {
    gl_FragColor.a = 1.0;
    gl_FragColor.rgb = 0.5 + v_norm * 0.5;
  }
);


class LightRayHelper {
 public:
  LightRayHelper();
  ~LightRayHelper();
  void setup();
  void drawOcclusion(const float* pm, const float* vm, std::vector<Particle>& particles);
  void drawLights(const float* pm, const float* vm, std::vector<Particle>& particles);
  void drawShadedParticles(const float *pm, const float* vm, std::vector<Particle>& particles);
 private:
  void drawColoredParticles(const float *pm, const float* vm, std::vector<Particle>& particles, float* col);
  void setupBuffers();
  void setupShaders();
  GLuint createShader(const char* vs, const char* fs);
 public:
  GLuint vao;
  GLuint vbo;
  GLuint prog_occ;
  GLuint prog_final;
  size_t num_vertices;
  
};

#endif
