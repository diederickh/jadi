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
// fragdata 0 = color (attachment 0)
// fragdata 1 = depth rgba (attachment 2)
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
    v_pos = u_vm * u_mm * a_pos;
  }
);

static const char* DOF_SCENE_FS = GLSL(120,
  varying vec3 v_norm;
  varying vec4 v_pos;
  
  vec4 pack(float d) {
    const vec4 bias = vec4(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 0.0);
    float r = d;
    float g = fract(r * 255.0);
    float b = fract(g * 255.0);
    float a = fract(b * 255.0);
    vec4 col = vec4(r, g, b, a);
    return col - (col.yzww * bias);
  }

  void main() {
    gl_FragData[0] = vec4(0.5 + 0.5 * v_norm, 1.0); 
    float far = 350.0;
    float near = 0.1;
    float linear_depth = length(v_pos) / (far - near);
    gl_FragData[1] = pack(linear_depth);
  }
);


// Apply Blur
// -----------------------------------------------------------
static const char* DOF_BLUR0_VS = GLSL(120,
  uniform mat4 u_pm;
  attribute vec4 a_pos;
  attribute vec2 a_tex;
  varying vec2 v_tex;
  varying vec4 v_pos;
  void main() {
    gl_Position = a_pos;
    v_tex = a_tex;
  }
);

static const char* DOF_BLUR0_FS = GLSL(120,
  uniform sampler2D u_depth_tex;
  uniform sampler2D u_scene_tex;
  uniform float u_time;
  uniform int u_mode;
  varying vec2 v_tex;
  
  float unpack(vec4 col) {
    const vec4 bits = vec4(1.0, 1.0/255.0, 1.0/(255.0 * 255.0), 1.0 / (255.0 * 255.0 * 255.0));
    return dot(col, bits);
  }

  float get_blur_diameter(float d) {
    float far = 350.0;
    float near = 0.1;

    float focus_distance = 26.5 + sin(u_time) * 5.0; //27.5; 26.5 + sin(u_time) * 1.0;
    float dd = d * (far - near);
    float xd = abs(dd - focus_distance);
    float xxd = (dd < focus_distance) ? focus_distance - xd : focus_distance + xd;
    
    float f = 12.0;
    float N = 4.8; 
    float ms = f / ( focus_distance - f);
    float blur_c = (f * ms) / N; // blur coefficient
    float b = blur_c * (xd  /xxd);

    return b * 13.0;
  }

  void main() {
    vec4 col = texture2D(u_scene_tex, v_tex);
    vec4 depth = texture2D(u_depth_tex, v_tex);
    float z = unpack(depth);
    float blur_amount = min(get_blur_diameter(z), 30.0);
    float count = 0.0;
    vec2 tex_offset = vec2(0.0, 1.0/768.0);
    if(u_mode == 1) {
      tex_offset = vec2(1.0/1024, 0.0);
    }
    vec4 blur_col = vec4(0.0);
    if(blur_amount >= 1.0) {
      float hb = blur_amount * 0.5;
      for(float i = 0.0; i < 30.0; ++i) {
        if(i >= blur_amount) {
          break;
        }
        float offset = i - hb;
        vec2 blur_tex_offset = v_tex + (tex_offset * offset);
        blur_col += texture2D(u_scene_tex, blur_tex_offset);
        ++count;
      }
    }

    if(count > 0.0) {
      gl_FragData[0].rgb = (blur_col / count).rgb;
    }
    else {
      gl_FragData[0].rgb = col.rgb;
    }
    gl_FragData[0].a = 1.0;
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
  uniform mat4 u_pm;
  varying vec2 v_tex;
  void main() {
    vec4 col = texture2D(u_tex, v_tex);
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
  
  GLuint debug_depth_prog; // used to draw depth buffer.
  GLuint scene_prog; // captures the scene
  GLuint blur0_prog; // first blur step

  // Used to draw particles
  GLuint vbo;
  GLuint vao;
  GLuint fbo;
  
  // textures
  GLuint depth_tex;
  GLuint linear_depth_tex;
  GLuint scene_tex;
  GLuint blur0_tex;
  GLuint blur1_tex;


  // debug drawing.
  GLuint image_prog;
  GLuint image_vao;
  GLuint image_vbo;
};
#endif
