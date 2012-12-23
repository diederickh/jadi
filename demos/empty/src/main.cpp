#include <jadi/Jadi.h>

#include <iostream>
#include "Demo.h"

Demo* demo_ptr;

// CALLBACKS
// ---------
void window_size_callback(GLFWwindow window, int w, int h);
int window_close_callback(GLFWwindow window);
void mouse_button_callback(GLFWwindow window, int button, int action);
void cursor_callback(GLFWwindow window, int x, int y);
void scroll_callback(GLFWwindow window, double x, double y);
void key_callback(GLFWwindow window, int key, int action);
void error_callback(int err, const char* msg);
void char_callback(GLFWwindow window, int chr);

// APPLICATION ENTRY
// -----------------
int main() {
  int width = 1024;
  int height = 768;
  demo_ptr = NULL;

  //// init
  glfwSetErrorCallback(error_callback);
  if(!glfwInit()) {
    printf("ERROR: cannot initialize GLFW.\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_DEPTH_BITS, 16);
  glfwWindowHint(GLFW_FSAA_SAMPLES, 4);

  GLFWwindow window = glfwCreateWindow(width, height, GLFW_WINDOWED, "Simulation", NULL);
  if(!window) {
    printf("ERROR: cannot open window.\n");
    exit(EXIT_FAILURE);
  }
  
  glfwSetWindowSizeCallback(window, window_size_callback);
  glfwSetWindowCloseCallback(window, window_close_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCharCallback(window, char_callback);

  glfwMakeContextCurrent(window);

  Demo demo;
  demo.mouse_x = demo.mouse_y = demo.prev_mouse_x = demo.prev_mouse_y = 0;
  demo.pressed_mouse_button = 0;
  demo.is_mouse_down = false;
  demo_ptr = &demo;
  demo.window = &window;
  demo.setup();
  
  bool running = true;
  while(running) {
    glfwPollEvents();

    demo.update();
    demo.draw();

    glfwSwapBuffers(window);
    running = !(glfwGetKey(window, GLFW_KEY_ESC));
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
};

// CALLBACK DEFS
// --------------
void window_size_callback(GLFWwindow window, int w, int h) {
  if(demo_ptr) {
    demo_ptr->onWindowResize(w,h);
  }
}

int window_close_callback(GLFWwindow window) {
  if(demo_ptr) {
    demo_ptr->onWindowClose();
  }
  return GL_TRUE;
}

void mouse_button_callback(GLFWwindow window, int button, int action) {
  if(!demo_ptr) {
    return;
  }
   if(action == GLFW_PRESS) {
    demo_ptr->onMouseDown(demo_ptr->mouse_x, demo_ptr->mouse_y, button);
    demo_ptr->pressed_mouse_button = button;
    demo_ptr->is_mouse_down = true;
  }
   else {
    demo_ptr->onMouseUp(demo_ptr->mouse_x, demo_ptr->mouse_y, button);
    demo_ptr->pressed_mouse_button = -1;
    demo_ptr->is_mouse_down = false;
  }
}

void cursor_callback(GLFWwindow window, int x, int y) {
  if(!demo_ptr) {
    return;
  }
  demo_ptr->mouse_x = x;
  demo_ptr->mouse_y = y;

  if(demo_ptr->is_mouse_down) {
    demo_ptr->onMouseDrag(x, y, (demo_ptr->prev_mouse_x - x), (demo_ptr->prev_mouse_y - y), demo_ptr->pressed_mouse_button);
  }
  else {
    demo_ptr->onMouseMove(x,y);
  }
  demo_ptr->prev_mouse_x = demo_ptr->mouse_x;
  demo_ptr->prev_mouse_y = demo_ptr->mouse_y;
}

void scroll_callback(GLFWwindow window, double x, double y) {
  if(!demo_ptr) {
    return;
  }
}

void char_callback(GLFWwindow window, int ch) {
  if(!demo_ptr) {
    return;
  }
  demo_ptr->onChar(ch);
}

void key_callback(GLFWwindow window, int key, int action) {
  if(!demo_ptr) {
    return;
  }
  if(action == GLFW_PRESS) {
    demo_ptr->onKeyDown(key);
  }
  else {
    demo_ptr->onKeyUp(key);
  }
}

void error_callback(int err, const char* msg) {
  printf("ERROR: %s, %d\n", msg, err);
}

