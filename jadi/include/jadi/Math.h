#ifndef JADI_MATH_H
#define JADI_MATH_H

struct Vec3 {
  Vec3(float xx = 0.0f, float yy = 0.0f, float zz = 0.0f);
  Vec3(const float s);
  
  // Assignment
  Vec3& operator=(const float s);
  Vec3& operator=(Vec3& other);
  Vec3& operator-=(const Vec3& other);
  Vec3& operator+=(const Vec3& other);
  Vec3& operator+=(const float s);
  Vec3& operator*=(const float s);
  Vec3& operator*=(Vec3& other);
  Vec3& operator/=(const float s);
};

#endif
