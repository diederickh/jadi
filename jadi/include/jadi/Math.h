#ifndef JADI_MATH_H
#define JADI_MATH_H

#include <math.h>
#include <stdio.h>

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// VEC3
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct Vec3 {
  Vec3(float xx = 0.0f, float yy = 0.0f, float zz = 0.0f);
  Vec3(const float s);

  Vec3& operator=(const float s);
  Vec3& operator=(const Vec3& other);
  Vec3& operator-=(const Vec3& other);
  Vec3& operator+=(const Vec3& other);
  Vec3& operator+=(const float s);
  Vec3& operator*=(const float s);
  Vec3& operator*=(Vec3& other);
  Vec3& operator/=(const float s);
  Vec3 operator-() const; // -v
  Vec3 operator-(const Vec3& other) const; // v1 - v2
  Vec3 operator+(const Vec3& other) const; // v1 + v2
  Vec3 operator*(const float s) const; // v1 *= 1.5;
  Vec3 operator/(const float s) const; // v1 /= 3.4
  
  void set(const float s);
  void set(const float xx, const float yy, const float zz);
  Vec3& normalize();
  float length();
  float lengthSquared();
  void print();
  const float* getPtr();

  float x;
  float y; 
  float z;
};


// ----------------------------------------------------------------------------
// Vec3 member functions
// ----------------------------------------------------------------------------
inline Vec3::Vec3(float xx, float yy, float zz):x(xx),y(yy),z(zz) {}
inline Vec3::Vec3(const float s):x(s),y(s),z(s){}
inline void Vec3::set(const float xx, const float yy, const float zz) {  x = xx;    y = yy;   z = zz; }
inline void Vec3::set(const float s) {  x = s;  y = s;  z = s; }       
inline float Vec3::length() { return sqrt(x*x + y*y + z*z); }
inline float Vec3::lengthSquared() {  return x*x + y*y + z*z; }
inline Vec3& Vec3::normalize() {  float l = length();  x /= l;  y /= l;  z /= l;  return *this; }
inline void Vec3::print() { printf("%f, %f, %f\n", x, y, z); }
inline const float* Vec3::getPtr() { return &x; }
inline Vec3 cross(const Vec3& a, const Vec3& b) {  Vec3 d; d.x = (a.y * b.z) - (b.y * a.z); d.y = (a.z * b.x) - (b.z * a.x);  d.z = (a.x * b.y) - (b.x * a.y); return d; }
inline float dot(const Vec3& a, const Vec3& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); } 

// ----------------------------------------------------------------------------
// Vec3 operators
// ----------------------------------------------------------------------------
inline Vec3& Vec3::operator=(const float s) {  x = s;  y = s; z = s;  return *this; }
inline Vec3& Vec3::operator=(const Vec3& o) { x = o.x; y = o.y; z = o.z; return *this;}
inline Vec3& Vec3::operator-=(const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this;}
inline Vec3& Vec3::operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this;}
inline Vec3& Vec3::operator+=(const float s) { x += s; y += s; z += s; return *this; }
inline Vec3& Vec3::operator*=(const float s) { x *= s; y *= s; z *= s; return *this; }
inline Vec3& Vec3::operator*=(Vec3& o) { x *= o.x; y *= o.y; z *= o.z; return *this; } 
inline Vec3& Vec3::operator/=(const float s) { x /=s; y /= s; z /= z; return *this; }

inline Vec3 Vec3::operator-() const { return Vec3(-x, -y, -z); }
inline Vec3 Vec3::operator-(const Vec3& o) const { return Vec3(x-o.x, y-o.y, z-o.z); } 
inline Vec3 Vec3::operator+(const Vec3& o) const { return Vec3(x+o.x, y+o.y, z+o.z); }
inline Vec3 Vec3::operator*(const float s) const { return Vec3(x*s, y*s, z*s); } 
inline Vec3 Vec3::operator/(const float s) const { return Vec3(x/s, y/s, z/s); } 

inline Vec3 operator+(float f, const Vec3& o) { return Vec3(f+o.x, f+o.y, f+o.z); }
inline Vec3 operator-(float f, const Vec3& o) { return Vec3(o.x-f, o.y-f, o.z-f); }
inline Vec3 operator*(float f, const Vec3& o) { return Vec3(o.x*f, o.y*f, o.z*f); }
inline Vec3 operator/(float f, const Vec3& o) { return Vec3(o.x/f, o.y/f, o.z/f); }
inline Vec3 operator*(const Vec3& a, const Vec3& b) { return Vec3(a.x * b.x, a.y * b.y, a.z * b.z); }

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Mat4
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SM4(a,b,tmp) t = m[a]; m[a] = m[b]; m[b] = t;

