#include "Demo.h"

Demo::Demo() {
}

Demo::~Demo() {
}

void Demo::setup() {
  ssao.setup(1024, 768);
  cam.setupArcball(1024, 768);
  cam.setPerspective(60.0f, 4.0f/3.0f, 0.1, 150.0f);
  cam.setPosition(0,0,50);

  glClearColor(.1f, .1f, .1f, 1.0f);
}

void Demo::update() {
  ssao.update();
}

void Demo::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  const float* pm = cam.pm().getPtr();
  const float* vm = cam.vm().getPtr();
  {
    ssao.beginScenePass();
    ssao.draw(pm, vm, NULL);
    ssao.endScenePass();
  }

  {
    ssao.applySSAO();
  }
  
  ssao.debugDraw();
  /*
  draw_string("Testing some string", 100.0, 100.0, 1.0, 0.0, 0.0);
  draw_string("Testing some string", 100.0, 120.0, 0.0, 1.0, 0.0);
  */
}

void Demo::onMouseDown(int x, int y, int button) {
  cam.onMouseDown(x, y);
}

void Demo::onMouseUp(int x, int y, int button) {
}

void Demo::onMouseDrag(int x, int y, int dx, int dy, int button) {
  cam.onMouseDrag(x,y);
}

void Demo::onMouseMove(int x, int y) {
}

void Demo::onChar(int ch) {
}

void Demo::onKeyDown(int key) {
}

void Demo::onKeyUp(int key) {
}

void Demo::onWindowResize(int w, int h) {
}

void Demo::onWindowClose() {
}
