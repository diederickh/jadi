#include "Demo.h"

Demo::Demo() {
}

Demo::~Demo() {
}

void Demo::setup() {
  dof.setup();
  cam.setPerspective(60.0f, 4.0f/3.0f, 0.1, 500.0f);
  cam.setPosition(0,0,10);
}

void Demo::update() {
  dof.update();
}

void Demo::draw() {
  const float* pm = cam.pm().getPtr();
  const float* vm = cam.vm().getPtr();
  dof.draw(pm, vm, NULL);
}

void Demo::onMouseDown(int x, int y, int button) {
}

void Demo::onMouseUp(int x, int y, int button) {
}

void Demo::onMouseDrag(int x, int y, int dx, int dy, int button) {
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