struct Mat4 {
  Mat4();
  Mat4(const Mat4& o);

  Mat4& operator=(const Mat4& o);
  Mat4 operator*(const Mat4& o) const;
  Mat4& operator*=(const Mat4& o);
  
  void identity();
  void transpose();
  void translate(const float x, const float y, const float z);
  void scale(const float s);
  void scale(const float x, const float y, const float z);
  
  void setPosition(const float x, const float y, const float z);
  void setScale(const float s);
  void setScale(const float x, const float y, const float z);
  void print();

  static Mat4 scaling(const float x, const float y, const float z);

  float& operator[](const unsigned int dx);
  const float* getPtr();
  float m[16];
};

inline Mat4::Mat4() {  memset(m, 0, sizeof(float) * 16);  m[0] = 1.0f;  m[5] = 1.0f;   m[10] = 1.0f; m[15] = 1.0f; }
inline Mat4::Mat4(const Mat4& o) {  if(this != &o) { memcpy(m, o.m, sizeof(float) * 16); } } 
inline void Mat4::identity() { memset(m, 0, sizeof(float) * 16);  m[0] = 1.0f;  m[5] = 1.0f;   m[10] = 1.0f; m[15] = 1.0f; }
inline void Mat4::transpose() { float t;  SM4(1,  4,  t);  SM4(2,  8,  t);  SM4(3,  12, t);  SM4(6,  9,  t); SM4(7,  13, t); SM4(11, 14, t); }
inline void Mat4::translate(const float x, const float y, const float z) {  m[12] += x; m[13] += y; m[14] += z; }
inline void Mat4::scale(const float s) {  Mat4 mm = Mat4::scaling(s,s,s);  *this *= mm; }
inline Mat4 Mat4::scaling(const float x, const float y, const float z) {  Mat4 mm;  mm[0] = x;  mm[5] = y;  mm[10] = z;  return mm; }
inline void Mat4::setScale(const float s) {  m[0] = s; m[5] = s; m[10] = s; }
inline void Mat4::setScale(const float x, const float y, const float z) { m[0] = x; m[5] = y; m[10] = z; } 
inline void Mat4::setPosition(const float x, const float y, const float z) {  m[12] = x; m[13] = y; m[14] = z; }
inline void Mat4::print() {  printf("%3.3f, %3.3f, %3.3f, %3.3f\n", m[0], m[4], m[8], m[12]);  printf("%3.3f, %3.3f, %3.3f, %3.3f\n", m[1], m[5], m[9], m[13]);  printf("%3.3f, %3.3f, %3.3f, %3.3f\n", m[2], m[6], m[10], m[14]);  printf("%3.3f, %3.3f, %3.3f, %3.3f\n", m[3], m[7], m[11], m[15]);  printf("\n");}
inline float& Mat4::operator[](const unsigned int dx) { return m[dx]; } 
inline const float* Mat4::getPtr() { return m; } 

inline Mat4 Mat4::operator*(const Mat4& o) const {
  Mat4 r;
  r.m[0]  =  m[0] * o.m[0]  +  m[4] * o.m[1]  +  m[8]  * o.m[2]  +  m[12] * o.m[3];
  r.m[1]  =  m[1] * o.m[0]  +  m[5] * o.m[1]  +  m[9]  * o.m[2]  +  m[13] * o.m[3];
  r.m[2]  =  m[2] * o.m[0]  +  m[6] * o.m[1]  +  m[10] * o.m[2]  +  m[14] * o.m[3];
  r.m[3]  =  m[3] * o.m[0]  +  m[7] * o.m[1]  +  m[11] * o.m[2]  +  m[15] * o.m[3];
  
  r.m[4]  =  m[0] * o.m[4]  +  m[4] * o.m[5]  +  m[8]  * o.m[6]  +  m[12] * o.m[7];
  r.m[5]  =  m[1] * o.m[4]  +  m[5] * o.m[5]  +  m[9]  * o.m[6]  +  m[13] * o.m[7];
  r.m[6]  =  m[2] * o.m[4]  +  m[6] * o.m[5]  +  m[10] * o.m[6]  +  m[14] * o.m[7];
  r.m[7]  =  m[3] * o.m[4]  +  m[7] * o.m[5]  +  m[11] * o.m[6]  +  m[15] * o.m[7];
  
  r.m[8]  =  m[0] * o.m[8]  +  m[4] * o.m[9]  +  m[8]  * o.m[10] +  m[12] * o.m[11];
  r.m[9]  =  m[1] * o.m[8]  +  m[5] * o.m[9]  +  m[9]  * o.m[10] +  m[13] * o.m[11];
  r.m[10] =  m[2] * o.m[8]  +  m[6] * o.m[9]  +  m[10] * o.m[10] +  m[14] * o.m[11];
  r.m[11] =  m[3] * o.m[8]  +  m[7] * o.m[9]  +  m[11] * o.m[10] +  m[15] * o.m[11];
  
  r.m[12] =  m[0] * o.m[12] +  m[4] * o.m[13] +  m[8]  * o.m[14] +  m[12] * o.m[15];
  r.m[13] =  m[1] * o.m[12] +  m[5] * o.m[13] +  m[9]  * o.m[14] +  m[13] * o.m[15];
  r.m[14] =  m[2] * o.m[12] +  m[6] * o.m[13] +  m[10] * o.m[14] +  m[14] * o.m[15];
  r.m[15] =  m[3] * o.m[12] +  m[7] * o.m[13] +  m[11] * o.m[14] +  m[15] * o.m[15];
  return r;
}

