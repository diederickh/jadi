#ifndef JADI_CAMERA_H
#define JADI_CAMERA_H

#include <jadi/Math.h>

struct Camera {
  Camera();
  ~Camera();
  void setPosition(const float x, const float y, const float z);
  void setPerspective(const float fov, const float aspect, const float f, const float n);
  Mat4& pm();
  Mat4& vm();

  Mat4 projection_matrix;
  Mat4 view_matrix;
};

inline Mat4& Camera::pm() {
  return projection_matrix;
}

inline Mat4& Camera::vm() {
  return view_matrix;
}
#endif
