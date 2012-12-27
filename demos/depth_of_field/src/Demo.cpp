#include "Demo.h"

Demo::Demo() {
}

Demo::~Demo() {
}

void Demo::setup() {
  dof.setup(1024, 768);
  cam.setupArcball(1024, 768);
  cam.setPerspective(60.0f, 4.0f/3.0f, 0.1, 150.0f);
  cam.setPosition(0,0,80);

  float s = 45.0f;
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
  {
    dof.beginScenePass();
    dof.draw(pm, vm, NULL, ps.particles);
    dof.endScenePass();
  }

  {
    dof.applyDOF();
  }
  
  dof.debugDraw();
 
  draw_string("Diffuse", 10, 10, 1, 1, 1);
  draw_string("Raw depth buffer", 10, 394, 0, 0, 0);
  draw_string("Linear depth buffer as rgb", 522, 394, 1, 1, 1);
  draw_string("Depth of field", 522, 10, 1, 1, 1);

  char buf[512];
  sprintf(buf, "fstop: %.3f (a,b)", dof.fstop); draw_string(buf, 10, 748, 1.0, 0.0, 1.0);
  sprintf(buf, "focus_distance: %.3f (c,d)", dof.focus_distance); draw_string(buf, 10, 735, 1.0, 0.0, 1.0);
  sprintf(buf, "focal_length: %.3f (e,f)", dof.focal_length); draw_string(buf, 10, 722, 1.0, 0.0, 1.0);
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
  if(ch == 'a') {
    dof.fstop += 0.01;
  }
  else if(ch == 'b') {
    dof.fstop -= 0.01;
  }
  else if(ch == 'c') {
    dof.focus_distance += 0.1;
  }
  else if(ch == 'd') {
    dof.focus_distance -= 0.1;
  }
  else if(ch == 'e') {
    dof.focal_length += 0.1;
  }
  else if(ch == 'f') {
    dof.focal_length -= 0.1;
  }
}

void Demo::onKeyDown(int key) {
}

void Demo::onKeyUp(int key) {
}

void Demo::onWindowResize(int w, int h) {
}

void Demo::onWindowClose() {
}
