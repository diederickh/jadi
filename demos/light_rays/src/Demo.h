#ifndef JADI_DEMO_H
#define JADI_DEMO_H

#include <jadi/Jadi.h>
#include "Particles.h"
#include "LightRays.h" /* applies the light ray shader; captures scene + makes rays */
#include "LightRayHelper.h" /* used to draw light ray related stuff */

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
  int mouse_x;
  int mouse_y;
  int prev_mouse_x;
  int prev_mouse_y;
  int pressed_mouse_button;
  bool is_mouse_down;
  GLFWwindow* window;

  // custom members
  Camera cam;
  Particles ps;
  LightRays rays;
  LightRayHelper helper;
};
#endif
