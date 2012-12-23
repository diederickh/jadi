#include "Demo.h"

Demo::Demo() {
}

Demo::~Demo() {
}

void Demo::setup() {
  dof.setup();
  cam.setupArcball(1024, 768);
  cam.setPerspective(60.0f, 4.0f/3.0f, 0.1, 500.0f);
  cam.setPosition(0,0,150);

  float s = 80.0f;
  ps.particles.insert(ps.particles.begin(), 250, Particle());
  for(std::vector<Particle>::iterator it = ps.particles.begin(); it != ps.particles.end(); ++it) {
    Particle& p = *it;
    p.velocity.set(random(-s,s),random(-s,s),random(-s,s));
    p.position.set(random(-s,s),random(-s,s),random(-s,s));
  }
  glClearColor(.1f, .1f, .1f, 1.0f);
}

void Demo::update() {
  ps.update();
  dof.update();
}

void Demo::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  const float* pm = cam.pm().getPtr();
  const float* vm = cam.vm().getPtr();
  dof.draw(pm, vm, NULL, ps.particles);
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
