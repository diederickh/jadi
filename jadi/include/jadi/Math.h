#ifndef JADI_MATH_H
#define JADI_MATH_H

#include <math.h>
#include <stdio.h>
#include <jadi/Utils.h>

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Vec3
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
inline Vec3& Vec3::normalize() {  float l = length();  if(l < 0.0001) { return *this; } x /= l;  y /= l;  z /= l;  return *this; }
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
// Mat4, column major (default GL)
// 0  4  8   12
// 1  5  9   13
// 2  6  10  14
// 3  7  11  15
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SM4(a,b,tmp) t = m[a]; m[a] = m[b]; m[b] = t;
struct Mat3;
struct Mat4 {
  Mat4();
  Mat4(const Mat4& o);
  //  Mat4(Mat3& o);

  Mat4& operator=(const Mat4& o);
  Mat4& operator=(Mat3& o);
  Mat4 operator*(const Mat4& o) const;
  Mat4& operator*=(const Mat4& o);
  
  void identity();
  void transpose();
  void translate(const float x, const float y, const float z);
  void setYRotation(const float radian);
  void setZRotation(const float radian);
  void scale(const float s);
  void scale(const float x, const float y, const float z);
  void ortho(const float l, const float r, const float b, const float t, const float n, const float f);
  void orthoTopLeft(const float w, const float h, const float n, const float f);
  void orthoBottomLeft(const float w, const float h, const float n, const float f);
  void frustum(const float l, const float r, const float b, const float t, const float n, const float f);
  void perspective(const float fov, const float aspect, const float n, const float f);
  
  void setPosition(const float x, const float y, const float z);
  void setScale(const float s);
  void setScale(const float x, const float y, const float z);
  void print();

