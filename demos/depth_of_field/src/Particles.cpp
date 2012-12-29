#include "Particles.h"

Particles::Particles() {
  zone_radius = 10;
  lower_threshold = 0.5;
  higher_threshold = 0.8;
  repel_strength = 0.01;
  align_strength = 0.04;
  attract_strength = 0.01;
  radius = 35.0f;
  min_speed = 0.4f;
  max_speed = 0.8f;
  min_predator_speed = 1.0f;
  max_predator_speed = 2.0f;
  last_time = 0;
  time_accum = 0;
  last_time = 0;
  dt = (1.0f/60.0f) * 1000.0f;
}

void Particles::update() {
  uint64_t now = millis();
  uint64_t diff = (now - last_time);
  if(diff > 32) {
    diff = 32;
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
  float pred_radius = 15.0f;
  float pred_radius_sq = pred_radius * pred_radius;
  for(std::vector<Particle>::iterator ita = particles.begin(); ita != particles.end(); ++ita) {
    Particle& a = *ita;
    dir = -a.position;
    ls = dir.lengthSquared();
    if(ls > radius_sq) {
      dir.normalize();
      dir *= (ls - radius_sq) * 0.00025f;
      a.forces += dir;
    }

    for(std::vector<Particle>::iterator pit = predators.begin(); pit != predators.end(); ++pit) {
      Particle& pred = *pit;
      dir = pred.position - a.position;
      ls = dir.lengthSquared();
      if(ls < pred_radius_sq) {
        F = (pred_radius_sq / ls) * 3.3f;
        dir.normalize();
        a.forces -= dir * F;
      }
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
        a.forces += dir;                                                                                                                                                                                    
        b.forces -= dir;  
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

  max_speed_sq = max_predator_speed * max_predator_speed;
  min_speed_sq = min_predator_speed * min_predator_speed;
  for(std::vector<Particle>::iterator it = predators.begin(); it != predators.end(); ++it) {
    Particle& p = *it;

    // move towards center
    dir = -p.position;
    ls = dir.lengthSquared();
    dir.normalize();
    dir *= 0.03;
    p.forces += dir;

    p.velocity += p.forces;
    p.velocity_norm = p.velocity;

    p.velocity_norm.normalize();
    ls = p.velocity.lengthSquared();

    if(ls > max_speed_sq) {
      p.velocity = p.velocity_norm * max_predator_speed;
    }
    else if(ls < min_speed_sq) {
      p.velocity = p.velocity_norm * min_predator_speed;
    }

    p.position += p.velocity;
    p.velocity *= 0.99;
    p.forces = 0;
  }
}
