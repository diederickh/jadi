#ifndef JADI_DEMO_H
#define JADI_DEMO_H

#include <jadi/Jadi.h>

// SCENE SHADER
// -----------------------------------------------
static const char* STATUE_SCENE_VS = GLSL(120,
                                   uniform mat4 u_pm;
                                   uniform mat4 u_vm;
                                   uniform mat4 u_mm;
                                   attribute vec4 a_pos;
                                   attribute vec3 a_norm;
                                   attribute vec2 a_texcoord;
                                   varying vec3 v_norm;
                                   varying vec2 v_texCoord;
                                   
                                   void main() {
                                     v_norm = a_norm;
                                     v_texCoord = a_texcoord;
                                     gl_Position = u_pm * u_vm * u_mm * a_pos;
                                   }
                                   );

static const char* STATUE_SCENE_FS = GLSL(120,
                                   uniform sampler2D u_texture;
                                   varying vec3 v_norm;
                                   varying vec2 v_texCoord;
                                   void main() {
                                     vec3 diffuse = texture2D(u_texture, v_texCoord.st).rgb;
                                     gl_FragColor.a = 1.0;
                                 //    gl_FragColor.rgb = vec3(1.0, 0.0, 0.0);
//                                     gl_FragColor.rgb = v_norm;
                                     gl_FragColor.rgb = diffuse;
                                   }
                                   );

class Demo {
 public:
  Demo();
  ~Demo();
  void setup();
  void update();
  void draw();
  void onMouseDown(int x, int y, int button);
  void onMouseUp(int x, int y, int button);
  void onMouseDrag(int x, int y, int dx, int dy, int button);
  void onMouseMove(int x, int y);
  void onChar(int ch); /* on keydown, gets character */
  void onKeyDown(int key); /* get physical key so same for 'A' and 'a' */
  void onKeyUp(int key); /* physical key */
  void onWindowResize(int w, int h);
  void onWindowClose();
  
 public:
  // base members
  void setupBuffers();
  void setupShaders();
  GLuint createProgram(const char* vs, const char* fs);
  void setupImages();
  
  int mouse_x;
  int mouse_y;
  int prev_mouse_x;
  int prev_mouse_y;
  int pressed_mouse_button;
  bool is_mouse_down;
  GLFWwindow* window;

  // custom members
  Camera cam;
  GLuint scene_prog;
  GLuint statue_vao;
  GLuint statue_vbo;
  size_t num_vertices;
  GLuint texture_id;

};
#endif
