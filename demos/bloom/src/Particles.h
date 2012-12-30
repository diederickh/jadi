#ifndef DEMO_PARTICLES_H
#define DEMO_PARTICLES_H

#include <jadi/Jadi.h>
#include <vector>

struct Particle {
  Vec3 position;
  Vec3 velocity;
  Vec3 velocity_norm;
  Vec3 forces;
};

struct Particles {
  Particles();
  void update();
  void integrate();
  std::vector<Particle> particles;
  std::vector<Particle> predators;
  
  float zone_radius;
  float attract_strength;
  float repel_strength;
  float align_strength;
  float lower_threshold;
  float higher_threshold;
  float max_speed;
  float min_speed;
  float max_predator_speed;
  float min_predator_speed;
  float ls;
  float F;
  float p;

  float radius;
  Vec3 dir;

  uint64_t dt;
  uint64_t time_accum;
  uint64_t last_time;
};

#endif 
