#include "Particles.h"

Particles::Particles() {
  zone_radius = 30;
  lower_threshold = 0.5;
  higher_threshold = 0.8;
  repel_strength = 0.001;
  align_strength = 0.001;
  attract_strength = 0.001;
  radius = 50.0f;
  min_speed = 1.1f;
  max_speed = 2.5f;
  last_time = 0;
  time_accum = 0;
  last_time = 0;
  dt = (1.0f/60.0f) * 1000.0f;
}

void Particles::update() {
  uint64_t now = millis();
  uint64_t diff = (now - last_time);
  if(diff > 48) {
    diff = 48;
  }

  time_accum += diff;
  while(time_accum >= dt) {
    time_accum -= dt;
    integrate();
  }
  last_time = millis();
}

void Particles::integrate() {
  float zone_radius_sq = zone_radius * zone_radius;
  float radius_sq = radius * radius;
  float thresh_delta = higher_threshold - lower_threshold;
  float s = 0.05;
  for(std::vector<Particle>::iterator ita = particles.begin(); ita != particles.end(); ++ita) {
    Particle& a = *ita;
    dir = -a.position;
    ls = dir.lengthSquared();
    if(ls > radius_sq) {
      dir.normalize();
      dir *= (ls - radius_sq) * 0.00025f;
      a.forces += dir;
    }
    Vec3 r(random(-s,s),random(-s,s),random(-s,s));
    a.forces += r;
    std::vector<Particle>::iterator itb = ita;
    for(++itb; itb != particles.end(); ++itb) {
      Particle& b = *itb;
      dir = a.position - b.position;
      ls = dir.lengthSquared();
      if(ls > zone_radius_sq) {
        continue;
      }
      p = ls / zone_radius_sq;
      if(p < lower_threshold) {
        // repel
        F = (lower_threshold / (p - 1.0f)) * repel_strength;
        dir.normalize();
        dir *= F;
        a.forces -= dir;
        b.forces += dir;
      }
      else if(p < higher_threshold) {
        // align
        p = (p-lower_threshold) / thresh_delta;
        F = p * align_strength;
        a.velocity += b.velocity_norm * F;
        b.velocity += a.velocity_norm * F;
      }
      else {
        p = (p - higher_threshold) / (1.0f - higher_threshold);                                                                                                                                       
        dir.normalize();                                                                                                                                                                                    
        F = p * attract_strength;                                                                                                                                                                        
        dir *= F;                                                                                                                                                                                           
        a.forces -= dir;                                                                                                                                                                                    
        b.forces += dir;  
      }
    }
  }

  float max_speed_sq = max_speed * max_speed;
  float min_speed_sq = min_speed * min_speed;
  for(std::vector<Particle>::iterator ita = particles.begin(); ita != particles.end(); ++ita) {
    Particle& a = *ita;
    a.velocity += a.forces;
    a.velocity_norm = a.velocity;
    a.velocity_norm.normalize();
    ls = a.velocity.lengthSquared();

    if(ls > max_speed_sq) {
      a.velocity = a.velocity_norm * max_speed;
    }
    else if(ls < min_speed_sq) {
      a.velocity = a.velocity_norm * min_speed;
    }

    a.position += a.velocity;
    a.velocity *= 0.99;
    a.forces = 0;
  }
}
