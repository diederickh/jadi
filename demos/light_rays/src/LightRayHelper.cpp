#include "LightRayHelper.h"

LightRayHelper::LightRayHelper() 
  :vao(0)
  ,vbo(0)
  ,prog_occ(0)
  ,prog_final(0)
  ,num_vertices(0)
{
}

LightRayHelper::~LightRayHelper() {
}

void LightRayHelper::drawOcclusion(const float* pm, const float* vm, std::vector<Particle>& particles) {
  float col[] = {0.0f, 0.0, 0.0, 1.0};
  drawColoredParticles(pm, vm, particles, col);
}

void LightRayHelper::drawLights(const float* pm, const float* vm, std::vector<Particle>& particles) {
  float col[] = {1.0f, 1.0, 1.0};
  drawColoredParticles(pm, vm, particles, col);
}

void LightRayHelper::drawColoredParticles(const float* pm, const float* vm, std::vector<Particle>& particles, float* col) {
  glBindVertexArray(vao);
  glUseProgram(prog_occ);
  Mat4 model;
  GLint upm = glGetUniformLocation(prog_occ, "u_pm");
  GLint uvm = glGetUniformLocation(prog_occ, "u_vm");
  GLint umm = glGetUniformLocation(prog_occ, "u_mm");
  GLint ucol = glGetUniformLocation(prog_occ, "u_color");
  glUniform3fv(ucol, 1, col);
  
  for(std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
    Particle& p = *it;
    model.setPosition(p.position.x, p.position.y, p.position.z);
    glUniformMatrix4fv(upm, 1, GL_FALSE, pm);
    glUniformMatrix4fv(uvm, 1, GL_FALSE, vm);
    glUniformMatrix4fv(umm, 1, GL_FALSE, model.getPtr());
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
  }
}

void LightRayHelper::drawShadedParticles(const float* pm, const float* vm, std::vector<Particle>& particles) {
  glBindVertexArray(vao);
  glUseProgram(prog_final);

  Mat4 model;
  GLint upm = glGetUniformLocation(prog_final, "u_pm");
  GLint uvm = glGetUniformLocation(prog_final, "u_vm");
  GLint umm = glGetUniformLocation(prog_final, "u_mm");
  
  for(std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
    Particle& p = *it;
    model.setPosition(p.position.x, p.position.y, p.position.z);
    glUniformMatrix4fv(upm, 1, GL_FALSE, pm);
    glUniformMatrix4fv(uvm, 1, GL_FALSE, vm);
    glUniformMatrix4fv(umm, 1, GL_FALSE, model.getPtr());
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
  }
}

void LightRayHelper::setup() {
  setupShaders();
  setupBuffers();
}

void LightRayHelper::setupBuffers() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  float* vertices;
  num_vertices = load_obj_file("sphere.obj", &vertices, true);
  glBufferData(GL_ARRAY_BUFFER,  num_vertices * 6 * sizeof(float), vertices, GL_STATIC_DRAW);
  delete[] vertices;

  glEnableVertexAttribArray(0); // pos
  glEnableVertexAttribArray(1); // norm
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)12);
}

void LightRayHelper::setupShaders() {
  prog_occ = createShader(LRH_SHARED_VS, LRH_OCCLUSION_FS);
  glBindAttribLocation(prog_occ, 0, "a_pos");
  glBindAttribLocation(prog_occ, 1, "a_norm");
  glLinkProgram(prog_occ);

  prog_final = createShader(LRH_SHARED_VS, LRH_FINAL_FS);
  glBindAttribLocation(prog_final, 0, "a_pos");
  glBindAttribLocation(prog_final, 1, "a_norm");
  glLinkProgram(prog_final);
}


GLuint LightRayHelper::createShader(const char* vs, const char* fs) {
  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vert_id, 1, &vs, NULL);
  glShaderSource(frag_id, 1, &fs, NULL);
  glCompileShader(vert_id); eglGetShaderInfoLog(vert_id);
  glCompileShader(frag_id); eglGetShaderInfoLog(frag_id);
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert_id);
  glAttachShader(prog, frag_id);
  return prog;
}
