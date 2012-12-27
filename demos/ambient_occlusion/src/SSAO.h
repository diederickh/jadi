#ifndef JADI_SSAOH
#define JADI_SSAOH

#include <jadi/Jadi.h>
#include <vector>
#include "Particles.h"

// CAPTURE DIFFUSE, RENDER DEPTH INTO DEPTH ATTACHMENT
// -----------------------------------------------------------
static const char* SSAO_SCENE_VS = GLSL(120,
  uniform mat4 u_pm;
  uniform mat4 u_vm;
  uniform mat4 u_mm;
  attribute vec4 a_pos;
  attribute vec3 a_norm;
  varying vec4 v_pos;
  varying vec3 v_norm;

  void main() {
    v_pos = u_vm * u_mm * a_pos;
    v_norm = a_norm;
    gl_Position = u_pm * v_pos;
    
  }
);

static const char* SSAO_SCENE_FS = GLSL(120,
  varying vec3 v_norm;
  varying vec4 v_pos;
  
  void main() {
    float far = 350.0;
    float near = 0.1;
    float linear_depth = length(v_pos) / (far - near);

    gl_FragData[0] = vec4(v_pos);
    gl_FragData[1] = vec4(0.5 + 0.5 * v_norm, linear_depth);
  }
);


// APPLY SSAO
// -----------------------------------------------------------
static const char* SSAO_VS = GLSL(120,
  uniform mat4 u_pm;
                                  
  attribute vec4 a_pos;
  attribute vec2 a_tex;
                                  
  varying vec2 v_texCoord;
  varying vec4 v_pos;
                                  
  void main() {
    v_pos = a_pos;
    gl_Position = a_pos;
    v_texCoord = a_tex;
  }
);

static const char* SSAO_FS = GLSL(120,
  uniform sampler2D u_normalAndDepthTex;  // view space normal and linear depth
  uniform sampler2D u_viewSpacePositionTex;  // view space normal and linear depth
//  uniform sampler2D u_randomJitterTex;  // Normalmap to randomize the sampling kernel
  uniform vec2 u_texelSize;

  uniform float u_occluderBias;
  uniform float u_samplingRadius;
  uniform vec2 u_attenuation; // .x constant, .y linear, .z quadratic

  varying vec2 v_texCoord;
  varying vec4 v_pos;
                                  
  /// Sample the ambient occlusion at the following UV coordinate.
  /// <param name="srcPosition">3D position of the source pixel being tested.</param>
  /// <param name="srcNormal">Normal of the source pixel being tested.</param>
  /// <param name="uv">UV coordinate to sample/test for ambient occlusion.</param>
  /// <returns>Ambient occlusion amount.</returns>
  float SamplePixels(vec3 srcPosition, vec3 srcNormal, vec2 uv)
  {
    // Get the 3D position of the destination pixel
    vec3 dstPosition = texture2D(u_viewSpacePositionTex, uv).xyz;

    // Calculate ambient occlusion amount between these two points
    // It is simular to diffuse lighting. Objects directly above the fragment cast
    // the hardest shadow and objects closer to the horizon have minimal effect.
    vec3 positionVec = dstPosition - srcPosition;
    float intensity = max(dot(normalize(positionVec), srcNormal) - u_occluderBias, 0.0);

    // Attenuate the occlusion, similar to how you attenuate a light source.
    // The further the distance between points, the less effect AO has on the fragment.
    float dist = length(positionVec);
    float attenuation = 1.0 / (u_attenuation.x + (u_attenuation.y * dist));

    return intensity * attenuation;
  }
                                  
  /// <summary>
  /// Fragment shader entry.
  /// <summary>
  void main ()
  {
    // Get position and normal vector for this fragment
    vec3 srcNormal = texture2D(u_normalAndDepthTex, v_texCoord).xyz;
//    vec2 randVec = normalize(texture2D(u_randomJitterTex, v_texCoord).xy * 2.0 - 1.0);
    vec2 randVec = vec2(0.5, 0.5);
    
    float srcDepth = texture2D(u_normalAndDepthTex, v_texCoord).w;
    vec3 srcPosition = texture2D(u_viewSpacePositionTex, v_texCoord).xyz;

    // The following variable specifies how many pixels we skip over after each
    // iteration in the ambient occlusion loop. We can't sample every pixel within
    // the sphere of influence because that's too slow. We only need to sample
    // some random pixels nearby to apprxomate the solution.
    //
    // Pixels far off in the distance will not sample as many pixels as those close up.
    float kernelRadius = u_samplingRadius * (1.0 - srcDepth);

    // Sample neighbouring pixels
    vec2 kernel[4];
    kernel[0] = vec2(0.0, 1.0); // top
    kernel[1] = vec2(1.0, 0.0); // right
    kernel[2] = vec2(0.0, -1.0);    // bottom
    kernel[3] = vec2(-1.0, 0.0);    // left

    const float Sin45 = 0.707107;   // 45 degrees = sin(PI / 4)

    // Sample from 16 pixels, which should be enough to appromixate a result. You can
    // sample from more pixels, but it comes at the cost of performance.
    float occlusion = 0.0;
    for (int i = 0; i < 4; ++i)
    {
      vec2 k1 = reflect(kernel[i], randVec);
      vec2 k2 = vec2(k1.x * Sin45 - k1.y * Sin45,
                     k1.x * Sin45 + k1.y * Sin45);
      k1 *= u_texelSize;
      k2 *= u_texelSize;
      
      occlusion += SamplePixels(srcPosition, srcNormal, v_texCoord + k1 * kernelRadius);
      occlusion += SamplePixels(srcPosition, srcNormal, v_texCoord + k2 * kernelRadius * 0.75);
      occlusion += SamplePixels(srcPosition, srcNormal, v_texCoord + k1 * kernelRadius * 0.5);
      occlusion += SamplePixels(srcPosition, srcNormal, v_texCoord + k2 * kernelRadius * 0.25);
    }

    // Average and clamp ambient occlusion
    occlusion /= 16.0;
    occlusion = clamp(occlusion, 0.0, 1.0);

    gl_FragColor = vec4(occlusion, occlusion, occlusion, 1.0);
  }
);


// DRAWING THE CONTENTS OF A TEXTURE
// -----------------------------------------------------------
static const char* SSAO_IMAGE_VS = GLSL(120,
  uniform mat4 u_pm;
  attribute vec4 a_pos;
  attribute vec2 a_tex;
  varying vec2 v_tex;
  void main() {
    gl_Position = a_pos;
    v_tex = a_tex;
  }
);

static const char* SSAO_IMAGE_FS = GLSL(120,
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
static const char* SSAO_DEPTH_FS = GLSL(120,
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


class SSAO {
 public:
  SSAO();
  ~SSAO();
  void setup(int w, int h);
  void update();
  void beginScenePass();
  void endScenePass();
  void applySSAO();
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
  
  GLuint depth_prog; // used to draw depth buffer.
  GLuint scene_prog; // captures the scene
  GLuint ssao_prog; // dof

  // Used to draw particles
  GLuint vbo;
  GLuint vao;
  GLuint fbo;
  
  // textures
  GLuint depth_tex;
  GLuint linear_depth_tex;
  GLuint scene_tex;

  // debug drawing.
  GLuint image_prog;
  GLuint image_vao;
  GLuint image_vbo;
};
#endif
