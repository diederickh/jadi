#include "Demo.h"

Demo::Demo() {
  printf("JADI DEMO.\n");
  Vec3 up(0.0, 1.0, 0.0);
  Vec3 right(1.0, 0.0, 0.0);
  Vec3 z = cross(up, right);
  Vec3 a(1.0, 2.0, 1.0);
  Vec3 b(1.0, 3.0, 2.0);
  Vec3 c = a * b;

  Mat4 t;
  t.translate(3.0,5.0, 2.0);
  t[2] = 3.3;
  t.scale(5.0);
  t.transpose();
  t.print();
}

Demo::~Demo() {
}

void Demo::setup() {
}

void Demo::update() {
}

void Demo::draw() {
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