  static Mat4 scaling(const float x, const float y, const float z);
  static Mat4 rotation(const float radiansX, const float radiansY, const float radiansZ);

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
inline const float* Mat4::getPtr() { return m; } 
inline float& Mat4::operator[](const unsigned int dx) { return m[dx]; } 

inline void Mat4::setZRotation(const float radian) {
  float ca = cos(radian);
  float sa = sin(radian);
  m[0] = ca;
  m[1] = sa;
  m[4] = -sa;
  m[5] = ca;
}

inline void Mat4::setYRotation(const float radian) {
  float ca = cos(radian);
  float sa = sin(radian);
  m[0] = ca;
  m[2] = -sa;
  m[8] = sa;
  m[10] = ca;
}

inline Mat4 Mat4::rotation(const float radiansX, const float radiansY, const float radiansZ) {
  Mat4 r;
  float cx = cosf(radiansX);
  float sx = sinf(radiansX);
  float cy = cosf(radiansY);
  float sy = sinf(radiansY);
  float cz = cosf(radiansZ);
  float sz = sinf(radiansZ);

  r.m[0]  =  (cy * cz);
  r.m[4]  = -(cy * sz);  
  r.m[8]  =  sy;
  r.m[12] = 0.0f;

  r.m[1]  =  (sx * sy * cz) + (cx * sz);
  r.m[5]  = -(sx * sy * sz) + (cx * cz);
  r.m[9]  = -(sx * cy); 
  r.m[13] = 0.0f;

  r.m[2]  = -(cx * sy * cz) + (sx * sz);
  r.m[6]  =  (cx * sy * sz) + (sx * cz);
  r.m[10] =  (cx * cy);
  r.m[14] = 0.0f;

  r.m[3]  = 0.0f;
  r.m[7]  = 0.0f;
  r.m[11] = 0.0f;
  r.m[15] = 1.0f;
  return r;
}

inline Mat4& Mat4::operator=(const Mat4& o) {
  if(&o == this) {
    return *this;
  }

  memcpy(m, o.m, sizeof(float) * 16);
  return *this;
}

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

inline void Mat4::frustum(const float l, const float r, const float b, const float t, const float n, const float f) {
  memset(m, 0, 16 * sizeof(float));
  m[0] = 2.0f * n / (r-l);
  m[5]  = 2.0f * n / (t-b);
  m[8]  = (r+l) / (r-l);
  m[9]  = (t+b) / (t-b);
  m[10] = -(f+n) / (f-n);
  m[11] = -1.0f;
  m[14] = -2.0f * f * n / (f-n);
  m[15] = 0.0f;
}

inline void Mat4::perspective(const float fov, const float aspect, const float n, const float f) {
  float hh = tanf(fov * DEG_TO_RAD * 0.5) * n;
  frustum(-hh*aspect, hh*aspect, -hh, hh, n, f);
}

inline void Mat4::ortho(const float l, const float r, const float b, const float t, const float n, const float f) {
  memset(m, 0, 16 * sizeof(float));
  float rml = r - l;
  float fmn = f - n;
  float tmb = t - b;
  m[0]  = 2.0f / rml;
  m[5]  = 2.0f / tmb;
  m[10] = -2.0f / fmn;
  m[12] = -(r+l)/rml;
  m[13] = -(t+b)/tmb;
  m[14] = -(f+n)/fmn;
}

inline void Mat4::orthoTopLeft(const float w, const float h, const float n, const float f) {
  ortho(0.0f, w, h, 0.0f, n, f);
}

inline void Mat4::orthoBottomLeft(const float w, const float h, const float n, const float f) {
  ortho(0.0f, w, 0.0, h, n, f);
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Mat3, column major
// 0  3  6 
// 1  4  7 
// 2  5  8
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct Mat3 {
  Mat3();
  Mat3(const Mat3& o);
  
  Mat3& operator=(const Mat3& o);
  Mat3& operator*=(const Mat3& o);
  Mat3 operator*(const Mat3& o);
  float& operator[](const unsigned int dx);
  
  const float* getPtr();
  void identity();
  void inverse();
  void transpose();
  void print();

  static Mat3 rotation(const float radiansX, const float radiansY, const float radiansZ);
  float m[9];
};
 
inline Mat3::Mat3() {  memset(m, 0, sizeof(float) * 9);  identity();}
inline Mat3::Mat3(const Mat3& o) {  if(this != &o) { memcpy(m, o.m, sizeof(float) * 9); } }
inline Mat3& Mat3::operator=(const Mat3& o) {  memcpy(m, o.m, sizeof(float) * 9); return *this; }

inline Mat3& Mat3::operator*=(const Mat3& o) {
  float f[9];
  f[0] = m[0] * o.m[0] + m[3] * o.m[1] + m[6] * o.m[2];
  f[1] = m[1] * o.m[0] + m[4] * o.m[1] + m[7] * o.m[2];
  f[2] = m[2] * o.m[0] + m[5] * o.m[1] + m[8] * o.m[2];
  
  f[3] = m[0] * o.m[3] + m[3] * o.m[4] + m[6] * o.m[5];
  f[4] = m[1] * o.m[3] + m[4] * o.m[4] + m[7] * o.m[5];
  f[5] = m[2] * o.m[3] + m[5] * o.m[4] + m[8] * o.m[5];
  
  f[6] = m[0] * o.m[6] + m[3] * o.m[7] + m[6] * o.m[8];
  f[7] = m[1] * o.m[6] + m[4] * o.m[7] + m[7] * o.m[8];
  f[8] = m[2] * o.m[6] + m[5] * o.m[7] + m[8] * o.m[8];
  memcpy(m, f, sizeof(float) * 9);
  return *this;
}

inline Mat3 Mat3::operator*(const Mat3& o) {
  Mat3 r;
  r.m[0] = m[0] * o.m[0] + m[3] * o.m[1] + m[6] * o.m[2];
  r.m[1] = m[1] * o.m[0] + m[4] * o.m[1] + m[7] * o.m[2];
  r.m[2] = m[2] * o.m[0] + m[5] * o.m[1] + m[8] * o.m[2];
  
  r.m[3] = m[0] * o.m[3] + m[3] * o.m[4] + m[6] * o.m[5];
  r.m[4] = m[1] * o.m[3] + m[4] * o.m[4] + m[7] * o.m[5];
  r.m[5] = m[2] * o.m[3] + m[5] * o.m[4] + m[8] * o.m[5];
  
  r.m[6] = m[0] * o.m[6] + m[3] * o.m[7] + m[6] * o.m[8];
  r.m[7] = m[1] * o.m[6] + m[4] * o.m[7] + m[7] * o.m[8];
  r.m[8] = m[2] * o.m[6] + m[5] * o.m[7] + m[8] * o.m[8];
  return r;
}

inline float& Mat3::operator[](const unsigned int dx) {  return m[dx]; }
inline const float* Mat3::getPtr() {  return m; }
inline void Mat3::identity() {  memset(m, 0, sizeof(float) * 9);  m[0] = m[4] = m[8] = 1.0f; }


static Mat3 inverse(Mat3& o) {

  Mat3 r;
  float cofactor0 = o.m[4]*o.m[8] - o.m[5]*o.m[7];
  float cofactor3 = o.m[2]*o.m[7] - o.m[1]*o.m[8];
  float cofactor6 = o.m[1]*o.m[5] - o.m[2]*o.m[4];
  float det = o.m[0]*cofactor0 + o.m[3]*cofactor3 + o.m[6]*cofactor6;
  float inv_det = 1.0f / det;
    
  r.m[0] = inv_det*cofactor0;
  r.m[1] = inv_det*cofactor3;
  r.m[2] = inv_det*cofactor6;
   
  r.m[3] = inv_det*(o.m[5]*o.m[6] - o.m[3]*o.m[8]);
  r.m[4] = inv_det*(o.m[0]*o.m[8] - o.m[2]*o.m[6]);
  r.m[5] = inv_det*(o.m[2]*o.m[3] - o.m[0]*o.m[5]);

  r.m[6] = inv_det*(o.m[3]*o.m[7] - o.m[4]*o.m[6]);
  r.m[7] = inv_det*(o.m[1]*o.m[6] - o.m[0]*o.m[7]);
  r.m[8] = inv_det*(o.m[0]*o.m[4] - o.m[1]*o.m[3]);

  return r;
}

inline void Mat3::inverse() {
  *this = ::inverse(*this);
}

inline void Mat3::transpose() {
  float f[9];
  f[0] = m[0];
  f[1] = m[3];
  f[2] = m[6];

  f[3] = m[1];
  f[4] = m[4];
  f[5] = m[7];
  
  f[6] = m[2];
  f[7] = m[5];
  f[8] = m[8];
  memcpy(m, f, sizeof(float) * 9);
}

inline void Mat3::print() {
  printf("%3.3f, %3.3f, %3.3f\n", m[0], m[3], m[6]);
  printf("%3.3f, %3.3f, %3.3f\n", m[1], m[4], m[7]);
  printf("%3.3f, %3.3f, %3.3f\n", m[2], m[5], m[8]);
  printf("\n");
}

inline Mat3 Mat3::rotation(const float radiansX, const float radiansY, const float radiansZ) {
  Mat3 r;

  float cx = cosf(radiansX);
  float sx = sinf(radiansX);
  float cy = cosf(radiansY);
  float sy = sinf(radiansY);
  float cz = cosf(radiansZ);
  float sz = sinf(radiansZ);
  
  r.m[0] = (cy * cz);
  r.m[3] = -(cy * sz);
  r.m[6] = sy;
  
  r.m[1] = (sx * sy * cz) + (cx * sz);
  r.m[4] = -(sx * sy * sz) + (cx * cz);
  r.m[7] = -(sx * cy);
  
  r.m[2] = -(cx * sy * cz) + (sx * sz);
  r.m[5] = (cx * sy * sz) + (sx * cz);
  r.m[8] = (cx * cy);
  return r;
}

inline Mat4& Mat4::operator=(Mat3& o) {
  
  m[0] = o[0];
  m[1] = o[1];
  m[2] = o[2];

  m[4] = o[3];
  m[5] = o[4];
  m[6] = o[5];

  m[8] = o[6];
  m[9] = o[7];
  m[10] = o[8];
  return *this;
}

static float sin_zero_half_pi(float a);
static float atan_positive(float y, float x);
  
class Quat {
 public:
  float x;
  float y;
  float z;
  float w;

  Quat(float x = 0, float y = 0, float z = 0, float w = 1);
  Quat(Mat4& m);
  Quat(Mat3& m);
  Quat(const Quat& q);

  void set(const float xx, const float yy, const float zz, const float ww);
  void normalize();
  float length();
  float lengthSquared();
  float dot(Quat& other);
  void identity();
  void inverse();
  void toMat4(Mat4& m);
  void toMat3(Mat3& m);
  Mat3 getMat3();
  Mat4 getMat4();
  void fromMat4(Mat4& m);
  void fromMat3(Mat3& m);
  void fromAngleAxis(const float radians, const float xx, const float yy, const float zz); 
  void fromEuler(const float radiansX, const float radiansY, const float radiansZ);
  void multiply(const Quat& q);
  void multiply(const Quat& q1, const Quat& q2, Quat* dst);
  Vec3 transform(const Vec3& v) const;
  void lerp(const Quat& q1, const Quat& q2, float t, Quat* dst);
  static void slerp(const Quat& from, const Quat& to, float t, Quat& result);
  void print();

  Vec3 operator*(const Vec3& v) const;
  Quat operator*(const Quat& other) const;
  Quat& operator*=(const Quat& other);
};


inline Quat::Quat(float x, float y, float z, float w ):x(x),y(y),z(z),w(w) {}

inline Quat::Quat(Mat4& m) {
  fromMat4(m);
}

inline Quat::Quat(Mat3& m) {
  fromMat3(m);
}

inline Quat::Quat(const Quat& q) 
            :x(q.x)
           ,y(q.y)
           ,z(q.z)
           ,w(q.w)
{
    
}

inline void Quat::set(const float xx, const float yy, const float zz, const float ww) {
  x = xx;
  y = yy; 
  z = zz;
  w = ww;
}

inline void Quat::normalize() {
  float n = x * x + y * y + z * z + w * w;
  if(n == 1.0f) {
    return;
  }
    
  n = sqrt(n);
  if(n < 0.000001f) {
    return;
  }

  n = 1.0 / n;
  x *= n;
  y *= n;
  z *= n;
  w *= n;
}

inline float Quat::length() {
  return sqrt(lengthSquared());
}

inline float Quat::lengthSquared() {
  return (x * x) + (y * y) + (z * z) + (w * w);
}

inline float Quat::dot(Quat& other) {
  return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

inline void Quat::identity() {
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
  w = 1.0f;
}

inline void Quat::inverse() {
  float n = x * x + y * y + z * z + w * w;
  if(n == 1.0f) {
    x = -x;
    y = -y;
    z = -z;
    return;
  }

  if(n < 0.000001f) {
    return;
  }
    
  n = 1.0f / n;
  x = (-x * n);
  y = (-y * n);
  z = (-z * n);
  w = w * n;
}

// @see [0], the { } sections are hints for compiler optimization
inline void Quat::toMat4(Mat4& m) {
  float x2 = x + x;
  float y2 = y + y;
  float z2 = z + z;

  {
    float xx2 = x * x2;
    float yy2 = y * y2;
    float zz2 = z * z2;

    m[0] = 1.0f - yy2 - zz2;
    m[5] = 1.0f - xx2 - zz2;
    m[10] = 1.0f - xx2 - yy2;
  }
  {
    float yz2 = y * z2;
    float wx2 = w * x2;

    m[6] = yz2 - wx2;
    m[9] = yz2 + wx2;
  }
  {
    float xy2 = x * y2;
    float wz2 = w * z2;
      
    m[1] = xy2 - wz2;
    m[4] = xy2 + wz2;
  }
  {
    float xz2 = x * z2;
    float wy2 = w * y2;
      
    m[8] = xz2 - wy2;
    m[2] = xz2 + wy2;
  }
}

inline Mat4 Quat::getMat4() {
  Mat4 m;
  toMat4(m);
  return m;
}

// @see [0], the { } sections are hints for compiler optimization
inline void Quat::toMat3(Mat3& m) {
  float x2 = x + x;
  float y2 = y + y;
  float z2 = z + z;

  {
    float xx2 = x * x2;
    float yy2 = y * y2;
    float zz2 = z * z2;

    m[0] = 1.0f - yy2 - zz2;
    m[4] = 1.0f - xx2 - zz2;
    m[8] = 1.0f - xx2 - yy2;
  }
  {
    float yz2 = y * z2;
    float wx2 = w * x2;

    m[5] = yz2 - wx2;
    m[7] = yz2 + wx2;
  }
  {
    float xy2 = x * y2;
    float wz2 = w * z2;
      
    m[1] = xy2 - wz2;
    m[3] = xy2 + wz2;
  }
  {
    float xz2 = x * z2;
    float wy2 = w * y2;
      
    m[6] = xz2 - wy2;
    m[2] = xz2 + wy2;
  }
}
  
inline Mat3 Quat::getMat3() {
  Mat3 m;
  toMat3(m);
  return m;
}

// @see [0]
inline void Quat::fromMat4(Mat4& m) {
  if(m[0] + m[5] + m[10] > 0.0f) {
    float t = +m[0] + m[5] + m[10] + 1.0f;
    float s = fast_sqrt(t) * 0.5f;
    w = s * t;
    z = (m[4] - m[1]) * s;
    y = (m[2] - m[8]) * s;
    x = (m[9] - m[6]) * s;
  }
  else if(m[0] > m[5] && m[0] > m[10]) {
    float t = +m[0] - m[5] - m[10] + 1.0f;
    float s = fast_sqrt(t) * 0.5f;
    x = s * t;
    y = (m[4] + m[1]) * s;
    z = (m[2] + m[8]) * s;
    w = (m[9] - m[6]) * s;
  }
  else if(m[5] > m[10]) {
    float t = -m[0] + m[5] - m[10] + 1.0f;
    float s = fast_sqrt(t) * 0.5f;
    y = s * t;
    x = (m[4] + m[1]) * s;
    w = (m[2] - m[8]) * s;
    z = (m[9] + m[6]) * s;
  }
  else {
    float t = -m[0] - m[5] + m[10] + 1.0f;
    float s = fast_sqrt(t) * 0.5f;
    z = s * t;
    w = (m[4] - m[1]) * s;
    x = (m[2] + m[8]) * s;
    y = (m[9] + m[6]) * s;
  }
}

// @see [0]
inline void Quat::fromMat3(Mat3& m) {
  if(m[0] + m[4] + m[8] > 0.0f) {
    float t = +m[0] + m[4] + m[8] + 1.0f;
    float s = fast_sqrt(t) * 0.5f;
    w = s * t;
    z = (m[3] - m[1]) * s;
    y = (m[2] - m[6]) * s;
    x = (m[7] - m[5]) * s;
  }
  else if(m[0] > m[4] && m[0] > m[8]) {
    float t = +m[0] - m[4] - m[8] + 1.0f;
    float s = fast_sqrt(t) * 0.5f;
    x = s * t;
    y = (m[3] + m[1]) * s;
    z = (m[2] + m[6]) * s;
    w = (m[7] - m[5]) * s;
  }
  else if(m[4] > m[8]) {
    float t = -m[0] + m[4] - m[8] + 1.0f;
    float s = fast_sqrt(t) * 0.5f;
    y = s * t;
    x = (m[3] + m[1]) * s;
    w = (m[2] - m[6]) * s;
    z = (m[7] + m[5]) * s;
  }
  else {
    float t = -m[0] - m[4] + m[9] + 1.0f;
    float s = fast_sqrt(t) * 0.5f;
    z = s * t;
    w = (m[3] - m[1]) * s;
    x = (m[2] + m[6]) * s;
    y = (m[7] + m[5]) * s;
  }
}

inline void Quat::fromAngleAxis(const float radians, const float xx, const float yy, const float zz) {
  const float ha = 0.5f * radians;
  const float s = sin(ha);
  w = cos(ha);
  x = s * xx;
  y = s * yy;
  z = s * zz;
}

inline void Quat::fromEuler(const float radiansX, const float radiansY, const float radiansZ) {

  // Assuming the angles are in radians.
  float c1 = cos(radiansY);
  float s1 = sin(radiansY);
  float c2 = cos(radiansZ);
  float s2 = sin(radiansZ);
  float c3 = cos(radiansX);
  float s3 = sin(radiansX);
  w = sqrt(1.0 + c1 * c2 + c1*c3 - s1 * s2 * s3 + c2*c3) / 2.0;
  float w4 = (4.0 * w);
  x = (c2 * s3 + c1 * s3 + s1 * s2 * c3) / w4 ;
  y = (s1 * c2 + s1 * c3 + c1 * s2 * s3) / w4 ;
  z = (-s1 * s3 + c1 * s2 * c3 +s2) / w4 ;
}

inline void Quat::multiply(const Quat& q) {
  multiply(*this, q, this);
}
  
inline void Quat::multiply(const Quat& q1, const Quat& q2, Quat* dst) {
  float xx = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
  float yy = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
  float zz = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
  float ww = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

  dst->x = xx;
  dst->y = yy;
  dst->z = zz;
  dst->w = ww;
}

inline Vec3 Quat::transform(const Vec3& v) const {
  float m = (2.0f * x * v.x + y * v.y + z * v.z);
  float c = 2.0f * w;
  float p = c * w - 1.0f;
  return Vec3(
              p * v.x + m * x + c * (y * v.z - z * v.y)
              ,p * v.y + m * y + c * (z * v.x - x * v.z)
              ,p * v.z + m * z + c * (x * v.y - y * v.x)
              );

}

inline void Quat::lerp(const Quat& q1, const Quat& q2, float t, Quat* dst) {
  if(t == 0.0f) {
    memcpy(&dst->x, &q1.x, sizeof(float) * 4);
    return;
  }
  else if(t == 1.0f) {
    memcpy(&dst->x, &q2.x, sizeof(float) * 4);
    return;
  }

  float t1 = 1.0f - t;
  dst->x = t1 * q1.x + t * q2.x;
  dst->y = t1 * q1.y + t * q2.y;
  dst->z = t1 * q1.z + t * q2.z;
  dst->w = t1 * q1.w + t * q2.w;
}

float sin_zero_half_pi(float a) {
  float s, t; 
  s = a * a; 
  t = -2.39e-08f; 
  t *= s; 
  t += 2.7526e-06f; 
  t *= s; 
  t += -1.98409e-04f; 
  t *= s; 
  t += 8.3333315e-03f; 
  t *= s; 
  t += -1.666666664e-01f; 
  t *= s; 
  t += 1.0f; 
  t *= a; 
  return t; 
}

float atan_positive(float y, float x) {
  float a, d, s, t; 
  if( y > x ) { 
    a = -x / y; 
    d = M_PI / 2; 
  } 
  else { 
    a = y / x; 
    d = 0.0f; 
  } 
  s = a * a; 
  t = 0.0028662257f; 
  t *= s;     
  t += -0.0161657367f; 
  t *= s; 
  t += 0.0429096138f; 
  t *= s; 
  t += -0.0752896400f; 
  t *= s; 
  t += 0.1065626393f; 
  t *= s; 
  t += -0.1420889944f; 
  t *= s; 
  t += 0.1999355085f; 
  t *= s; 
  t += -0.3333314528f; 
  t *= s; 
  t += 1.0f; 
  t *= a; 
  t += d; 
  return t; 
}
  
inline void Quat::slerp(const Quat& from, const Quat& to, float t, Quat& result) {
  if(t <= 0.0f) {
    memcpy(&result.x, &from.x, sizeof(float) * 4);
    return;
  }
  else if(t >= 1.0f) {
    memcpy(&result.x, &to.x, sizeof(float) * 4);
    return;
  }
    
  float cosom, abs_cosom, sinom, sin_sqr, omega, scale0, scale1; 
  cosom = from.x * to.x + from.y * to.y + from.z * to.z + from.w * to.w; 
  abs_cosom = fabs( cosom ); 
  if ( ( 1.0f - abs_cosom ) > 1e-6f ) { 
    sin_sqr = 1.0f - abs_cosom * abs_cosom; 
    sinom = fast_sqrt( sin_sqr ); 
    omega = atan_positive( sin_sqr * sinom, abs_cosom ); 
    scale0 = sin_zero_half_pi( ( 1.0f - t ) * omega ) * sinom; 
    scale1 = sin_zero_half_pi( t * omega ) * sinom; 
  } else { 
    scale0 = 1.0f - t; 
    scale1 = t; 
  } 
  scale1 = ( cosom >= 0.0f ) ? scale1 : -scale1; 
  result.x = scale0 * from.x + scale1 * to.x; 
  result.y = scale0 * from.y + scale1 * to.y; 
  result.z = scale0 * from.z + scale1 * to.z; 
  result.w = scale0 * from.w + scale1 * to.w;
}

inline Quat Quat::operator*(const Quat& other) const {
  Quat result(*this);
  result.multiply(other);
  return result;
}

inline Vec3 Quat::operator*(const Vec3& v) const {
  return transform(v);
}

inline Quat& Quat::operator*=(const Quat& other) {
  multiply(other);
  return *this;
}

inline void Quat::print() {
  printf("%f, %f, %f, %f\n", x, y, z, w);
}

#endif
