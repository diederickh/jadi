#ifndef JADI_LIGHT_RAYS_H
#define JADI_LIGHT_RAYS_H
#include <jadi/Jadi.h>


// SHARED VERTEX SHADER
static const char* LR_SHARED_VS = GLSL(120,
  attribute vec4 a_pos;
  attribute vec2 a_tex;
  varying vec2 v_tex;
  void main() {
    gl_Position = a_pos;
    v_tex = a_tex;
  }
);

// USED TO DEBUG DRAW THE TEXTURES
static const char* LR_IMAGE_FS = GLSL(120, 
  varying vec2 v_tex;
  uniform sampler2D u_tex;
  void main() {
    gl_FragColor = texture2D(u_tex, v_tex);
    gl_FragColor.a = 1.0;
  }
);

static const char* LR_RAYS_FS = GLSL(120,
  uniform sampler2D u_tex;
  uniform float u_exposure;
  uniform float u_decay;
  uniform float u_density;
  uniform float u_weight;

  varying vec2 v_tex;
  const int NUM_SAMPLES = 100;
  void main() {
    vec2 light_pos = vec2(0.5, 0.5);
    vec2 tex_coord = v_tex;
    vec2 delta_tex = v_tex - light_pos;
    delta_tex = delta_tex * (1.0 / float(NUM_SAMPLES)) * u_density;
    float illum_decay = 1.0;

    gl_FragColor = vec4(0);
    vec4 sample;
    for(int i = 0; i < NUM_SAMPLES; ++i) {
      tex_coord -= delta_tex;
      sample = texture2D(u_tex, tex_coord);
      sample *= illum_decay * u_weight;
      gl_FragColor += sample;
      illum_decay *= u_decay;
    }
    gl_FragColor *= u_exposure;
    gl_FragColor.a = 1.0;
  }
);

class LightRays {
 public:
  LightRays();
  ~LightRays();
  void setup(int w, int h);
  void debugDraw();
  void beginOcclusionAndLightPass();
  void endOcclusionAndLightPass();
  void beginShadedPass(); /* we capture the shaded scene too so we can nicely debug draw everything .. this is not really needed */
  void endShadedPass();
  void createRays();
 private:
  void setupBuffers();
  void setupShaders();
  GLuint createShader(const char* vs, const char* fs);
  void drawTexture(GLuint tex, int x, int y, int w, int h);
 public:  
  GLuint image_prog;
  GLuint rays_prog;
  GLuint vao;
  GLuint vbo;
  GLuint fbo;
  GLuint tex_rays; // attachment 0
  GLuint tex_occlusion; // attachment 1
  GLuint tex_shaded; // attachment 2
  GLuint depth_rbo;
  int fbo_w;
  int fbo_h;

  // Uniforms
  GLint uexposure;
  GLint utex;
  GLint udecay;
  GLint udensity;
  GLint uweight;

  // Used to control the rays
  float exposure;
  float decay;
  float density;
  float weight;

};
#endif
