#include "DepthOfField.h"

DepthOfField::DepthOfField() {
}

DepthOfField::~DepthOfField() {
}

void DepthOfField::setup() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  setupShaders();
  setupBuffers();
}

void DepthOfField::update() {
  
}

void DepthOfField::draw(const float* pm, const float* vm, const float* nm, std::vector<Particle>& particles) {
  glUseProgram(debug_prog);
  glBindVertexArray(vao);

  //mm.setYRotation(sin(millis() * 0.001) * TWO_PI);
  glUniformMatrix4fv(glGetUniformLocation(debug_prog, "u_pm"), 1, GL_FALSE, pm);
  glUniformMatrix4fv(glGetUniformLocation(debug_prog, "u_vm"), 1, GL_FALSE, vm);

  for(std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
    Particle& p = *it;
    mm.setPosition(p.position.x, p.position.y, p.position.z);
    glUniformMatrix4fv(glGetUniformLocation(debug_prog, "u_mm"), 1, GL_FALSE, mm.getPtr());
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
  }
}


void DepthOfField::setupShaders() {
  // DEBUG SHADER
  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vert_id, 1, &DOF_DEBUG_VS, NULL);
  glShaderSource(frag_id, 1, &DOF_DEBUG_FS, NULL);
  glCompileShader(vert_id); eglGetShaderInfoLog(vert_id);
  glCompileShader(frag_id); eglGetShaderInfoLog(frag_id);
  debug_prog = glCreateProgram();
  glAttachShader(debug_prog, vert_id);
  glAttachShader(debug_prog, frag_id);
  glLinkProgram(debug_prog);
}

void DepthOfField::setupBuffers() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  float* vertices;
  num_vertices = load_obj_file("sphere.obj", &vertices, true);
  glBufferData(GL_ARRAY_BUFFER,  num_vertices * 6 * sizeof(float), vertices, GL_STATIC_DRAW);
  delete[] vertices;

  GLint pos = glGetAttribLocation(debug_prog, "a_pos");
  GLint norm = glGetAttribLocation(debug_prog, "a_norm");
  glEnableVertexAttribArray(pos);
  glEnableVertexAttribArray(norm);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)0);
  glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)12);
}