inline Mat4& Mat4::operator*=(const Mat4& o) { 

  Mat4 r;
  r.m[0]  =  m[0] * o.m[0]  +  m[4] * o.m[1]  +  m[8]  * o.m[2]  +  m[12] * o.m[3];
  r.m[1]  =  m[1] * o.m[0]  +  m[5] * o.m[1]  +  m[9]  * o.m[2]  +  m[13] * o.m[3];
  r.m[2]  =  m[2] * o.m[0]  +  m[6] * o.m[1]  +  m[10] * o.m[2]  +  m[14] * o.m[3];
  r.m[3]  =  m[3] * o.m[0]  +  m[7] * o.m[1]  +  m[11] * o.m[2]  +  m[15] * o.m[3];

  r.m[4]  =  m[0] * o.m[4]  +  m[4] * o.m[5]  +  m[8]  * o.m[6]  +  m[12] * o.m[7];
  r.m[5]  =  m[1] * o.m[4]  +  m[5] * o.m[5]  +  m[9]  * o.m[6]  +  m[13] * o.m[7];
  r.m[6]  =  m[2] * o.m[4]  +  m[6] * o.m[5]  +  m[10] * o.m[6]  +  m[14] * o.m[7];
  r.m[7]  =  m[3] * o.m[4]  +  m[7] * o.m[5]  +  m[11] * o.m[6]  +  m[15] * o.m[7];

  r.m[8]  =  m[0] * o.m[8]  +  m[4] * o.m[9]  +  m[8]  * o.m[10] +  m[12] * o.m[11];
  r.m[9]  =  m[1] * o.m[8]  +  m[5] * o.m[9]  +  m[9]  * o.m[10] +  m[13] * o.m[11];
  r.m[10] =  m[2] * o.m[8]  +  m[6] * o.m[9]  +  m[10] * o.m[10] +  m[14] * o.m[11];
  r.m[11] =  m[3] * o.m[8]  +  m[7] * o.m[9]  +  m[11] * o.m[10] +  m[15] * o.m[11];

  r.m[12] =  m[0] * o.m[12] +  m[4] * o.m[13] +  m[8]  * o.m[14] +  m[12] * o.m[15];
  r.m[13] =  m[1] * o.m[12] +  m[5] * o.m[13] +  m[9]  * o.m[14] +  m[13] * o.m[15];
  r.m[14] =  m[2] * o.m[12] +  m[6] * o.m[13] +  m[10] * o.m[14] +  m[14] * o.m[15];
  r.m[15] =  m[3] * o.m[12] +  m[7] * o.m[13] +  m[11] * o.m[14] +  m[15] * o.m[15];

  // and copy
  m[0]  = r.m[0];
  m[1]  = r.m[1];
  m[2]  = r.m[2];
  m[3]  = r.m[3];
  m[4]  = r.m[4];
  m[5]  = r.m[5];
  m[6]  = r.m[6];
  m[7]  = r.m[7];
  m[8]  = r.m[8];
  m[9]  = r.m[9];
  m[10] = r.m[10];
  m[11] = r.m[11];
  m[12] = r.m[12];
  m[13] = r.m[13];
  m[14] = r.m[14];
  m[15] = r.m[15];

  return *this;
}

#endif
