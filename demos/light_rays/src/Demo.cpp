#include "Demo.h"

Demo::Demo() {
}

Demo::~Demo() {
}

void Demo::setup() {
  cam.setupArcball(1024, 768);
  cam.setPerspective(60.0f, 4.0f/3.0f, 0.1, 150.0f);
  cam.setPosition(0,0,80);

  // PARTICLES
  float s = 45.0f;
  ps.particles.insert(ps.particles.begin(), 250, Particle());
  for(std::vector<Particle>::iterator it = ps.particles.begin(); it != ps.particles.end(); ++it) {
    Particle& p = *it;
    p.velocity.set(random(-s,s),random(-s,s),random(-s,s));
    p.position.set(random(-s,s),random(-s,s),random(-s,s));
  }

  // PREDATORS
  ps.predators.insert(ps.predators.begin(), 30, Particle());
  for(std::vector<Particle>::iterator it = ps.predators.begin(); it != ps.predators.end(); ++it) {
    Particle& p = *it;
    p.velocity.set(random(-s,s),random(-s,s),random(-s,s));
    p.position.set(random(-s,s),random(-s,s),random(-s,s));
  }
  glClearColor(.1f, .1f, .1f, 1.0f);

  // RAYS
  rays.setup(1024, 768);
  helper.setup();
}

void Demo::update() {
  ps.update();
}

void Demo::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  const float* pm = cam.pm().getPtr();
  const float* vm = cam.vm().getPtr();

  // STEP 1: render scene with light and occluding objects
  {
    rays.beginOcclusionAndLightPass();
    helper.drawLights(pm, vm, ps.predators);
    helper.drawOcclusion(pm, vm, ps.particles);
    rays.endOcclusionAndLightPass();
  }

  // STEP 2: create rays
  {
    rays.createRays();
  }
  
  // STEP 3: draw rays + shaded scene
  {
    rays.beginShadedPass(); // we capture the shaded scene so we can debug draw all stages; this is not really needed
    helper.drawShadedParticles(pm, vm, ps.particles);
    rays.endShadedPass();
  }

  // Draw the result + differnet steps
  rays.debugDraw();


  // some titles
  draw_string("Rays buffer", 10, 10, 1.0, 1.0, 1.0);
  draw_string("Occlusion buffer", 10, 394, 1.0, 1.0, 1.0);
  draw_string("Shaded scene", 522, 394, 1.0, 1.0, 1.0);
  draw_string("Occlusion buffer + Shaded scene + Rays buffer", 522, 10, 1.0, 1.0, 1.0);
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
