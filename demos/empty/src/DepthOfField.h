#ifndef JADI_DEPTH_OF_FIELDH
#define JADI_DEPTH_OF_FIELDH

#include <jadi/Jadi.h>
#include <vector>
#include "Particles.h"

// DEBUG SHADER 
// -----------------------------------------------
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


// STEP 1: CAPTURE DIFFUSE, RENDER DEPTH INTO DEPTH ATTACHMENT
// attachment 0 = color
// -----------------------------------------------------------
static const char* DOF_SCENE_VS = GLSL(120,
  uniform mat4 u_pm;
  uniform mat4 u_vm;
  uniform mat4 u_mm;
  attribute vec4 a_pos;
  attribute vec3 a_norm;
  varying vec4 v_pos;
  varying vec3 v_norm;
  void main() {
    gl_Position = u_pm * u_vm * u_mm * a_pos;
    v_norm = a_norm;
  }
);

static const char* DOF_SCENE_FS = GLSL(120,
  varying vec3 v_norm;
  void main() {
    gl_FragData[0] = vec4(0.5 + 0.5 * v_norm, 1.0); 
    
  }
);


// STEP 2: APPLY H-BLUR
// -----------------------------------------------------------
static const char* DOF_BLUR0_VS = GLSL(120,
  uniform mat4 u_pm;
  attribute vec4 a_pos;
  attribute vec2 a_tex;
  varying vec2 v_tex;
  void main() {
    gl_Position = a_pos;
    v_tex = a_tex;
  }
);

static const char* DOF_BLUR0_FS = GLSL(120,
  uniform sampler2D u_depth_tex;
  uniform sampler2D u_diffuse_tex;
  varying vec2 v_tex;
  void main() {
    vec4 col = texture2D(u_tex, v_tex);
    gl_FragColor = col;
  }
);


// DRAWING THE CONTENTS OF A TEXTURE
// -----------------------------------------------------------
static const char* DOF_IMAGE_VS = GLSL(120,
  uniform mat4 u_pm;
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
    gl_FragColor.a = 1.0;
  }
);

// FRAGMENT SHADER TO DRAW DEPTH BUFFER TEXTURE (DEBUG)
// -----------------------------------------------------------
static const char* DOF_DEBUG_DEPTH_IMAGE_FS = GLSL(120,
  uniform sampler2D u_tex;
  varying vec2 v_tex;
  void main() {
    vec4 col = texture2D(u_tex, v_tex);
    gl_FragColor = col;
    float d = pow(col.r, 70.0); // gamma correct to draw depth texture
    gl_FragColor = vec4(d);
    gl_FragColor.a = 1.0;
  }
);


class DepthOfField {
 public:
  DepthOfField();
  ~DepthOfField();
  void setup(int w, int h);
  void update();
  void beginScenePass();
  void endScenePass();
  void applyDOF();
  void debugDraw();
  void draw(const float* pm, const float* vm, const float* nm, std::vector<Particle>& particles);
 private:
  void setupShaders();
  void setupBuffers();
  void setupFBO();
  GLuint createProgram(const char* vs, const char* fs);
  GLuint createTexture(int w, int h, GLenum iformat, GLenum eformat);
  void drawTexture(GLuint prog, GLuint tex, int x, int y, int w, int h);
 public:

  int fbo_w;
  int fbo_h;
  Mat4 mm;
  size_t num_vertices;
  
  GLuint debug_prog;
  GLuint debug_depth_prog; // used to draw depth buffer.
  GLuint scene_prog; // captures the scene
  GLuint blur0_prog; // first blur step

  // Used to draw particles
  GLuint vbo;
  GLuint vao;
  GLuint fbo;
  

  // textures
  GLuint depth_tex;
  GLuint scene_tex;
  GLuint blur0_tex;
  GLuint blur1_tex;


  // debug drawing.
  GLuint image_prog;
  GLuint image_vao;
  GLuint image_vbo;
};
#endif
