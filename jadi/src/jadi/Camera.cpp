#include <jadi/Camera.h>

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::setPosition(const float x, const float y, const float z) {
  view_matrix.setPosition(x, y, -z);
}

void Camera::setPerspective(const float fov, const float aspect, const float f, const float n) {
  projection_matrix.perspective(fov, aspect, f, n);
}

